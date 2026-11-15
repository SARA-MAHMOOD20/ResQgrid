// File:       tests/test_router.cpp
// Owner:      Rabia
// Purpose:    Tests for MinHeap, Environment (block/unblock) and Router
//             (Dijkstra costs, routes, nearest charger). Own main(), own target.
// Depends on: MinHeap, Environment, Router, ScenarioLoader (to load Scenario 01)
// Used by:    ctest / "before a PR" check
// Milestone:  M1 (MinHeap), M2 (Router)
#include <iostream>
#include "containers/MinHeap.h"
#include "core/Environment.h"
#include "core/Router.h"
#include "core/ScenarioLoader.h"
#include "core/World.h"

static int g_checks = 0, g_failed = 0, g_todo = 0;
#define CHECK(cond) do { ++g_checks; if (!(cond)) { ++g_failed; \
    cerr << "FAIL " << __FILE__ << ":" << __LINE__ << "  " #cond "\n"; } } while (0)
#define TODO_TEST(name) do { ++g_todo; cout << "  TODO  " << name << "\n"; } while (0)

static void testMinHeap() {
    // TODO(Rabia): push DistEntry values in random order; popMin returns them sorted;
    // popMin on empty returns false.
    // See: report Section 5.5   |   Done when: pops in sorted order
    TODO_TEST("MinHeap: pops in sorted order");
}

static void testEnvironmentBlock() {
    // TODO(Rabia): build a 3-location fake map by hand; block(c) -> isOpen(c) false seen
    // from BOTH ends' adjacency entries; block again -> false (redundant);
    // unblock -> networkVersion + 1.
    // See: report Section 5.4   |   Done when: blocking one road hides it both ways
    TODO_TEST("Environment: blocking one road hides it both ways");
}

static void testScenario01Costs() {
    // TODO(Rabia): load data/scenario_01_tiny_trace.txt and check the KEY
    // SHORTEST-PATH COSTS in its header: 1->5 = 8 (route 1-3-2-5), 7->6 = 6,
    // 8->6 = 3, 8->7 = 9, 7->5 = 4; after blocking connection 4: 2->5 = 9
    // (2-4-7-5), 5->4 = 6. Nearest open charger from 8 is 7.
    // See: report Section 6.1   |   Done when: Scenario 01 key costs all match
    TODO_TEST("Router: Scenario 01 key costs and routes match");
}

int main() {
    cout << "test_router\n";
    testMinHeap();
    testEnvironmentBlock();
    testScenario01Costs();
    cout << g_checks << " checks, " << g_failed << " failed, " << g_todo << " tests still TODO\n";
    return g_failed == 0 ? 0 : 1;
}
