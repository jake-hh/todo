#include <gtest/gtest.h>
#include "../src/smartArray/SmartArray.h"

// Helper to produce typed test values from an int seed
template <typename T> T val(int n);
template <> int val<int>(int n) { return n; }
template <> std::string val<std::string>(int n) { return std::to_string(n); }

using TestTypes = ::testing::Types<int, std::string>;

template <typename T>
class SmartArrayTest : public ::testing::Test {};
TYPED_TEST_SUITE(SmartArrayTest, TestTypes);

// ── Step 1: Constructors ──────────────────────────────────────────────────────

TYPED_TEST(SmartArrayTest, DefaultConstructor) {
    SmartArray<TypeParam> a;
    EXPECT_EQ(a.size(), 0u);
    EXPECT_EQ(a.capacity(), 16u);
    EXPECT_TRUE(a.isEmpty());
}

TYPED_TEST(SmartArrayTest, SizeConstructor) {
    SmartArray<TypeParam> a(8u);
    EXPECT_EQ(a.size(), 0u);
    EXPECT_EQ(a.capacity(), 8u);
    EXPECT_TRUE(a.isEmpty());
}

// ── Step 2: pushBack / at / operator[] ───────────────────────────────────────

TYPED_TEST(SmartArrayTest, PushBackAtOperatorRoundTrip) {
    SmartArray<TypeParam> a;
    a.pushBack(val<TypeParam>(10));
    a.pushBack(val<TypeParam>(20));
    a.pushBack(val<TypeParam>(30));

    EXPECT_EQ(a.size(), 3u);
    EXPECT_FALSE(a.isEmpty());
    EXPECT_EQ(a.at(0), val<TypeParam>(10));
    EXPECT_EQ(a.at(1), val<TypeParam>(20));
    EXPECT_EQ(a.at(2), val<TypeParam>(30));
    EXPECT_EQ(a[0], val<TypeParam>(10));
    EXPECT_EQ(a[1], val<TypeParam>(20));
    EXPECT_EQ(a[2], val<TypeParam>(30));
}

// ── Step 7: insert ───────────────────────────────────────────────────────────

TYPED_TEST(SmartArrayTest, InsertAtHead) {
    SmartArray<TypeParam> a;
    a.pushBack(val<TypeParam>(2));
    a.pushBack(val<TypeParam>(3));
    a.insert(0u, val<TypeParam>(1));
    EXPECT_EQ(a.size(), 3u);
    EXPECT_EQ(a.at(0), val<TypeParam>(1));
    EXPECT_EQ(a.at(1), val<TypeParam>(2));
    EXPECT_EQ(a.at(2), val<TypeParam>(3));
}

TYPED_TEST(SmartArrayTest, InsertAtMiddle) {
    SmartArray<TypeParam> a;
    a.pushBack(val<TypeParam>(1));
    a.pushBack(val<TypeParam>(3));
    a.insert(1u, val<TypeParam>(2));
    EXPECT_EQ(a.size(), 3u);
    EXPECT_EQ(a.at(0), val<TypeParam>(1));
    EXPECT_EQ(a.at(1), val<TypeParam>(2));
    EXPECT_EQ(a.at(2), val<TypeParam>(3));
}

TYPED_TEST(SmartArrayTest, InsertAtTail) {
    SmartArray<TypeParam> a;
    a.pushBack(val<TypeParam>(1));
    a.pushBack(val<TypeParam>(2));
    a.insert(2u, val<TypeParam>(3));
    EXPECT_EQ(a.size(), 3u);
    EXPECT_EQ(a.at(2), val<TypeParam>(3));
}

TYPED_TEST(SmartArrayTest, InsertOutOfRangeThrows) {
    SmartArray<TypeParam> a;
    a.pushBack(val<TypeParam>(1));
    EXPECT_THROW(a.insert(5u, val<TypeParam>(0)), std::out_of_range);
}

// ── Step 6: clear ────────────────────────────────────────────────────────────

TYPED_TEST(SmartArrayTest, ClearResetsSize) {
    SmartArray<TypeParam> a;
    a.pushBack(val<TypeParam>(1));
    a.pushBack(val<TypeParam>(2));
    unsigned capBefore = a.capacity();
    a.clear();
    EXPECT_EQ(a.size(), 0u);
    EXPECT_EQ(a.capacity(), capBefore);
    EXPECT_TRUE(a.isEmpty());
}

// ── Step 5: resize ───────────────────────────────────────────────────────────

TYPED_TEST(SmartArrayTest, ResizeShrink) {
    SmartArray<TypeParam> a;
    a.pushBack(val<TypeParam>(1));
    a.pushBack(val<TypeParam>(2));
    a.pushBack(val<TypeParam>(3));
    unsigned capBefore = a.capacity();
    a.resize(1u);
    EXPECT_EQ(a.size(), 1u);
    EXPECT_EQ(a.capacity(), capBefore);
    EXPECT_EQ(a.at(0), val<TypeParam>(1));
}

TYPED_TEST(SmartArrayTest, ResizeGrowDefaultConstructed) {
    SmartArray<TypeParam> a;
    a.pushBack(val<TypeParam>(5));
    a.resize(3u);
    EXPECT_EQ(a.size(), 3u);
    EXPECT_EQ(a.at(1), TypeParam{});
    EXPECT_EQ(a.at(2), TypeParam{});
}

TYPED_TEST(SmartArrayTest, ResizeGrowWithElem) {
    SmartArray<TypeParam> a;
    a.pushBack(val<TypeParam>(5));
    a.resize(3u, val<TypeParam>(9));
    EXPECT_EQ(a.size(), 3u);
    EXPECT_EQ(a.at(1), val<TypeParam>(9));
    EXPECT_EQ(a.at(2), val<TypeParam>(9));
}

// ── Step 4: reserve ──────────────────────────────────────────────────────────

TYPED_TEST(SmartArrayTest, ReserveIncreasesCapacity) {
    SmartArray<TypeParam> a;
    a.pushBack(val<TypeParam>(1));
    a.reserve(32u);
    EXPECT_GE(a.capacity(), 32u);
    EXPECT_EQ(a.size(), 1u);
}

TYPED_TEST(SmartArrayTest, ReserveNoOpWhenSmaller) {
    SmartArray<TypeParam> a;
    unsigned capBefore = a.capacity();
    a.reserve(capBefore - 1);
    EXPECT_EQ(a.capacity(), capBefore);
    EXPECT_EQ(a.size(), 0u);
}

TYPED_TEST(SmartArrayTest, ReserveNoOpWhenEqual) {
    SmartArray<TypeParam> a;
    unsigned capBefore = a.capacity();
    a.reserve(capBefore);
    EXPECT_EQ(a.capacity(), capBefore);
}

// ── Step 8: erase ────────────────────────────────────────────────────────────

TYPED_TEST(SmartArrayTest, EraseSingleShifts) {
    SmartArray<TypeParam> a;
    a.pushBack(val<TypeParam>(1));
    a.pushBack(val<TypeParam>(2));
    a.pushBack(val<TypeParam>(3));
    a.erase(1u);
    EXPECT_EQ(a.size(), 2u);
    EXPECT_EQ(a.at(0), val<TypeParam>(1));
    EXPECT_EQ(a.at(1), val<TypeParam>(3));
}

TYPED_TEST(SmartArrayTest, EraseSingleOutOfRangeThrows) {
    SmartArray<TypeParam> a;
    a.pushBack(val<TypeParam>(1));
    EXPECT_THROW(a.erase(5u), std::out_of_range);
}

TYPED_TEST(SmartArrayTest, EraseRangeShifts) {
    SmartArray<TypeParam> a;
    a.pushBack(val<TypeParam>(1));
    a.pushBack(val<TypeParam>(2));
    a.pushBack(val<TypeParam>(3));
    a.pushBack(val<TypeParam>(4));
    a.erase(1u, 2u);
    EXPECT_EQ(a.size(), 2u);
    EXPECT_EQ(a.at(0), val<TypeParam>(1));
    EXPECT_EQ(a.at(1), val<TypeParam>(4));
}

TYPED_TEST(SmartArrayTest, EraseRangeOutOfRangeThrows) {
    SmartArray<TypeParam> a;
    a.pushBack(val<TypeParam>(1));
    a.pushBack(val<TypeParam>(2));
    EXPECT_THROW(a.erase(1u, 5u), std::out_of_range);
}

// ── Step 9: popBack / at out-of-range ────────────────────────────────────────

TYPED_TEST(SmartArrayTest, PopBackDecreasesSize) {
    SmartArray<TypeParam> a;
    a.pushBack(val<TypeParam>(1));
    a.pushBack(val<TypeParam>(2));
    unsigned capBefore = a.capacity();
    a.popBack();
    EXPECT_EQ(a.size(), 1u);
    EXPECT_EQ(a.capacity(), capBefore);
}

TYPED_TEST(SmartArrayTest, PopBackOnEmptyThrows) {
    SmartArray<TypeParam> a;
    EXPECT_THROW(a.popBack(), std::out_of_range);
}

TYPED_TEST(SmartArrayTest, AtOutOfRangeThrows) {
    SmartArray<TypeParam> a;
    a.pushBack(val<TypeParam>(1));
    EXPECT_THROW(a.at(5u), std::out_of_range);
}

// ── Step 3: pushBack past capacity ───────────────────────────────────────────

TYPED_TEST(SmartArrayTest, PushBackGrowsCapacity) {
    SmartArray<TypeParam> a(2u);
    a.pushBack(val<TypeParam>(1));
    a.pushBack(val<TypeParam>(2));
    a.pushBack(val<TypeParam>(3)); // triggers grow
    EXPECT_EQ(a.size(), 3u);
    EXPECT_GT(a.capacity(), 2u);
    EXPECT_EQ(a.at(0), val<TypeParam>(1));
    EXPECT_EQ(a.at(1), val<TypeParam>(2));
    EXPECT_EQ(a.at(2), val<TypeParam>(3));
}

TYPED_TEST(SmartArrayTest, InsertGrowsCapacity) {
    SmartArray<TypeParam> a(2u);
    a.pushBack(val<TypeParam>(1));
    a.pushBack(val<TypeParam>(2));
    a.insert(1u, val<TypeParam>(9)); // triggers grow
    EXPECT_EQ(a.size(), 3u);
    EXPECT_GT(a.capacity(), 2u);
    EXPECT_EQ(a.at(0), val<TypeParam>(1));
    EXPECT_EQ(a.at(1), val<TypeParam>(9));
    EXPECT_EQ(a.at(2), val<TypeParam>(2));
}

// ── Extra edge cases ──────────────────────────────────────────────────────────

TYPED_TEST(SmartArrayTest, EraseLastElement) {
    SmartArray<TypeParam> a;
    a.pushBack(val<TypeParam>(1));
    a.pushBack(val<TypeParam>(2));
    a.erase(1u);
    EXPECT_EQ(a.size(), 1u);
    EXPECT_EQ(a.at(0), val<TypeParam>(1));
}

TYPED_TEST(SmartArrayTest, ClearThenReuse) {
    SmartArray<TypeParam> a;
    a.pushBack(val<TypeParam>(1));
    a.pushBack(val<TypeParam>(2));
    a.clear();
    a.pushBack(val<TypeParam>(3));
    EXPECT_EQ(a.size(), 1u);
    EXPECT_EQ(a.at(0), val<TypeParam>(3));
}

TYPED_TEST(SmartArrayTest, ResizeSameSize) {
    SmartArray<TypeParam> a;
    a.pushBack(val<TypeParam>(1));
    a.pushBack(val<TypeParam>(2));
    unsigned capBefore = a.capacity();
    a.resize(2u);
    EXPECT_EQ(a.size(), 2u);
    EXPECT_EQ(a.capacity(), capBefore);
    EXPECT_EQ(a.at(0), val<TypeParam>(1));
    EXPECT_EQ(a.at(1), val<TypeParam>(2));
}

TYPED_TEST(SmartArrayTest, EraseZeroCount) {
    SmartArray<TypeParam> a;
    a.pushBack(val<TypeParam>(1));
    a.pushBack(val<TypeParam>(2));
    a.erase(0u, 0u);
    EXPECT_EQ(a.size(), 2u);
    EXPECT_EQ(a.at(0), val<TypeParam>(1));
    EXPECT_EQ(a.at(1), val<TypeParam>(2));
}
