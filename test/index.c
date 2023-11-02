#include<stdio.h>

int main(int argc,char *argv[]){
  
//对数组进行每位操作
   /*int buf[8]={0};
   int i=0;
   
   for(i=0;i<8;i++){
        printf("请输入第%d位:",i+1);
        scanf("%d",&buf[i]);
        printf("数组第%d位为:%d\n",i+1,buf[i]);
        printf("地址为%p\n",&buf[i]);
   }
   return 0;
*/
//按位与查找1的位置
    int var;
    int i=0;
    scanf("%d",&var);
    for(i=0;i<8;i++){
        
        if(var&0x01==1){
            
            printf("倒数第%d位是1",i+1);
            break;
        }
        var>>=1;
        

    }
}