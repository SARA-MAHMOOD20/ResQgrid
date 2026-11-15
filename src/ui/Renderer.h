// File:       src/ui/Renderer.h
// Owner:      Sara
// Purpose:    Draws the current World with SFML 3: roads (blocked in red),
//             locations by type, robots (including partway along a road),
//             requests by urgency.
// Depends on: World (read only), GraphLayout, SFML 3 Graphics
// Used by:    UIApp
// Milestone:  M2 (loaded map drawn), M4 (polish)
//
// SFML 3 notes: sf::Font::openFromFile (not loadFromFile); sf::Text needs a
// font in its constructor; positions are sf::Vector2f.
#ifndef RESQGRID_RENDERER_H
#define RESQGRID_RENDERER_H

#include <SFML/Graphics.hpp>
#include "../core/Types.h"
#include "../core/World.h"
#include "GraphLayout.h"

class Renderer {
public:
    explicit Renderer(sf::RenderTarget& target);

    // TODO(Sara): font.openFromFile(path); remember whether it worked
    // (labels are skipped if no font is available).
    // See: plan Section 6 (Renderer)   |   Done when: location IDs visible on screen
    bool loadFont(const Str& path);

    // TODO(Sara): drawRoads, drawLocations, drawRequests, drawRobots (that order: robots on top).
    // See: plan Section 6   |   Done when: provided file is readable on screen
    void draw(const World& world, const GraphLayout& layout);

    bool hasFont() const { return fontLoaded; }
    const sf::Font& getFont() const { return font; }

private:
    // TODO(Sara): one line per connection; OPEN grey, BLOCKED red.
    // See: plan Section 6   |   Done when: blocked roads stand out
    void drawRoads(const World& world, const GraphLayout& layout);

    // TODO(Sara): one shape/colour per LocationType; down chargers look different.
    // See: plan Section 6   |   Done when: every location type is recognisable
    void drawLocations(const World& world, const GraphLayout& layout);

    // TODO(Sara): marker at the request location, colour by urgency 1-5; only reported, unfinished ones.
    // See: plan Section 6   |   Done when: pending requests visible by urgency
    void drawRequests(const World& world, const GraphLayout& layout);

    // TODO(Sara): circle per robot at robotPosition(); colour by state.
    // See: plan Section 6   |   Done when: robots move smoothly along roads
    void drawRobots(const World& world, const GraphLayout& layout);

    // TODO(Sara): at a location -> its point; on a road -> interpolate between
    // the two ends by progress / cost.
    // See: report Section 4.1 (position on a road)   |   Done when: robot drawn partway along a road
    sf::Vector2f robotPosition(const World& world, const Robot& robot, const GraphLayout& layout) const;

    sf::RenderTarget& target;
    sf::Font          font;
    bool              fontLoaded = false;
};

#endif // RESQGRID_RENDERER_H
