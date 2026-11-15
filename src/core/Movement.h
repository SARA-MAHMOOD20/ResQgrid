// File:       src/core/Movement.h
// Owner:      Sara
// Purpose:    Phase 5 (low-battery check) and phase 6 (robot actions): move
//             `speed` units along the route (crossing locations within one
//             step), work, recharge; battery updates (Report 4.1, 4.2, 4.5).
// Depends on: World (Robot, Request, Environment), Router, Logger
// Used by:    Simulation
// Milestone:  M3
//
// Rules to respect:
//   * Robots act in increasing robot-ID order (world.robotsById).
//   * Move: battery -= batteryPerCostUnit per unit travelled. If speed exceeds
//     the rest of a road, reach the next location and continue in the same step.
//   * A robot that reaches its request location starts work in the NEXT step;
//     each work step: remainingWork -= 1, battery -= batteryPerWorkStep.
//     remainingWork == 0 -> COMPLETED (completionTime = t), robot AVAILABLE.
//   * Recharge at an OPEN charger: +rechargePerStep up to maxBattery; RECHARGING
//     robots are not candidates until full, then AVAILABLE.
//   * Battery never goes below 0 (M5 invariant check).
#ifndef RESQGRID_MOVEMENT_H
#define RESQGRID_MOVEMENT_H

#include "World.h"
#include "Router.h"
#include "../output/Logger.h"

class Movement {
public:
    Movement(World& world, Router& router, Logger& log);

    // TODO(Sara): phase 5 - every robot still AVAILABLE with battery below
    // lowBatteryPercent% of maxBattery goes RETURNING to the nearest open charger
    // (router.run(robot location), nearestOpenCharger, routeTo). No reachable
    // charger: stay AVAILABLE and log a warning (Report 11).
    // See: report Sections 4.2 (phase 5) and 4.5   |   Done when: Scenario 01 Robot 2 -> RETURNING target=7 at t=0
    void lowBatteryCheck();

    // TODO(Sara): phase 6 - for each robot in ID order call move / work / recharge
    // depending on its state; count busySteps for utilization.
    // See: report Sections 4.1 and 4.2 (phase 6)   |   Done when: Scenario 01 battery values match
    void actAll(int t);

    int completedThisStep() const { return completedNow; }

private:
    // TODO(Sara): advance up to speed units; arrive at locations; pop the route.
    // See: report Section 4.1   |   Done when: Scenario 01 Robot 3 reaches 6 at t=5
    void move(Robot& r, int t);

    // TODO(Sara): one work step on the assigned request (IN_PROGRESS); complete it when done.
    // See: report Sections 4.1 and 4.5   |   Done when: Scenario 01 Request 2 COMPLETED at t=7
    void work(Robot& r, int t);

    // TODO(Sara): add rechargePerStep (cap at max); AVAILABLE when full.
    // See: report Section 4.5   |   Done when: Scenario 01 Robot 2 recharges t=9..14 and is AVAILABLE at 15
    void recharge(Robot& r);

    // TODO(Sara): robot reached the end of its route: TRAVELLING -> PERFORMING
    // (request ASSIGNED -> IN_PROGRESS, work starts next step) or
    // RETURNING -> RECHARGING; log ARRIVE.
    // See: report Sections 4.3 and 4.4   |   Done when: Scenario 01 Robot 2 at charger 7 with battery 3 at t=8
    void arrive(Robot& r, int t);

    // TODO(Sara): battery * 100 < lowBatteryPercent * maxBattery.
    // See: report Section 4.5   |   Done when: 12/60 (20%) is low, 18/60 (30%) is not
    bool isLowBattery(const Robot& r) const;

    World&  world;
    Router& router;
    Logger& log;
    int     completedNow = 0;
};

#endif // RESQGRID_MOVEMENT_H
