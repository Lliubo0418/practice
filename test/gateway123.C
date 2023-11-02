#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>


typedef enum TypeNum{
	type1=1,
	type2,
	type3
}TypeNum;

typedef enum Format{
	CAN_CAN,
	CAN_CANFD,
	CANFD_CAN,
	CANFD_CANFD,
	CAN_LIN,
	LIN_CAN,
	CAN_IOn,
	LIN_IOn
}Format;

typedef enum Is_Enable{
	DISABLE,
	ENABLE
}Is_Enable;

typedef struct GateWay_Rule{
	   TypeNum Filter_TypeNum;                        //网关类型
	   Format Format_Converse;                       //格式转换
	   uint8_t  SrcChn[16];                            //源通道
	   uint8_t  TarChn[16];                            //目标通道


	   //联合体
	   union Filter_Type{
	   	uint16_t Filter_Mask;                   //掩码模式
	   	uint16_t Filter_Range;                  //范围过滤
	   	uint16_t Filter_None;                   //不做任何过滤
	   }Filter_Type;

	   char GateWay_Reserve[400];                //预留400个字节
}G_Rule;




void Channeln_Enable(G_Rule grule){
	int len =sizeof(grule.SrcChn);
	int i,j;
	for(i=len;i>0;i--){
		switch(grule.SrcChn[i]){
		case 1:
			grule.SrcChn[i]=ENABLE;
			break;
		case 0:
			grule.SrcChn[i]=DISABLE;
			break;
		default:
			break;
		}
	}
	for(j=len;j>0;j--){
			switch(grule.TarChn[i]){
			case 1:
				grule.TarChn[i]=ENABLE;
				break;
			case 0:
				grule.TarChn[i]=DISABLE;
				break;
			default:
				break;
			}
		}
}
    
   



int main(){
    G_Rule g_rule;
    g_rule.SrcChn[6]=1;
    g_rule.SrcChn[5]=1;
    int len=sizeof(g_rule.SrcChn);
    int i;
    for(i=0;i<len;i++){
    printf("SrcChn[%d]=%d\n",i,g_rule.SrcChn[i]);
   }
}