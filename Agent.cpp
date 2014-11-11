#include "Agent.h"
#include "Model.h"
#include "Utility.h"
#include <iostream>

using std::cout;
using std::endl;
using std::string;

const int default_health_c = 5;
const int default_speed_c = 5;

//Constructs the Agent by calling agent and moving_object,
//and then setting the default values for its private variables
Agent::Agent(const std::string& name_, Point location_):
Sim_object(name_), Moving_object(location_, default_speed_c),
health(default_health_c), speed(default_speed_c),
state(Agent_state_e::ALIVE)
{
    cout << "Agent " << name_ << " constructed" << endl;
}
//Destructs by simply announcing its death
Agent::~Agent()
{
    cout << "Agent " << get_name() << " destructed" << endl;
}
//Returns current location by calling get_current_location
Point Agent::get_location() const
{
    return get_current_location();
}
//Returns is_currently_moving; if dead, has already been set to not move
bool Agent::is_moving() const
{
    return is_currently_moving();
}//if is currently dead, is no longer moving.
//Orders the agent to begin moving to the given destination
void Agent::move_to(Point destination_)
{
    if(destination_ == get_current_location()) {
        cout << get_name() << ": I'm already there" << endl;
        return;
    }
    cout << get_name() <<  ": I'm on the way" << endl;
    start_moving(destination_);
}
//Stops moving and announces they've stopped moving
void Agent::stop()
{
    if(is_currently_moving())  {
        cout << get_name() << ": I'm stopped" << endl;
        stop_moving();
    }
}
//Decrements the health lost by the attack by calling lose_health
void Agent::take_hit(int attack_strength, Agent *attacker_ptr)
{
    lose_health(attack_strength);
}
//Loses health; if it dies, shouts "Arrggh!"
//Otherwise shouts "Ouch!" and continues its business
void Agent::lose_health(int attack_strength)
{
    health -= attack_strength;
    if(health <= 0) {
        state = Agent_state_e::DYING;
        stop_moving();
        cout << get_name() << ": Arrggh!" << endl;
        return;
    }
    cout << get_name() << ": Ouch!" << endl;
}
//Updates the state of the agent based on its movement or state of dying.
//If dying, tells Model that it's gone.
void Agent::update()
{
    switch(state) {
        case Agent_state_e::ALIVE:
            update_Movement();
            break;
        case Agent_state_e::DYING:
            g_Model_ptr->notify_gone(get_name());
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
//Calls update location; if there, announces such.
//If not, announces it's taken another step.
//Either way, notifies model.
void Agent::update_Movement()
{
    if(update_location()) {
        cout << get_name() << ": I'm there!" << endl;
    }
    else if(is_currently_moving()) {
        cout << get_name() << ": step..." << endl;
    }
    g_Model_ptr->notify_location(get_name(), get_current_location());
}
//Outputs all information on the current state of the agent.
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
//Tells Model about the current location of the Agent.
void Agent::broadcast_current_state()
{
    g_Model_ptr->notify_location(get_name(), get_location());
}
//Throws error that it can't work
void Agent::start_working(Structure*, Structure*)
{
    throw Error(get_name() + ": Sorry, I can't work!");
}
//Throws error that it can't attack
void Agent::start_attacking(Agent*)
{
    throw Error(get_name() +  + ": Sorry, I can't attack!");
}
