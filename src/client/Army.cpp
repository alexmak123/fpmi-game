#include "Army.h"

Army::Army(World *world, int _id) :
    attackRad(50, 32),
    moveRad(500, 32),
    id(_id),
    home(world)
{
    units.insert(new Unit(ARMY_POSITIONS[id], 100, 300, 50, 500, 50, ARMY_COLORS[id]));
    moveRad.setFillColor(MOVE_COLOR);
    attackRad.setFillColor(ATTACK_COLOR);
    attackRad.setOrigin(50, 50);
}

void Army::update(float delta)
{
    vector<Unit*> dead;
    for(Unit* u : units) 
        if(!u->update(delta))
            dead.push_back(u);

    for(Unit* u : dead)
    {
        units.erase(u);
        delete u;
    }
}

void Army::render(sf::RenderWindow& window)
{   
    for(Unit* u : units) 
        u->render(window);
    
    if(selected) {
        moveRad.setRadius(selected->getEnergy());
        moveRad.setOrigin(selected->getEnergy(), selected->getEnergy());
        moveRad.setPosition(selected->getPosition());
        window.draw(moveRad);

        if(selected->getEnergy() >= 400) {
            window.draw(attackRad);
            attackRad.setPosition(selected->getPosition());
        }
    }
}

void Army::newMove()
{
    if(selected)
        selected->finishAnimation();
    active = 1;
    // gold += 10;
    for(Unit* u : units)
        u->newMove();
    
    // cout << id << " new move\n";
}

void Army::endMove()
{
    active = 0;
    selected = 0;
    // cout << id << " end move\n";
}

bool Army::recruit(sf::Vector2f point, int unitType) {
    if(unitType == 1) {
        if(gold >= 20){
            gold -= 20;
            units.insert(new Unit(point, 100, 300, 50, 500, 50, ARMY_COLORS[id]));
            return 1;
        }
    }
    return 0;
}

void Army::action(Unit* u, sf::Vector2f point)
{  
    if(selected == 0)
        return;
    if(u == 0) 
    {
        if(selected->moveTo(point)) {
            animation = 1;
            Town* t = home->seizeTown(point);
            if(t)
                t->setOwner(this);
        }
    } 
    else
        selected->attack(u);
}

bool Army::select(sf::Vector2f point) 
{
    if(isAnimating())
        return 0;
    // cout << point << (*units.begin())->getPosition() << endl;
    float normToUnit = 400; // max dist to detect click
    Unit *pointedUnit = 0;
    for (auto u : units)
    {
        if (normToUnit > norm(u->getPosition() - point))
        {
            normToUnit = normToUnit;
            pointedUnit = u;
        }
    }
    if(pointedUnit == 0)
        return 0;
    selected = pointedUnit;
    return 1;
}

const set<Unit*>* Army::getUnits() const 
{
    return &units;
}

bool Army::isAnimating() const
{
    if(!selected)
        return 0;
    return selected->isAnimating();
}

bool Army::unselect()
{
    if(!selected)
        return 1;
    if(selected->isAnimating())
        return 0;
    selected = 0;
    return 1;
}

int Army::getGold() const
{
    return gold;
}

void Army::giveGold(int amount)
{
    gold += amount;
}

int Army::getId()
{
    return id;
}

Army::~Army()
{
    for(auto u : units)
        delete u;
}