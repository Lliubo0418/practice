#include<stdio.h>
#include<string.h>
#include<stdint.h>
#include<stdlib.h>
#include<stdbool.h>


typedef enum Format_Exchange{
    CAN_CAN,
    CAN_CANFD,
    CANFD_CAN,
    CAN_LIN,
    LIN_CAN,
    CAN_IOn,
    LIN_IOn
}Format_Exchange;

 typedef enum Fiter_Rule{
    rule1=0x01,
    rule2=0x20,    //6
    rule3=0xff,
    rule4=0xf,     //1\2\3\4
    rule5=0x4e,    //7\4\3\2
    rule6=0xf4     //5\6\7\8\3
}F_Rule;

typedef struct GateWay_Rule{
    uint16_t FORMAT;
    bool SrcChn[16];
    bool TarChn[16];
    //uint16_t Src;
    //uint16_t Tar;
    uint16_t Filter_rulen;
}G_Rule;
 //   char SrcChn[4];             //0x0~0x9  CAN0-CAN9  0xA~0xC  LIN0-LIN2
 //   char TarChn[4];             //0x0~0x9  CAN0-CAN9  0xA~0xC  LIN0-LIN2

 //   char SrcChn[16];
 //   char TarChn[16];

int main(){
   
    int Id=0x123;
    G_Rule grule;
    grule.FORMAT=CAN_CAN;
    //grule.Src=1;
   // grule.Tar=2;
    grule.Filter_rulen=rule4;
    
    
    grule.SrcChn[4]=1;
    grule.SrcChn[5]=1;

    grule.TarChn[6]=1;
    grule.TarChn[7]=1;
    grule.TarChn[8]=1;
   
    /*
    TCANFD F2;
    F2.FIdxChn=2;
    F2.FProperties=ACAN->FProperties;
    F2.FFDProperties =MASK_CANFDProp_IS_FD;
    F2.FIdentifier=ACAN->FIdentifier;
    F2.FDLC=15;
    F2.load_data(ACAN->FData);
    */
   int len=sizeof(grule.SrcChn);
   int i;
   for(i=0;i<len;i++){
   printf("SrcChn[%d]=%d\n",i,grule.SrcChn[i]);
   }
   printf("rule4=%d\n",rule4);
    for(Id=0x123;Id<0x13f;Id++){
    if((Id&grule.Filter_rulen)==rule4){
        printf("Id=%d通过过滤\n",Id);
    }
    else{
        printf("ID=%d报文没有通过\n",Id);
    }
    }
    return 0;
    
}
