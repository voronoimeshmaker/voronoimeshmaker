//==============================================================================
// File        : MohidNGHDF5Writer2D.cpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : MohidNG 2D Voronoi mesh package writer implementation.
//==============================================================================

//==============================================================================
// c++ includes
//==============================================================================
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <numeric>
#include <string>
#include <string_view>
#include <utility>
#include <vector>


//==============================================================================
// external includes
//==============================================================================
#include <hdf5.h>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/audit/MeshConnectivityGeometryAudit2D.hpp>
#include <vmm/audit/MeshPatchAudit2D.hpp>
#include <vmm/error/MeshException.hpp>
#include <vmm/export/MohidNGHDF5Writer2D.hpp>

namespace vmm::io {
namespace {

using vmm::core::CellId;
using vmm::core::BoundaryPatchId;
using vmm::core::is_valid;

class Hdf5Handle final {
public:
    Hdf5Handle() = default;
    Hdf5Handle(hid_t id, herr_t (*close_function)(hid_t)) noexcept : id_(id), close_(close_function) {}
    Hdf5Handle(const Hdf5Handle&) = delete;
    Hdf5Handle& operator=(const Hdf5Handle&) = delete;

    Hdf5Handle(Hdf5Handle&& other) noexcept : id_(other.id_), close_(other.close_)
    {
        other.id_ = -1;
        other.close_ = nullptr;
    }

    Hdf5Handle& operator=(Hdf5Handle&& other) noexcept
    {
        if(this != &other) {
            close();
            id_ = other.id_;
            close_ = other.close_;
            other.id_ = -1;
            other.close_ = nullptr;
        }
        return *this;
    }

    ~Hdf5Handle() { close(); }

    [[nodiscard]] hid_t id() const noexcept { return id_; }
    [[nodiscard]] bool valid() const noexcept { return id_ >= 0; }

private:
    void close() noexcept
    {
        if(id_ >= 0 && close_ != nullptr) {
            close_(id_);
        }
    }

    hid_t id_{-1};
    herr_t (*close_)(hid_t){nullptr};
};

[[noreturn]] void throw_hdf5_error(std::string_view context, std::string_view message)
{
    vmm::error::throw_parse_error(context, message);
}

void require_hdf5(bool condition, std::string_view context, std::string_view message)
{
    if(!condition) {
        throw_hdf5_error(context, message);
    }
}

[[nodiscard]] Hdf5Handle create_group(hid_t parent, const char* name)
{
    Hdf5Handle group{H5Gcreate2(parent, name, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT), H5Gclose};
    require_hdf5(group.valid(), "write_mohidng_hdf5_mesh_2d", std::string{"Cannot create group: "} + name);
    return group;
}

[[nodiscard]] Hdf5Handle create_simple_space(std::span<const hsize_t> dimensions)
{
    Hdf5Handle space{H5Screate_simple(static_cast<int>(dimensions.size()), dimensions.data(), nullptr), H5Sclose};
    require_hdf5(space.valid(), "write_mohidng_hdf5_mesh_2d", "Cannot create HDF5 dataspace.");
    return space;
}

[[nodiscard]] Hdf5Handle create_scalar_space()
{
    Hdf5Handle space{H5Screate(H5S_SCALAR), H5Sclose};
    require_hdf5(space.valid(), "write_mohidng_hdf5_mesh_2d", "Cannot create scalar HDF5 dataspace.");
    return space;
}

[[nodiscard]] Hdf5Handle create_string_type(std::size_t width)
{
    Hdf5Handle type{H5Tcopy(H5T_C_S1), H5Tclose};
    require_hdf5(type.valid(), "write_mohidng_hdf5_mesh_2d", "Cannot create HDF5 string type.");
    require_hdf5(H5Tset_size(type.id(), width) >= 0,
                 "write_mohidng_hdf5_mesh_2d",
                 "Cannot set HDF5 string type width.");
    require_hdf5(H5Tset_strpad(type.id(), H5T_STR_NULLTERM) >= 0,
                 "write_mohidng_hdf5_mesh_2d",
                 "Cannot set HDF5 string padding.");
    return type;
}

void write_string_attribute(hid_t object, const char* name, const std::string& value)
{
    Hdf5Handle type{H5Tcopy(H5T_C_S1), H5Tclose};
    require_hdf5(type.valid(), "write_mohidng_hdf5_mesh_2d", "Cannot create HDF5 attribute string type.");
    require_hdf5(H5Tset_size(type.id(), H5T_VARIABLE) >= 0,
                 "write_mohidng_hdf5_mesh_2d",
                 "Cannot create variable-length string attribute.");
    const auto space = create_scalar_space();
    Hdf5Handle attribute{H5Acreate2(object, name, type.id(), space.id(), H5P_DEFAULT, H5P_DEFAULT), H5Aclose};
    require_hdf5(attribute.valid(), "write_mohidng_hdf5_mesh_2d", std::string{"Cannot create attribute: "} + name);
    const char* raw_value = value.c_str();
    require_hdf5(H5Awrite(attribute.id(), type.id(), &raw_value) >= 0,
                 "write_mohidng_hdf5_mesh_2d",
                 std::string{"Cannot write attribute: "} + name);
}

void write_int_attribute(hid_t object, const char* name, int value)
{
    const auto space = create_scalar_space();
    Hdf5Handle attribute{H5Acreate2(object, name, H5T_NATIVE_INT, space.id(), H5P_DEFAULT, H5P_DEFAULT), H5Aclose};
    require_hdf5(attribute.valid(), "write_mohidng_hdf5_mesh_2d", std::string{"Cannot create attribute: "} + name);
    require_hdf5(H5Awrite(attribute.id(), H5T_NATIVE_INT, &value) >= 0,
                 "write_mohidng_hdf5_mesh_2d",
                 std::string{"Cannot write attribute: "} + name);
}

void write_int64_dataset(hid_t parent, const char* name, std::span<const std::int64_t> values)
{
    const hsize_t dimensions[] = {static_cast<hsize_t>(values.size())};
    const auto space = create_simple_space(dimensions);
    Hdf5Handle dataset{H5Dcreate2(parent, name, H5T_NATIVE_INT64, space.id(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT),
                       H5Dclose};
    require_hdf5(dataset.valid(), "write_mohidng_hdf5_mesh_2d", std::string{"Cannot create dataset: "} + name);
    require_hdf5(H5Dwrite(dataset.id(), H5T_NATIVE_INT64, H5S_ALL, H5S_ALL, H5P_DEFAULT, values.data()) >= 0,
                 "write_mohidng_hdf5_mesh_2d",
                 std::string{"Cannot write dataset: "} + name);
}

void write_int32_dataset(hid_t parent, const char* name, std::span<const std::int32_t> values)
{
    const hsize_t dimensions[] = {static_cast<hsize_t>(values.size())};
    const auto space = create_simple_space(dimensions);
    Hdf5Handle dataset{H5Dcreate2(parent, name, H5T_NATIVE_INT32, space.id(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT),
                       H5Dclose};
    require_hdf5(dataset.valid(), "write_mohidng_hdf5_mesh_2d", std::string{"Cannot create dataset: "} + name);
    require_hdf5(H5Dwrite(dataset.id(), H5T_NATIVE_INT32, H5S_ALL, H5S_ALL, H5P_DEFAULT, values.data()) >= 0,
                 "write_mohidng_hdf5_mesh_2d",
                 std::string{"Cannot write dataset: "} + name);
}

void write_real_dataset_1d(hid_t parent, const char* name, std::span<const double> values)
{
    const hsize_t dimensions[] = {static_cast<hsize_t>(values.size())};
    const auto space = create_simple_space(dimensions);
    Hdf5Handle dataset{H5Dcreate2(parent, name, H5T_NATIVE_DOUBLE, space.id(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT),
                       H5Dclose};
    require_hdf5(dataset.valid(), "write_mohidng_hdf5_mesh_2d", std::string{"Cannot create dataset: "} + name);
    require_hdf5(H5Dwrite(dataset.id(), H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, values.data()) >= 0,
                 "write_mohidng_hdf5_mesh_2d",
                 std::string{"Cannot write dataset: "} + name);
}

void write_real_dataset_2d(hid_t parent, const char* name, std::span<const double> values, hsize_t columns)
{
    require_hdf5(columns > 0U && values.size() % static_cast<std::size_t>(columns) == 0U,
                 "write_mohidng_hdf5_mesh_2d",
                 std::string{"Invalid 2D dataset shape: "} + name);
    const hsize_t dimensions[] = {static_cast<hsize_t>(values.size() / static_cast<std::size_t>(columns)), columns};
    const auto space = create_simple_space(dimensions);
    Hdf5Handle dataset{H5Dcreate2(parent, name, H5T_NATIVE_DOUBLE, space.id(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT),
                       H5Dclose};
    require_hdf5(dataset.valid(), "write_mohidng_hdf5_mesh_2d", std::string{"Cannot create dataset: "} + name);
    require_hdf5(H5Dwrite(dataset.id(), H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, values.data()) >= 0,
                 "write_mohidng_hdf5_mesh_2d",
                 std::string{"Cannot write dataset: "} + name);
}

void write_fixed_string_dataset(hid_t parent, const char* name, std::span<const std::string> values)
{
    std::size_t width = 1U;
    for(const auto& value : values) {
        width = std::max(width, value.size() + 1U);
    }

    std::vector<char> buffer(values.size() * width, '\0');
    for(std::size_t index = 0U; index < values.size(); ++index) {
        const auto copy_count = std::min(values[index].size(), width - 1U);
        std::copy_n(values[index].data(), copy_count, buffer.data() + (index * width));
    }

    const auto type = create_string_type(width);
    const hsize_t dimensions[] = {static_cast<hsize_t>(values.size())};
    const auto space = create_simple_space(dimensions);
    Hdf5Handle dataset{H5Dcreate2(parent, name, type.id(), space.id(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT),
                       H5Dclose};
    require_hdf5(dataset.valid(), "write_mohidng_hdf5_mesh_2d", std::string{"Cannot create dataset: "} + name);
    require_hdf5(H5Dwrite(dataset.id(), type.id(), H5S_ALL, H5S_ALL, H5P_DEFAULT, buffer.data()) >= 0,
                 "write_mohidng_hdf5_mesh_2d",
                 std::string{"Cannot write dataset: "} + name);
}

[[nodiscard]] std::vector<std::int64_t> contiguous_ids(std::size_t size)
{
    std::vector<std::int64_t> ids(size);
    std::iota(ids.begin(), ids.end(), std::int64_t{0});
    return ids;
}

[[nodiscard]] std::int64_t cell_id_to_hdf5(CellId cell_id) noexcept
{
    return is_valid(cell_id) ? static_cast<std::int64_t>(cell_id.value) : -1;
}

[[nodiscard]] std::int32_t patch_id_to_hdf5(BoundaryPatchId patch_id) noexcept
{
    return is_valid(patch_id) ? static_cast<std::int32_t>(patch_id.value) : -1;
}

void validate_before_export(const vmm::mesh::FiniteVolumeMesh2D& mesh)
{
    const auto geometry_report = vmm::audit::audit_mesh_connectivity_geometry_2d(mesh);
    if(!geometry_report.valid()) {
        const auto code = geometry_report.empty() ? std::string{"unknown"} : geometry_report.issues.front().code;
        vmm::error::throw_invalid_argument("write_mohidng_hdf5_mesh_2d",
                                           "Cannot export invalid finite-volume mesh. First geometry issue: " + code);
    }

    const auto patch_report = vmm::audit::audit_mesh_patches_2d(mesh);
    if(!patch_report.valid()) {
        const auto code = patch_report.empty() ? std::string{"unknown"} : patch_report.issues.front().code;
        vmm::error::throw_invalid_argument("write_mohidng_hdf5_mesh_2d",
                                           "Cannot export invalid finite-volume mesh. First patch issue: " + code);
    }
}

void validate_raster_samples(const vmm::mesh::FiniteVolumeMesh2D& mesh,
                             const vmm::raster::CellRasterSamples2D& samples)
{
    if(!samples.has_consistent_storage()) {
        vmm::error::throw_invalid_argument("write_mohidng_hdf5_mesh_2d",
                                           "Raster sample arrays have inconsistent storage.");
    }
    if(samples.size() != mesh.cell_count()) {
        vmm::error::throw_invalid_argument("write_mohidng_hdf5_mesh_2d",
                                           "Raster sample count must match finite-volume cell count.");
    }
    for(std::size_t cell = 0U; cell < mesh.cell_count(); ++cell) {
        if(samples.cell_id[cell] != CellId{cell}) {
            vmm::error::throw_invalid_argument("write_mohidng_hdf5_mesh_2d",
                                               "Raster sample cell ids must follow finite-volume cell rows.");
        }
        if(samples.valid[cell] != 0U && samples.valid[cell] != 1U) {
            vmm::error::throw_invalid_argument("write_mohidng_hdf5_mesh_2d",
                                               "Raster sample validity flags must be 0 or 1.");
        }
    }
}

void write_root_metadata(hid_t file, const MohidNGHDF5Writer2DOptions& options)
{
    write_int_attribute(file, "dimension", 2);
    write_string_attribute(file, "schema_version", kMohidNGVoronoiMeshPackageSchemaVersion);
    write_string_attribute(file, "producer", "VoronoiMeshMaker");
    write_string_attribute(file, "coordinate_reference_system", options.coordinate_reference_system);
    write_string_attribute(file, "note", options.note);
}

void write_nodes(hid_t file, const vmm::mesh::FiniteVolumeMesh2D& mesh)
{
    const auto group = create_group(file, "nodes");
    const auto ids = contiguous_ids(mesh.node_count());
    std::vector<double> xy;
    xy.reserve(mesh.node_count() * 2U);
    for(std::size_t node = 0U; node < mesh.node_count(); ++node) {
        xy.push_back(mesh.nodes.x[node]);
        xy.push_back(mesh.nodes.y[node]);
    }
    write_int64_dataset(group.id(), "id", ids);
    write_real_dataset_2d(group.id(), "xy", xy, 2U);
}

void write_cells(hid_t file, const vmm::mesh::FiniteVolumeMesh2D& mesh)
{
    const auto group = create_group(file, "cells");
    const auto ids = contiguous_ids(mesh.cell_count());
    std::vector<double> centres;
    centres.reserve(mesh.cell_count() * 2U);
    for(std::size_t cell = 0U; cell < mesh.cell_count(); ++cell) {
        centres.push_back(mesh.cells.centre_x[cell]);
        centres.push_back(mesh.cells.centre_y[cell]);
    }
    write_int64_dataset(group.id(), "id", ids);
    write_real_dataset_2d(group.id(), "centre", centres, 2U);
    write_real_dataset_1d(group.id(), "area", mesh.cells.area);
}

void write_faces(hid_t file, const vmm::mesh::FiniteVolumeMesh2D& mesh)
{
    const auto group = create_group(file, "faces");
    const auto ids = contiguous_ids(mesh.face_count());
    std::vector<std::int64_t> owners;
    std::vector<std::int64_t> neighbours;
    std::vector<std::int32_t> patch_ids;
    std::vector<double> centres;
    std::vector<double> normals;
    owners.reserve(mesh.face_count());
    neighbours.reserve(mesh.face_count());
    patch_ids.reserve(mesh.face_count());
    centres.reserve(mesh.face_count() * 2U);
    normals.reserve(mesh.face_count() * 2U);

    for(std::size_t face = 0U; face < mesh.face_count(); ++face) {
        owners.push_back(cell_id_to_hdf5(mesh.faces.owner[face]));
        neighbours.push_back(cell_id_to_hdf5(mesh.faces.neighbour[face]));
        patch_ids.push_back(patch_id_to_hdf5(mesh.faces.patch_id[face]));
        centres.push_back(mesh.faces.centre_x[face]);
        centres.push_back(mesh.faces.centre_y[face]);
        normals.push_back(mesh.faces.normal_x[face]);
        normals.push_back(mesh.faces.normal_y[face]);
    }

    write_int64_dataset(group.id(), "id", ids);
    write_int64_dataset(group.id(), "owner", owners);
    write_int64_dataset(group.id(), "neighbour", neighbours);
    write_real_dataset_2d(group.id(), "centre", centres, 2U);
    write_real_dataset_2d(group.id(), "unit_normal", normals, 2U);
    write_real_dataset_1d(group.id(), "length", mesh.faces.length);
    write_int32_dataset(group.id(), "patch_id", patch_ids);
}

void write_boundary_patches(hid_t file, const vmm::mesh::FiniteVolumeMesh2D& mesh)
{
    const auto group = create_group(file, "boundary_patches");
    std::vector<std::int32_t> ids;
    std::vector<std::string> names;
    std::vector<std::string> types;
    ids.reserve(mesh.patch_count());
    names.reserve(mesh.patch_count());
    types.reserve(mesh.patch_count());

    for(std::size_t patch = 0U; patch < mesh.patch_count(); ++patch) {
        ids.push_back(static_cast<std::int32_t>(mesh.patches.id[patch].value));
        names.push_back(mesh.patches.name[patch]);
        types.emplace_back(mesh.patches.type[patch].name);
    }

    write_int32_dataset(group.id(), "id", ids);
    write_fixed_string_dataset(group.id(), "name", names);
    write_fixed_string_dataset(group.id(), "type", types);
}

void write_raster_samples(hid_t file,
                          const vmm::raster::CellRasterSamples2D& samples,
                          const std::string& field_name)
{
    const auto group = create_group(file, "cell_fields");
    write_string_attribute(group.id(), "raster_field_name", field_name);

    std::vector<std::int64_t> cell_ids;
    std::vector<std::int32_t> valid;
    cell_ids.reserve(samples.size());
    valid.reserve(samples.size());
    for(std::size_t index = 0U; index < samples.size(); ++index) {
        cell_ids.push_back(cell_id_to_hdf5(samples.cell_id[index]));
        valid.push_back(static_cast<std::int32_t>(samples.valid[index]));
    }

    write_int64_dataset(group.id(), "cell_id", cell_ids);
    write_real_dataset_1d(group.id(), "value", samples.value);
    write_int32_dataset(group.id(), "valid", valid);
}

} // namespace

void write_mohidng_hdf5_mesh_2d(const std::filesystem::path& file_path,
                                const vmm::mesh::FiniteVolumeMesh2D& mesh,
                                const MohidNGHDF5Writer2DOptions& options)
{
    if(options.require_valid_mesh) {
        validate_before_export(mesh);
    }
    if(options.raster_samples != nullptr) {
        validate_raster_samples(mesh, *options.raster_samples);
    }

    if(file_path.has_parent_path()) {
        std::filesystem::create_directories(file_path.parent_path());
    }

    H5Eset_auto2(H5E_DEFAULT, nullptr, nullptr);
    Hdf5Handle file{H5Fcreate(file_path.string().c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT), H5Fclose};
    require_hdf5(file.valid(), "write_mohidng_hdf5_mesh_2d", "Cannot create MohidNG HDF5 mesh package.");

    write_root_metadata(file.id(), options);
    write_nodes(file.id(), mesh);
    write_cells(file.id(), mesh);
    write_faces(file.id(), mesh);
    write_boundary_patches(file.id(), mesh);
    if(options.raster_samples != nullptr) {
        write_raster_samples(file.id(), *options.raster_samples, options.raster_field_name);
    }
}

} // namespace vmm::io
