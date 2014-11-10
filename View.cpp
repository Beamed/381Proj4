#include "View.h"
#include "Geometry.h"//points
#include "Utility.h"//errors
#include <cmath>//get_subscripts
#include <iostream>//cout and endl
#include <vector>//our container for the map
#include <iomanip>//setw and precision
#include <algorithm>//std::copy
#include <iterator>//ostream_iterator

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::setw;
using std::ios;
using std::copy;
using std::ostream_iterator;

const int default_size_c = 25;
const double default_scale_c = 2.0;
const double default_origin_x_c = -10.0;
const double default_origin_y_c = -10.0;
const int min_map_size_c = 7;
const int max_map_size_c = 30;
const double lower_scale_bound_c = 0.0;
const string empty_tile_c = ". ";
const string multiple_objs_in_tile_c = "* ";
const int num_chars_per_tile_c = 2;
const int axis_print_frequency_c = 3;


//Constructs the View with the default values according to spec.
View::View() : size(default_size_c), scale(default_scale_c),
    origin(default_origin_x_c, default_origin_y_c)
{}
//Updates or creates the location of any object w/the given name
void View::update_location(const string &name, Point location)
{
    objects[name] = location;//just overwrite old one; otherwise
    //ignore. easy-peasy!
}
//removes the given name from the map of objects
//if no object matches, does nothing.
void View::update_remove(const string &name)
{
    objects.erase(name);
}
//draws the map as specified to stdout
void View::draw()
{
    cout << "Display size: " <<
        size << ", scale: " << scale << ", origin: " <<
        origin << endl;
    vector<vector<string>> map(size, vector<string>(size, empty_tile_c));
    //create a 30x30 vector of strings initialized to the default empty tile
    bool first_outside = true;//so we know if we print ", " before an
    //object outside the map or not
    for(const auto &object: objects) {
        int x, y;//x and y location of object
        if(get_subscripts(x, y, object.second)) {
            if(map[y][x] == empty_tile_c)
                map[y][x] = object.first.substr(num_chars_per_tile_c);
            else
                map[y][x] = multiple_objs_in_tile_c;
        }
        else {
            if(!first_outside) {
                cout << ", ";
            }
            cout << object.second;
            first_outside = false;//none of the others will be first outside
            //if any others
        }
    }
    //now that we have built our vector, we are safe to output:
    for(int y = 0; y < size; y++) {
        int map_y = (size - y) * scale;//to calculate if we need to print this
        if(map_y % axis_print_frequency_c == 0) {
            auto old_settings = cout.precision();//save old settings
            cout << std::fixed << std::setprecision(0);
            double real_y = ((size - y) * scale) + origin.y;
            cout << setw(4) << real_y << " ";
            cout.precision(old_settings);//restore old settings
        }
        else {
            cout << "     ";
        }
        for(const vector<string>& line : map) {
            ostream_iterator<string> out_iter(cout);
            copy(line.begin(), line.end(), out_iter);
        }
    }//we have now printed each row, now to print the x axis
    for(int x = 0; x < size; x++) {
        int map_x = (size - x) * scale;//to determine if we print here:
        if(map_x % axis_print_frequency_c == 0) {
            auto old_settings = cout.precision();
            cout << std::fixed << std::setprecision(0);
            cout << "  " << setw(4) << ((size - x) * scale) + origin.x;
            cout.precision(old_settings);
        }
    }
    cout << endl;//phew; done!
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
//sets the origin equal to the specified origin
void View::set_origin(Point origin_) {
    origin = origin_;
}
//restores parameters to the default values
void View::set_defaults()
{
    size = default_size_c;
    scale = default_scale_c;
    origin = Point(default_origin_x_c, default_origin_y_c);
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

