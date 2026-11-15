// File:       src/core/Rerouter.h
// Owner:      Rabia
// Purpose:    Phase 3: after ALL events of the step, find the moving robots
//             affected by a road or station change and reroute them, or
//             release their request (Report 4.7, 7.5).
// Depends on: World (Environment, Robot, Request), Router, IndexedMaxHeap, Logger
// Used by:    Simulation
// Milestone:  M3
//
// Policy (Report 4.7), for each TRAVELLING or RETURNING robot:
//   * on the blocked road  -> turn back to the location it came from (battery
//                              is used for the distance back), reroute from there;
//   * blocked road later in the route -> reroute from the next location;
//   * no feasible route    -> request back to PENDING (a reassignment), robot
//                              AVAILABLE, or RETURNING if its battery is low.
// A charging station going down is handled the same way for robots heading to
// it or recharging there. Detection is a scan of every moving robot's route,
// O(R * L) (Report 7.5).
#ifndef RESQGRID_REROUTER_H
#define RESQGRID_REROUTER_H

#include "World.h"
#include "Router.h"
#include "../containers/IndexedMaxHeap.h"
#include "../output/Logger.h"

class Rerouter {
public:
    Rerouter(World& world, Router& router, IndexedMaxHeap& pending, Logger& log);

    // TODO(Rabia): if !world.networkChangedThisStep return; otherwise check
    // every robot in ID order (world.robotsById) and apply the policy above.
    // See: report Section 4.7   |   Done when: Scenario 01 t=4 and Scenario 04 t=3 match
    void rerouteAffected();

    int rerouteCount() const      { return reroutes; }
    int reassignmentCount() const { return reassignments; }

private:
    // TODO(Rabia): true if the robot's current road or any road left in its route is BLOCKED.
    // See: report Section 7.5   |   Done when: Scenario 04 "blocked later in route" detected
    bool routeBlocked(const Robot& r) const;

    // TODO(Rabia): true if the robot is heading to / recharging at a charger that is now down.
    // See: report Section 4.7   |   Done when: Scenario 04 station-down case detected
    bool chargerDown(const Robot& r) const;

    // TODO(Rabia): robot on a blocked road goes back to `location`: onConn = -1,
    // progress = 0, battery and distance charged for `progress` units.
    // See: report Sections 4.7 and 7.6   |   Done when: Scenario 01 Robot 1 is back at 2 at t=4
    void turnBack(Robot& r);

    // TODO(Rabia): new route from the robot's (next) location to its target
    // (request location, or nearest open charger when RETURNING); battery
    // feasibility as in Report 4.5. False if there is no feasible route.
    // See: report Section 4.7   |   Done when: Scenario 01 new route 2-4-7-5 cost 9
    bool replan(Robot& r);

    // TODO(Rabia): request -> PENDING (keep remainingWork), clear both links,
    // pending.insert, ++reassignments, robot -> AVAILABLE or RETURNING.
    // See: report Sections 4.3 and 4.7   |   Done when: logged as RELEASE and counted
    void releaseRequest(Robot& r);

    World&          world;
    Router&         router;
    IndexedMaxHeap& pending;
    Logger&         log;
    int reroutes = 0;
    int reassignments = 0;
};

#endif // RESQGRID_REROUTER_H
