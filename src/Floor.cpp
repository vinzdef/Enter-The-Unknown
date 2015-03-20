#include "Floor.hpp"

void Floor::loadTileMap(MapData map)
{
    tileMap.load(tileSheet, map, tileSize); 
    bounds = {0,0,(int)map[0].size() * tileSize, (int)map.size() * tileSize};
}

void Floor::setTileSheet(sf::Texture &ts)
{tileSheet = ts; tileSheet.setSmooth(false);}

void Floor::loadCollidables(MapData map)
{
    map2Layer(map, collidables);
}

void Floor::update(sf::Time deltaT)
{}

void Floor::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(tileMap);

    if (drawBoundings)
        for (auto &g : tileMap.grid)
            if (isInBounds(g.getPosition()))
                target.draw(g);

    for (auto &obj : collidables)
        if (isInBounds(obj.getPosition()))
            target.draw(obj);

    if (drawBoundings)
        for(auto &b : boundings)
            if (isInBounds(b.getPosition()))
                target.draw(b);

    target.draw(exit);
}

const bool Floor::isInBounds(const sf::Vector2f &tilePosition) const
{
    //TILE LEFT X IS > WORLD LEFT X (WORLD CENTER X - HALF WORLD WIDTH)
    bool inLeft = tilePosition.x >= viewBounds.left - (viewBounds.width/2 + drawOffset * tileSize);

    //TILE LEFT X IS < WORLD RIGHT X (WORLD CENTER X + HALF WORLD WIDTH)
    bool inRight = tilePosition.x <= viewBounds.left + (viewBounds.width/2 + drawOffset * tileSize);

    //TILE TOP Y > WORLD TOP Y (WORLD CENTER Y - HALF WORLD HEIGHT)
    bool inTop = tilePosition.y >= viewBounds.top - (viewBounds.height/2 + drawOffset * tileSize);

    //TILE BOTTOM Y < WORLD BOTTOM Y (WORLD CENTER Y + HALF WORLD HEIGHT)
    bool inBottom = tilePosition.y <= viewBounds.top + (viewBounds.height/2 + drawOffset * tileSize);

    if (inLeft && inRight && inTop && inBottom) 
        return true;

    return false;
}

template<typename Object>
void Floor::map2Layer(const MapData &source, std::vector<Object> &destination)
{
    for (int i = 0; i < source.size(); i++)
        for (int j = 0; j < source[i].size(); j++)
            if (source[i][j] >= 0)
            {
                sf::Vector2i tilePosition = {source[i][j] * tileSize, (source[i][j] * tileSize) % ((int)tileSheet.getSize().x / tileSize)};
                Object tile{tileSheet, {tilePosition, {tileSize, tileSize}}};
                tile.setPosition(j * tileSize , i * tileSize);                
                destination.push_back(tile);

                sf::RectangleShape shape;
                shape.setPosition(j * tileSize - tileSize/2, i * tileSize -tileSize/2);
                shape.setSize({(float)tileSize, (float)tileSize});
                shape.setFillColor(sf::Color::Transparent);
                shape.setOutlineColor(sf::Color::Red);
                shape.setOutlineThickness(.5f);
                boundings.push_back(shape);
            }
}