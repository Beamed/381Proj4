#include "Structure.h"
#include <iostream>

using std::string;
using std::cout;
using std::endl;

const double default_amount_c = 0.0;

Structure::Structure(const string &name_, Point location):
    Sim_object(name_), cur_location(location)
{
    cout << "Structure " << name_ << " constructed" << endl;
}

Structure::~Structure()
{
    cout << "Structure " << get_name() << " destructed" << endl;
}

void Structure::describe() const
{
    cout << get_name() << " at " << cur_location << endl;
}

void Structure::broadcast_current_state()
{
    //TODO: update model with this info
}

double Structure::withdraw(double amount_to_get)
{
    return default_amount_c;
}


