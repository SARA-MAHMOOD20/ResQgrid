// File:       src/ui/UIApp.h
// Owner:      Sara
// Purpose:    The SFML 3 window (--gui): controls (step, play, pause, speed,
//             run to end), robot and request panels, log panel, final-stats
//             screen and load-error screen. Drives Simulation::step().
// Depends on: Simulation (read-only accessors + step()), GraphLayout, Renderer, SFML 3
// Used by:    main.cpp (--gui branch, only when built with RESQGRID_HAS_GUI)
// Milestone:  M1 (window), M4 (connected to step()), M5 (final/error screens)
//
// Screen layout:
//   +------------------------------+-----------+
//   |  map (Renderer)              |  side     |
//   |                              |  panel    |
//   +------------------------------+  (status, |
//   |  log panel                   |  buttons, |
//   |                              |  robots,  |
//   +------------------------------+  requests)|
// Keys: Space play/pause, Right or N one step, + / - speed, E run to end, Esc quit.
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

    // Opens the window and runs until it is closed. Returns 0.
    int run();

private:
    struct Button {
        sf::FloatRect box;
        Str           label;
    };
    static const int BUTTON_COUNT = 5;      // Step, Play/Pause, Slower, Faster, End
    static const int SPEED_COUNT = 7;

    void layoutScreen(float width, float height);     // map / panel / log rectangles
    void handleEvents();
    void handleClick(sf::Vector2f point);
    void stepOnce();
    void runToEnd();
    void changeSpeed(int delta);
    void update(float dtSeconds);
    void render();

    void drawSidePanel();
    void drawButtons(float top);
    float drawRequestPanel(float top);      // returns the y where it ended
    void drawRobotPanel(float top);
    void drawLogPanel();
    void drawFinalStats();
    void drawErrorScreen();

    // Small helpers.
    void drawText(const Str& text, float x, float y, unsigned size, sf::Color color, bool bold = false);
    void drawBox(const sf::FloatRect& box, sf::Color fill, sf::Color outline);
    bool loadFont();
    static Str uiFolder();
    static Str capabilityShort(Capability cap);
    static Str oneDecimal(double value);

    Simulation&      sim;
    bool             loaded;
    sf::RenderWindow window;
    GraphLayout      layout;
    Renderer         renderer;              // draws into `window` (declared after it)

    sf::FloatRect    mapArea;
    sf::FloatRect    panelArea;
    sf::FloatRect    logArea;
    Button           buttons[BUTTON_COUNT];

    bool  playing = false;
    float speeds[SPEED_COUNT] = {0.5f, 1.0f, 2.0f, 4.0f, 8.0f, 16.0f, 32.0f};  // steps per second
    int   speedIndex = 2;
    float accumulator = 0.0f;
};

#endif // RESQGRID_UI_APP_H
