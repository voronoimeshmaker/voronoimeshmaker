#include <vmm/core/StrongIds.hpp>

#include <gtest/gtest.h>

#include <type_traits>

using namespace vmm::core;

TEST(StrongIdsTest, DefaultConstructedIdsAreInvalid)
{
    EXPECT_FALSE(is_valid(CellId{}));
    EXPECT_FALSE(is_valid(FaceId{}));
    EXPECT_FALSE(is_valid(NodeId{}));
    EXPECT_FALSE(is_valid(SiteId{}));
    EXPECT_FALSE(is_valid(BoundaryPatchId{}));
}

TEST(StrongIdsTest, ExplicitIdsAreValidAndExposeValueOnlyExplicitly)
{
    constexpr CellId cell{3U};
    constexpr FaceId face{7U};

    static_assert(is_valid(cell));
    static_assert(is_valid(face));
    static_assert(to_index(cell) == 3U);
    static_assert(to_index(face) == 7U);

    EXPECT_TRUE(is_valid(cell));
    EXPECT_EQ(cell.value, 3U);
    EXPECT_EQ(face.value, 7U);
}

TEST(StrongIdsTest, IdTypesDoNotConvertToIntegerOrEachOther)
{
    static_assert(!std::is_convertible_v<CellId, EntityIdValue>);
    static_assert(!std::is_convertible_v<FaceId, EntityIdValue>);
    static_assert(!std::is_constructible_v<CellId, FaceId>);
    static_assert(!std::is_constructible_v<FaceId, CellId>);

    SUCCEED();
}

TEST(StrongIdsTest, SameEntityIdsAreComparable)
{
    EXPECT_EQ(CellId{4U}, CellId{4U});
    EXPECT_NE(CellId{4U}, CellId{5U});
    EXPECT_LT(CellId{4U}, CellId{5U});
}
