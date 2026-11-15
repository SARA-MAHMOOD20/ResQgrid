// File:       src/core/Movement.cpp
// Owner:      Sara
// Purpose:    Robot actions (move, work, recharge) and the low-battery rule.
// Depends on: Movement.h (World, Router, Logger)
// Used by:    Simulation
// Milestone:  M3
#include "Movement.h"

Movement::Movement(World& w, Router& r, Logger& l) : world(w), router(r), log(l) {}

void Movement::lowBatteryCheck() {
    // TODO(Sara): see Movement.h.
    // See: report Sections 4.2 and 4.5   |   Done when: Scenario 01 Robot 2 -> RETURNING target=7 at t=0
}

void Movement::actAll(int t) {
    // TODO(Sara): completedNow = 0; for each index in world.robotsById: switch on state.
    // See: report Section 4.2 (phase 6)   |   Done when: Scenario 01 battery values match
    (void)t;
    completedNow = 0;
}

void Movement::move(Robot& r, int t) {
    // TODO(Sara): see Movement.h.
    // See: report Section 4.1   |   Done when: Scenario 01 Robot 1 is 1 unit along 2-5 at t=4
    (void)r;
    (void)t;
}

void Movement::work(Robot& r, int t) {
    // TODO(Sara): see Movement.h.
    // See: report Section 4.1   |   Done when: Scenario 01 Request 1 COMPLETED at t=16, battery 80
    (void)r;
    (void)t;
}

void Movement::recharge(Robot& r) {
    // TODO(Sara): see Movement.h.
    // See: report Section 4.5   |   Done when: Scenario 01 Robot 2 battery 60 at t=14
    (void)r;
}

void Movement::arrive(Robot& r, int t) {
    // TODO(Sara): see Movement.h.
    // See: report Sections 4.3 and 4.4   |   Done when: state transitions logged correctly
    (void)r;
    (void)t;
}

bool Movement::isLowBattery(const Robot& r) const {
    // TODO(Sara): integer comparison, no floating point.
    // See: report Section 4.5   |   Done when: 12/60 is low, 18/60 is not
    (void)r;
    return false;
}
