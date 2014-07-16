#include <yarp/os/all.h>
#include <drc_shared/yarp_command_interface.hpp>

/**
 * @brief auxiliary struct to specify a constraint between T and B.
 **/
template<class T, class B> 
struct derived_constraint {
    
    /**
     * @brief constraint: T must be a subclass of B.
     *
     * @param ps a pointer to a T.
     * @return void
     **/
    static void constraints(T* ps) {
        B* pb = ps; 
    }
    
    /**
     * @brief constraint trigger at compile time.
     *
     **/
    derived_constraint() {
        void(*fp)(T*) = constraints; 
    }
};


/**
 * @brief generic module template with a switch interface and a custom rate thread. 
 * The template type T must be a subclass of a yarp::os::RateThread.
 * 
 * @author Luca Muratore (luca.muratore@iit.it)
 **/
template<class T> 
class generic_module: public yarp::os::RFModule {
protected:
    T* thread;
    std::string module_prefix;
    double module_period;
    double thread_period;
    walkman::drc::yarp_switch_interface* switch_interface;
    bool isALive;
    
public: 
    /**
     * @brief constructor of the generic module.
     *
     * @param module_prefix module name.
     * @param module_period period of the module in second.
     * @param thread_period period of the run thread in millisecond.
     **/
    generic_module(std::string module_prefix, double module_period, double thread_period) : module_prefix(module_prefix),
                                                                                            module_period(module_period),
                                                                                            thread_period(thread_period) {
        // check that T is a RateThread subclass (at compile time)
        derived_constraint<T, yarp::os::RateThread>();
        switch_interface = new walkman::drc::yarp_switch_interface(module_prefix);
        // not alive
        isALive = false;
    }
    
    /**
     * @brief create a new custom thread and make it start
     *
     * @return true if the thread correctly starts. False othetwise.
     **/
    bool configure()
    {
        // create the thread 
        thread = new T(thread_period);
        // start the thread 
        if(!thread->start())
        {
            delete thread;
            return false;
        }
	isALive = true;
        return true;
    }

    /**
     * @brief stop and delete the thread.
     *
     * @return always true.
     **/
    bool close() {
        // could happend that isAlive is false here -> close called in automatic after updateModule return false
        if(isALive){
            thread->stop();
            delete thread;
        } 
	isALive = false;
        return true;
    }
    
    /**
     * @brief pause the thread.
     *
     * @return always true
     **/
    virtual bool pause() {
        thread->suspend();
        return true;
    }
    
    /**
     * @brief resume the thread.
     *
     * @return always true
     **/
    virtual bool resume() {
        thread->resume();
        return true;
    }
    
    /**
     * @brief check if the moduel is alive.
     *
     * @return true if the module is alive. False otherwise.
     **/
    bool isAlive() {
        return isALive;
    }
    
    /**
     * @brief getter for the period of the module.
     *
     * @return the module of the period.
     **/
    virtual double getPeriod() {
        return module_period;
    }
    
    /**
     * @brief update module dunction, called every module_period as specified in the constructor.
     * Gets the command from the switch interface and executes it.
     * 
     * Possible commands are: start, stop, pause, resume and quit.
     * 
     *
     * @return false after a quit command. True otherwise.
     **/
    virtual bool updateModule() {
        std::string switch_command;
        // get the command
        if(switch_interface->getCommand(switch_command)) {
            std::cout<<"Switch Interface received: "<<switch_command<<std::endl;
            //stop command
            if(switch_command == "stop") {
                if(this->isAlive()) {
                      std::cout<<"Stopping module"<<std::endl;
                      this->close();
                }
            }
            //start command
            else if(switch_command == "start") {
                if(this->isAlive()) {
                    std::cout<<"Starting module"<<std::endl;
                    this->close();
                }
                std::cout<<"Starting module"<<std::endl;       
                if(this->configure()) {
                    std::cout<<"Module is started"<<std::endl;
                }
                else {
                    std::cout<<"Error starting Module"<<std::endl;
                }
            }
            // pause command
            else if(switch_command == "pause") {
                if(this->isAlive()) {
                std::cout<<"Module Suspended"<<std::endl;
                this->pause();
                }
            }
            // resume command
            else if(switch_command == "resume") {
                if(this->isAlive()) {
                    std::cout<<"Module Resumed"<<std::endl;
                    this->resume();
                }
            }
            // quit command
            else if(switch_command == "quit") {
                std::cout<<"Quit"<<std::endl;
                //stop the module
                /*if(this->isAlive()){
                    std::cout<<"Stopping thread"<<std::endl;
                    this->close();
                }*/
                std::cout<<"Everything is closed"<<std::endl;
                
                //delete switch_interface;
                // stop the module
                return false;
            }
            else {
                std::cout<<switch_command<<" is not vaild"<<std::endl;
            }
        }
        return true;
    }
  
};