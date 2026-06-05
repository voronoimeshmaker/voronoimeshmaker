//==============================================================================
// File        : StrongIds.hpp
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Strong finite-volume entity identifiers with invalid sentinels.
//==============================================================================
#pragma once

#include <compare>
#include <cstdint>
#include <limits>
#include <type_traits>

namespace vmm::core {

using EntityIdValue = std::uint64_t;
inline constexpr EntityIdValue invalid_entity_id_value = std::numeric_limits<EntityIdValue>::max();

template<class Tag>
struct StrongId final {
    EntityIdValue value{invalid_entity_id_value};

    constexpr StrongId() noexcept = default;
    explicit constexpr StrongId(EntityIdValue value_) noexcept : value(value_) {}

    [[nodiscard]] static constexpr StrongId invalid() noexcept { return StrongId{}; }
    [[nodiscard]] friend constexpr bool operator==(StrongId lhs, StrongId rhs) noexcept = default;
    [[nodiscard]] friend constexpr auto operator<=>(StrongId lhs, StrongId rhs) noexcept = default;
};

struct CellIdTag final {};
struct FaceIdTag final {};
struct NodeIdTag final {};
struct SiteIdTag final {};
struct BoundaryPatchIdTag final {};

using CellId = StrongId<CellIdTag>;
using FaceId = StrongId<FaceIdTag>;
using NodeId = StrongId<NodeIdTag>;
using SiteId = StrongId<SiteIdTag>;
using BoundaryPatchId = StrongId<BoundaryPatchIdTag>;

template<class Tag>
[[nodiscard]] constexpr bool is_valid(StrongId<Tag> id) noexcept
{
    return id.value != invalid_entity_id_value;
}

template<class Tag>
[[nodiscard]] constexpr EntityIdValue to_index(StrongId<Tag> id) noexcept
{
    return id.value;
}

static_assert(std::is_standard_layout_v<CellId>, "CellId must preserve standard-layout storage");
static_assert(std::is_trivially_copyable_v<CellId>, "CellId must be trivially copyable");
static_assert(!std::is_convertible_v<CellId, EntityIdValue>, "Strong IDs must not convert implicitly");

} // namespace vmm::core
