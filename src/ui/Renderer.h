// File:       src/ui/Renderer.h
// Owner:      Sara
// Purpose:    Draws the current World with SFML 3, styled as an Islamabad-like
//             city (sector blocks, green belts, Margalla Hills to the north)
//             in a warm Big Hero 6 palette: roads (blocked in red), locations
//             by type, the remaining route of moving robots, requests by
//             urgency, robots (including partway along a road) with a battery bar.
//             The city look is decoration only: it never changes the map data.
// Depends on: World (read only), GraphLayout, SFML 3 Graphics
// Used by:    UIApp
// Milestone:  M2 (loaded map drawn), M4 (polish)
//
// Sprites are optional: if <folder>/robot.png or <folder>/person.png is
// missing, plain shapes are drawn instead. robot.png should be light/white:
// it is tinted with the robot's capability colour.
// SFML 3 notes: sf::Font::openFromFile, sf::Texture::loadFromFile return
// bool; sf::Text and sf::Sprite have no default constructor, so they are
// created where they are drawn.
#ifndef RESQGRID_RENDERER_H
#define RESQGRID_RENDERER_H

#include <SFML/Graphics.hpp>
#include "../core/Types.h"
#include "../core/World.h"
#include "GraphLayout.h"

class Renderer {
public:
    explicit Renderer(sf::RenderTarget& target);

    // Loads a .ttf font; labels are skipped if this fails.
    bool loadFont(const Str& path);

    // Loads robot.png and person.png from `folder`. Returns how many loaded (0-2).
    int loadSprites(const Str& folder);

    // Rectangle of the window where the map is drawn.
    void setMapArea(const sf::FloatRect& area) { mapArea = area; }

    // Draws everything, back to front: roads, routes, locations, requests, robots.
    void draw(const World& world, const GraphLayout& layout);

    // Screen position of a location (layout 0..1 scaled into the map area).
    sf::Vector2f toScreen(const GraphLayout& layout, int loc) const;

    bool hasFont() const { return fontLoaded; }
    const sf::Font& getFont() const { return font; }

    // Shared colours (the UI panels use the same ones).
    static sf::Color capabilityColor(Capability cap);
    static sf::Color stateColor(RobotState state);
    static sf::Color urgencyColor(int urgency);

private:
    // Background: green belts, Margalla Hills along the top and, for grid
    // maps, one sector block per grid cell labelled like Islamabad (letters
    // north to south starting at D, numbers growing towards the west).
    void drawCity(const World& world, const GraphLayout& layout);
    void drawRoads(const World& world, const GraphLayout& layout);
    void drawRoutes(const World& world, const GraphLayout& layout);
    void drawLocations(const World& world, const GraphLayout& layout);
    void drawRequests(const World& world, const GraphLayout& layout);
    void drawRobots(const World& world, const GraphLayout& layout);

    // At a location -> its point; on a road -> between the two ends,
    // progress / cost of the way from `location` (Report 4.1).
    sf::Vector2f robotPosition(const World& world, const Robot& robot, const GraphLayout& layout) const;

    // A straight line `thickness` pixels wide (a thin four-cornered shape).
    void drawLine(sf::Vector2f a, sf::Vector2f b, float thickness, sf::Color color);

    // Text centred on a point (does nothing without a font).
    void drawLabel(const Str& text, sf::Vector2f centre, unsigned size, sf::Color color);

    sf::RenderTarget& target;
    sf::Font          font;
    bool              fontLoaded = false;
    sf::Texture       robotTexture;
    bool              hasRobotSprite = false;
    sf::Texture       personTexture;
    bool              hasPersonSprite = false;
    sf::FloatRect     mapArea;
    float             radius = 7.0f;      // location size, set from the map size in draw()
};

#endif // RESQGRID_RENDERER_H
