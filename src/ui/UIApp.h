// File:       src/ui/UIApp.h
// Owner:      Sara
// Purpose:    The SFML 3 window (--gui): controls (step, play, pause, speed),
//             robot and request panels, log panel, final-stats screen and
//             load-error screen. Drives Simulation::step() from the UI loop.
// Depends on: Simulation (read-only accessors + step()), GraphLayout, Renderer, SFML 3
// Used by:    main.cpp (--gui branch, only when built with RESQGRID_HAS_GUI)
// Milestone:  M1 (empty window), M4 (connected to step()), M5 (final/error screens)
//
// SFML 3 event loop shape:
//     while (const auto event = window.pollEvent()) {
//         if (event->is<sf::Event::Closed>()) window.close();
//         else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) { ... key->code ... }
//     }
#ifndef RESQGRID_UI_APP_H
#define RESQGRID_UI_APP_H

#include <SFML/Graphics.hpp>
#include "../core/Types.h"
#include "../core/Simulation.h"
#include "GraphLayout.h"
#include "Renderer.h"

class UIApp {
public:
    static const unsigned WINDOW_WIDTH = 1280;
    static const unsigned WINDOW_HEIGHT = 800;

    // `loaded` = result of Simulation::load(); false -> show the load-error screen.
    UIApp(Simulation& sim, bool loaded);

    // TODO(Sara): create the window, layout.compute(...), load a font, then loop:
    // handleEvents(); update(dt); render(); until the window closes. Return 0.
    // See: plan Section 6 (UIApp)   |   Done when: full provided run watchable start to end
    int run();

private:
    // TODO(Sara): close, Space = play/pause, Right/N = one step, +/- = speed.
    // See: plan Section 6   |   Done when: every control works
    void handleEvents();

    // TODO(Sara): while playing, call sim.step() stepsPerSecond times per second (accumulator).
    // See: plan Section 6   |   Done when: speed control changes the pace
    void update(float dtSeconds);

    // TODO(Sara): clear, renderer.draw, panels, final-stats or error screen, display.
    // See: plan Section 6   |   Done when: full provided run watchable start to end
    void render();

    // TODO(Sara): text panels from sim.getWorld() / getPending() / getLogger().recent(i).
    // See: plan Section 6   |   Done when: robot and request states readable at every step
    void drawRobotPanel();
    void drawRequestPanel();
    void drawLogPanel();

    // TODO(Sara): sim.getStats() when finished; sim.getLoader() messages when !loaded.
    // See: plan Section 5 (M5)   |   Done when: invalid file shows its errors in the window
    void drawFinalStats();
    void drawErrorScreen();

    Simulation&      sim;
    bool             loaded;
    sf::RenderWindow window;
    GraphLayout      layout;
    Renderer         renderer;     // draws into `window` (declared after it)
    bool             playing = false;
    float            stepsPerSecond = 2.0f;
    float            accumulator = 0.0f;
};

#endif // RESQGRID_UI_APP_H
