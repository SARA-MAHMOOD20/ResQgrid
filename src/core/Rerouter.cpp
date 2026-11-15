// File:       src/core/Rerouter.cpp
// Owner:      Rabia
// Purpose:    Rerouting robots after road / station changes (phase 3).
// Depends on: Rerouter.h (World, Router, IndexedMaxHeap, Logger)
// Used by:    Simulation
// Milestone:  M3
#include "Rerouter.h"

Rerouter::Rerouter(World& w, Router& r, IndexedMaxHeap& p, Logger& l)
    : world(w), router(r), pending(p), log(l) {}

void Rerouter::rerouteAffected() {
    // TODO(Rabia): scan moving robots in ID order; turnBack / replan / releaseRequest.
    // See: report Section 4.7   |   Done when: Scenario 01 t=4 and Scenario 04 t=3 match
}

bool Rerouter::routeBlocked(const Robot& r) const {
    // TODO(Rabia): check r.onConn and every connection in r.route (walk r.route.first()).
    // See: report Section 7.5   |   Done when: true exactly for affected robots
    (void)r;
    return false;
}

bool Rerouter::chargerDown(const Robot& r) const {
    // TODO(Rabia): RETURNING/RECHARGING and !env.isOpenCharger(r.targetLocation).
    // See: report Section 4.7   |   Done when: Scenario 04 station-down case detected
    (void)r;
    return false;
}

void Rerouter::turnBack(Robot& r) {
    // TODO(Rabia): move the robot back to r.location, charging battery for r.progress units.
    // See: report Section 4.7   |   Done when: Scenario 01 Robot 1 back at location 2
    (void)r;
}

bool Rerouter::replan(Robot& r) {
    // TODO(Rabia): router.run(from); router.routeTo(target, r.route); ++reroutes; log REROUTE.
    // See: report Section 4.7   |   Done when: Scenario 01 "REROUTE robot=1 on=4 back_to=2 route=2-4-7-5 cost=9"
    (void)r;
    return false;
}

void Rerouter::releaseRequest(Robot& r) {
    // TODO(Rabia): see Rerouter.h.
    // See: report Sections 4.3 and 4.7   |   Done when: request is PENDING again and in the heap
    (void)r;
}
