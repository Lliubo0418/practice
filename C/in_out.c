#include<stdio.h>

int main(void){

    float weight,height;
    char name[40];

    printf("please input your name\n");
    scanf("%s",name);
    printf("please input your weight in KG\n");
    scanf("%f",&weight);
    printf("please input your height in cm\n");
    scanf("%f",&height);

    printf("hello %s,your weight is %fkg,your height is %fcm\n",name,weight,height);

    if((weight>=80)&&(height<180)){
        printf("i have to tell you,you are too fat,you should keep fit!\n");

        
    }
    else if ((weight<80)&&(weight>=60))
        {
            printf("i think you are very slim\n");
        }

    return 0;
}