Installation Notes
==================

The MOOS/LCM Bridge was developed and tested on Ubuntu. Contact Anthony Spears
<aspears@gatech.edum> if you have difficulties installing or using the
MOOS/LCM Bridge

Commands that should be entered at the terminal are denoted with a '$'

Dependencies
==============
LCM - https://lcm-proj.github.io/
MOOS - http://oceanai.mit.edu/moos-ivp/pmwiki/pmwiki.php?n=Site.Download

Compatability
================
As of December 10th, 2018, I have tested moos-lcm with LCM 1.3.1 and moos-ivp 17.7.

Acquiring the Code
===================
Install git  
$ sudo apt-get install git-core

Make a clone of the repo  
$ git clone git://github.gatech.edu/aspears3/moos-lcm-bridge

Building the Code
==================
Navigate to top-level project directory

Build moos-lcm  
$ cd ~/moos-lcm-bridge/mooslcm
$ cmake .
$ make

Add the bin directory to the user's shell path
================================================
Edit the .bashrc file to include the bin directories
Add the following line:
PATH=$PATH:~/moos-lcm-bridge/mooslcm/bin

Launch Example
===============
Run the MOOS mission file
$ pAntler missions/counter/counter.moos

In the uMS window, observe MOOSDB state  
Make sure HostName is LOCALHOST and Port is 9000. Click on "Connect"

In a new terminal, start the moos-lcm bridge
$ MOOS_LCM_BRIDGE missions/counter/counter.xml missions/counter/counter.moos

In a new terminal, observe the LCM communications
$ lcm-spy

Run the counter lcm publisher
$ LcmCounter

At this point, you should see the counters updating on both sides.


Bridge Configuration
========================

The Bridge is configured using an XML file that specifies the names of LCM and
MOOS topics and the associated data types. Look at
/path/to/moos-ros-bridge/moosros/mooslcmconfig.xml for an example. In the XML file,
each message consists of the following members:

moosname - the name of the moos variable

lcmname - the name of the lcm topic

moostype - the type of the moos variable

lcmtype - the type of the lcm topic

direction - the direction of the data over the bridge (either toMOOS or toLCM).

----------------------------------------
Anthony Spears <aspears@gatech.edu>
