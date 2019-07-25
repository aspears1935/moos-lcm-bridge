
//* The MOOS LCM Bridge
/**
 * 
 * @file
 *
 * @author Anthony Spears <anthony.spears@eas.gatech.edu>
 *
 * @version 1.0
 *
 * @date June 1 2018
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

//TODO: Move lcm typedefs to include folder
#include "MOOSNode.h"
#include <lcm/lcm-cpp.hpp>
#include <lcm/lcm_coretypes.h>
#include "moos_t.hpp"
#include "moos_double_t.hpp"
#include "moos_string_t.hpp"
#include <vector>

//default constructor
MOOSNode::MOOSNode()
{ 
}

void MOOSNode::AssignPublisher(vector<MsgContainer> *new_msgVec){
     msgVec = new_msgVec;
}

//default (virtual) destructor 
MOOSNode::~MOOSNode(){
}

bool MOOSNode::toMOOS(std::string moosName, double data){
  //TODO: Change MOOSTime() call to bring in the timestamp from the original LCM msg. Also elsewhere.
     m_Comms.Notify(moosName,data,MOOSTime());
     return true;
}

bool MOOSNode::toMOOS(std::string moosName, std::string myString){
     m_Comms.Notify(moosName,myString,MOOSTime());
     return true;
}

//TODO: Support for binary-string from LCM to MOOS
bool MOOSNode::toMOOSBinaryString(std::string moosName, std::string myString){
    m_Comms.Notify(moosName,(void *)myString.c_str(),myString.size(),MOOSTime());
	return true;
}

bool MOOSNode::OnNewMail (MOOSMSG_LIST &NewMail){
     MOOSMSG_LIST::iterator p;
     for( p = NewMail.begin() ; p != NewMail.end() ; p++ ){
	  CMOOSMsg & rMsg = *p;

	  //Every time a new type is added, a new IF statement is
	  //required here to handle the conversion

	  vector<MsgContainer>::iterator it;

	  for ( it = msgVec->begin() ; it < msgVec->end() ; it++ ){
	       if(MOOSStrCmp(rMsg.GetKey(), it->moosName)){
		    if(rMsg.IsDouble() && (strcmp(it->moosType.c_str(),"double") == 0) ){
			 if( it->lcmType == "moos_t"){
			      moos_lcm_bridge_types::moos_t msg;
			      msg.timestamp = rMsg.GetTime();
			      msg.name = rMsg.GetName();
			      msg.val_str = rMsg.GetString();
			      msg.val_dbl = rMsg.GetDouble();
			      msg.source = rMsg.GetSource();
			      msg.data_type = rMsg.m_cDataType;
			      msg.msg_type = rMsg.m_cMsgType;
			      msg.source_community = rMsg.GetCommunity();
			      //it->lcm->publish("MOOS-LCM-BRIDGE",&msg);
			      it->lcm->publish(it->lcmName,&msg);
			 }else if( it->lcmType == "moos_double_t"){
			      moos_lcm_bridge_types::moos_double_t msg;
			      msg.timestamp = rMsg.GetTime(); 
			      //msg.name = rMsg.GetName();
			      msg.value = rMsg.GetDouble();
			      it->lcm->publish(it->lcmName,&msg);
			 }
		    }else if(rMsg.IsString() && (strcmp(it->moosType.c_str(), "string") == 0) ){
		      if( it->lcmType == "moos_t"){
		              moos_lcm_bridge_types::moos_t msg;
			      msg.timestamp = rMsg.GetTime();
			      msg.name = rMsg.GetName();
			      msg.val_str = rMsg.GetString();
			      msg.val_dbl = rMsg.GetDouble();
			      msg.source = rMsg.GetSource();
			      msg.data_type = rMsg.m_cDataType;
			      msg.msg_type = rMsg.m_cMsgType;
			      msg.source_community = rMsg.GetCommunity();
			      //      it->lcm->publish("MOOS-LCM-BRIDGE",&msg);
			      it->lcm->publish(it->lcmName,&msg);
		      }else if( it->lcmType == "moos_string_t"){
			      moos_lcm_bridge_types::moos_string_t msg;
			      msg.timestamp = rMsg.GetTime(); 
			      //msg.name = rMsg.GetName();
			      msg.value = rMsg.GetString();
			      it->lcm->publish(it->lcmName,&msg);
		      }
		    }
	       }//end if MOOSStrCmp
	  }//end Iterator for
     }//end Mailbox for
     return true;
}
 
/* 
   called by the base class when the application has made contact with 
   the MOOSDB and a channel has been opened . Place code to specify what 
   notifications you want to receive here . 
*/ 
bool MOOSNode::OnConnectToServer()
{
     DoRegistrations();
     return true;
}

/*
  Called by the base class periodically. This is where you place code 
  which does the work of the application 
*/ 
bool MOOSNode::Iterate(){
     return true;
}

/* 
   called by the base class before the first :: Iterate is called . Place 
   startup code here − especially code which reads configuration data from the 
   mission file 
*/ 
bool MOOSNode::OnStartUp()
{
     appTick = 1;
     commsTick = 1;

     if(!m_MissionReader.GetConfigurationParam("AppTick",appTick)){
	  MOOSTrace("Warning, AppTick not set.\n");
     }
  
     if(!m_MissionReader.GetConfigurationParam("CommsTick",commsTick)){
	  MOOSTrace("Warning, CommsTick not set.\n");
     }

     SetAppFreq(appTick);
     SetCommsFreq(commsTick);
  
     DoRegistrations();

     return true;
}

void MOOSNode::DoRegistrations(){
     vector<MsgContainer>::iterator it;
     cout << "MOOS Registering for " << msgVec->size() << " channels" << endl;

     for ( it = msgVec->begin() ; it < msgVec->end() ; it++ ){

       m_Comms.Register(it->moosName,0);

	  char buf[512];
	  sprintf(buf,"Subscribing to %s\n",it->moosName.c_str());
	  MOOSTrace(buf);
     } 
}
