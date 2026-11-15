// File:       src/ui/Renderer.cpp
// Owner:      Sara
// Purpose:    Drawing the map, robots and requests with SFML 3.
// Depends on: Renderer.h (World, GraphLayout, SFML Graphics)
// Used by:    UIApp
// Milestone:  M2
#include "Renderer.h"

Renderer::Renderer(sf::RenderTarget& t) : target(t) {}

bool Renderer::loadFont(const Str& path) {
    // TODO(Sara): fontLoaded = font.openFromFile(path); return fontLoaded.
    // See: plan Section 6   |   Done when: labels appear when the font exists, no crash when it doesn't
    (void)path;
    return false;
}

void Renderer::draw(const World& world, const GraphLayout& layout) {
    // TODO(Sara): call the four draw helpers in order.
    // See: plan Section 6   |   Done when: provided file is readable on screen
    (void)world;
    (void)layout;
}

void Renderer::drawRoads(const World& world, const GraphLayout& layout) {
    // TODO(Sara): sf::VertexArray(sf::PrimitiveType::Lines) or thin rectangles.
    // See: plan Section 6   |   Done when: blocked roads in red
    (void)world;
    (void)layout;
}

void Renderer::drawLocations(const World& world, const GraphLayout& layout) {
    // TODO(Sara): sf::CircleShape / sf::RectangleShape per location type.
    // See: plan Section 6   |   Done when: every type recognisable
    (void)world;
    (void)layout;
}

void Renderer::drawRequests(const World& world, const GraphLayout& layout) {
    // TODO(Sara): see Renderer.h.
    // See: plan Section 6   |   Done when: pending requests visible by urgency
    (void)world;
    (void)layout;
}

void Renderer::drawRobots(const World& world, const GraphLayout& layout) {
    // TODO(Sara): see Renderer.h.
    // See: plan Section 6   |   Done when: robots drawn partway along roads
    (void)world;
    (void)layout;
}

sf::Vector2f Renderer::robotPosition(const World& world, const Robot& robot, const GraphLayout& layout) const {
    // TODO(Sara): see Renderer.h.
    // See: report Section 4.1   |   Done when: robot drawn partway along a road
    (void)world;
    (void)robot;
    (void)layout;
    return sf::Vector2f(0.f, 0.f);
}
