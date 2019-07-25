// LCM example program.
//
// compile with:
//  $ g++ -o send_message send_message.cpp -llcm
//
// On a system with pkg-config, you can also use:
//  $ g++ -o send_message send_message.cpp `pkg-config --cflags --libs lcm`

#include <stdio.h>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <ctime>

#include <lcm/lcm-cpp.hpp>

//#include "../moos_lcm_bridge_types/moos_t.hpp"
#include "moos_double_t.hpp"
#include "moos_string_t.hpp"
#include <gss/nav_solution_t.hpp>

class Handler
{
public:
  ~Handler() {}
  
  void handleMessage(const lcm::ReceiveBuffer* rbuf,
		     const std::string& chan,
		     const gss::nav_solution_t* msg)
  {
    using namespace std;
    int i;
    
    lcm::LCM lcm;
    
    if(!lcm.good())
      return;

    std::time_t timestamp_now = std::time(NULL);
    
    printf("Received message on channel \"%s\":\n", chan.c_str());
    printf("  timestamp   = %lld\n", (long long)msg->unix_time);
    printf("  faking time = %lld\n", (long long) timestamp_now);
    printf("  abs position    = (%f, %f, %f)\n",
	   msg->absolute_position[0], msg->absolute_position[1], msg->absolute_position[2]);
    printf("  orientation = (%f, %f, %f)\n",
	   msg->attitude[0], msg->attitude[1],
	   msg->attitude[2]);
    printf("\n");

    //Publish NAV_LAT
    moos_lcm_bridge_types::moos_double_t moos_msg_nav_lat;
    moos_msg_nav_lat.timestamp = timestamp_now; //msg->unix_time;
    //    moos_msg_nav_lat.name = "NAV_LAT";
    moos_msg_nav_lat.value = msg->absolute_position[1];
    //lcm.publish("LCM2MOOS", &moos_msg_nav_lat);
    lcm.publish("NAV_LAT", &moos_msg_nav_lat);

    //Publish NAV_LONG
    moos_lcm_bridge_types::moos_double_t moos_msg_nav_long;
    moos_msg_nav_long.timestamp = timestamp_now; //msg->unix_time;
    //    moos_msg_nav_long.name = "NAV_LONG";
    moos_msg_nav_long.value = msg->absolute_position[0];
    //lcm.publish("LCM2MOOS", &moos_msg_nav_long);
    lcm.publish("NAV_LONG", &moos_msg_nav_long);

    //Publish NAV_HEADING
    moos_lcm_bridge_types::moos_double_t moos_msg_nav_heading;
    moos_msg_nav_heading.timestamp = timestamp_now; //msg->unix_time;
    //    moos_msg_nav_heading.name = "NAV_HEADING";
    moos_msg_nav_heading.value = msg->attitude[2];
    lcm.publish("NAV_HEADING", &moos_msg_nav_heading);

    //Publish NAV_SPEED
    moos_lcm_bridge_types::moos_double_t moos_msg_nav_speed;
    moos_msg_nav_speed.timestamp = timestamp_now; //msg->unix_time;
    //    moos_msg_nav_speed.name = "NAV_SPEED";
    moos_msg_nav_speed.value = msg->speed_over_ground;
    //    std::cout << moos_msg_nav_speed.val_dbl << std::endl;
    lcm.publish("NAV_SPEED", &moos_msg_nav_speed);

    //Publish NAV_DEPTH
    moos_lcm_bridge_types::moos_double_t moos_msg_nav_depth;
    moos_msg_nav_depth.timestamp = timestamp_now; //msg->unix_time;
    //    moos_msg_nav_depth.name = "NAV_DEPTH";
    moos_msg_nav_depth.value = msg->depth;
    lcm.publish("NAV_DEPTH", &moos_msg_nav_depth);

    //Publish IVPHELM_STATE
    moos_lcm_bridge_types::moos_string_t moos_msg_ivphelm_state;
    moos_msg_ivphelm_state.timestamp = timestamp_now; //msg->unix_time;
    //    moos_msg_ivphelm_state.name = "IVPHELM_STATE";
    moos_msg_ivphelm_state.value = "DRIVE";
    lcm.publish("IVPHELM_STATE", &moos_msg_ivphelm_state);
  }
};


int main(int argc, char** argv)
{
  lcm::LCM lcm;
  
  if(!lcm.good())
    return 1;

  Handler handlerObject;
  lcm.subscribe("OPENINS_NAV_SOLUTION", &Handler::handleMessage, &handlerObject);

  while(0 == lcm.handle());

  return 0;
}
