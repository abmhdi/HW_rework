#include "device_driver.h"

extern bool token;
extern RX_FIFO* HEAD;
extern DEV_StateTypeDef state;
bool device_debug=0;
/**
  * @brief  New_Msg_Mem_Allocation
  *         Allocates a shared ram area for the devices to push their data.
  * @param  pdata: a pointer to a buffer holding the data.
  * @param  priority: priority of the device:
  *         The lowest is the priority, the highest is the location of data in the queue.
  * @retval pointer to an RX_FIFO cell.
  */
RX_FIFO* New_Msg_Mem_Allocation(short* pdata,char* pId, short priority)
{
    int i;
    RX_FIFO* temp = (RX_FIFO*)malloc(sizeof(RX_FIFO));
    strcpy(temp->signature,pId);
    for(i=0;i<MAX_DATA_COUNT;i++)
    {
        temp->data[i] = pdata[i];
    }
    temp->priority = priority;
    temp->next = NULL;

    return temp;
}
/*****************************************************************************************/
/**
  * @brief  Add_message
  *         push a messages in the rx_fifo according to device priority
  * @param  head:a pointer to the pointer to the head of the queue.
  * @param  priority: priority of the device:
  *         The lowest is the priority, the highest is the location of data in the queue
  * @retval None
  */
void Add_message(RX_FIFO** head, short* d,char* id, short p)
{
    RX_FIFO* start = (*head);
    RX_FIFO* temp =New_Msg_Mem_Allocation(d,id,p);

    if ((*head)->priority > p) {

        temp->next = *head;
        (*head) = temp;
    }
    else {

        while ((start->next != NULL) && (start->next->priority < p))
        {
            start = start->next;
        }
        temp->next = start->next;
        start->next = temp;
    }
}

/*****************************************************************************************/
/**
  * @brief  DEVICE_FSM_Run
  *         run the internal device state machine.
  * @param  pDev:a pointer to the device descriptor structure.
  * @param  rst: software reset of the device, if 0 the reset will be released.
  * @retval DEV status
  */

DEV_StateTypeDef DEVICE_FSM_Run (Device_Descriptor * pDev, bool rst )
{
    int i;
    short msrmnt = 0x11;
    if (pDev->is_attached)
    {

        switch(state)
           {
            case DEV_IDLE :
                if(device_debug)printf("******** Device is currently in idle state **************\n");
                pDev->data_toggle =1;
                for(i=0;i<MAX_DATA_COUNT;i++)
                {
                    pDev->data[i]= 0;
                }
                state= DEV_MEASURE;
                break;
            case DEV_MEASURE :
                if(device_debug)printf("******** Device is currently performing measurement **************\n");
                for(i=0;i<MAX_DATA_COUNT;i++)
                {
                    pDev->data[i]= msrmnt*(i+1);
                }
                state= DEV_DataReady_Notify;
                break;
            case DEV_DataReady_Notify :
                if(device_debug)printf("******** data measurement are ready now and monitor is now notified **************\n");
                pDev->data_ready_notif = 1;
                state= DEV_TOKEN_WAIT;
                break;
            case DEV_TOKEN_WAIT :
                if(device_debug)printf("*****[INFO] Waiting for token !! *****\n");
                if(token){
                    state = DEV_Data_Send;
                }else{state = DEV_TOKEN_WAIT;}
                break;
             case DEV_Data_Send :
                if(device_debug)printf("*****[INFO]  token is now received and data will be added to shared fifo queue  *****\n");
                Add_message(&HEAD,pDev->data,pDev->dev_id,pDev->msg_prio);
                pDev->data_toggle = 0;
                state= DEV_ACK_WAIT;
                break;
            case DEV_ACK_WAIT :
                if(device_debug)printf("*****[INFO] Waiting for acknowledge !! *****\n");
                if(pDev->data_toggle){
                    state = DEV_ACK;
                }else{state = DEV_ACK_WAIT;}
                break;
            case DEV_ACK :
                if(device_debug)printf("*****[INFO] acknowledge Done !! *****\n");
                token = 0;
                state = DEV_IDLE;
                break;
            case DEV_UNDER_RESET :
                if(device_debug)printf("******** Device is currently under reset **************\n");
                if(!rst)state = DEV_IDLE;
                break;
            default:
                break;
           }
    }else
    {
        if(device_debug)printf("***** [WARNING]: Device is not yet attached to the bus ***** \n");
    }
    return state;
}
