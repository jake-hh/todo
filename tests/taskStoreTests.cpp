#include <gtest/gtest.h>
#include "../src/app/data/TaskStore.h"

// Helper: create a minimal task in the store
static unsigned makeTask(TaskStore& store, const std::string& title = "task") {
    return store.create(title, "", 0, 0, -1);
}

// ── create ────────────────────────────────────────────────────────────────────

TEST(TaskStoreTest, CreateAssignsSequentialIds) {
    TaskStore store;
    unsigned a = makeTask(store, "a");
    unsigned b = makeTask(store, "b");
    unsigned c = makeTask(store, "c");
    EXPECT_EQ(a, 0u);
    EXPECT_EQ(b, 1u);
    EXPECT_EQ(c, 2u);
}

TEST(TaskStoreTest, CreateStoresAllFields) {
    TaskStore store;
    unsigned id = store.create("title", "desc", 2, 1, 9999);
    const Task& t = store.get(id);
    EXPECT_EQ(t.title, "title");
    EXPECT_EQ(t.description, "desc");
    EXPECT_EQ(t.priority, 2);
    EXPECT_EQ(t.status, 1);
    EXPECT_EQ(t.dueDate, 9999);
    EXPECT_GT(t.createdAt, 0);
}

TEST(TaskStoreTest, SizeIncreasesOnCreate) {
    TaskStore store;
    EXPECT_EQ(store.size(), 0u);
    makeTask(store);
    EXPECT_EQ(store.size(), 1u);
    makeTask(store);
    EXPECT_EQ(store.size(), 2u);
}

// ── get ───────────────────────────────────────────────────────────────────────

TEST(TaskStoreTest, GetReturnsCorrectTask) {
    TaskStore store;
    unsigned id = makeTask(store, "hello");
    EXPECT_EQ(store.get(id).title, "hello");
}

TEST(TaskStoreTest, GetThrowsForMissingId) {
    TaskStore store;
    EXPECT_THROW(store.get(99), std::out_of_range);
}

TEST(TaskStoreTest, GetConstOverload) {
    TaskStore store;
    unsigned id = makeTask(store, "const");
    const TaskStore& cstore = store;
    EXPECT_EQ(cstore.get(id).title, "const");
}

// ── update ────────────────────────────────────────────────────────────────────

TEST(TaskStoreTest, UpdateModifiesFields) {
    TaskStore store;
    unsigned id = makeTask(store, "old");
    Task t = store.get(id);
    t.title = "new";
    t.priority = 3;
    store.update(id, t);
    EXPECT_EQ(store.get(id).title, "new");
    EXPECT_EQ(store.get(id).priority, 3);
}

TEST(TaskStoreTest, UpdatePreservesId) {
    TaskStore store;
    unsigned id = makeTask(store);
    Task t = store.get(id);
    t.id = 999; // should be ignored
    store.update(id, t);
    EXPECT_EQ(store.get(id).id, id);
}

TEST(TaskStoreTest, UpdateThrowsForMissingId) {
    TaskStore store;
    Task t{};
    EXPECT_THROW(store.update(99, t), std::out_of_range);
}

// ── remove ────────────────────────────────────────────────────────────────────

TEST(TaskStoreTest, RemoveDeletesTask) {
    TaskStore store;
    unsigned id = makeTask(store);
    store.remove(id);
    EXPECT_EQ(store.size(), 0u);
    EXPECT_THROW(store.get(id), std::out_of_range);
}

TEST(TaskStoreTest, RemoveThrowsForMissingId) {
    TaskStore store;
    EXPECT_THROW(store.remove(99), std::out_of_range);
}

// ── nextId ────────────────────────────────────────────────────────────────────

TEST(TaskStoreTest, NextIdAlwaysExceedsAllIds) {
    TaskStore store;
    unsigned a = makeTask(store);
    unsigned b = makeTask(store);
    unsigned c = makeTask(store);
    store.remove(b);
    // next id should be > max remaining id
    unsigned next = makeTask(store);
    EXPECT_GT(next, a);
    EXPECT_GT(next, c);
}

TEST(TaskStoreTest, RecalcNextIdAfterLoad) {
    TaskStore store;
    makeTask(store); // id 0
    makeTask(store); // id 1
    makeTask(store); // id 2
    store.recalcNextId();
    unsigned next = makeTask(store);
    EXPECT_EQ(next, 3u);
}

TEST(TaskStoreTest, RecalcNextIdOnEmptyStore) {
    TaskStore store;
    store.recalcNextId();
    unsigned next = makeTask(store);
    EXPECT_EQ(next, 0u);
}
