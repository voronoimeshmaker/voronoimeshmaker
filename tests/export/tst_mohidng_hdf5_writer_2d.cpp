//==============================================================================
// c++ includes
//==============================================================================
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>


//==============================================================================
// external includes
//==============================================================================
#include <gtest/gtest.h>
#include <hdf5.h>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/domain/PlanarCell2D.hpp>
#include <vmm/error/MeshException.hpp>
#include <vmm/export/MohidNGHDF5Writer2D.hpp>
#include <vmm/mesh/FiniteVolumeMeshBuilder2D.hpp>

using namespace vmm::core;
using namespace vmm::domain;
using namespace vmm::mesh;

namespace {

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

PlanarCell2D make_cell(std::vector<Point2D> vertices, Point2D site, GeneratorID generator_id)
{
    return PlanarCell2D{Polygon2D{std::move(vertices)}, site, generator_id};
}

FiniteVolumeMesh2D make_mesh()
{
    const std::vector<PlanarCell2D> cells{
        make_cell({{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}}, {0.5, 0.5}, 0U),
        make_cell({{1.0, 0.0}, {2.0, 0.0}, {2.0, 1.0}, {1.0, 1.0}}, {1.5, 0.5}, 1U),
    };

    FiniteVolumeMeshBuilder2DOptions options;
    options.default_boundary_patch_name = "fallback-wall";
    options.default_boundary_patch_type = BoundaryPatchTypeTraits::Wall;
    options.boundary_patches = {
        BoundaryPatchRing2D{Polygon2D{{{0.0, 0.0}, {2.0, 0.0}, {2.0, 1.0}, {0.0, 1.0}}},
                            "outer-land",
                            BoundaryPatchTypeTraits::LandBoundary}};
    return build_finite_volume_mesh_2d(cells, options);
}

Hdf5Handle open_file(const std::filesystem::path& path)
{
    H5Eset_auto2(H5E_DEFAULT, nullptr, nullptr);
    Hdf5Handle file{H5Fopen(path.string().c_str(), H5F_ACC_RDONLY, H5P_DEFAULT), H5Fclose};
    EXPECT_TRUE(file.valid());
    return file;
}

Hdf5Handle open_dataset(hid_t file, const char* path)
{
    Hdf5Handle dataset{H5Dopen2(file, path, H5P_DEFAULT), H5Dclose};
    EXPECT_TRUE(dataset.valid()) << path;
    return dataset;
}

Hdf5Handle open_group(hid_t file, const char* path)
{
    Hdf5Handle group{H5Gopen2(file, path, H5P_DEFAULT), H5Gclose};
    EXPECT_TRUE(group.valid()) << path;
    return group;
}

std::vector<hsize_t> dataset_dimensions(hid_t file, const char* path)
{
    const auto dataset = open_dataset(file, path);
    Hdf5Handle space{H5Dget_space(dataset.id()), H5Sclose};
    EXPECT_TRUE(space.valid()) << path;
    const auto rank = H5Sget_simple_extent_ndims(space.id());
    std::vector<hsize_t> dimensions(static_cast<std::size_t>(rank));
    EXPECT_GE(H5Sget_simple_extent_dims(space.id(), dimensions.data(), nullptr), 0);
    return dimensions;
}

std::string read_string_attribute(hid_t object, const char* name)
{
    Hdf5Handle attribute{H5Aopen(object, name, H5P_DEFAULT), H5Aclose};
    EXPECT_TRUE(attribute.valid()) << name;
    Hdf5Handle type{H5Aget_type(attribute.id()), H5Tclose};
    EXPECT_TRUE(type.valid()) << name;

    if(H5Tis_variable_str(type.id()) > 0) {
        char* raw = nullptr;
        EXPECT_GE(H5Aread(attribute.id(), type.id(), &raw), 0);
        std::string value = raw == nullptr ? std::string{} : std::string{raw};
        if(raw != nullptr) {
            H5free_memory(raw);
        }
        return value;
    }

    const auto width = H5Tget_size(type.id());
    std::vector<char> buffer(width + 1U, '\0');
    EXPECT_GE(H5Aread(attribute.id(), type.id(), buffer.data()), 0);
    return std::string{buffer.data()};
}

int read_int_attribute(hid_t object, const char* name)
{
    Hdf5Handle attribute{H5Aopen(object, name, H5P_DEFAULT), H5Aclose};
    EXPECT_TRUE(attribute.valid()) << name;
    int value{};
    EXPECT_GE(H5Aread(attribute.id(), H5T_NATIVE_INT, &value), 0);
    return value;
}

std::vector<std::int64_t> read_int64_dataset(hid_t file, const char* path)
{
    const auto dataset = open_dataset(file, path);
    const auto dimensions = dataset_dimensions(file, path);
    EXPECT_EQ(dimensions.size(), 1U);
    std::vector<std::int64_t> values(static_cast<std::size_t>(dimensions[0]));
    EXPECT_GE(H5Dread(dataset.id(), H5T_NATIVE_INT64, H5S_ALL, H5S_ALL, H5P_DEFAULT, values.data()), 0);
    return values;
}

std::vector<std::int32_t> read_int32_dataset(hid_t file, const char* path)
{
    const auto dataset = open_dataset(file, path);
    const auto dimensions = dataset_dimensions(file, path);
    EXPECT_EQ(dimensions.size(), 1U);
    std::vector<std::int32_t> values(static_cast<std::size_t>(dimensions[0]));
    EXPECT_GE(H5Dread(dataset.id(), H5T_NATIVE_INT32, H5S_ALL, H5S_ALL, H5P_DEFAULT, values.data()), 0);
    return values;
}

std::vector<double> read_double_dataset(hid_t file, const char* path)
{
    const auto dataset = open_dataset(file, path);
    const auto dimensions = dataset_dimensions(file, path);
    std::size_t count = 1U;
    for(const auto dimension : dimensions) {
        count *= static_cast<std::size_t>(dimension);
    }
    std::vector<double> values(count);
    EXPECT_GE(H5Dread(dataset.id(), H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, values.data()), 0);
    return values;
}

std::vector<std::string> read_string_dataset(hid_t file, const char* path)
{
    const auto dataset = open_dataset(file, path);
    const auto dimensions = dataset_dimensions(file, path);
    EXPECT_EQ(dimensions.size(), 1U);
    Hdf5Handle type{H5Dget_type(dataset.id()), H5Tclose};
    EXPECT_TRUE(type.valid()) << path;
    const auto width = H5Tget_size(type.id());
    std::vector<char> buffer(static_cast<std::size_t>(dimensions[0]) * width, '\0');
    EXPECT_GE(H5Dread(dataset.id(), type.id(), H5S_ALL, H5S_ALL, H5P_DEFAULT, buffer.data()), 0);

    std::vector<std::string> values;
    for(std::size_t index = 0U; index < static_cast<std::size_t>(dimensions[0]); ++index) {
        std::string value{buffer.data() + (index * width), width};
        const auto end = value.find('\0');
        if(end != std::string::npos) {
            value.resize(end);
        }
        values.push_back(std::move(value));
    }
    return values;
}

} // namespace

TEST(MohidNGHDF5Writer2DTest, WritesMohidNGSchemaPackage)
{
    const auto mesh = make_mesh();
    const auto output = std::filesystem::temp_directory_path() / "vmm_tst_mohidng_writer_2d.mngpkg.h5";

    vmm::io::MohidNGHDF5Writer2DOptions options;
    options.coordinate_reference_system = "EPSG:4326";
    options.note = "writer schema test";
    vmm::raster::CellRasterSamples2D raster_samples;
    raster_samples.cell_id = {CellId{0U}, CellId{1U}};
    raster_samples.value = {3.5, 7.5};
    raster_samples.valid = {1U, 0U};
    options.raster_field_name = "bathymetry";
    options.raster_samples = &raster_samples;
    vmm::io::write_mohidng_hdf5_mesh_2d(output, mesh, options);

    const auto file = open_file(output);
    ASSERT_TRUE(file.valid());
    EXPECT_EQ(read_string_attribute(file.id(), "schema_version"), vmm::io::kMohidNGVoronoiMeshPackageSchemaVersion);
    EXPECT_EQ(read_string_attribute(file.id(), "producer"), "VoronoiMeshMaker");
    EXPECT_EQ(read_string_attribute(file.id(), "coordinate_reference_system"), "EPSG:4326");
    EXPECT_EQ(read_string_attribute(file.id(), "note"), "writer schema test");
    EXPECT_EQ(read_int_attribute(file.id(), "dimension"), 2);

    EXPECT_EQ(dataset_dimensions(file.id(), "/nodes/id"), std::vector<hsize_t>{mesh.node_count()});
    EXPECT_EQ(dataset_dimensions(file.id(), "/nodes/xy"), (std::vector<hsize_t>{mesh.node_count(), 2U}));
    EXPECT_EQ(dataset_dimensions(file.id(), "/cells/id"), std::vector<hsize_t>{mesh.cell_count()});
    EXPECT_EQ(dataset_dimensions(file.id(), "/cells/centre"), (std::vector<hsize_t>{mesh.cell_count(), 2U}));
    EXPECT_EQ(dataset_dimensions(file.id(), "/faces/id"), std::vector<hsize_t>{mesh.face_count()});
    EXPECT_EQ(dataset_dimensions(file.id(), "/faces/centre"), (std::vector<hsize_t>{mesh.face_count(), 2U}));
    EXPECT_EQ(dataset_dimensions(file.id(), "/faces/unit_normal"), (std::vector<hsize_t>{mesh.face_count(), 2U}));
    EXPECT_EQ(dataset_dimensions(file.id(), "/boundary_patches/id"), std::vector<hsize_t>{mesh.patch_count()});
    EXPECT_EQ(dataset_dimensions(file.id(), "/cell_fields/cell_id"), std::vector<hsize_t>{mesh.cell_count()});
    EXPECT_EQ(dataset_dimensions(file.id(), "/cell_fields/value"), std::vector<hsize_t>{mesh.cell_count()});
    EXPECT_EQ(dataset_dimensions(file.id(), "/cell_fields/valid"), std::vector<hsize_t>{mesh.cell_count()});

    const auto node_ids = read_int64_dataset(file.id(), "/nodes/id");
    ASSERT_EQ(node_ids.size(), mesh.node_count());
    EXPECT_EQ(node_ids.front(), 0);
    EXPECT_EQ(node_ids.back(), static_cast<std::int64_t>(mesh.node_count() - 1U));

    const auto owners = read_int64_dataset(file.id(), "/faces/owner");
    const auto neighbours = read_int64_dataset(file.id(), "/faces/neighbour");
    const auto patch_ids = read_int32_dataset(file.id(), "/faces/patch_id");
    ASSERT_EQ(owners.size(), mesh.face_count());
    ASSERT_EQ(neighbours.size(), mesh.face_count());
    ASSERT_EQ(patch_ids.size(), mesh.face_count());
    EXPECT_EQ(owners[0], static_cast<std::int64_t>(mesh.faces.owner[0].value));
    EXPECT_EQ(neighbours[0], is_valid(mesh.faces.neighbour[0]) ? static_cast<std::int64_t>(mesh.faces.neighbour[0].value) : -1);
    EXPECT_EQ(patch_ids[0], is_valid(mesh.faces.patch_id[0]) ? static_cast<std::int32_t>(mesh.faces.patch_id[0].value) : -1);

    const auto patch_names = read_string_dataset(file.id(), "/boundary_patches/name");
    const auto patch_types = read_string_dataset(file.id(), "/boundary_patches/type");
    ASSERT_EQ(patch_names.size(), mesh.patch_count());
    ASSERT_EQ(patch_types.size(), mesh.patch_count());
    EXPECT_EQ(patch_names[0], "fallback-wall");
    EXPECT_EQ(patch_names[1], "outer-land");
    EXPECT_EQ(patch_types[1], "land-boundary");

    const auto lengths = read_double_dataset(file.id(), "/faces/length");
    ASSERT_EQ(lengths.size(), mesh.face_count());
    EXPECT_GT(lengths.front(), 0.0);

    const auto raster_cell_ids = read_int64_dataset(file.id(), "/cell_fields/cell_id");
    const auto raster_values = read_double_dataset(file.id(), "/cell_fields/value");
    const auto raster_valid = read_int32_dataset(file.id(), "/cell_fields/valid");
    ASSERT_EQ(raster_cell_ids.size(), mesh.cell_count());
    ASSERT_EQ(raster_values.size(), mesh.cell_count());
    ASSERT_EQ(raster_valid.size(), mesh.cell_count());
    const auto cell_fields_group = open_group(file.id(), "/cell_fields");
    EXPECT_EQ(read_string_attribute(cell_fields_group.id(), "raster_field_name"), "bathymetry");
    EXPECT_EQ(raster_cell_ids[0], 0);
    EXPECT_DOUBLE_EQ(raster_values[0], 3.5);
    EXPECT_EQ(raster_valid[0], 1);
    EXPECT_EQ(raster_valid[1], 0);
}

TEST(MohidNGHDF5Writer2DTest, RejectsInvalidMeshBeforeExport)
{
    auto mesh = make_mesh();
    mesh.cells.area[0] = -1.0;
    const auto output = std::filesystem::temp_directory_path() / "vmm_tst_mohidng_writer_invalid.mngpkg.h5";

    EXPECT_THROW(vmm::io::write_mohidng_hdf5_mesh_2d(output, mesh), vmm::error::MeshException);
}

TEST(MohidNGHDF5Writer2DTest, RejectsRasterSamplesWithWrongCellIds)
{
    const auto mesh = make_mesh();
    vmm::raster::CellRasterSamples2D raster_samples;
    raster_samples.cell_id = {CellId{0U}, CellId{99U}};
    raster_samples.value = {1.0, 2.0};
    raster_samples.valid = {1U, 1U};
    vmm::io::MohidNGHDF5Writer2DOptions options;
    options.raster_samples = &raster_samples;
    const auto output = std::filesystem::temp_directory_path() / "vmm_tst_mohidng_writer_bad_raster.mngpkg.h5";

    EXPECT_THROW(vmm::io::write_mohidng_hdf5_mesh_2d(output, mesh, options), vmm::error::MeshException);
}
