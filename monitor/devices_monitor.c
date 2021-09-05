#include "devices_monitor.h"

DEV_StateTypeDef state = DEV_UNDER_RESET;
char* devices_id[]={"TEMPERATURE_SENSOR","LEVEL_SENSOR","PRESSURE_SENSOR"};
short devices_prios[]={2,0,7};
extern bool token;
extern RX_FIFO* HEAD;
extern short* frame_payload;
extern Device_Descriptor devices_list[MAX_DEVICES];
extern volatile int msgCount;
bool monitor_debug = 0;
/**
  * @brief  Device_Connect
  *         connect the device (add it) to the devicew list.
  * @param  pDev:a pointer to the device descriptor structure.
  * @param  id: pointer to a buffer holding the id string of device
  * @param  dev_prio: priority of the device in the bus.
  * @retval  status : 1 if device is successfully added, 0 if not
  */
bool Device_Connect(Device_Descriptor* pDev_Desc,char* id, short dev_prio)
{
    bool return_status = 0;

    if(strlen(id) > MAX_ID_LEN)
    {
        if(monitor_debug)printf("***** [error!!] wrong string length ***** \n");
        return return_status;
    }

    if(pDev_Desc->is_attached != 1)
    {
        strcpy(pDev_Desc->dev_id, id);
        pDev_Desc->msg_prio = dev_prio;
        pDev_Desc->is_attached = 1;
        return_status = 1;

    }
    return return_status;

}
/**
  * @brief  Connect_All_Devices
  *         connect all the available devices (add them) to the devices list.
  * @param  None
  * @retval None
  */
void Connect_All_Devices(void)
{
     int i ;

    for(i=0;i<MAX_DEVICES;i++)
        {
         if (Device_Connect(&devices_list[i],devices_id[i],devices_prios[i]))
         {
               printf("****[INFO] %s has been connected successfully ***** \n",devices_id[i]);
         }else{printf("****[INFO] %s could not be  connected ***** \n",devices_id[i]);}
        }
}

/**
  * @brief  MONITOR_Process
  *         Background process of the devices internal state machine.
  * @param  pDev_Desc:a pointer to the device descriptor structure.
  * @param  state: state of the device (one of DEV_StateTypeDef enumeration members)
  * @retval None
  */
void MONITOR_Process(Device_Descriptor* pDev_Desc,DEV_StateTypeDef state)
{
    if((pDev_Desc->is_attached ==1)&&(state != DEV_UNDER_RESET)){
        switch(state)
        {
            case DEV_DataReady_Notify :
                token = 1;
                if(monitor_debug)printf("******** token is given to the device **************\n");
                break;
             case DEV_Data_Send :
                pDev_Desc->data_toggle = 1;
                if(monitor_debug)printf("******** acknowledge toggling %d **************\n",pDev_Desc->data_toggle);
                break;
            case DEV_ACK :
                msgCount++;
                if(monitor_debug)printf("******** new message have been received **************\n");
                break;
            default:
                break;
        }
    }
}

/**
  * @brief  Get_FIFO_Level
  *         Return the level of the fifo queue.
  * @param  head :a pointer to the pointer to the head of the fifo queue.
  * @retval integer value of the fifo level
  */

int Get_FIFO_Level(RX_FIFO** head)
{
    if( (*head) != NULL){
    int level=1;
    RX_FIFO* temp;
    temp = (*head);
    while(temp->next != NULL)
    {
        level++;
        temp = temp->next;
    }
    return level;
    }else{
    return 0 ;
    }
}

/**
  * @brief  Format_and_Echo_Frame
  *         Format and print the frame on the console.
  * @param  ptrData:a pointer to thedata payload.
  * @param  ptrId:a pointer to the head ofdevice id.
  * @retval None
  */
void Format_and_Echo_Frame(short* ptrData, char* ptrId)
{
    printf("*****[Messages Log !!][From|%s|we got|0x%X||0x%X||0x%X||0x%X||0x%X||0x%X||0x%X||0x%X|]\n\n",\
           ptrId,ptrData[0],ptrData[1],ptrData[2],ptrData[3],ptrData[4],ptrData[5],ptrData[6],ptrData[7]);
}

/**
  * @brief  pop
  *         Delete the message with highest priority from the queue
  * @param  head :a pointer to the pointer to the head of the fifo queue.
  * @retval None
  */
void pop(RX_FIFO** head)
{
    RX_FIFO* temp = (*head);
    (*head) = (*head)->next;
    free(temp);
}
