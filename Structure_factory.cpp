#include "Structure_factory.h"
#include "Town_Hall.h"
#include "Farm.h"
#include "Geometry.h"
#include "Utility.h"

using std::string;

const string farm_name_c = "Farm";
const string town_hall_name_c = "Town_Hall";

Structure* create_structure(const string& name, const string& type,
                            Point location)
{
    if(type == farm_name_c) {
        return new Farm(name, location);
    }
    else if(type == town_hall_name_c) {
        return new Town_Hall(name, location);
    }
    else {
        throw Error("Trying to create structure of unknown type!");
    }
}
