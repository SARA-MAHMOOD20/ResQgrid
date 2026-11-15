// File:       src/output/Statistics.cpp
// Owner:      Farwa
// Purpose:    Computing and printing the final metrics.
// Depends on: Statistics.h (Robot, Request, Logger)
// Used by:    see Statistics.h
// Milestone:  M3
#include "Statistics.h"

void Statistics::reset() {
    // TODO(Farwa): zero every field (or *this = Statistics();).
    // See: plan Section 6   |   Done when: all fields are 0 after reset()
}

void Statistics::addCounters(int reroutes_, int conflicts_, int reassignments_,
                             int batteryRejections_, int dijkstraRuns_) {
    reroutes = reroutes_;
    conflicts = conflicts_;
    reassignments = reassignments_;
    batteryRejections = batteryRejections_;
    dijkstraRuns = dijkstraRuns_;
}

void Statistics::finalize(const DynamicArray<Robot>& robots, const DynamicArray<Request>& requests,
                          int stepsRun_, double runTimeMs_) {
    // TODO(Farwa): loop over requests and robots, fill every outcome / derived field.
    // See: report Appendix A   |   Done when: Scenario 01 response times are 16, 7, 26, 15
    (void)robots;
    (void)requests;
    stepsRun = stepsRun_;
    runTimeMs = runTimeMs_;
}

void Statistics::print(ostream& out) const {
    // TODO(Farwa): print every metric, one per line, with labels.
    // See: spec section 8   |   Done when: Scenario 01 shows 4 completed, 0 failed, 1 reroute, 2 rejections
    out << "=== Final statistics (not implemented yet) ===\n";
    out << "steps run: " << stepsRun << "\n";
}

void Statistics::writeToLog(Logger& log) const {
    // TODO(Farwa): log.write("STATS completed=... failed=... ...").
    // See: report Section 9.5   |   Done when: the log file ends with the statistics
    (void)log;
}
