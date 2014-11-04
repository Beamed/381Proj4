/* A Structure is a Sim_object with a location and interface to derived types */

#include <string>
#include "Geometry.h"
#include "Sim_object.h"

class Structure : public Sim_object {
    
public:
    //constructor takes name and location of structure
    Structure(const std::string &name_, Point location);
    //Destructor made void to enforce abstract class
    virtual ~Structure();
    //Returns the current location
    Point get_location() { return cur_location; }
    
    //Per the spec, does nothing.
    void update(){}
    
    // output information about the current state
    virtual void describe() const;
    
    // ask model to notify views of current state
    void broadcast_current_state();
    
    // fat interface for derived types
    virtual double withdraw(double amount_to_get);
    //does nothing:
    virtual void deposit(double amount_to_give) {}
private:
    Point cur_location;
};
