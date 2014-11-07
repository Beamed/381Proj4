#include "Model.h"
#include "Sim_object.h"
#include "Agent_factory.h"
#include "Structure_factory.h"
#include "Geometry.h"
#include "View.h"
#include "Agent.h"
#include "Structure.h"
#include <algorithm>
#include <functional>

using std::for_each;
using std::any_of;
using std::string;
using std::bind;
using namespace std::placeholders;

Model* g_Model_ptr = new Model();

const int default_starting_time_c = 0;
//Initializes the initial objects, sets time to start at 0
Model::Model() : time(default_starting_time_c)
{
    //initialize initial objects:
    add_structure(create_structure("Rivendale", "Farm", Point(10., 10.)));
    add_structure(create_structure("Sunnybrook", "Farm", Point(0., 30.)));
    add_structure(create_structure("Shire", "Town_Hall", Point(20., 20.)));
    add_structure(create_structure("Paduca", "Town_Hall", Point(30., 30.)));
	
    add_agent(create_agent("Pippin", "Peasant", Point(5., 10.)));
    add_agent(create_agent("Merry", "Peasant", Point(0., 25.)));
    add_agent(create_agent("Zug", "Soldier", Point(20., 30.)));
    add_agent(create_agent("Bug", "Soldier", Point(15., 20.)));
}

void Model::add_structure(Structure * structure)
{
    objects.insert(structure);
    structures.insert(structure);
}

//Adds the agent to the set of agents; assumes none with same name
void Model::add_agent(Agent * agent)
{
    objects.insert(agent);
    agents.insert(agent);
}

//Destroys all the objects
Model::~Model()
{
    for_each(objects.begin(), objects.end(), [] (Sim_object* obj) {
        delete obj;
    });
}
//Returns true if any object has the name provided
bool Model::is_name_in_use(const string &name) const
{
    return any_of(objects.begin(), objects.end(),
                  bind(Equal_to_obj_ptr_str(), _1, name));
}

bool Model::is_structure_present(const std::string &name) const
{
    return any_of(structures.begin(), structures.end(),
                  bind(Equal_to_obj_ptr_str(), _1, name));
}

//Just calls the get_name fcn to compare names of each pointer
bool Model::Less_than_obj_ptr::operator()(Sim_object *p1, Sim_object *p2)
{
    return (p1->get_name() < p2->get_name());
}
//Returns true if p's name is equal to the name provided
bool Model::Equal_to_obj_ptr_str::operator()(Sim_object* p, const string& name)
{
    return p->get_name()==name;
}
