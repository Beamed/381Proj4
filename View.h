/* *** View class ***
The View class encapsulates the data and functions needed to generate the map
display, and control its properties. It has a "memory" for the names and locations
of the to-be-plotted objects.

Usage: 
1. Call the update_location function with the name and position of each object
to be plotted. If the object is not already in the View's memory, it will be added
along with its location. If it is already present, its location will be set to the 
supplied location. If a single object changes location, its location can be separately
updated with a call to update_location. 
2. Call the update_remove function with the name of any object that should no longer
be plotted. This must be done *after* any call to update_location that 
has the same object name since update_location will add any object name supplied.
3. Call the draw function to print out the map. 
4. As needed, change the origin, scale, or displayed size of the map 
with the appropriate functions. Since the view "remembers" the previously updated
information, immediately calling the draw function will print out a map showing the previous objects
using the new settings.
*/

#ifndef VIEW_H
#define VIEW_H

#include <string>
#include <map>
#include "Geometry.h"

class View {
public:
	// default constructor sets the default size, scale, and origin
	View(); 
	
	// Save the supplied name and location for future use in a draw() call
	// If the name is already present,the new location replaces the previous one.
	void update_location(const std::string& name, Point location);
	
	// Remove the name and its location; no error if the name is not present.
	void update_remove(const std::string& name);
	
	// prints out the current map
	void draw();
	
	// Discard the saved information - drawing will show only a empty pattern
	void clear(); 
	
	// modify the display parameters
	// if the size is out of bounds will throw Error("New map size is too big!")
	// or Error("New map size is too small!")
	void set_size(int size_);
	
	// If scale is not postive, will throw Error("New map scale must be positive!");
	void set_scale(double scale_);
	
	// any values are legal for the origin
	void set_origin(Point origin_);
	
	// set the parameters to the default values
	void set_defaults();
private:
    //returns true if point is within the grid, false otherwise
    bool get_subscripts(int &ix, int &iy, Point location);
    
    int size;
    double scale;
    Point origin;
    std::map<std::string, Point> objects;
    
    //Returns the label that "should" be printed,
    //according to the equation given by spec.
    double get_axis_label(int coord, double origin_mod);
};

#endif
