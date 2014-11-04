/* The Sim_object class provides the interface for all of simulation objects. 
It also stores the object's name, and has pure virtual accessor functions for 
the object's position and other information. */
#include <string>

class Point;//incomplete fwd declaration

class Sim_object {
public:
	Sim_object(const std::string& name_);
    
    virtual ~Sim_object();
	
	const std::string& get_name() const
    {return name;}
			
	// ask model to notify views of current state
    virtual void broadcast_current_state() {}
    
    virtual Point get_location() const;
    virtual void describe() const;
    virtual void update();

private:
	std::string name;
};

