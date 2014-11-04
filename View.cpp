#include "View.h"
#include "Geometry.h"
#include "Utility.h"
#include <cmath>
#include <iostream>

using std::cout;
using std::endl;

const int default_size_c = 25;
const double default_scale_c = 2.0;
const double default_origin_x_c = -10.0;
const double default_origin_y_c = -10.0;
const int min_map_size_c = 7;
const int max_map_size_c = 30;
const double lower_scale_bound_c = 0.0;

//Constructs the View with the default values according to spec.
View::View() : size(default_size_c), scale(default_scale_c),
    origin(default_origin_x_c, default_origin_y_c)
{}
//Updates or creates the location of any object w/the given name
void View::update_location(const std::string &name, Point location)
{
    objects[name] = location;//just overwrite old one; otherwise
    //ignore. easy-peasy!
}
//removes the given name from the map of objects
//if no object matches, does nothing.
void View::update_remove(const std::string &name)
{
    objects.erase(name);
}
//draws the map as specified to stdout
void View::draw()
{
    cout << "Display size: " <<
        size << ", scale: " << scale << ", origin: " <<
        origin << endl;
    
    //TODO: this
    
}
//clears the saved information
void View::clear()
{
    objects.clear();
}
//sets the size of the view to the given size.
//throws an error if out of bounds.
void View::set_size(int size_)
{
    if(size_ > max_map_size_c) {
        throw Error("New map size is too big!");
    }
    if(size_ < min_map_size_c) {
        throw Error("New map size is too small!");
    }
    size = size_;
}

//sets the scale to the given scale number
//Throws an error if it is out of bounds.
void View::set_scale(double scale_)
{
    if(scale_ <= lower_scale_bound_c) {
        throw Error("New map scale must be positive!");
    }
    scale = scale_;
}

void View::set_origin(Point origin_) {
    origin = origin_;
}

// Calculate the cell subscripts corresponding to the supplied location parameter, 
// using the current size, scale, and origin of the display. 
// This function assumes that origin is a  member variable of type Point, 
// scale is a double value, and size is an integer for the number of rows/columns 
// currently being used for the grid.
// Return true if the location is within the grid, false if not
bool View::get_subscripts(int &ix, int &iy, Point location)
{
	// adjust with origin and scale
	Cartesian_vector subscripts = (location - origin) / scale;
	// truncate coordinates to integer after taking the floor
	// floor function will return the largest integer smaller than the supplied value
	// even for negative values, so -0.05 => -1., which will be outside the array.
	ix = int(floor(subscripts.delta_x));
	iy = int(floor(subscripts.delta_y));
	// if out of range, return false
	if ((ix < 0) || (ix >= size) || (iy < 0) || (iy >= size)) {
		return false;
		}
	else
		return true;
}

