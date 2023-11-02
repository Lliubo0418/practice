#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"cJSON.h"


  char *json="{                       \
      \"1\" : {                       \
          \"0x123\" : [2],            \
          \"0x345\" : [5, 6],         \
          \"0x678\" : [9, 10, 11, 12]    \
      },                              \
      \"2\" : {                       \
          \"0x234\" : [1, 3, 5],      \
          \"0x456\" : [7]             \
      }                               \
  }";
int main(){

    double begin,end;
    //载入一段json字符串
        cJSON *cjsonTest = cJSON_Parse(json);
        if (!cjsonTest) {
            printf("parse fail");
        }
        // 按照key="1"读取json对象
        cJSON *config01 = cJSON_GetObjectItem(cjsonTest, "1");
        char *target = "0x678";
        // 下面的代码需要测算耗时：
        // begin
        begin=clock();
        cJSON *roules = cJSON_GetObjectItem(config01, target);
        int num = cJSON_GetArraySize(roules);
        printf("%s of channel 1 ==>:\r\n", target);
        for (int i = 0; i < num; i++) {
            cJSON *ch = cJSON_GetArrayItem(roules, i);
            printf("%d\r\n", ch->valueint);// 该语句实际需要替换为CAN通道发送函数
        }
        end=clock();

        printf("耗时为：%f ns\r\n",end-begin);

}