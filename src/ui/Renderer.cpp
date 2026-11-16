// File:       src/ui/Renderer.cpp
// Owner:      Sara
// Purpose:    Drawing the map, robots and requests with SFML 3.
// Depends on: Renderer.h (World, GraphLayout, SFML Graphics), <cmath> for sqrt
// Used by:    UIApp
// Milestone:  M2
#include "Renderer.h"
#include <cmath>

// Colours used for the map itself: warm, earthy, no blue.
static const sf::Color GREEN_BELT(40, 50, 37);        // parks between sectors
static const sf::Color SECTOR_FILL(58, 54, 46);       // built-up sector block
static const sf::Color SECTOR_EDGE(74, 68, 58);
static const sf::Color SECTOR_TEXT(128, 118, 102);
static const sf::Color HILL_DARK(30, 44, 30);
static const sf::Color HILL_LIGHT(44, 62, 40);
static const sf::Color ROAD_COLOR(134, 122, 106);
static const sf::Color BLOCKED_COLOR(232, 58, 44);
static const sf::Color LABEL_COLOR(196, 184, 166);

Renderer::Renderer(sf::RenderTarget& t) : target(t) {}

bool Renderer::loadFont(const Str& path) {
    fontLoaded = font.openFromFile(path);
    return fontLoaded;
}

int Renderer::loadSprites(const Str& folder) {
    hasRobotSprite = robotTexture.loadFromFile(folder + "/robot.png");
    hasPersonSprite = personTexture.loadFromFile(folder + "/person.png");
    if (hasRobotSprite) robotTexture.setSmooth(true);
    if (hasPersonSprite) personTexture.setSmooth(true);
    int count = 0;
    if (hasRobotSprite) count++;
    if (hasPersonSprite) count++;
    return count;
}

// ---------------------------------------------------------------------------
// Colours
// ---------------------------------------------------------------------------
sf::Color Renderer::capabilityColor(Capability cap) {
    switch (cap) {
        case Capability::MEDICAL: return sf::Color(240, 104, 112);
        case Capability::FIRE:    return sf::Color(245, 146, 56);
        case Capability::SUPPLY:  return sf::Color(240, 206, 82);
        case Capability::SEARCH:  return sf::Color(162, 210, 92);
        case Capability::GENERAL: return sf::Color(246, 239, 226);   // Baymax white
    }
    return sf::Color::White;
}

sf::Color Renderer::stateColor(RobotState state) {
    switch (state) {
        case RobotState::AVAILABLE:  return sf::Color(124, 200, 110);
        case RobotState::TRAVELLING: return sf::Color(250, 178, 124);
        case RobotState::PERFORMING: return sf::Color(226, 74, 58);
        case RobotState::RETURNING:  return sf::Color(240, 210, 90);
        case RobotState::RECHARGING: return sf::Color(206, 122, 196);
        case RobotState::OFFLINE:    return sf::Color(120, 112, 104);
    }
    return sf::Color::White;
}

sf::Color Renderer::urgencyColor(int urgency) {
    if (urgency <= 1) return sf::Color(120, 210, 120);
    if (urgency == 2) return sf::Color(190, 220, 90);
    if (urgency == 3) return sf::Color(250, 200, 70);
    if (urgency == 4) return sf::Color(250, 140, 60);
    return sf::Color(240, 60, 60);
}

// ---------------------------------------------------------------------------
// Drawing
// ---------------------------------------------------------------------------
sf::Vector2f Renderer::toScreen(const GraphLayout& layout, int loc) const {
    float side = 40.0f;         // left / right / bottom margin
    float top = 64.0f;          // extra room at the top: robots are drawn above their point
    float x = mapArea.position.x + side + layout.x(loc) * (mapArea.size.x - 2 * side);
    float y = mapArea.position.y + top + layout.y(loc) * (mapArea.size.y - top - side);
    return sf::Vector2f(x, y);
}

void Renderer::draw(const World& world, const GraphLayout& layout) {
    if (layout.count() != world.env.locationCount()) {
        return;                             // layout not computed for this map
    }
    // Smaller symbols when there are many locations.
    int n = world.env.locationCount();
    if (n <= 60)       radius = 8.0f;
    else if (n <= 150) radius = 6.0f;
    else               radius = 3.5f;

    drawCity(world, layout);
    drawRoads(world, layout);
    drawRoutes(world, layout);
    drawLocations(world, layout);
    drawRequests(world, layout);
    drawRobots(world, layout);
}

void Renderer::drawCity(const World& world, const GraphLayout& layout) {
    float left = mapArea.position.x;
    float top = mapArea.position.y;
    float right = left + mapArea.size.x;

    // Green belts: the whole map is park, sectors are drawn on top.
    sf::RectangleShape park(mapArea.size);
    park.setPosition(mapArea.position);
    park.setFillColor(GREEN_BELT);
    target.draw(park);

    // Margalla Hills along the north edge: a row of triangles with
    // different (but always the same) widths and heights.
    float base = top + 46;
    for (int i = 0; ; ++i) {
        float x = left - 30 + i * 52;
        if (x > right + 40) {
            break;
        }
        float height = 16.0f + (float)((i * 37) % 24);
        float width = 80.0f + (float)((i * 53) % 44);
        sf::ConvexShape hill(3);
        hill.setPoint(0, sf::Vector2f(x - width / 2, base));
        hill.setPoint(1, sf::Vector2f(x, base - height));
        hill.setPoint(2, sf::Vector2f(x + width / 2, base));
        hill.setFillColor((i % 2 == 0) ? HILL_DARK : HILL_LIGHT);
        target.draw(hill);
    }
    drawLabel("MARGALLA HILLS", sf::Vector2f(left + 110, top + 10), 10, LABEL_COLOR);

    if (!layout.isGrid()) {
        return;                     // non-grid maps: park and hills only
    }

    // One sector per grid cell. Cell (r, c) lies between location r*cols+c
    // (top-left corner) and location (r+1)*cols+c+1 (bottom-right corner).
    int cols = layout.gridCols();
    int rows = layout.gridRows();
    int n = world.env.locationCount();
    float inset = 7.0f;             // gap = green belt along the roads
    for (int r = 0; r + 1 < rows; ++r) {
        for (int c = 0; c + 1 < cols; ++c) {
            int topLeft = r * cols + c;
            int bottomRight = (r + 1) * cols + c + 1;
            if (bottomRight >= n) {
                continue;           // last row is not full
            }
            sf::Vector2f a = toScreen(layout, topLeft);
            sf::Vector2f b = toScreen(layout, bottomRight);
            float w = b.x - a.x - 2 * inset;
            float h = b.y - a.y - 2 * inset;
            if (w <= 0 || h <= 0) {
                continue;
            }
            sf::RectangleShape block(sf::Vector2f(w, h));
            block.setPosition(sf::Vector2f(a.x + inset, a.y + inset));
            block.setFillColor(SECTOR_FILL);
            block.setOutlineThickness(1.0f);
            block.setOutlineColor(SECTOR_EDGE);
            target.draw(block);

            // Islamabad naming: letters north -> south from D, numbers
            // growing towards the west (so the right-most column is 5).
            Str letter(1, (char)('D' + r));
            int number = 5 + (cols - 2 - c);
            drawLabel(letter + "-" + to_string(number),
                      sf::Vector2f((a.x + b.x) / 2, (a.y + b.y) / 2), 13, SECTOR_TEXT);
        }
    }
}

void Renderer::drawRoads(const World& world, const GraphLayout& layout) {
    for (int c = 0; c < world.env.connectionCount(); ++c) {
        const Connection& road = world.env.connections[c];
        sf::Vector2f a = toScreen(layout, road.a);
        sf::Vector2f b = toScreen(layout, road.b);

        if (road.status == ConnStatus::OPEN) {
            drawLine(a, b, 2.0f, ROAD_COLOR);
        } else {
            // Blocked: red line with a small X in the middle.
            drawLine(a, b, 3.0f, BLOCKED_COLOR);
            sf::Vector2f mid((a.x + b.x) / 2, (a.y + b.y) / 2);
            float s = radius * 0.8f;
            drawLine(sf::Vector2f(mid.x - s, mid.y - s), sf::Vector2f(mid.x + s, mid.y + s), 3.0f, BLOCKED_COLOR);
            drawLine(sf::Vector2f(mid.x - s, mid.y + s), sf::Vector2f(mid.x + s, mid.y - s), 3.0f, BLOCKED_COLOR);
        }
    }
}

void Renderer::drawRoutes(const World& world, const GraphLayout& layout) {
    // The rest of each moving robot's route, faint, in its capability colour.
    for (int i = 0; i < world.robots.size(); ++i) {
        const Robot& robot = world.robots[i];
        if (!robot.isMoving()) {
            continue;
        }
        sf::Color color = capabilityColor(robot.capability);
        color.a = 110;

        sf::Vector2f from = robotPosition(world, robot, layout);
        int at = robot.location;
        if (robot.isOnRoad()) {
            // Finish the current road first.
            at = world.env.connections[robot.onConn].other(robot.location);
            sf::Vector2f to = toScreen(layout, at);
            drawLine(from, to, 3.0f, color);
            from = to;
        }
        for (const LinkedList<int>::Node* node = robot.route.first(); node != nullptr; node = node->next) {
            int next = world.env.connections[node->value].other(at);
            sf::Vector2f to = toScreen(layout, next);
            drawLine(from, to, 3.0f, color);
            from = to;
            at = next;
        }
    }
}

void Renderer::drawLocations(const World& world, const GraphLayout& layout) {
    int n = world.env.locationCount();
    bool showLabels = fontLoaded && n <= 80;

    for (int i = 0; i < n; ++i) {
        const Location& loc = world.env.locations[i];
        sf::Vector2f p = toScreen(layout, i);
        float r = radius;

        if (loc.type == LocationType::JUNCTION) {
            sf::CircleShape dot(r * 0.55f);
            dot.setOrigin(sf::Vector2f(r * 0.55f, r * 0.55f));
            dot.setPosition(p);
            dot.setFillColor(sf::Color(160, 148, 132));
            target.draw(dot);
        } else if (loc.type == LocationType::BUILDING || loc.type == LocationType::DEPOT) {
            sf::RectangleShape box(sf::Vector2f(2 * r, 2 * r));
            box.setOrigin(sf::Vector2f(r, r));
            box.setPosition(p);
            if (loc.type == LocationType::BUILDING) box.setFillColor(sf::Color(185, 150, 110));
            else                                    box.setFillColor(sf::Color(235, 140, 50));
            box.setOutlineThickness(1.0f);
            box.setOutlineColor(sf::Color(30, 30, 40));
            target.draw(box);
        } else if (loc.type == LocationType::HOSPITAL) {
            sf::CircleShape disc(r + 1);
            disc.setOrigin(sf::Vector2f(r + 1, r + 1));
            disc.setPosition(p);
            disc.setFillColor(sf::Color::White);
            target.draw(disc);
            // Red cross: two thin rectangles.
            sf::RectangleShape bar(sf::Vector2f(r * 1.3f, r * 0.4f));
            bar.setOrigin(sf::Vector2f(r * 0.65f, r * 0.2f));
            bar.setPosition(p);
            bar.setFillColor(sf::Color(220, 40, 50));
            target.draw(bar);
            bar.setSize(sf::Vector2f(r * 0.4f, r * 1.3f));
            bar.setOrigin(sf::Vector2f(r * 0.2f, r * 0.65f));
            target.draw(bar);
        } else if (loc.type == LocationType::CHARGING) {
            sf::CircleShape disc(r + 1);
            disc.setOrigin(sf::Vector2f(r + 1, r + 1));
            disc.setPosition(p);
            disc.setOutlineThickness(2.0f);
            if (loc.stationOpen) {
                disc.setFillColor(sf::Color(70, 200, 110));
                disc.setOutlineColor(sf::Color(200, 255, 210));
            } else {
                disc.setFillColor(sf::Color(70, 70, 80));          // station down
                disc.setOutlineColor(BLOCKED_COLOR);
            }
            target.draw(disc);
        } else if (loc.type == LocationType::COMMAND) {
            sf::CircleShape diamond(r + 3, 4);                     // a 4-point circle is a diamond
            diamond.setOrigin(sf::Vector2f(r + 3, r + 3));
            diamond.setPosition(p);
            diamond.setFillColor(sf::Color(226, 74, 58));          // command centre: Baymax red
            diamond.setOutlineThickness(1.5f);
            diamond.setOutlineColor(sf::Color(246, 239, 226));
            target.draw(diamond);
        }

        if (showLabels) {
            drawLabel(to_string(loc.id), sf::Vector2f(p.x, p.y + r + 9), 11, LABEL_COLOR);
        }
    }
}

void Renderer::drawRequests(const World& world, const GraphLayout& layout) {
    // Several requests at one place are stacked; count them per location.
    DynamicArray<int> stacked;
    stacked.assign(world.env.locationCount(), 0);

    float height = radius * 3.2f;                                  // person sprite height
    for (int i = 0; i < world.requests.size(); ++i) {
        const Request& q = world.requests[i];
        if (!q.reported || q.isFinished()) {
            continue;
        }
        int slot = stacked[q.location];
        stacked[q.location] = slot + 1;

        sf::Vector2f p = toScreen(layout, q.location);
        p.x += radius + 4 + slot * (radius + 2);                   // to the right of the location
        p.y -= radius;

        sf::Color ring = urgencyColor(q.urgency);
        bool waiting = (q.state == RequestState::PENDING);
        if (!waiting) ring.a = 120;                                // already has a robot

        // Coloured disc under the person = urgency (1 green ... 5 red).
        sf::CircleShape base(radius * 0.7f);
        base.setOrigin(sf::Vector2f(radius * 0.7f, radius * 0.7f));
        base.setPosition(p);
        base.setFillColor(ring);
        if (q.unreachable) {
            base.setOutlineThickness(2.0f);
            base.setOutlineColor(sf::Color(214, 96, 200));         // no route right now
        }
        target.draw(base);

        if (hasPersonSprite) {
            // Soft light halo behind the person so a dark sprite stays visible.
            float haloRadius = height * 0.42f;
            sf::CircleShape halo(haloRadius);
            halo.setOrigin(sf::Vector2f(haloRadius, haloRadius));
            halo.setPosition(sf::Vector2f(p.x, p.y - height * 0.5f));
            sf::Color glow = ring;
            glow.a = waiting ? 70 : 35;
            halo.setFillColor(glow);
            target.draw(halo);

            sf::Sprite person(personTexture);
            sf::Vector2u size = personTexture.getSize();
            float scale = height / (float)size.y;
            person.setOrigin(sf::Vector2f(size.x / 2.0f, (float)size.y));   // feet on the disc
            person.setScale(sf::Vector2f(scale, scale));
            person.setPosition(p);
            if (!waiting) person.setColor(sf::Color(255, 255, 255, 150));
            target.draw(person);
        }
    }
}

void Renderer::drawRobots(const World& world, const GraphLayout& layout) {
    // Robots standing at the same location are placed side by side.
    DynamicArray<int> standing;
    standing.assign(world.env.locationCount(), 0);

    float height = radius * 4.5f;                                  // robot sprite height
    if (!hasRobotSprite) {
        height = radius * 2.6f;                                    // a disc is shorter
    }
    for (int i = 0; i < world.robots.size(); ++i) {
        const Robot& robot = world.robots[i];
        sf::Vector2f p = robotPosition(world, robot, layout);
        if (!robot.isOnRoad() && robot.location != NO_INDEX) {
            int slot = standing[robot.location];
            standing[robot.location] = slot + 1;
            p.x -= slot * (radius * 2.2f);                         // to the left of the location
        }

        sf::Color body = capabilityColor(robot.capability);
        if (robot.state == RobotState::OFFLINE) body.a = 90;

        if (hasRobotSprite) {
            sf::Sprite sprite(robotTexture);
            sf::Vector2u size = robotTexture.getSize();
            float scale = height / (float)size.y;
            sprite.setOrigin(sf::Vector2f(size.x / 2.0f, (float)size.y));   // feet on the point
            sprite.setScale(sf::Vector2f(scale, scale));
            sprite.setPosition(sf::Vector2f(p.x, p.y + radius * 0.5f));
            sprite.setColor(body);                                  // tint by capability
            target.draw(sprite);
        } else {
            sf::CircleShape disc(radius + 2);
            disc.setOrigin(sf::Vector2f(radius + 2, radius + 2));
            disc.setPosition(sf::Vector2f(p.x, p.y - radius));
            disc.setFillColor(body);
            target.draw(disc);
        }

        // State dot (top right) and battery bar (below).
        sf::CircleShape dot(3.5f);
        dot.setOrigin(sf::Vector2f(3.5f, 3.5f));
        dot.setPosition(sf::Vector2f(p.x + radius + 2, p.y - height + 4));
        dot.setFillColor(stateColor(robot.state));
        dot.setOutlineThickness(1.0f);
        dot.setOutlineColor(sf::Color(20, 20, 30));
        target.draw(dot);

        float barWidth = radius * 2.6f;
        float fraction = 0.0f;
        if (robot.maxBattery > 0) {
            fraction = (float)robot.battery / (float)robot.maxBattery;
        }
        sf::RectangleShape back(sf::Vector2f(barWidth, 3.0f));
        back.setPosition(sf::Vector2f(p.x - barWidth / 2, p.y + radius * 0.5f + 2));
        back.setFillColor(sf::Color(40, 40, 50));
        target.draw(back);
        sf::RectangleShape fill(sf::Vector2f(barWidth * fraction, 3.0f));
        fill.setPosition(back.getPosition());
        if (fraction > 0.5f)       fill.setFillColor(sf::Color(110, 220, 130));
        else if (fraction >= 0.3f) fill.setFillColor(sf::Color(240, 210, 80));
        else                       fill.setFillColor(sf::Color(235, 70, 70));
        target.draw(fill);

        drawLabel("R" + to_string(robot.id), sf::Vector2f(p.x, p.y - height - 6), 11, sf::Color::White);
    }
}

sf::Vector2f Renderer::robotPosition(const World& world, const Robot& robot, const GraphLayout& layout) const {
    if (robot.location == NO_INDEX) {
        return sf::Vector2f(mapArea.position.x, mapArea.position.y);
    }
    sf::Vector2f from = toScreen(layout, robot.location);
    if (!robot.isOnRoad()) {
        return from;
    }
    const Connection& road = world.env.connections[robot.onConn];
    sf::Vector2f to = toScreen(layout, road.other(robot.location));
    float f = 0.0f;
    if (road.cost > 0) {
        f = (float)robot.progress / (float)road.cost;
    }
    return sf::Vector2f(from.x + (to.x - from.x) * f, from.y + (to.y - from.y) * f);
}

void Renderer::drawLine(sf::Vector2f a, sf::Vector2f b, float thickness, sf::Color color) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    float length = sqrt(dx * dx + dy * dy);
    if (length < 0.001f) {
        return;
    }
    // Offset at right angles to the line, half the thickness each side.
    float ox = -dy / length * thickness / 2;
    float oy = dx / length * thickness / 2;

    sf::ConvexShape quad(4);
    quad.setPoint(0, sf::Vector2f(a.x + ox, a.y + oy));
    quad.setPoint(1, sf::Vector2f(b.x + ox, b.y + oy));
    quad.setPoint(2, sf::Vector2f(b.x - ox, b.y - oy));
    quad.setPoint(3, sf::Vector2f(a.x - ox, a.y - oy));
    quad.setFillColor(color);
    target.draw(quad);
}

void Renderer::drawLabel(const Str& text, sf::Vector2f centre, unsigned size, sf::Color color) {
    if (!fontLoaded) {
        return;
    }
    sf::Text label(font, text, size);
    sf::FloatRect bounds = label.getLocalBounds();
    label.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2, bounds.position.y + bounds.size.y / 2));
    label.setPosition(centre);
    label.setFillColor(color);
    target.draw(label);
}
