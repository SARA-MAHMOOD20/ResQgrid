// File:       src/output/Statistics.h
// Owner:      Farwa
// Purpose:    Final metrics of a run (spec section 8, Report Appendix A):
//             completed / failed / remaining, reroutes, conflicts,
//             reassignments, battery rejections, distance, battery used,
//             response times, utilization, run time.
// Depends on: Robot, Request, Logger, DynamicArray
// Used by:    Simulation, main.cpp, UIApp (final-stats screen)
// Milestone:  M3
//
// Counters that only another module can see (conflicts, rejections, reroutes,
// reassignments, Dijkstra runs) are kept by that module and copied in with
// addCounters() at the end, so Statistics does not depend on them.
// Per-robot distance / battery / busy steps and per-request completion times
// are read from the robots and requests in finalize().
#ifndef RESQGRID_STATISTICS_H
#define RESQGRID_STATISTICS_H

#include <iostream>
#include "../core/Types.h"
#include "../core/Robot.h"
#include "../core/Request.h"
#include "../containers/DynamicArray.h"
#include "Logger.h"

class Statistics {
public:
    // --- request outcomes ---
    int completed = 0;
    int failed = 0;
    int failedUnreachable = 0;
    int failedNotServed = 0;
    int remaining = 0;              // not finished when the run stopped (0 after failing leftovers)

    // --- coordination counters (copied in by Simulation) ---
    int reroutes = 0;
    int conflicts = 0;
    int reassignments = 0;
    int batteryRejections = 0;
    int dijkstraRuns = 0;

    // --- movement / battery ---
    long long totalDistance = 0;
    long long totalBatteryUsed = 0;

    // --- derived ---
    double avgResponse = 0.0;
    int    maxResponse = 0;
    double utilization = 0.0;       // TODO(Farwa): define in README (suggested: busy steps / (robots * steps))
    int    stepsRun = 0;
    double runTimeMs = 0.0;

    // TODO(Farwa): set every field back to 0.
    // See: plan Section 6 (Statistics)   |   Done when: two runs in one process give the same numbers
    void reset();

    void addCounters(int reroutes, int conflicts, int reassignments, int batteryRejections, int dijkstraRuns);

    // TODO(Farwa): count outcomes from `requests`, response times
    // (completionTime - reportTime, Report Appendix A) for COMPLETED ones,
    // sum distance / battery / busy steps from `robots`, compute averages.
    // See: report Appendix A   |   Done when: Scenario 01 gives 4 / 0 / 1 reroute / 2 rejections
    void finalize(const DynamicArray<Robot>& robots, const DynamicArray<Request>& requests,
                  int stepsRun, double runTimeMs);

    // TODO(Farwa): print a readable block of all metrics (console / final screen).
    // See: spec section 8 (final statistics)   |   Done when: printed after every console run
    void print(ostream& out) const;

    // TODO(Farwa): write the same metrics as "STATS key=value" log lines.
    // See: report Section 9.5   |   Done when: the log file ends with the statistics
    void writeToLog(Logger& log) const;
};

#endif // RESQGRID_STATISTICS_H
