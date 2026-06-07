#include <gtest/gtest.h>
#include <sstream>
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
    EXPECT_EQ(a.capacity(), 0u);
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
    a.reserve(8);
    a.reserve(4);
    EXPECT_EQ(a.capacity(), 8u);
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

TYPED_TEST(SmartArrayTest, InsertIntoEmpty) {
    SmartArray<TypeParam> a;
    a.insert(0u, val<TypeParam>(1));
    EXPECT_EQ(a.size(), 1u);
    EXPECT_EQ(a.at(0), val<TypeParam>(1));
}

TYPED_TEST(SmartArrayTest, EraseAtHead) {
    SmartArray<TypeParam> a;
    a.pushBack(val<TypeParam>(1));
    a.pushBack(val<TypeParam>(2));
    a.pushBack(val<TypeParam>(3));
    a.erase(0u);
    EXPECT_EQ(a.size(), 2u);
    EXPECT_EQ(a.at(0), val<TypeParam>(2));
    EXPECT_EQ(a.at(1), val<TypeParam>(3));
}

TYPED_TEST(SmartArrayTest, ReservePreservesElements) {
    SmartArray<TypeParam> a;
    a.pushBack(val<TypeParam>(42));
    a.reserve(32u);
    EXPECT_EQ(a.at(0), val<TypeParam>(42));
}

TYPED_TEST(SmartArrayTest, ResizeGrowTriggersGrow) {
    SmartArray<TypeParam> a(2u);
    a.pushBack(val<TypeParam>(1));
    a.pushBack(val<TypeParam>(2));
    a.resize(5u, val<TypeParam>(9));
    EXPECT_EQ(a.size(), 5u);
    EXPECT_GE(a.capacity(), 5u);
    EXPECT_EQ(a.at(0), val<TypeParam>(1));
    EXPECT_EQ(a.at(1), val<TypeParam>(2));
    EXPECT_EQ(a.at(2), val<TypeParam>(9));
    EXPECT_EQ(a.at(4), val<TypeParam>(9));
}

TYPED_TEST(SmartArrayTest, SizeConstructorZeroThenPushBack) {
    SmartArray<TypeParam> a(0u);
    EXPECT_EQ(a.capacity(), 0u);
    a.pushBack(val<TypeParam>(1));
    EXPECT_EQ(a.size(), 1u);
    EXPECT_GE(a.capacity(), 1u);
    EXPECT_EQ(a.at(0), val<TypeParam>(1));
}

TYPED_TEST(SmartArrayTest, EraseOnEmptyThrows) {
    SmartArray<TypeParam> a;
    EXPECT_THROW(a.erase(0u), std::out_of_range);
}

TYPED_TEST(SmartArrayTest, ResizeToZero) {
    SmartArray<TypeParam> a;
    a.pushBack(val<TypeParam>(1));
    a.pushBack(val<TypeParam>(2));
    a.resize(0u);
    EXPECT_EQ(a.size(), 0u);
    EXPECT_TRUE(a.isEmpty());
}

TYPED_TEST(SmartArrayTest, EraseRangeAll) {
    SmartArray<TypeParam> a;
    a.pushBack(val<TypeParam>(1));
    a.pushBack(val<TypeParam>(2));
    a.pushBack(val<TypeParam>(3));
    a.erase(0u, 3u);
    EXPECT_EQ(a.size(), 0u);
    EXPECT_TRUE(a.isEmpty());
}

TYPED_TEST(SmartArrayTest, EraseRangeOnEmptyThrows) {
    SmartArray<TypeParam> a;
    EXPECT_THROW(a.erase(0u, 1u), std::out_of_range);
}

TYPED_TEST(SmartArrayTest, InsertOutOfRangeOnEmptyThrows) {
    SmartArray<TypeParam> a;
    EXPECT_THROW(a.insert(1u, val<TypeParam>(1)), std::out_of_range);
}

// ── Fill constructor (size, elem) ─────────────────────────────────────────────

TYPED_TEST(SmartArrayTest, FillConstructorSetsAllElements) {
    SmartArray<TypeParam> a(3u, val<TypeParam>(7));
    EXPECT_EQ(a.size(), 3u);
    EXPECT_EQ(a.capacity(), 3u);
    EXPECT_EQ(a.at(0), val<TypeParam>(7));
    EXPECT_EQ(a.at(1), val<TypeParam>(7));
    EXPECT_EQ(a.at(2), val<TypeParam>(7));
}

TYPED_TEST(SmartArrayTest, FillConstructorZeroSize) {
    SmartArray<TypeParam> a(0u, val<TypeParam>(7));
    EXPECT_EQ(a.size(), 0u);
    EXPECT_TRUE(a.isEmpty());
}

TYPED_TEST(SmartArrayTest, FillConstructorCanPushBack) {
    SmartArray<TypeParam> a(2u, val<TypeParam>(1));
    a.pushBack(val<TypeParam>(2));
    EXPECT_EQ(a.size(), 3u);
    EXPECT_EQ(a.at(0), val<TypeParam>(1));
    EXPECT_EQ(a.at(2), val<TypeParam>(2));
}

// ── operator== ───────────────────────────────────────────────────────────────

TYPED_TEST(SmartArrayTest, EqualityBothEmpty) {
    SmartArray<TypeParam> a, b;
    EXPECT_TRUE(a == b);
}

TYPED_TEST(SmartArrayTest, EqualitySameElements) {
    SmartArray<TypeParam> a, b;
    a.pushBack(val<TypeParam>(1));
    a.pushBack(val<TypeParam>(2));
    b.pushBack(val<TypeParam>(1));
    b.pushBack(val<TypeParam>(2));
    EXPECT_TRUE(a == b);
}

TYPED_TEST(SmartArrayTest, EqualityDifferentSize) {
    SmartArray<TypeParam> a, b;
    a.pushBack(val<TypeParam>(1));
    EXPECT_FALSE(a == b);
}

TYPED_TEST(SmartArrayTest, EqualityDifferentElements) {
    SmartArray<TypeParam> a, b;
    a.pushBack(val<TypeParam>(1));
    b.pushBack(val<TypeParam>(2));
    EXPECT_FALSE(a == b);
}

TYPED_TEST(SmartArrayTest, EqualityFillConstructed) {
    SmartArray<TypeParam> a(3u, val<TypeParam>(5));
    SmartArray<TypeParam> b(3u, val<TypeParam>(5));
    EXPECT_TRUE(a == b);
}

TYPED_TEST(SmartArrayTest, EqualityDifferentCapacitySameContent) {
    SmartArray<TypeParam> a;       // cap=16
    SmartArray<TypeParam> b(32u);  // cap=32
    a.pushBack(val<TypeParam>(1));
    b.pushBack(val<TypeParam>(1));
    EXPECT_TRUE(a == b);
}

// ── shrinkToFit ───────────────────────────────────────────────────────────────

TYPED_TEST(SmartArrayTest, ShrinkToFitReducesCapacity) {
    SmartArray<TypeParam> a;
    a.pushBack(val<TypeParam>(1));
    a.pushBack(val<TypeParam>(2));
    a.pushBack(val<TypeParam>(3));
    EXPECT_GT(a.capacity(), 3u);
    a.shrinkToFit();
    EXPECT_EQ(a.capacity(), 3u);
    EXPECT_EQ(a.size(), 3u);
    EXPECT_EQ(a.at(0), val<TypeParam>(1));
    EXPECT_EQ(a.at(1), val<TypeParam>(2));
    EXPECT_EQ(a.at(2), val<TypeParam>(3));
}

TYPED_TEST(SmartArrayTest, ShrinkToFitWhenAlreadyFit) {
    SmartArray<TypeParam> a(2u);
    a.pushBack(val<TypeParam>(1));
    a.pushBack(val<TypeParam>(2));
    a.shrinkToFit();
    EXPECT_EQ(a.capacity(), 2u);
    EXPECT_EQ(a.size(), 2u);
}

TYPED_TEST(SmartArrayTest, ShrinkToFitEmptyThenPushBack) {
    SmartArray<TypeParam> a;
    a.shrinkToFit();
    EXPECT_EQ(a.capacity(), 0u);
    a.pushBack(val<TypeParam>(1));
    EXPECT_EQ(a.size(), 1u);
    EXPECT_GE(a.capacity(), 1u);
    EXPECT_EQ(a.at(0), val<TypeParam>(1));
}

// ── pushFront ─────────────────────────────────────────────────────────────────

TYPED_TEST(SmartArrayTest, PushFrontInsertsAtBeginning) {
    SmartArray<TypeParam> a;
    a.pushBack(val<TypeParam>(2));
    a.pushBack(val<TypeParam>(3));
    a.pushFront(val<TypeParam>(1));
    EXPECT_EQ(a.size(), 3u);
    EXPECT_EQ(a.at(0), val<TypeParam>(1));
    EXPECT_EQ(a.at(1), val<TypeParam>(2));
    EXPECT_EQ(a.at(2), val<TypeParam>(3));
}

TYPED_TEST(SmartArrayTest, PushFrontOnEmpty) {
    SmartArray<TypeParam> a;
    a.pushFront(val<TypeParam>(1));
    EXPECT_EQ(a.size(), 1u);
    EXPECT_EQ(a.at(0), val<TypeParam>(1));
}

TYPED_TEST(SmartArrayTest, PushFrontGrowsCapacity) {
    SmartArray<TypeParam> a(2u);
    a.pushBack(val<TypeParam>(1));
    a.pushBack(val<TypeParam>(2));
    a.pushFront(val<TypeParam>(0));
    EXPECT_EQ(a.size(), 3u);
    EXPECT_GT(a.capacity(), 2u);
    EXPECT_EQ(a.at(0), val<TypeParam>(0));
    EXPECT_EQ(a.at(1), val<TypeParam>(1));
    EXPECT_EQ(a.at(2), val<TypeParam>(2));
}

// ── popFront ──────────────────────────────────────────────────────────────────

TYPED_TEST(SmartArrayTest, PopFrontRemovesFirstElement) {
    SmartArray<TypeParam> a;
    a.pushBack(val<TypeParam>(1));
    a.pushBack(val<TypeParam>(2));
    a.pushBack(val<TypeParam>(3));
    a.popFront();
    EXPECT_EQ(a.size(), 2u);
    EXPECT_EQ(a.at(0), val<TypeParam>(2));
    EXPECT_EQ(a.at(1), val<TypeParam>(3));
}

TYPED_TEST(SmartArrayTest, PopFrontOnEmptyThrows) {
    SmartArray<TypeParam> a;
    EXPECT_THROW(a.popFront(), std::out_of_range);
}

TYPED_TEST(SmartArrayTest, PopFrontUntilEmpty) {
    SmartArray<TypeParam> a;
    a.pushBack(val<TypeParam>(1));
    a.pushBack(val<TypeParam>(2));
    a.popFront();
    a.popFront();
    EXPECT_TRUE(a.isEmpty());
    EXPECT_THROW(a.popFront(), std::out_of_range);
}

// ── operator<< ───────────────────────────────────────────────────────────────

TYPED_TEST(SmartArrayTest, StreamOutputEmpty) {
    SmartArray<TypeParam> a;
    std::ostringstream oss;
    oss << a;
    EXPECT_EQ(oss.str(), "[]");
}

TYPED_TEST(SmartArrayTest, StreamOutputSingleElement) {
    SmartArray<TypeParam> a;
    a.pushBack(val<TypeParam>(1));
    std::ostringstream oss;
    oss << a;
    EXPECT_EQ(oss.str(), "[1]");
}

TYPED_TEST(SmartArrayTest, StreamOutputMultipleElements) {
    SmartArray<TypeParam> a;
    a.pushBack(val<TypeParam>(1));
    a.pushBack(val<TypeParam>(2));
    a.pushBack(val<TypeParam>(3));
    std::ostringstream oss;
    oss << a;
    EXPECT_EQ(oss.str(), "[1, 2, 3]");
}

// ── Double-destruction regression ─────────────────────────────────────────────
// erase/popBack call ar[i].~T() explicitly on the vacated slot. If grow() fires
// before that slot is refilled, delete[] ar destructs it a second time — UB for
// non-trivial types like std::string.
// Scenario: fill cap=2 → erase → resize past cap (grow fires immediately,
// destructed slot 1 is never reassigned before delete[] ar).

TEST(SmartArrayStringTest, PopBackBeforeGrowDoesNotDoubleDestruct) {
    // Strings must exceed SSO threshold (~15 chars) so their data is heap-allocated
    // and ASAN can detect the double-free on delete[] ar.
    // popBack destructs ar[len-1] while it still owns heap data (unlike erase,
    // which move-assigns out of the slot first). grow() then calls delete[] ar,
    // which destructs that slot again — freeing already-freed memory.
    const std::string X(32, 'x');
    const std::string Y(32, 'y');
    const std::string Z(32, 'z');
    SmartArray<std::string> a(2u);
    a.pushBack(X);
    a.pushBack(Y);
    a.popBack();          // destructs ar[1] (live heap string); len=1, cap=2
    a.resize(5u, Z);      // size=5 > cap=2: grow fires, delete[] ar destructs slot 1 again
    EXPECT_EQ(a.size(), 5u);
    EXPECT_EQ(a.at(0), X);
    EXPECT_EQ(a.at(1), Z);
    EXPECT_EQ(a.at(4), Z);
}
