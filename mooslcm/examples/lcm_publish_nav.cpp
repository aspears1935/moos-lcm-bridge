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

#include "moos_t.hpp"
#include <gss/nav_solution_t.hpp>

int main(int argc, char ** argv)
{
    lcm::LCM lcm;
    if(!lcm.good())
        return 1;
    
    for(int i=0; i<100; i++)
      {
      std::time_t result = std::time(NULL);
        gss::nav_solution_t nav_msg;
	nav_msg.unix_time = (long) result; //msg_time;
	nav_msg.count_publish = 0;
	nav_msg.absolute_position[0]=-84.512518+i*0.00001;
	nav_msg.absolute_position[1]=29.916413-i*.00001;
	nav_msg.absolute_position[2]=0;
	nav_msg.speed_over_ground=1;
	nav_msg.depth=0;
	nav_msg.attitude[0]=0;
	nav_msg.attitude[1]=0;
	nav_msg.attitude[2]=101+5*i;
	
	//	nav_msg.relative_position[0]=0;
	//	nav_msg.relative_position[1]=0;
	//	nav_msg.relative_position[2]=0;
	nav_msg.altitude_above_bottom = 0;
	nav_msg.altitude_ok = true;
	nav_msg.relative_position_ok = true;
	nav_msg.absolute_position_ok = true;
	nav_msg.have_bottom_lock = true;
	
	lcm.publish("OPENINS_NAV_SOLUTION", &nav_msg);
	std::cout << i << std::endl;
	sleep(1);
      }
    return 0;
}
