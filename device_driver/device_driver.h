#ifndef  DEVICE_DRIVER_H
#define  DEVICE_DRIVER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_DATA_COUNT  8              /* the maximal amount of data byte to be sent in a message */
#define MAX_ID_LEN      32

typedef struct _RX_FIFO {
    char signature[MAX_ID_LEN]; /* add the name id of the device to be recognized later by the monitor */
    short data[MAX_DATA_COUNT];
    short priority;
    struct _RX_FIFO* next;
} RX_FIFO;

typedef enum{
    DEV_IDLE = 0,
    DEV_MEASURE,            /* During this state the device is performing measurements : state entered after reset is released */
    DEV_DataReady_Notify,   /* During this state the measurements are ready to be sent : state entered when measurement is completed */
    DEV_TOKEN_WAIT,
    DEV_Data_Send,
    DEV_ACK_WAIT,
    DEV_ACK,
    DEV_UNDER_RESET
}DEV_StateTypeDef;

typedef struct _device_descriptor
{
    bool is_attached;                                                /* if equal to 1,the device is attached to devices list (the virtual bus) */
	char dev_id[MAX_ID_LEN];                                         /* ID of the device : the unique name */
	short msg_prio;                                                  /* message  priority */
	bool data_toggle;                                                /*  data toggle is used as a kind of acknowledge:
                                                                         when a device add a message, it toggles the data_toggle,
                                                                         when the host receives it it toggles it again(inspired from the USB protocol)*/
	bool data_ready_notif;                                           /* if equal to 1, a measurement is ready to be sent */
	short data[MAX_DATA_COUNT];                                      /* data measurement payload */
	DEV_StateTypeDef (*Run_Funct)(struct _device_descriptor*, bool );/* pointer to the run device internal state machine function */
}Device_Descriptor;


RX_FIFO* New_Msg_Mem_Allocation(short* pdata,char* pId, short priority);
void Add_message(RX_FIFO** head, short* d,char* id, short p);
DEV_StateTypeDef DEVICE_FSM_Run (Device_Descriptor * pDev, bool rst );

#endif
