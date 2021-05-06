#include "Army.h"

Army::Army(int _id) :
    attackRad(50, 32),
    id(_id)
{
    units.insert(new Unit(ARMY_POSITIONS[id],  1, 100, 300, 40, 150, 50, ARMY_COLORS[id] ));
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
        if(selected->canAttack()) {
            attackRad.setPosition(selected->getPosition());
            float r = selected->getAtkRad();
            attackRad.setRadius(r);
            attackRad.setOrigin(sf::Vector2f(r,r));
            window.draw(attackRad);
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

bool Army::recruit(sf::Vector2f point, int unitType) 
{
    switch ( unitType )
    {
    case 1:
        if(gold >= 20){
            gold -= 20;
            units.insert(new Unit(point, 1, 100, 300, 40, 150, 50, ARMY_COLORS[id]));
            return 1;
        }
        else
            return 0;
        break;                          
    case 2:
        if(gold >= 50){
            gold -= 50;
            units.insert(new Unit(point, 2, 80, 300, 40, 150, 300, ARMY_COLORS[id]));
            return 1;
        }
        else
            return 0;
        break; 
    case 3:
        if(gold >= 80){
            gold -= 80;
            units.insert(new Unit(point, 3, 250, 300, 80, 100, 50, ARMY_COLORS[id]));
            return 1;
        }
        else
            return 0;
        break;
    case 4:
        if(gold >= 120){
            gold -= 120;
            units.insert(new Unit(point, 4, 100, 300, 30, 250, 50, ARMY_COLORS[id]));
            return 1;
        }
        else
            return 0;
        break;
    
    default:
        return 0;
        break;
    }

    return 0;
}

void Army::attack(Unit* u)
{
    if(selected) selected->attack(u);
}

void Army::moveTo(std::deque<sf::Vector2f> _path, float newEnergy)
{
    if(selected)
        selected->moveTo(_path, newEnergy);
}

Unit* Army::getSelectedUnit() const 
{
    return selected;
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