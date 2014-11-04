#include "Soldier.h"
#include <iostream>

const int default_strength_c = 2;
const double default_range_c = 2.0;

using std::string;
using std::cout;
using std::endl;

Soldier::Soldier(const string& name_, Point location) :
Agent(name_, location), is_attacking(false), target(nullptr),
strength(default_strength_c), range(default_range_c)
{
    cout << "Soldier " << name_ << " constructed" << endl;
}

Soldier::~Soldier()
{
    cout << "Soldier " << get_name() << " destructed" << endl;
}

void Soldier::update()
{
    Agent::update();
    if(!is_alive() || !is_attacking) {
        return;
    }//do nothing further if dead or not attacking
    //if this far, means alive & attacking
    if(!target->is_alive()) {
        state_Message("Target is dead");
        is_attacking = false;
    }
}
