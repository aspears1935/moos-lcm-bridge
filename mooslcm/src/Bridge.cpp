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
 * MOOS Database and LCM.  
 */

//TODO: Add timestamps to toMoos() calls

// Basic LCM Include
#include <lcm/lcm-cpp.hpp>

// Standard Library Headers
#include <sstream>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> 
#include <vector>
//#include <map>
#include <string>

// MOOS Includes
#include <MOOS/libMOOS/App/MOOSApp.h>
#include "MOOSNode.h"
#include "MsgContainer.h"

// BOOST Includes
#include <boost/thread.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

// RapidXML Parser
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"

// LCM TYPES
#include <lcm/lcm_coretypes.h>
#include "moos_t.hpp"
#include "moos_double_t.hpp"
#include "moos_string_t.hpp"

#define FAIL 1
#define SUCCESS 0

//Declare namespaces in use
using namespace rapidxml;
using namespace std;

vector<MsgContainer> subVec;

/*
 * MOOS to LCM Interface
 */
MOOSNode MOOSApp;
void MOOSInit(const char * sMissionFile){
     const char * sMOOSName = "MOOS_LCM_BRIDGE";
   
     MOOSApp.Run(sMOOSName, sMissionFile);
}

/*
 * LCM to MOOS Interface
 * LCM doesn't have any native message definitions, so we'll create some for MOOS and add support for any other necessary msgs
 */
class Handler_MOOS
{
public:
  ~Handler_MOOS() {}

  void handleMessage(const lcm::ReceiveBuffer* rbuf,
		     const std::string& chan,
		     const moos_lcm_bridge_types::moos_t* msg)
  {
    int i;
    cout << "Received message on channel \"" << chan.c_str() << "\"" <<endl;
    cout << "  timestamp   = " << (long long)msg->timestamp <<endl;
    cout << "  name    = " << msg->name <<endl;
    cout << "  val_str    = " << msg->val_str <<endl;
    cout <<"  val_dbl    = " << msg->val_dbl <<endl;
    cout << "  source    = " << msg->source <<endl;
    cout << "  data_type    = " << msg->data_type <<endl;
    cout <<"  msg_type    = " << msg->msg_type <<endl;
    cout << "  source_community    = " << msg->source_community <<endl;
    if(msg->data_type == 'S')
      MOOSApp.toMOOS(msg->name, msg->val_str);
    else if(msg->data_type == 'D')
      MOOSApp.toMOOS(msg->name, msg->val_dbl);
  }
};


class Handler_MOOS_Double
{
public:
  ~Handler_MOOS_Double() {}

  void handleMessage(const lcm::ReceiveBuffer* rbuf,
		     const std::string& chan,
		     const moos_lcm_bridge_types::moos_double_t* msg)
  {
    cout << "Received moos_double_t message on channel \"" << chan.c_str() << "\"" <<endl;
    cout << "  timestamp   = " << (long long)msg->timestamp <<endl;
    cout << "  value    = " << msg->value <<endl;

    vector<MsgContainer>::iterator it;
    for ( it = subVec.begin() ; it < subVec.end() ; it++ ){
      if(strcmp(it->lcmName.c_str(), chan.c_str()) == 0)
	MOOSApp.toMOOS(it->moosName,msg->value);
//      MOOSApp.toMOOS(msg->name, msg->value);
    }
  }
};

class Handler_MOOS_String
{
public:
  ~Handler_MOOS_String() {}

  void handleMessage(const lcm::ReceiveBuffer* rbuf,
		     const std::string& chan,
		     const moos_lcm_bridge_types::moos_string_t* msg)
  {
    //    int i;
    cout << "Received moos_string_t message on channel \"" << chan.c_str() << "\"" <<endl;
    cout << "  timestamp   = " << (long long)msg->timestamp <<endl;
    cout << "  value    = " << msg->value <<endl;

    vector<MsgContainer>::iterator it;
    for ( it = subVec.begin() ; it < subVec.end() ; it++ ){
      if(strcmp(it->lcmName.c_str(), chan.c_str()) == 0)
	MOOSApp.toMOOS(it->moosName,msg->value);
      //    MOOSApp.toMOOS(msg->name, msg->value);
    }
  }
};


/*
 * Parse the XML file to search for LCM Subscribers
 * Create LCM Subscribers
 * Also create vectors to hold pointers for publishers and subscribers
 * Send the list of LCM publishers to the MOOS App to create the MOOS subscribers
 */
int CreateLCMSub(xml_node<> *node, vector<MsgContainer> *pubVec, vector<MsgContainer> *subVec, lcm::LCM *n){
     // MOOS Publishers to LCM Subscribers (This MOOS app will need to subscribe to these topics)
     // Just create vector of subscribers and call MOOS subscriptions method later
     if(strcmp(node->first_node("direction")->value(),"toLCM") == SUCCESS){
       if(strcmp(node->first_node("lcmtype")->value(),"moos_t") == SUCCESS){
	 pubVec->push_back(MsgContainer(n,
					node->first_node("moosname")->value(), 
					node->first_node("lcmname")->value(), 
					node->first_node("moostype")->value(), 
					node->first_node("lcmtype")->value()));
       }else if(strcmp(node->first_node("lcmtype")->value(),"moos_double_t") == SUCCESS){
	 pubVec->push_back(MsgContainer(n,
					node->first_node("moosname")->value(), 
					node->first_node("lcmname")->value(), 
					node->first_node("moostype")->value(), 
					node->first_node("lcmtype")->value()));
       }else if(strcmp(node->first_node("lcmtype")->value(),"moos_string_t") == SUCCESS){
	 pubVec->push_back(MsgContainer(n,
					node->first_node("moosname")->value(), 
					node->first_node("lcmname")->value(), 
					node->first_node("moostype")->value(), 
					node->first_node("lcmtype")->value()));
       }else{
	 cout << "ERROR PARSING XML CONFIG FILE (to LCM)\n";
	 return FAIL;
       }
	 
     // LCM Publishers to MOOS Subscribers
     // This LCM App will have to subscribe to these topics and publish them to the MOOSDB
     }else if(strcmp(node->first_node("direction")->value(),"toMOOS") == SUCCESS){
       if(strcmp(node->first_node("lcmtype")->value(),"moos_double_t") == SUCCESS){
	 Handler_MOOS_Double MOOSDoubleHandler;
	 //n->subscribe("LCM2MOOS", &Handler_MOOS_Double::handleMessage, &MOOSDoubleHandler);
	 n->subscribe(node->first_node("lcmname")->value(), &Handler_MOOS_Double::handleMessage, &MOOSDoubleHandler);
	 subVec->push_back(MsgContainer(node->first_node("moosname")->value(),
					node->first_node("lcmname")->value()));
       }else if(strcmp(node->first_node("lcmtype")->value(),"moos_string_t") == SUCCESS){
	 Handler_MOOS_String MOOSStringHandler;
	 //n->subscribe("LCM2MOOS", &Handler_MOOS_Double::handleMessage, &MOOSDoubleHandler);
	 n->subscribe(node->first_node("lcmname")->value(), &Handler_MOOS_String::handleMessage, &MOOSStringHandler);
	 subVec->push_back(MsgContainer(node->first_node("moosname")->value(),
					node->first_node("lcmname")->value()));
       }else if(strcmp(node->first_node("lcmtype")->value(),"moos_t") == SUCCESS){
	 Handler_MOOS MOOShandlerObject;
	 //	    n->subscribe(node->first_node("lcmname")->value(), &Handler_MOOS::handleMessage, &MOOShandlerObject);
	 n->subscribe("LCM2MOOS", &Handler_MOOS::handleMessage, &MOOShandlerObject);
	 subVec->push_back(MsgContainer(node->first_node("moosname")->value(),
					node->first_node("lcmname")->value())); 
	 /*
       }else if(strcmp(node->first_node("lcmtype")->value(),"string-binary") == SUCCESS){
	 // Binary-string support
	 subVec->push_back(MsgContainer(//n->subscribe(node->first_node("lcmname")->value(),
					//		&Handler::handleMessage6, &handlerObject),
					node->first_node("moosname")->value(),
	 				node->first_node("lcmname")->value()));  
	 */
       }else{
	 cout << "ERROR PARSING XML CONFIG FILE\n";
	 return FAIL;
       }
     }
     
     return SUCCESS;
}

int main(int argc, char **argv)
{
     if(argc < 3){
       cout << "Invalid number of parameters" << endl;
       cout << "argc is" << argc << ", but it should be at least 3." << endl;
       cout << "Usage:\n\t./moos-lcm-bridge <mooslcmconfig.xml> <mission.moos>" << endl;
       return 0;
     }

     //Initialize LCM Communications
     lcm::LCM lcm;
     if(!lcm.good())
       return 1;
     cout << "LCM STARTED" << endl;
     
     //Read in complete XML Document
     string str,strTotal;
     ifstream in;

     //Check for existence of config files
     struct stat stFileInfo;
     int intStat;
     intStat = stat(argv[1],&stFileInfo);
     if(intStat != 0) {
       cout << "\n******\nCONFIG FILE MISSING\n" << argv[1] << " does not exist!\n******\n";
       return 0;
     }

     //Open file and read the entire file into the strTotal buffer
     in.open(argv[1]);
     getline(in,str);

     while ( in ) {
          strTotal += str;
          getline(in,str);
     }

     //cout << strTotal << endl;
     
     //Convert C++ string to char*
     char * xmlDoc = new char [strTotal.size()+1];
     strcpy (xmlDoc, strTotal.c_str());

     //Parse XML Document
     xml_document<> doc;    // character type defaults to char
     doc.parse<0>(xmlDoc);    // 0 means default parse flags

     vector<MsgContainer> pubVec;

     //Get First Topic/Message
     xml_node<> *node = doc.first_node()->first_node();

     //strcmp(node->first_node("direction")->value(),"toLCM")
     //     cout << node->first_node("direction")->value() << endl;
     if(CreateLCMSub(node,&pubVec,&subVec,&lcm) == FAIL)
       return 0;

     //Process all Topics/Messages
     while( (node = node->next_sibling()) != 0 )
          if(CreateLCMSub(node,&pubVec,&subVec,&lcm) == FAIL)
               return 0;

     //Kick off the MOOS Loop in a separate thread
     //before entering the LCM Loop
     //cout << pubVec.size() << endl;
     MOOSApp.AssignPublisher(&pubVec);
     boost::thread MOOSThread(MOOSInit, argv[2]);

     while (0 == lcm.handle());

     return 0;
}
