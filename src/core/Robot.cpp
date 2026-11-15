// File:       src/core/Robot.cpp
// Owner:      Farwa
// Purpose:    Robot state machine and capability check.
// Depends on: Robot.h (Types, Queue)
// Used by:    see Robot.h
// Milestone:  M2
#include "Robot.h"

Robot::Robot(int id_, int startLocation, Capability cap, int maxBattery_, int initialBattery)
    : id(id_), capability(cap), maxBattery(maxBattery_), battery(initialBattery),
      location(startLocation) {}

bool Robot::canServe(Category cat) const {
    // TODO(Farwa): GENERAL -> true; otherwise compare capability with cat
    // (the first four Capability values line up with Category).
    // See: report Section 6.2   |   Done when: GENERAL serves all 4 categories; FIRE does not serve MEDICAL
    (void)cat;
    return false;
}

bool Robot::isValidTransition(RobotState from, RobotState to) {
    // TODO(Farwa): return true only for the transitions listed in Robot.h.
    // See: report Sections 4.4 and 4.7   |   Done when: every arrow in the figure is true, everything else false
    (void)from;
    (void)to;
    return false;
}

bool Robot::setState(RobotState newState) {
    // TODO(Farwa): check isValidTransition(state, newState); set state if valid.
    // See: report Section 4.4   |   Done when: invalid transition returns false and is logged (by the caller)
    (void)newState;
    return false;
}
