// File:       src/core/Request.cpp
// Owner:      Farwa
// Purpose:    Request lifecycle state machine and priority key.
// Depends on: Request.h (Types)
// Used by:    see Request.h
// Milestone:  M2
#include "Request.h"

Request::Request(int id_, int reportTime_, int location_, Category cat, int urgency_, int duration_)
    : id(id_), reportTime(reportTime_), location(location_), category(cat),
      urgency(urgency_), duration(duration_), remainingWork(duration_) {}

long long Request::priorityKey(int agingInterval) const {
    // TODO(Farwa): return (long long)agingInterval * urgency - reportTime.
    // See: report Section 5.6   |   Done when: Scenario 01 gives 145 for Req 4 and 98 for Req 3
    (void)agingInterval;
    return 0;
}

bool Request::isValidTransition(RequestState from, RequestState to) {
    // TODO(Farwa): return true only for the transitions listed in Request.h.
    // See: report Section 4.3   |   Done when: COMPLETED->anything is false
    (void)from;
    (void)to;
    return false;
}

bool Request::setState(RequestState newState) {
    // TODO(Farwa): check isValidTransition(state, newState); set state if valid.
    // See: report Section 4.3   |   Done when: invalid transition returns false and is logged (by the caller)
    (void)newState;
    return false;
}
