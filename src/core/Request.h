// File:       src/core/Request.h
// Owner:      Farwa
// Purpose:    One rescue request: data from the file, lifecycle state machine,
//             assigned robot, remaining work, unreachable flag and the
//             priority key used by the pending heap (Report 4.3, 5.6, 5.9).
// Depends on: Types
// Used by:    World, ScenarioLoader, Coordinator, Movement, EventProcessor,
//             Rerouter, Statistics, Renderer, UIApp
// Milestone:  M2
//
// A request is invisible until its report time (reported == false). At that
// step it becomes PENDING (Report 4.2, phase 2).
// remainingWork lives HERE, not in the robot, so a second robot continues the
// work after an interruption (Report 4.3).
#ifndef RESQGRID_REQUEST_H
#define RESQGRID_REQUEST_H

#include "Types.h"

class Request {
public:
    // --- from the REQUESTS section ---
    int      id = 0;
    int      reportTime = 0;
    int      location = NO_INDEX;           // location index
    Category category = Category::MEDICAL;
    int      urgency = 1;                   // 1 (low) .. 5 (critical)
    int      duration = 1;                  // work steps

    // --- dynamic state ---
    bool         reported = false;          // true from reportTime on
    RequestState state = RequestState::PENDING;
    int          remainingWork = 0;         // starts at duration
    int          assignedRobot = NO_INDEX;  // robot index (set/cleared together with Robot::assignedRequest)
    bool         unreachable = false;       // no route found (Report 4.6)
    int          unreachableVersion = -1;   // Environment::networkVersion when marked unreachable
    FailReason   failReason = FailReason::NONE;
    int          completionTime = -1;       // step at which it became COMPLETED

    Request() = default;
    Request(int id, int reportTime, int location, Category cat, int urgency, int duration);

    // TODO(Farwa): key = agingInterval * urgency - reportTime (Report 5.6).
    // The heap breaks equal keys by smaller ID; that is NOT part of this value.
    // See: report Section 5.6   |   Done when: Scenario 01 gives 145 for Req 4 and 98 for Req 3
    long long priorityKey(int agingInterval) const;

    // TODO(Farwa): allowed transitions (Report 4.3 figure):
    //   PENDING->ASSIGNED, ASSIGNED->IN_PROGRESS, IN_PROGRESS->COMPLETED,
    //   ASSIGNED->PENDING (robot offline / no route),
    //   IN_PROGRESS->PENDING (robot offline, work kept),
    //   PENDING->FAILED (simulation ends).
    // See: report Section 4.3   |   Done when: invalid transition returns false
    static bool isValidTransition(RequestState from, RequestState to);

    // TODO(Farwa): same pattern as Robot::setState; the caller logs a rejection.
    // See: report Section 4.3   |   Done when: invalid transition returns false and is logged
    bool setState(RequestState newState);

    // Response time = completion step - report time (Report Appendix A).
    int responseTime() const { return completionTime - reportTime; }

    bool isFinished() const { return state == RequestState::COMPLETED || state == RequestState::FAILED; }
};

#endif // RESQGRID_REQUEST_H
