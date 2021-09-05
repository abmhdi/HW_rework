#include "main.h"

volatile int msgCount = 0;
Device_Descriptor devices_list[MAX_DEVICES];
short dram[MAX_DATA_COUNT]= {0};
RX_FIFO *HEAD;
short* frame_payload;
bool token= 0;
char* dev_signature;
int main()
{   int level=0 ,i,j,k=0;
    /* Allocate ram space for rx fifo queue with the lowest priority and initializes its data to zeros */
    HEAD = New_Msg_Mem_Allocation(dram,"NONE",255);
    /* Connect all the devices to the list*/
    if(HEAD != NULL){Connect_All_Devices();}
    else{ printf("***** [error!!]messages fifo allocation failure *****");}

    while(k<50)
    {
        for(i=0;i<MAX_DEVICES;i++)
        {
            /* Release all the devices rest and run their internal state machines process*/
            MONITOR_Process(&devices_list[i], DEVICE_FSM_Run(&devices_list[i],0));
        }
        level = Get_FIFO_Level(&HEAD);
        if(level>3)
        {
            printf(" ***** [INFO] Currently a totalo of %d have been received ***** \n",msgCount);
            for(j=0;j<MAX_DEVICES;j++){
                    frame_payload = HEAD->data;
                    dev_signature = HEAD->signature;
                    Format_and_Echo_Frame(frame_payload,dev_signature);
                    pop(&HEAD);
            }

        }
        k++;

    }

    return 0;
}
