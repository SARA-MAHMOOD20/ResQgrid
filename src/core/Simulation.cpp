// File:       src/core/Simulation.cpp
// Owner:      Sara
// Purpose:    The 7-phase step loop and termination rule.
// Depends on: Simulation.h (all core modules, Logger, Statistics), <chrono>
// Used by:    main.cpp, UIApp
// Milestone:  M3
#include "Simulation.h"
#include <chrono>

Simulation::Simulation()
    : router(world.env),
      coord(world, router, logger),
      events(world, coord.pending(), logger),
      rerouter(world, router, coord.pending(), logger),
      movement(world, router, logger) {}

bool Simulation::load(const Str& path) {
    // TODO(Sara): see Simulation.h (also reset stats and finished).
    // See: report Section 9.4   |   Done when: invalid files return false; provided file returns true
    finished = false;
    world.currentTime = 0;
    return loader.load(path, world);
}

bool Simulation::step() {
    // TODO(Sara): phases 1-7 in order (see Simulation.h), then termination check.
    // See: report Sections 4.2 and 4.8   |   Done when: Scenario 01 ends at t=28
    if (finished) {
        return false;
    }
    finish();   // stub: no phases yet, so end immediately
    return true;
}

void Simulation::runAll() {
    // TODO(Sara): measure with chrono::steady_clock around the step() loop; store runTimeMs
    // BEFORE finish() runs so stats get it (or pass it in).
    // See: report Section 4.8   |   Done when: Scenario 01 ends at t=28
    auto start = chrono::steady_clock::now();
    while (step()) {
    }
    auto end = chrono::steady_clock::now();
    runTimeMs = chrono::duration<double, milli>(end - start).count();
}

void Simulation::releaseNewRequests(int t) {
    // TODO(Sara): see Simulation.h.
    // See: report Section 4.2 (phase 2)   |   Done when: same-time requests enter in file order
    (void)t;
}

bool Simulation::allDone() const {
    // TODO(Sara): every request finished && nextRequest == requests.size() && nextEvent == events.size().
    // See: report Section 4.8   |   Done when: Scenario 01 stops at t=28
    return true;
}

void Simulation::finish() {
    // TODO(Sara): fail leftovers with a reason; stats.addCounters(...); stats.finalize(...).
    // See: report Sections 4.3 and 4.8   |   Done when: Scenario 02 leftovers FAILED with a reason
    stats.finalize(world.robots, world.requests, world.currentTime, runTimeMs);
    finished = true;
}
