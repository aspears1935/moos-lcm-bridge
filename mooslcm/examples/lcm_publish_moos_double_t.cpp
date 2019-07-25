// file: send_message.cpp
//
// LCM example program.
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

#include "moos_double_t.hpp"

int main(int argc, char ** argv)
{
    lcm::LCM lcm;
    if(!lcm.good())
        return 1;
    
    std::time_t result = std::time(NULL);
    moos_lcm_bridge_types::moos_double_t msg;
    msg.timestamp = (long) result; //msg_time;
    msg.value = 17;
    lcm.publish("NAV_HEADING", &msg);
        
    return 0;
}
