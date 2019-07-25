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
    int counter = 0;
    
    while(lcm.good())
      {
	result = std::time(NULL);
	msg.timestamp = (long) result; 
	msg.value = counter;
	lcm.publish("LCM_COUNTER", &msg);
	counter++;
	sleep(1);
      }
    return 0;
}
