// File:       tests/test_coordinator.cpp
// Owner:      Sara
// Purpose:    Tests for IndexedMaxHeap and the Coordinator (priority order,
//             tie-breaks, battery rejections), plus a whole-run check on
//             Scenario 01. Own main(), own target.
// Depends on: IndexedMaxHeap, Coordinator, Simulation, ScenarioLoader
// Used by:    ctest / "before a PR" check
// Milestone:  M1 (heap), M3 (Coordinator, Scenario 01 / 03)
#include <iostream>
#include "containers/IndexedMaxHeap.h"
#include "core/Coordinator.h"
#include "core/Simulation.h"

static int g_checks = 0, g_failed = 0, g_todo = 0;
#define CHECK(cond) do { ++g_checks; if (!(cond)) { ++g_failed; \
    cerr << "FAIL " << __FILE__ << ":" << __LINE__ << "  " #cond "\n"; } } while (0)
#define TODO_TEST(name) do { ++g_todo; cout << "  TODO  " << name << "\n"; } while (0)

static void testIndexedMaxHeap() {
    // TODO(Sara): init(10); insert items with different keys -> extractMax in key order;
    // equal keys -> smaller tieId first; changeKey raises a low item to the top
    // (SET_URGENCY); contains() true/false; heap[pos[x]] == x after every operation.
    // See: report Section 5.6   |   Done when: test SET_URGENCY moves a request up
    TODO_TEST("IndexedMaxHeap: order, ties, changeKey moves a request up");
}

static void testScenario03Assignment() {
    // TODO(Sara): load data/scenario_03_ties_and_conflicts.txt, run step 0, compare the
    // ASSIGN order and chosen robots with the EXPECTED OUTCOME in the file header
    // (specialist before GENERAL, higher battery, smaller ID; requests 11-15 out of ID order).
    // See: report Section 6.2   |   Done when: Scenario 03 t=0 assignment order matches
    TODO_TEST("Coordinator: Scenario 03 t=0 assignment order and tie-breaks");
}

static void testScenario01Run() {
    // TODO(Sara): run data/scenario_01_tiny_trace.txt to the end: ends at t=28;
    // 4 completed, 0 failed, 1 reroute, 2 battery rejections; response times
    // 16, 7, 26, 15 for requests 1-4 (Report Appendix A).
    // See: report Appendix A   |   Done when: Scenario 01 ends at t=28 and matches the hand trace
    TODO_TEST("Simulation: Scenario 01 matches the hand trace");
}

int main() {
    cout << "test_coordinator\n";
    testIndexedMaxHeap();
    testScenario03Assignment();
    testScenario01Run();
    cout << g_checks << " checks, " << g_failed << " failed, " << g_todo << " tests still TODO\n";
    return g_failed == 0 ? 0 : 1;
}
