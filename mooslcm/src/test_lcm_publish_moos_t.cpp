// file: test_lcm_publish_moos_t.cpp
// Contact: Anthony Spears <aspears@gatech.edu>
// 
//
// compile with:
//  $ g++ -o send_message send_message.cpp -llcm
//
// On a system with pkg-config, you can also use:
//  $ g++ -o send_message send_message.cpp `pkg-config --cflags --libs lcm`

#include <ctime>
#include <lcm/lcm-cpp.hpp>
#include <unistd.h>
#include <iostream>

#include "moos_t.hpp"


int main(int argc, char ** argv)
{
    lcm::LCM lcm;
    if(!lcm.good())
        return 1;

    std::time_t result = std::time(NULL);
    
    moos_lcm_bridge_types::moos_t msg;
    msg.timestamp = (long) result; 
    msg.name = "test name";
    msg.val_str = "test string";
    msg.val_dbl = 17;
    msg.source = "mysource";
    msg.data_type = 'D';
    msg.msg_type = 'N';
    msg.source_community = "mysoucecommunity";

    lcm.publish("MOOS-LCM-BRIDGE-OUT", &msg);

    return 0;
}
