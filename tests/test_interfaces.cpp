#include <yarp/os/Network.h>
#include <drc_shared/yarp_command_interface.hpp>
#include <drc_shared/yarp_status_interface.h>

using namespace walkman::drc;

int main(int argc, char* argv[])
{
    yarp::os::Network yarp;
    if(!yarp.checkNetwork()){
        std::cout<<"yarpserver not running, pls run yarpserver"<<std::endl;
        return 0;}
        yarp.init();
 
    yarp_command_interface a("test_module");
    yarp_switch_interface b("test_module");
    yarp_status_interface c("test_module");
        
}
