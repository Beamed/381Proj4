#include "Model.h"
#include "Sim_object.h"
#include "Agent_factory.h"
#include "Structure_factory.h"
#include "Geometry.h"
#include "View.h"
#include "Agent.h"
#include "Structure.h"
#include "Utility.h"
#include <functional>//bind
#include <algorithm>//for_each
#include <utility>//make_pair

using std::string;
using std::for_each;
using std::mem_fn;
using std::map;
using std::bind;
using std::list;
using namespace std::placeholders;

//initialize to nullptr:
Model* g_Model_ptr = nullptr;

const int default_starting_time_c = 0;
//Initializes the initial objects, sets time to start at 0
Model::Model() : time(default_starting_time_c)
{
    //initialize initial objects:
    insert_structure(create_structure("Rivendale", "Farm", Point(10., 10.)));
    insert_structure(create_structure("Sunnybrook", "Farm", Point(0., 30.)));
    insert_structure(create_structure("Shire", "Town_Hall", Point(20., 20.)));
    insert_structure(create_structure("Paduca", "Town_Hall", Point(30., 30.)));
	
    insert_agent(create_agent("Pippin", "Peasant", Point(5., 10.)));
    insert_agent(create_agent("Merry", "Peasant", Point(0., 25.)));
    insert_agent(create_agent("Zug", "Soldier", Point(20., 30.)));
    insert_agent(create_agent("Bug", "Soldier", Point(15., 20.)));
}
//Inserts structure and has it broadcast its state
void Model::add_structure(Structure * structure)
{
    insert_structure(structure);
    structure->broadcast_current_state();
}

//Inserts the agent into containers and has it broadcast its state
void Model::add_agent(Agent * agent)
{
    insert_agent(agent);
    agent->broadcast_current_state();
}
//Adds the structure to the map of structures; assumes none with same name
void Model::insert_structure(Structure* structure)
{
    objects.insert(structure);
    structures.insert(make_pair(structure->get_name(), structure));
}

//Adds the agent to the map of agents; assumes none with same name
void Model::insert_agent(Agent* agent)
{
    objects.insert(agent);
    agents.insert(make_pair(agent->get_name(), agent));
}

//Returns the structure pointer with the requested name.
//If not found, will throw Error("Structure not found!")
Structure* Model::get_structure_ptr(const string& name) const
{
    auto struct_ptr = structures.find(name);
    if(struct_ptr == structures.end()) {
        throw Error{"Structure not found!"};
    }
    return struct_ptr->second;
}

//Destroys all the objects
Model::~Model()
{//delete each object pointer, let the containers fall out of scope:
    for_each(objects.begin(), objects.end(), [] (Sim_object* p) {
        delete p;
    });
}
//Returns true if any object has the name provided
bool Model::is_name_in_use(const string &name) const
{//why redo code? :
    return is_agent_present(name) || is_structure_present(name);
}
//Returns true if the structure with the name is present
bool Model::is_structure_present(const string &name) const
{
    return structures.find(name) != structures.end();
}
//Returns true if the agent with the name is present
bool Model::is_agent_present(const string &name) const
{
    return agents.find(name) != agents.end();
}
//Returns an agent with the given name, throws an error if not found.
Agent* Model::get_agent_ptr(const string &name) const
{
    auto agent_iter = agents.find(name);
    if(agent_iter == agents.end()) {
        throw Error{"Agent not found!"};
    }
    return agent_iter->second;
}
//calls the describe function for each of the objects
void Model::describe() const
{
    for_each(objects.begin(), objects.end(), mem_fn(&Sim_object::describe));
}
//calls the update function for each of the objects
//increments the time
void Model::update()
{
    time++;
    for_each(objects.begin(), objects.end(), mem_fn(&Sim_object::update));
    list<Agent*> disappearing_agents;
    for(auto agent_iter = agents.begin(); agent_iter != agents.end();
        ++agent_iter) {
        if(agent_iter->second->is_disappearing())
            disappearing_agents.push_back(agent_iter->second);
    }
    for_each(disappearing_agents.begin(), disappearing_agents.end(),
             bind(&Model::remove_agent, this, _1));
}

//Removes the given agent from each container and deletes them.
void Model::remove_agent(Agent* agent)
{
    string name = agent->get_name();
    agents.erase(name);
    objects.erase(agent);
    delete agent;
}

//compares the two objects lexicographically by calling get_name
bool Model::Less_than_obj_ptr::operator()(Sim_object *p1,
                                          Sim_object *p2)
{
    return p1->get_name() < p2->get_name();
}

//Inserts the view into the list of views
void Model::attach(View *view)
{
    views.push_back(view);
    for_each(objects.begin(), objects.end(),
             mem_fn(&Sim_object::broadcast_current_state));
    //this way we ensure the view is "up to date"
}
//Removes the view from the list of views.
void Model::detach(View *view)
{
    views.remove(view);
}
//calls each view's update_location function to update the location of named obj
void Model::notify_location(const string &name, Point location)
{
    if(views.empty()) return;//do nothing if no views to update
    for_each(views.begin(), views.end(),
             bind(&View::update_location, _1, name, location));
}
//calls each view's update_remove function to remove the named obj
void Model::notify_gone(const std::string &name)
{
    if(views.empty()) return;//do nothing if no views to update
    for_each(views.begin(), views.end(), bind(&View::update_remove, _1, name));
}
