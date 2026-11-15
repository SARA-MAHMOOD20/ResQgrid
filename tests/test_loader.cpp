// File:       tests/test_loader.cpp
// Owner:      Rabia
// Purpose:    Tests for HashMap and ScenarioLoader (tokenizer, parseInt, the
//             three invalid files, the valid scenarios). Own main(), own target.
// Depends on: HashMap, ScenarioLoader, World
// Used by:    ctest / "before a PR" check
// Milestone:  M1 (HashMap), M2 (loader)
//
// RESQGRID_DATA_DIR is set by CMake to <repo>/data, so tests find the files
// whatever folder they run from.
#include <iostream>
#include "containers/HashMap.h"
#include "core/ScenarioLoader.h"
#include "core/World.h"

static int g_checks = 0, g_failed = 0, g_todo = 0;
#define CHECK(cond) do { ++g_checks; if (!(cond)) { ++g_failed; \
    cerr << "FAIL " << __FILE__ << ":" << __LINE__ << "  " #cond "\n"; } } while (0)
#define TODO_TEST(name) do { ++g_todo; cout << "  TODO  " << name << "\n"; } while (0)

static Str dataFile(const Str& name) { return Str(RESQGRID_DATA_DIR) + "/" + name; }

static void testHashMap() {
    // TODO(Rabia): init(100); insert 1..100 -> all true; insert 50 again -> false (duplicate);
    // find every key with the right value; find(1000) false; keys 1, 7, 1000 (non-consecutive).
    // See: report Section 5.3   |   Done when: duplicate insert detected
    TODO_TEST("HashMap: insert/find, duplicate insert detected");
}

static void testTokenizeAndParseInt() {
    // TODO(Rabia): tokenize("  1   Junction_1 JUNCTION ") == 3 tokens; parseInt("42"),
    // parseInt("-3") ok; parseInt("4x"), parseInt(""), parseInt("3.5") rejected.
    // See: report Section 9.4   |   Done when: all cases behave as listed
    TODO_TEST("tokenize + parseInt");
}

static void testInvalidFiles() {
    // TODO(Rabia): load each file in data/invalid/ -> false, and the error lines
    // match the ones planted in the file's header comment:
    //   invalid_01_count_mismatch, invalid_02_bad_values_and_references
    //   (18 errors + E18 as a warning, Report 9.4), invalid_03_broken_structure.
    // See: report Section 9.4   |   Done when: all 3 invalid files give exactly their planted errors
    (void)dataFile;
    TODO_TEST("invalid_01..03 rejected with exactly their planted errors");
}

static void testValidFiles() {
    // TODO(Rabia): scenario_provided (56/117/12/60/20) and scenario_01..05 load with
    // 0 errors and the right counts; events sorted by time with ties in file order
    // (scenario_04 lists events out of order); requests sorted by report time.
    // NOTE: scenario_04 needs the fix in README "Known issues" first.
    // See: report Sections 5.7 and 9   |   Done when: every valid file loads with the right counts
    TODO_TEST("valid scenarios load; events/requests stable-sorted");
}

int main() {
    cout << "test_loader\n";
    testHashMap();
    testTokenizeAndParseInt();
    testInvalidFiles();
    testValidFiles();
    cout << g_checks << " checks, " << g_failed << " failed, " << g_todo << " tests still TODO\n";
    return g_failed == 0 ? 0 : 1;
}
