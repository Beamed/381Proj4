#include "Agent.h"
#include "Model.h"
#include "Utility.h"
#include <iostream>

using std::cout;
using std::endl;
using std::string;

const int default_health_c = 5;
const int default_speed_c = 5;

Agent::Agent(const std::string& name_, Point location_):
Sim_object(name_), Moving_object(location_, default_speed_c),
health(default_health_c), speed(default_speed_c),
state(Agent_state_e::ALIVE)
{
    cout << "Agent " << name_ << " constructed" << endl;
}

Agent::~Agent()
{
    cout << "Agent " << get_name() << " destructed" << endl;
}

Point Agent::get_location() const
{
    return get_current_location();
}

bool Agent::is_moving() const
{
    return is_currently_moving();
}//if is currently dead, is no longer moving.

void Agent::move_to(Point destination_)
{
    if(destination_ == get_current_location()) {
        state_Message("I'm already there");
        return;
    }
    state_Message("I'm on the way");
    start_moving(destination_);
}

void Agent::stop()
{
    state_Message("I'm stopped");
    stop_moving();
}

void Agent::state_Message(const string& msg) const
{
    cout << get_name() << ": " << msg;
}

void Agent::take_hit(int attack_strength, Agent *attacker_ptr)
{
    lose_health(attack_strength);
    
}

void Agent::lose_health(int attack_strength)
{
    health -= attack_strength;
    if(health <= 0) {
        state = Agent_state_e::DYING;
        stop_moving();
        state_Message("Arrggh!");
        return;
    }
    state_Message("Ouch!");
}

void Agent::update()
{
    switch(state) {
        case Agent_state_e::ALIVE:
            update_Movement();
            break;
        case Agent_state_e::DYING:
            //TODO: notify Model to tell Views it's no longer there
            state = Agent_state_e::DEAD;
            break;
        case Agent_state_e::DEAD:
            state = Agent_state_e::DISAPPEARING;
            break;
        case Agent_state_e::DISAPPEARING:
            //do nothing
            break;
    }
}

void Agent::update_Movement()
{
    if(update_location()) {
        state_Message("I'm there!");
        return;
    }
    state_Message("step...");
    //TODO: notify Model to tell Views it's location has changed
}

void Agent::describe() const
{
    cout << get_name() << " at " << get_current_location() << endl;
    cout << "   Health is " << health << endl;
    if(is_moving()) {
        cout << "   Moving at speed " << speed << " to " <<
            get_current_destination() << endl;
    }
    else {
        cout << "   Stopped" << endl;
    }
    switch(state) {
        case Agent_state_e::DYING:
            cout << "   Is dying" << endl;
            break;
        case Agent_state_e::DEAD:
            cout << "   Is dead" << endl;
            break;
        case Agent_state_e::DISAPPEARING:
            cout << "   Is disappearing" << endl;
            break;
        default:
            //do nothing
            break;
    }
}

void Agent::broadcast_current_state()
{
    //TODO: update Model here
}

void Agent::start_working(Structure*, Structure*)
{
    throw Error(get_name() + ": Sorry, I can't work!");
}

void Agent::start_attacking(Agent*)
{
    throw Error(get_name() +  + ": Sorry, I can't attack!");
}
