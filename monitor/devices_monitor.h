#include "device_driver.h"

#define MAX_DEVICES  3              /* the maximal amount of data byte to be sent in a message */

bool Device_Connect(Device_Descriptor* pDev_Desc,char* id, short dev_prio);
void MONITOR_Process(Device_Descriptor* pDev_Desc,DEV_StateTypeDef state);
void Connect_All_Devices(void);
int Get_FIFO_Level(RX_FIFO** head);
void Format_and_Echo_Frame(short* ptrData, char* ptrId);
void pop(RX_FIFO** head);
