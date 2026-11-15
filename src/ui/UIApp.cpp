// File:       src/ui/UIApp.cpp
// Owner:      Sara
// Purpose:    SFML 3 window, controls and panels.
// Depends on: UIApp.h (Simulation, GraphLayout, Renderer, SFML)
// Used by:    main.cpp (--gui)
// Milestone:  M4
#include "UIApp.h"
#include <iostream>

UIApp::UIApp(Simulation& s, bool l) : sim(s), loaded(l), renderer(window) {}

int UIApp::run() {
    // TODO(Sara): window.create(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "ResQGrid");
    // window.setFramerateLimit(60); sf::Clock for dt; main loop as in UIApp.h.
    // See: plan Section 6   |   Done when: full provided run watchable start to end
    cout << "GUI not implemented yet (UIApp::run is a stub).\n";
    return 0;
}

void UIApp::handleEvents() {
    // TODO(Sara): SFML 3 pollEvent loop (see UIApp.h).
    // See: plan Section 6   |   Done when: every control works
}

void UIApp::update(float dtSeconds) {
    // TODO(Sara): accumulator += dt; step while accumulator >= 1 / stepsPerSecond.
    // See: plan Section 6   |   Done when: play / pause / speed work
    (void)dtSeconds;
}

void UIApp::render() {
    // TODO(Sara): window.clear(); renderer.draw(sim.getWorld(), layout); panels; window.display().
    // See: plan Section 6   |   Done when: provided file draws on screen
}

void UIApp::drawRobotPanel() {
    // TODO(Sara): one line per robot: ID, state, battery, location, request.
    // See: plan Section 6   |   Done when: robot states readable at every step
}

void UIApp::drawRequestPanel() {
    // TODO(Sara): pending / assigned / completed / failed lists or counts.
    // See: spec section 6 (display pending, assigned, completed, failed)   |   Done when: lists update each step
}

void UIApp::drawLogPanel() {
    // TODO(Sara): last N lines from sim.getLogger().recent(i).
    // See: plan Section 6   |   Done when: newest log lines visible
}

void UIApp::drawFinalStats() {
    // TODO(Sara): show sim.getStats() fields after the run ends.
    // See: plan Section 5 (M5)   |   Done when: final screen matches the console statistics
}

void UIApp::drawErrorScreen() {
    // TODO(Sara): list sim.getLoader().error(i) with line numbers.
    // See: plan Section 5 (M5)   |   Done when: invalid_01 shows its errors in the window
}
