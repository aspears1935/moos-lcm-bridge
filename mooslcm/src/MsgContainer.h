//* The MOOS LCM Bridge
/**
 * 
 * @file
 *
 * @author Anthony Spears <anthony.spears@eas.gatech.edu>
 *
 * @version 1.0
 *
 * @date June 1, 2018
 * 
 * @section LICENSE
 * 
 * Georgia Institute of Technology
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * The MOOS LCM Bridge allows for communication between a 
 * MOOS Database and an LCM application.  
 */

#ifndef _MSG_CONTAINER_H_ 
#define _MSG_CONTAINER_H_

#include <lcm/lcm-cpp.hpp>
#include <string.h>

using namespace std;

class MsgContainer{
public:
	// from MOOS to LCM
  MsgContainer(lcm::LCM *new_lcm, string new_moosName, string new_lcmName, string new_moosType, string new_lcmType){
	  //		pub      = new_pub;
          lcm = new_lcm;
	  moosName = new_moosName;
	  lcmName  = new_lcmName;
	  moosType = new_moosType;
	  lcmType  = new_lcmType;
	}

	// from LCM to MOOS
	MsgContainer(string new_moosName, string new_lcmName){
	  //	        sub      = new_sub;
	  //lcm = new_lcm
	  moosName = new_moosName;
	  lcmName  = new_lcmName;
	}

	lcm::LCM *lcm;

	string moosName;
	string lcmName;
	string moosType;
	string lcmType;
};

#endif
