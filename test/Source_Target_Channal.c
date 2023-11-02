#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define CANFD0      (uint16_t)0x0001
#define CANFD1      (uint16_t)0x0002
#define CANFD2      (uint16_t)0x0004
#define CANFD3      (uint16_t)0x0008
#define CANFD4      (uint16_t)0x0010
#define CANFD5      (uint16_t)0x0020
#define CANFD6      (uint16_t)0x0040
#define CANFD7      (uint16_t)0x0080
#define CANFD8      (uint16_t)0x0100
#define CANFD9      (uint16_t)0x0200

typedef enum CAN_Mode{
    CAN_CLASSIC,
    CANFD
}CAN_Mode;

uint16_t Source_Channal;
uint16_t Target_Channal;
//void Source_CANFDx_Init(uint16_t CANFDx);

void Source_CANFDx_Init(uint16_t Source_CANFDx);
void Target_CANFDx_Init(uint16_t Target_CANFDx);

void CANFD0_Init(){
    printf("CANFD0_Init Success\n");
}
void CANFD1_Init(){
    printf("CANFD1_Init Success\n");
}
void CANFD2_Init(){
    printf("CANFD2_Init Success\n");
}
void CANFD3_Init(){
    printf("CANFD3_Init Success\n");
}
void CANFD4_Init(){
    printf("CANFD4_Init Success\n");
}
void CANFD5_Init(){
    printf("CANFD5_Init Success\n");
}
void CANFD6_Init(){
    printf("CANFD6_Init Success\n");
}
void CANFD7_Init(){
    printf("CANFD7_Init Success\n");
}
void CANFD8_Init(){
    printf("CANFD8_Init Success\n");
}
void CANFD9_Init(){
    printf("CANFD9_Init Success\n");
}


#if 0
int main(){
    printf("test start:\n");
    Source_CANFDx_Init(CANFD1);
    Source_CANFDx_Init(CANFD0);
    printf("test end\n");
    return 0;
}

void Source_CANFDx_Init(uint16_t CANFDx){

    Source_Channal = Source_Channal&(~(0xFFFF))|CANFDx;
    int i=0;
    for(i=0;i<16;i++){
        
        if((Source_Channal>>i)&1==1){
            switch (i)
            {
            case 0:
                CANFD0_Init();
                break;
            case 1:
                CANFD1_Init();
                break;   
            case 2:
                CANFD2_Init();
                break;
            case 3:
                CANFD3_Init();
                break;    
            case 4:
                CANFD4_Init();
                break;
            case 5:
                CANFD5_Init();
                break;   
            case 6:
                CANFD6_Init();
                break;
            case 7:
                CANFD7_Init();
                break; 
            case 8:
                CANFD8_Init();
                break;
            case 9:
                CANFD9_Init();
                break; 

            default:
                break;
            }
        }
    }
}

#endif

int main(){
    printf("test start:\n");
//    Source_CANFDx_Init(CANFD1);
//    Source_CANFDx_Init(CANFD0);
    Source_CANFDx_Init(0x200);
    printf("源通道初始化完毕\n");
    Target_CANFDx_Init(0x4A);
    printf("目标通道初始化完毕\n");
    printf("test end\n");
    return 0;
}

void Source_CANFDx_Init(uint16_t Source_CANFDx){
    Source_Channal = Source_Channal&(~(0xFFFF))|Source_CANFDx;
    int i=0;
    for(i=0;i<16;i++){
        
        if((Source_Channal>>i)&1==1){
            switch (i)
            {
            case 0:
                CANFD0_Init();
                break;
            case 1:
                CANFD1_Init();
                break;   
            case 2:
                CANFD2_Init();
                break;
            case 3:
                CANFD3_Init();
                break;    
            case 4:
                CANFD4_Init();
                break;
            case 5:
                CANFD5_Init();
                break;   
            case 6:
                CANFD6_Init();
                break;
            case 7:
                CANFD7_Init();
                break; 
            case 8:
                CANFD8_Init();
                break;
            case 9:
                CANFD9_Init();
                break; 

            default:
                break;
            }
        }
    }
}
void Target_CANFDx_Init(uint16_t Target_CANFDx){
    Target_Channal = Target_Channal&(~(0xFFFF))|Target_CANFDx;
    int i=0;
    for(i=0;i<16;i++){
        
        if((Target_Channal>>i)&1==1){
            switch (i)
            {
            case 0:
                CANFD0_Init();
                break;
            case 1:
                CANFD1_Init();
                break;   
            case 2:
                CANFD2_Init();
                break;
            case 3:
                CANFD3_Init();
                break;    
            case 4:
                CANFD4_Init();
                break;
            case 5:
                CANFD5_Init();
                break;   
            case 6:
                CANFD6_Init();
                break;
            case 7:
                CANFD7_Init();
                break; 
            case 8:
                CANFD8_Init();
                break;
            case 9:
                CANFD9_Init();
                break; 

            default:
                break;
            }
        }
    }
}