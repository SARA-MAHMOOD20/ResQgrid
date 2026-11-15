// File:       tests/test_containers.cpp
// Owner:      Farwa
// Purpose:    Unit tests for Farwa's containers: DynamicArray, LinkedList,
//             Queue, MergeSort (+ Types.h round-trips). Own main(), own CMake target.
// Depends on: DynamicArray, LinkedList, Queue, MergeSort, Types
// Used by:    ctest / "before a PR" check
// Milestone:  M1 (all container tests pass)
//
// Pattern: replace TODO_TEST(...) with real CHECK(...) lines. The program
// exits 1 if any CHECK fails, so ctest goes red. Run under valgrind or with
// -fsanitize=address to catch leaks ("no leaks" checks in the plan).
#include <iostream>
#include "containers/DynamicArray.h"
#include "containers/LinkedList.h"
#include "containers/Queue.h"
#include "containers/MergeSort.h"
#include "core/Types.h"

static int g_checks = 0, g_failed = 0, g_todo = 0;
#define CHECK(cond) do { ++g_checks; if (!(cond)) { ++g_failed; \
    cerr << "FAIL " << __FILE__ << ":" << __LINE__ << "  " #cond "\n"; } } while (0)
#define TODO_TEST(name) do { ++g_todo; cout << "  TODO  " << name << "\n"; } while (0)

static void testDynamicArray() {
    // TODO(Farwa): push_back 10,000 ints and read them back; size() == 10000;
    // copy constructor and assignment are deep copies; reserve; assign; pop_back; clear.
    // See: report Section 5.2   |   Done when: test pushes 10,000 ints and reads them back
    TODO_TEST("DynamicArray: 10,000 push_back + read back, deep copy");
}

static void testLinkedList() {
    // TODO(Farwa): pushBack 1,2,3 / pushFront 0 -> iterate gives 0,1,2,3; popFront order;
    // popFront on empty returns false; copy is deep; no leaks.
    // See: plan Section 6   |   Done when: test push, pop, iterate, no leaks
    TODO_TEST("LinkedList: push, pop, iterate, no leaks");
}

static void testQueue() {
    // TODO(Farwa): enqueue 1..100, dequeue gives 1..100; front() does not remove; clear().
    // See: report Section 5.8   |   Done when: test FIFO order kept
    TODO_TEST("Queue: FIFO order kept");
}

static void testMergeSort() {
    // TODO(Farwa): sort records {key, originalPos} by key only; equal keys must
    // keep increasing originalPos; also empty array and one element.
    // See: report Section 5.7   |   Done when: test equal keys keep their original order
    TODO_TEST("MergeSort: stable (equal keys keep original order)");
}

static void testTypesRoundTrip() {
    // TODO(Farwa): for every value of every enum, fromString(toString(v), out) && out == v;
    // fromString("HOUSE", LocationType&) and fromString("GENERAL", Category&) are false.
    // See: report Section 9.2   |   Done when: every enum round-trips
    TODO_TEST("Types.h: every enum round-trips");
}

int main() {
    cout << "test_containers\n";
    testDynamicArray();
    testLinkedList();
    testQueue();
    testMergeSort();
    testTypesRoundTrip();
    cout << g_checks << " checks, " << g_failed << " failed, " << g_todo << " tests still TODO\n";
    return g_failed == 0 ? 0 : 1;
}
