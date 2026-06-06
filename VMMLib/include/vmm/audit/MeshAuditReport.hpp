//==============================================================================
// File        : MeshAuditReport.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Structured audit report for finite-volume mesh validation.
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program. If not, see <https://www.gnu.org/licenses/>.
//
// @file MeshAuditReport.hpp
// @brief Structured issue collection for VMM mesh audits.
// @ingroup vmm_audit
//==============================================================================
#pragma once

//==============================================================================
// c++ includes
//==============================================================================
#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>


//==============================================================================
// VoronoiMeshMaker includes
//==============================================================================
#include <vmm/core/StrongIds.hpp>


namespace vmm::audit {

struct MeshAuditSeverity final {
    std::string_view name;
    std::uint8_t rank;

    [[nodiscard]] friend constexpr bool operator==(MeshAuditSeverity lhs, MeshAuditSeverity rhs) noexcept
    {
        return lhs.rank == rhs.rank && lhs.name == rhs.name;
    }

    [[nodiscard]] friend constexpr bool operator!=(MeshAuditSeverity lhs, MeshAuditSeverity rhs) noexcept
    {
        return !(lhs == rhs);
    }
};

struct MeshAuditSeverityTraits final {
    static constexpr MeshAuditSeverity Info{"info", 0U};
    static constexpr MeshAuditSeverity Warning{"warning", 10U};
    static constexpr MeshAuditSeverity Error{"error", 20U};
    static constexpr MeshAuditSeverity Fatal{"fatal", 30U};

    [[nodiscard]] static constexpr bool invalidates_mesh(MeshAuditSeverity severity) noexcept
    {
        return severity.rank >= Error.rank;
    }

    [[nodiscard]] static constexpr bool stops_audit(MeshAuditSeverity severity) noexcept
    {
        return severity.rank >= Fatal.rank;
    }
};

struct MeshAuditIssue final {
    MeshAuditSeverity severity{MeshAuditSeverityTraits::Info};
    std::string code;
    std::string message;
    vmm::core::CellId cell_id{};
    vmm::core::FaceId face_id{};
    vmm::core::NodeId node_id{};
    vmm::core::SiteId site_id{};
    vmm::core::BoundaryPatchId patch_id{};
};

struct MeshAuditReport final {
    std::vector<MeshAuditIssue> issues;

    [[nodiscard]] bool valid() const noexcept
    {
        for(const auto& issue : issues) {
            if(MeshAuditSeverityTraits::invalidates_mesh(issue.severity)) {
                return false;
            }
        }
        return true;
    }

    [[nodiscard]] bool has_fatal_errors() const noexcept
    {
        for(const auto& issue : issues) {
            if(MeshAuditSeverityTraits::stops_audit(issue.severity)) {
                return true;
            }
        }
        return false;
    }

    [[nodiscard]] std::size_t issue_count() const noexcept { return issues.size(); }
    [[nodiscard]] bool empty() const noexcept { return issues.empty(); }
    [[nodiscard]] std::span<const MeshAuditIssue> all_issues() const noexcept { return issues; }

    void add(MeshAuditIssue issue) { issues.push_back(std::move(issue)); }

    void add(MeshAuditSeverity severity, std::string code, std::string message)
    {
        issues.push_back(MeshAuditIssue{severity, std::move(code), std::move(message)});
    }
};

[[nodiscard]] inline MeshAuditIssue make_cell_issue(MeshAuditSeverity severity,
                                                    std::string code,
                                                    std::string message,
                                                    vmm::core::CellId cell_id)
{
    MeshAuditIssue issue{severity, std::move(code), std::move(message)};
    issue.cell_id = cell_id;
    return issue;
}

[[nodiscard]] inline MeshAuditIssue make_face_issue(MeshAuditSeverity severity,
                                                    std::string code,
                                                    std::string message,
                                                    vmm::core::FaceId face_id)
{
    MeshAuditIssue issue{severity, std::move(code), std::move(message)};
    issue.face_id = face_id;
    return issue;
}

} // namespace vmm::audit
