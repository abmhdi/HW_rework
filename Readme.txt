/** 
  ****************************************************************************************************************************
  * @file    homework1\devtest\Readme.txt
  * @author  Mahdi ABIDI
  * @brief   Description of the the behavior of the devives and their monitor .
  ****************************************************************************************************************************
  
  
/** 
  ***************@brief: Contents of the device_driver.c/device_driver.h files************************************************
  *@ A DEVICE is modeled with <Device_Descriptor> a structures holding the necessary parameters and identifiers for the device.
  *  It contains also a pointer to a function which will run the internal software state machine of the device.
  * The states are defined in the <DEV_StateTypeDef> enumeration.
  *	tha device has a fixed priority and can push its data to a shared <RX_FIFO> a priority queue based on a linked list.
  *	the device synchronizes itself with the host (the devices monitor) using token,data_toggle and data_ready_notif variables.

  
/** 
  ***************@brief: Contents of the devices_monitor.c/devices_monitor.h files********************************************
  *@ The Devives monitor is responsible on Connecting all the available devices: Adding them to the list (table of structures)
  *	 On new messages notification, the monitor will process the device internal state machine and retrieve 
  *     the messages on RX FIFO full event and echo them on terminal.	
  
/** 
  ***************@brief: Contents of the main application main.c file ********************************************************
  * a basic demonstration of interaction devices/monitor for a few iteration (while(k<50)and not while(1))
  * this file contains the global declaration and a setshared variables