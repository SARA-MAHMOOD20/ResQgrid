// File:       src/ui/UIApp.cpp
// Owner:      Sara
// Purpose:    SFML 3 window, controls and panels.
// Depends on: UIApp.h (Simulation, GraphLayout, Renderer, SFML)
// Used by:    main.cpp (--gui)
// Milestone:  M4
#include "UIApp.h"
#include <iostream>

// CMake sets this to <repo>/src/ui so fonts and sprites are found from any folder.
#ifndef RESQGRID_UI_DIR
#define RESQGRID_UI_DIR "src/ui"
#endif

// Theme colours: Big Hero 6 warmth (Baymax red, cream, amber on charcoal). No blue.
static const sf::Color BACKGROUND(26, 23, 22);
static const sf::Color MAP_BACK(40, 50, 37);
static const sf::Color PANEL_BACK(35, 31, 29);
static const sf::Color PANEL_LINE(72, 64, 58);
static const sf::Color BUTTON_BACK(54, 47, 43);
static const sf::Color TEXT_MAIN(242, 234, 220);
static const sf::Color TEXT_DIM(172, 160, 145);
static const sf::Color ACCENT(226, 74, 58);        // Baymax red
static const sf::Color AMBER(242, 170, 60);

static const float PANEL_WIDTH = 400.0f;
static const float LOG_HEIGHT = 170.0f;

UIApp::UIApp(Simulation& s, bool l) : sim(s), loaded(l), renderer(window) {
    buttons[0].label = "Step";
    buttons[1].label = "Play";
    buttons[2].label = "Slower";
    buttons[3].label = "Faster";
    buttons[4].label = "End";
}

int UIApp::run() {
    window.create(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "ResQGrid");
    window.setFramerateLimit(60);

    if (!loadFont()) {
        cerr << "warning: no font found, text will not be shown\n";
    }
    int sprites = renderer.loadSprites(uiFolder() + "/sprites");
    if (sprites < 2) {
        cerr << "note: robot.png / person.png not found in src/ui/sprites, drawing shapes instead\n";
    }

    layoutScreen((float)WINDOW_WIDTH, (float)WINDOW_HEIGHT);
    if (loaded) {
        layout.compute(sim.getWorld().env);
    }

    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        handleEvents();
        update(dt);
        render();
    }
    return 0;
}

// ---------------------------------------------------------------------------
// Layout and input
// ---------------------------------------------------------------------------
void UIApp::layoutScreen(float width, float height) {
    float mapWidth = width - PANEL_WIDTH;
    mapArea = sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(mapWidth, height - LOG_HEIGHT));
    logArea = sf::FloatRect(sf::Vector2f(0, height - LOG_HEIGHT), sf::Vector2f(mapWidth, LOG_HEIGHT));
    panelArea = sf::FloatRect(sf::Vector2f(mapWidth, 0), sf::Vector2f(PANEL_WIDTH, height));
    renderer.setMapArea(mapArea);

    // Buttons in one row near the top of the side panel.
    float x = panelArea.position.x + 16;
    float y = 96;
    float w = (PANEL_WIDTH - 32 - 4 * 6) / BUTTON_COUNT;
    for (int i = 0; i < BUTTON_COUNT; ++i) {
        buttons[i].box = sf::FloatRect(sf::Vector2f(x + i * (w + 6), y), sf::Vector2f(w, 30));
    }
}

void UIApp::handleEvents() {
    while (const auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        } else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
            float w = (float)resized->size.x;
            float h = (float)resized->size.y;
            if (w < PANEL_WIDTH + 300) w = PANEL_WIDTH + 300;   // keep some map visible
            if (h < LOG_HEIGHT + 300) h = LOG_HEIGHT + 300;
            window.setView(sf::View(sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(w, h))));
            layoutScreen(w, h);
        } else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
            if (key->code == sf::Keyboard::Key::Escape) {
                window.close();
            } else if (key->code == sf::Keyboard::Key::Space) {
                playing = !playing;
            } else if (key->code == sf::Keyboard::Key::Right || key->code == sf::Keyboard::Key::N) {
                stepOnce();
            } else if (key->code == sf::Keyboard::Key::Equal || key->code == sf::Keyboard::Key::Add) {
                changeSpeed(+1);
            } else if (key->code == sf::Keyboard::Key::Hyphen || key->code == sf::Keyboard::Key::Subtract) {
                changeSpeed(-1);
            } else if (key->code == sf::Keyboard::Key::E) {
                runToEnd();
            }
        } else if (const auto* click = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (click->button == sf::Mouse::Button::Left) {
                handleClick(window.mapPixelToCoords(click->position));
            }
        }
    }
}

void UIApp::handleClick(sf::Vector2f point) {
    if (buttons[0].box.contains(point)) stepOnce();
    if (buttons[1].box.contains(point)) playing = !playing;
    if (buttons[2].box.contains(point)) changeSpeed(-1);
    if (buttons[3].box.contains(point)) changeSpeed(+1);
    if (buttons[4].box.contains(point)) runToEnd();
}

void UIApp::stepOnce() {
    playing = false;            // a manual step pauses autoplay
    sim.step();
}

void UIApp::runToEnd() {
    playing = false;
    while (sim.step()) {
        // keep stepping until the simulation reports it is finished
    }
}

void UIApp::changeSpeed(int delta) {
    speedIndex += delta;
    if (speedIndex < 0) speedIndex = 0;
    if (speedIndex >= SPEED_COUNT) speedIndex = SPEED_COUNT - 1;
}

void UIApp::update(float dtSeconds) {
    if (!playing || sim.isFinished()) {
        accumulator = 0.0f;
        if (sim.isFinished()) playing = false;
        return;
    }
    accumulator += dtSeconds;
    float secondsPerStep = 1.0f / speeds[speedIndex];
    int stepsThisFrame = 0;
    while (accumulator >= secondsPerStep && stepsThisFrame < 50) {   // cap work per frame
        sim.step();
        accumulator -= secondsPerStep;
        stepsThisFrame++;
    }
}

// ---------------------------------------------------------------------------
// Drawing
// ---------------------------------------------------------------------------
void UIApp::render() {
    window.clear(BACKGROUND);

    if (!loaded) {
        drawErrorScreen();
        window.display();
        return;
    }

    drawBox(mapArea, MAP_BACK, MAP_BACK);
    renderer.draw(sim.getWorld(), layout);
    drawLogPanel();
    drawSidePanel();
    if (sim.isFinished()) {
        drawFinalStats();
    }
    window.display();
}

void UIApp::drawSidePanel() {
    drawBox(panelArea, PANEL_BACK, PANEL_LINE);
    const World& world = sim.getWorld();
    float x = panelArea.position.x + 16;

    drawText("ResQGrid", x, 14, 24, ACCENT, true);
    Str name = world.scenarioName.empty() ? Str("scenario") : world.scenarioName;
    drawText("Islamabad  |  " + name, x + 130, 22, 13, TEXT_DIM);

    Str status = "Paused";
    if (sim.isFinished()) status = "Finished";
    else if (playing)     status = "Running";
    drawText("t = " + to_string(sim.currentTime()) + " / " + to_string(world.params.maxSteps)
             + "    " + status, x, 52, 16, TEXT_MAIN);
    drawText("speed " + oneDecimal(speeds[speedIndex]) + " steps/s", x + 230, 54, 13, AMBER);

    drawButtons(buttons[0].box.position.y);
    float y = drawRequestPanel(140);
    drawRobotPanel(y + 10);
}

void UIApp::drawButtons(float top) {
    (void)top;
    buttons[1].label = playing ? "Pause" : "Play";
    for (int i = 0; i < BUTTON_COUNT; ++i) {
        drawBox(buttons[i].box, BUTTON_BACK, (i == 1 && playing) ? AMBER : ACCENT);
        if (renderer.hasFont()) {
            sf::Text label(renderer.getFont(), buttons[i].label, 13);
            sf::FloatRect b = label.getLocalBounds();
            float lx = buttons[i].box.position.x + (buttons[i].box.size.x - b.size.x) / 2 - b.position.x;
            float ly = buttons[i].box.position.y + (buttons[i].box.size.y - b.size.y) / 2 - b.position.y;
            label.setPosition(sf::Vector2f(lx, ly));
            label.setFillColor(TEXT_MAIN);
            window.draw(label);
        }
    }
}

float UIApp::drawRequestPanel(float top) {
    const World& world = sim.getWorld();
    float x = panelArea.position.x + 16;

    // Count requests by state.
    int notYet = 0, pending = 0, assigned = 0, working = 0, done = 0, failed = 0;
    for (int i = 0; i < world.requests.size(); ++i) {
        const Request& q = world.requests[i];
        if (!q.reported && !q.isFinished())               notYet++;
        else if (q.state == RequestState::PENDING)        pending++;
        else if (q.state == RequestState::ASSIGNED)       assigned++;
        else if (q.state == RequestState::IN_PROGRESS)    working++;
        else if (q.state == RequestState::COMPLETED)      done++;
        else if (q.state == RequestState::FAILED)         failed++;
    }

    drawText("Requests", x, top, 16, TEXT_MAIN, true);
    float y = top + 24;
    drawText("waiting " + to_string(pending) + "   assigned " + to_string(assigned)
             + "   working " + to_string(working), x, y, 13, TEXT_MAIN);
    y += 18;
    drawText("completed " + to_string(done) + "   failed " + to_string(failed)
             + "   not reported " + to_string(notYet), x, y, 13, TEXT_DIM);
    y += 24;

    // The most urgent waiting requests: pick the best one, then the next best, ...
    drawText("Most urgent waiting:", x, y, 13, TEXT_DIM);
    y += 18;
    const int SHOW = 5;
    int shownIndex[SHOW];
    int shown = 0;
    while (shown < SHOW) {
        int best = NO_INDEX;
        long long bestKey = 0;
        for (int i = 0; i < world.requests.size(); ++i) {
            const Request& q = world.requests[i];
            if (!q.reported || q.state != RequestState::PENDING) continue;
            bool already = false;
            for (int s = 0; s < shown; ++s) {
                if (shownIndex[s] == i) already = true;
            }
            if (already) continue;
            long long key = q.priorityKey(world.params.agingInterval);
            if (best == NO_INDEX || key > bestKey || (key == bestKey && q.id < world.requests[best].id)) {
                best = i;
                bestKey = key;
            }
        }
        if (best == NO_INDEX) break;
        shownIndex[shown] = best;
        shown++;

        const Request& q = world.requests[best];
        sf::CircleShape dot(5);
        dot.setPosition(sf::Vector2f(x, y + 4));
        dot.setFillColor(Renderer::urgencyColor(q.urgency));
        window.draw(dot);
        Str line = "Q" + to_string(q.id) + "  urgency " + to_string(q.urgency)
                 + "  at " + to_string(world.env.locations[q.location].id)
                 + "  waiting " + to_string(sim.currentTime() - q.reportTime);
        if (q.unreachable) line += "  (no route)";
        drawText(line, x + 16, y, 13, TEXT_MAIN);
        y += 18;
    }
    if (shown == 0) {
        drawText("none", x + 16, y, 13, TEXT_DIM);
        y += 18;
    }
    return y;
}

void UIApp::drawRobotPanel(float top) {
    const World& world = sim.getWorld();
    float x = panelArea.position.x + 16;
    drawText("Robots", x, top, 16, TEXT_MAIN, true);
    float y = top + 24;

    float bottom = panelArea.position.y + panelArea.size.y - 10;
    int total = world.robots.size();
    for (int k = 0; k < total; ++k) {
        if (y + 17 > bottom - 18 && k < total - 1) {
            drawText("... and " + to_string(total - k) + " more", x, y, 13, TEXT_DIM);
            break;
        }
        // Robots listed in ID order when the loader filled robotsById.
        int i = (world.robotsById.size() == total) ? world.robotsById[k] : k;
        const Robot& r = world.robots[i];

        sf::CircleShape dot(5);
        dot.setPosition(sf::Vector2f(x, y + 4));
        dot.setFillColor(Renderer::stateColor(r.state));
        window.draw(dot);

        int percent = (r.maxBattery > 0) ? r.battery * 100 / r.maxBattery : 0;
        Str where;
        if (r.isOnRoad()) where = "road " + to_string(world.env.connections[r.onConn].id);
        else if (r.location != NO_INDEX) where = "at " + to_string(world.env.locations[r.location].id);
        if (r.targetLocation != NO_INDEX) where += " -> " + to_string(world.env.locations[r.targetLocation].id);

        drawText("R" + to_string(r.id), x + 16, y, 13, Renderer::capabilityColor(r.capability), true);
        drawText(capabilityShort(r.capability), x + 56, y, 13, TEXT_DIM);
        drawText(toString(r.state), x + 104, y, 13, TEXT_MAIN);
        drawText(to_string(percent) + "%", x + 210, y, 13, percent < 30 ? sf::Color(235, 90, 90) : TEXT_MAIN);
        drawText(where, x + 256, y, 13, TEXT_DIM);
        y += 17;
    }
}

void UIApp::drawLogPanel() {
    drawBox(logArea, PANEL_BACK, PANEL_LINE);
    float x = logArea.position.x + 14;
    drawText("Log", x, logArea.position.y + 8, 14, TEXT_MAIN, true);

    const Logger& log = sim.getLogger();
    int lineHeight = 16;
    int fits = (int)((logArea.size.y - 36) / lineHeight);
    int count = log.recentCount();
    int first = count - fits;
    if (first < 0) first = 0;

    float y = logArea.position.y + 30;
    for (int i = first; i < count; ++i) {
        drawText(log.recent(i), x, y, 12, (i == count - 1) ? TEXT_MAIN : TEXT_DIM);
        y += lineHeight;
    }
    if (count == 0) {
        drawText("(no log lines yet)", x, y, 12, TEXT_DIM);
    }
}

void UIApp::drawFinalStats() {
    const Statistics& st = sim.getStats();
    float w = 470, h = 330;
    float bx = mapArea.position.x + (mapArea.size.x - w) / 2;
    float by = mapArea.position.y + (mapArea.size.y - h) / 2;
    drawBox(sf::FloatRect(sf::Vector2f(bx, by), sf::Vector2f(w, h)), sf::Color(30, 27, 25, 240), ACCENT);

    float x = bx + 24;
    float y = by + 18;
    drawText("Simulation finished at t = " + to_string(sim.currentTime()), x, y, 18, ACCENT, true);
    y += 36;

    // Two columns: label on the left, value on the right.
    Str labels[12] = {
        "Completed", "Failed", "Reroutes", "Conflicts", "Reassignments", "Battery rejections",
        "Distance travelled", "Battery used", "Avg / max response", "Robot utilization",
        "Dijkstra runs", "Run time",
    };
    Str values[12] = {
        to_string(st.completed),
        to_string(st.failed) + "  (unreachable " + to_string(st.failedUnreachable)
            + ", not served " + to_string(st.failedNotServed) + ")",
        to_string(st.reroutes),
        to_string(st.conflicts),
        to_string(st.reassignments),
        to_string(st.batteryRejections),
        to_string(st.totalDistance),
        to_string(st.totalBatteryUsed),
        oneDecimal(st.avgResponse) + " / " + to_string(st.maxResponse) + " steps",
        oneDecimal(st.utilization * 100.0) + "%",
        to_string(st.dijkstraRuns),
        oneDecimal(st.runTimeMs) + " ms",
    };
    for (int i = 0; i < 12; ++i) {
        drawText(labels[i], x, y, 14, TEXT_DIM);
        drawText(values[i], x + 170, y, 14, TEXT_MAIN);
        y += 21;
    }
}

void UIApp::drawErrorScreen() {
    const ScenarioLoader& loader = sim.getLoader();
    float x = 40;
    float y = 30;
    drawText("Scenario file is invalid", x, y, 26, sf::Color(240, 90, 90), true);
    y += 40;
    drawText(to_string(loader.errorCount()) + " error(s), " + to_string(loader.warningCount())
             + " warning(s). Fix the file and start again.", x, y, 15, TEXT_DIM);
    y += 34;

    float bottom = window.getView().getSize().y - 30;
    for (int i = 0; i < loader.errorCount() && y < bottom; ++i) {
        const LoadMessage& m = loader.error(i);
        drawText("error    line " + to_string(m.line) + ":  " + m.text, x, y, 14, TEXT_MAIN);
        y += 20;
    }
    for (int i = 0; i < loader.warningCount() && y < bottom; ++i) {
        const LoadMessage& m = loader.warning(i);
        drawText("warning  line " + to_string(m.line) + ":  " + m.text, x, y, 14, TEXT_DIM);
        y += 20;
    }
}

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------
void UIApp::drawText(const Str& text, float x, float y, unsigned size, sf::Color color, bool bold) {
    if (!renderer.hasFont()) {
        return;
    }
    sf::Text label(renderer.getFont(), text, size);
    label.setPosition(sf::Vector2f(x, y));
    label.setFillColor(color);
    if (bold) {
        label.setStyle(sf::Text::Bold);
    }
    window.draw(label);
}

void UIApp::drawBox(const sf::FloatRect& box, sf::Color fill, sf::Color outline) {
    sf::RectangleShape rect(box.size);
    rect.setPosition(box.position);
    rect.setFillColor(fill);
    rect.setOutlineThickness(1.0f);
    rect.setOutlineColor(outline);
    window.draw(rect);
}

bool UIApp::loadFont() {
    // Our bundled font first, then common system fonts (Linux, Windows).
    Str candidates[3] = {
        uiFolder() + "/fonts/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "C:/Windows/Fonts/arial.ttf",
    };
    for (int i = 0; i < 3; ++i) {
        if (renderer.loadFont(candidates[i])) {
            return true;
        }
    }
    return false;
}

Str UIApp::uiFolder() {
    return RESQGRID_UI_DIR;
}

Str UIApp::capabilityShort(Capability cap) {
    switch (cap) {
        case Capability::MEDICAL: return "MED";
        case Capability::FIRE:    return "FIRE";
        case Capability::SUPPLY:  return "SUP";
        case Capability::SEARCH:  return "SRCH";
        case Capability::GENERAL: return "GEN";
    }
    return "?";
}

Str UIApp::oneDecimal(double value) {
    // e.g. 12.345 -> "12.3" (no printf / stream formatting needed).
    bool negative = value < 0;
    if (negative) value = -value;
    long long tenths = (long long)(value * 10.0 + 0.5);
    Str result = to_string(tenths / 10) + "." + to_string(tenths % 10);
    return negative ? "-" + result : result;
}
