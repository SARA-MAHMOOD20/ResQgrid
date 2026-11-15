// File:       src/core/Simulation.h
// Owner:      Sara
// Purpose:    The simulation engine: loads a scenario, runs the 7-phase step
//             loop, applies the termination rule, and gives the UI read-only
//             access to everything (Report 4.2, 4.8).
// Depends on: World, ScenarioLoader, Router, Coordinator, Movement,
//             EventProcessor, Rerouter, Logger, Statistics
// Used by:    main.cpp (console mode), UIApp (GUI mode)
// Milestone:  M3
//
// One step at time t (Report 4.2), in exactly this order:
//   1. Events        EventProcessor::applyEventsAt(t)
//   2. New requests  reportTime == t -> PENDING, Coordinator::addPending (file order)
//   3. Reroute       Rerouter::rerouteAffected()  (only if the network changed)
//   4. Assignment    Coordinator::assignStep()
//   5. Low battery   Movement::lowBatteryCheck()
//   6. Action        Movement::actAll(t)          (robots in ID order)
//   7. Record        log lines / statistics for the step
// Termination (Report 4.8): stop at the first step where t == maxSteps, or
// every request is COMPLETED/FAILED with no requests still to be reported and
// no events left. Then every unfinished request becomes FAILED (Report 4.3).
#ifndef RESQGRID_SIMULATION_H
#define RESQGRID_SIMULATION_H

#include "World.h"
#include "ScenarioLoader.h"
#include "Router.h"
#include "Coordinator.h"
#include "Movement.h"
#include "EventProcessor.h"
#include "Rerouter.h"
#include "../output/Logger.h"
#include "../output/Statistics.h"

class Simulation {
public:
    Simulation();

    // TODO(Sara): loader.load(path, world); on success coord.init(), reset the
    // clock, stats and finished flag. Returns false if the file had errors
    // (read them through loader()).
    // See: report Section 9.4   |   Done when: invalid files return false; provided file returns true
    bool load(const Str& path);

    // TODO(Sara): if finished return false; logger.setTime(t); run phases 1-7;
    // check termination (finish() if met); otherwise ++currentTime. Returns
    // true if a step was executed. The UI calls this once per tick.
    // See: report Sections 4.2 and 4.8   |   Done when: Scenario 01 ends at t=28
    bool step();

    // TODO(Sara): time the whole run with <chrono>; call step() until finished.
    // See: report Section 4.8   |   Done when: Scenario 01 ends at t=28; two runs give identical logs
    void runAll();

    bool isFinished() const { return finished; }

    // --- read-only access for main.cpp and the UI ---
    int                   currentTime() const { return world.currentTime; }
    const World&          getWorld() const    { return world; }
    const ScenarioLoader& getLoader() const   { return loader; }
    const Statistics&     getStats() const    { return stats; }
    const Logger&         getLogger() const   { return logger; }
    const IndexedMaxHeap& getPending() const  { return coord.pending(); }

    // Logger setup before running (main.cpp: --log, --quiet).
    Logger& log() { return logger; }

private:
    // TODO(Sara): phase 2 - advance world.nextRequest while reportTime == t:
    // reported = true, remainingWork = duration, log REQUEST, coord.addPending.
    // See: report Section 4.2 (phase 2)   |   Done when: same-time requests enter in file order
    void releaseNewRequests(int t);

    // TODO(Sara): termination rule 2 (all finished, nothing left to report, no events left).
    // See: report Section 4.8   |   Done when: Scenario 01 stops at t=28, not at max_steps
    bool allDone() const;

    // TODO(Sara): mark unfinished requests FAILED (reason unreachable / not served),
    // copy module counters into stats (addCounters), stats.finalize, finished = true.
    // See: report Sections 4.3 and 4.8   |   Done when: Scenario 02 leftover requests FAILED with a reason
    void finish();

    // Member order matters: world and logger must be constructed before the
    // modules that keep references to them.
    World          world;
    Logger         logger;
    ScenarioLoader loader;
    Statistics     stats;
    Router         router;
    Coordinator    coord;
    EventProcessor events;
    Rerouter       rerouter;
    Movement       movement;
    bool           finished = false;
    double         runTimeMs = 0.0;
};

#endif // RESQGRID_SIMULATION_H
