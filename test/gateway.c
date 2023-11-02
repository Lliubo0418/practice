/*
Pretended Networking ID Filter 1 register
这个寄存器包含FLT_ID1目标值，以及用于过滤传入消息ID的IDE和RTR目标值。FLT_ID1用于等于、小于、大于比较，或者作为ID范围检测的下限值。只能在冻结模式下写入

Pretended Networking ID Filter 2 Register / ID Mask register
该寄存器包含FLT_ID2目标值，仅用于ID范围检测的上限值。另外，当选择了精确的ID过滤条件时，该寄存器用于存储ID掩码。
IDE_MSK和RTR_MSK位同时用于ID过滤(exact和range)，使FLT_IDE和FLT_RTR分别用作ID接收过滤器的一部分。该寄存器只能在冻结模式下写入。
该寄存器用于定义扩展帧格式(29位)的ID筛选器值
FLT_ID2[28:0]，或者标准帧格式(11位)的ID过滤值，考虑FLT_ID2[28:18]
([17:0]范围内的其他位没有意义)。仅用于ID过滤范围。
它也可以用来定义扩展帧格式(29位)的掩码值，考虑IDMASK[28:0]，或者标准帧格式(11位)的掩码值，考虑IDMASK[28:18]([17:0]范围内的其他位没有意义)。仅用于精确ID过滤。

*/
#include<stdint.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>

#define CH1  CAN0
#define CH2  CAN1
#define CH3  CAN2
#define CH4  CAN3
#define CH5  CAN4
#define CH6  CAN5
#define CH7  CAN6
#define CH8  CAN7
#define CH9  CAN8
#define CH10 CAN9
#define CH11 LIN0
#define CH12 LIN1
#define CH13 LIN2


  
#define CAN_FilterScale_16bit       ((uint8_t)0x00) /* Two 16-bit filters */
#define CAN_FilterScale_32bit       ((uint8_t)0x01) /* One 32-bit filter */

#define CAN_FilterMode_IdMask       ((uint8_t)0x00)  /* identifier mask mode */
#define CAN_FilterMode_IdList       ((uint8_t)0x01)  /* identifier list mode */


#define Rx_MAILBOX_CAN0 0UL
#define Tx_MAILBOX_CAN0 1UL
#define Rx_MAILBOX_CAN1 2UL
#define Tx_MAILBOX_CAN1 3UL

enum Format_Exchange{
    CAN_CANFD,
    CANFD_CAN,
    CAN_LIN,
    LIN_CAN,
    CAN_IOn,
    LIN_IOn,
};

typedef enum{DISABLE=0,ENABLE=!DISABLE}FunctionalState; 

int CAN_STD_ID=0x1FF;

typedef struct Gateway_Rule{
    int Format;
    SrcChn *srcChn;
    TarChn *tarChn;

}Rule;


typedef struct SrcChn
{
    bool  CAN0;
    bool  CAN1; 
    bool  CAN2;
    bool  CAN3;
    bool  CAN4;
    bool  CAN5;
    bool  CAN6;
    bool  CAN7;
    bool  CAN8;
    bool  CAN9;

    bool  LIN0;
    bool  LIN1;
    bool  LIN2;
}SrcChn;


typedef struct TarChn
{
    bool  CAN0;
    bool  CAN1; 
    bool  CAN2;
    bool  CAN3;
    bool  CAN4;
    bool  CAN5;
    bool  CAN6;
    bool  CAN7;
    bool  CAN8;
    bool  CAN9;

    bool  LIN0;
    bool  LIN1;
    bool  LIN2;
}TarChn;

typedef struct {
    uint8_t CAN_FilterNumber;         //过滤器号
    uint8_t CAN_FilterMode;           //过滤模式
    uint8_t CAN_FilterScale;          //过滤器规格
    uint16_t CAN_FilterIdHigh;        
    uint16_t CAN_FilterIdLow;
    uint16_t CAN_FilterMaskIdHigh;    //高位掩码
    uint16_t CAN_FilterMaskIdLow;     //低位掩码
    uint16_t CAN_FilterFIFOAssignment;              //fifo关联
    FunctionalState CAN_FilterActivation;    //过滤器是否激活
}CAN_FilterInitTypeDef;

typedef struct  
{
    uint32_t IdType;             //std、ext
    uint32_t FilterIndex;        
    uint32_t FilterMode;         //掩码还是列表
    uint32_t FilterConfig;       //过滤器配置
    uint32_t FilterId;           //32位ID
    uint32_t FilterIdMask;       //过滤器掩码


}CANFD_FilterInitTypeDef;




int main(){
    enum Format_Exchange form_exchange;
    Rule rule1;

    rule1.Format=CAN_CANFD;
   


    //源通道1
    rule1.srcChn->CAN0=1;


    //目标通道3，4，8
    rule1.tarChn->CAN2=1;
    rule1.tarChn->CAN3=1;
    rule1.tarChn->can7=1;

    
    CAN_FilterInitTypeDef CAN_RXFilter;

    CAN_RXFilter.CAN_FilterNumber=0;
    CAN_RXFilter.CAN_FilterMode=CAN_FilterMode_IdMask;
    CAN_RXFilter.CAN_FilterScale=CAN_FilterScale_32bit;
    CAN_RXFilter.CAN_FilterIdHigh=0X0000;            //16high
    CAN_RXFilter.CAN_FilterIdLow=0X01FF;             //16low
    CAN_RXFilter.CAN_FilterMaskIdHigh=0X0000;
    CAN_RXFilter.CAN_FilterMaskIdLow=0X123;         //屏蔽1、2、6、9位
    CAN_RXFilter.CAN_FilterActivation=ENABLE;







    

}