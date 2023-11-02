#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/***
#define MAX_columnSIZE 8
#define MAX_lineSIZE 8
int main(){
    int buffer[MAX_lineSIZE][MAX_columnSIZE];

    int CANMsg[8]={0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7};
    int i,j;
    for(i=0;i<MAX_lineSIZE;i++){
        for(j=0;j<MAX_columnSIZE;j++){
        buffer[i][j]=CANMsg[j];
        printf("buffer[%d][%d]=%d  ",i,j,buffer[i][j]);
    }
    }
}
***/

#define MAX_BUFFERSIZE 64

typedef struct {
	uint32_t cs;       /*!< Code and Status*/
    uint32_t id;       /*!< ID of the message */
    uint8_t data[64];  /*!< Data bytes of the CAN message*/
    uint8_t length;    /*!< Length of payload in bytes */
} can_message_t;

///int CANFD_Msg_Send(int canID,int canData[64],int length);

int main(){
    int CAN_MAILBOX_MSG[8]={0,1,2,3,4,5,6,7};      //邮箱中的报文
    int buffer[MAX_BUFFERSIZE];      //定义接收buffer来存放多条报文，完成组包

    int *mailbox_ptr=NULL;           //定义邮箱指针
    int *buffer_ptr=NULL;            //定义buffer指针

    mailbox_ptr=CAN_MAILBOX_MSG;      //邮箱指针指向邮箱地址
    buffer_ptr=buffer;                //buffer指针指向buffer


        int flag=1;
                                                             //CAN转CANFD时，buffer满触发一次中断，发送一帧CANFD报文
        int i=0;                                             ///循环接收
        for(i;i<MAX_BUFFERSIZE;i++){
        *buffer_ptr=*mailbox_ptr;
        buffer_ptr++;
        mailbox_ptr++;
        printf("buffer[%d]=%d\n",i,buffer[i]);
            if(mailbox_ptr>&CAN_MAILBOX_MSG[7]){
                mailbox_ptr=CAN_MAILBOX_MSG;
            }
            while (buffer_ptr>&buffer[MAX_BUFFERSIZE-1])
            {
                flag=0;
                //CANFD_Msg_Send();
                i=-1;
                buffer_ptr=buffer;
            }
        }
    
   





    
    }
    /**
int CANFD_Msg_Send(uint16_t canID,uint16_t canData[64],uint16_t length){
    uint16_t canID=0x123;
    uint8_t i=0;
    can_message_t sendFDMsg;

    sendFDMsg.id=canID;
    sendFDMsg.length=64U;

    for(i=0;i<64;i++){
        sendFDMsg.data[i]=i<Length?recvMsg_CAN0.data[i]+1:0;
    }
    //CAN_Send(&can_pal0_instance,TX_MAIL_BOX_CAN0,&sendFDMsg);
	//while(CAN_GetTransferStatus(&can_pal0_instance,TX_MAIL_BOX_CAN0)==STATUS_BUSY);
}
**/
