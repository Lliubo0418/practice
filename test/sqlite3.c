#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sqlite3.h>


#define DATABASE "fruitshop.db"


//水果增加
void insert_data(sqlite3 *db){
    char sql[128]={0};
    char name_temp[32]={0};
    char time_temp[32]={0};
    float price_temp;
    int stock_temp;
    char *errmsg;

    printf("请输入水果种类：");
    scanf("%s",name_temp);

    printf("请输入价格：");
    scanf("%f",&price_temp);

    printf("请输入进货数量：");
    scanf("%d",&stock_temp);

    printf("请输入进货时间：");
    scanf("%s",time_temp);

    sprintf(sql,"insert into fruit(name,price,stock,nowtime) values('%s',%f,%d,'%s');",name_temp,price_temp,stock_temp,time_temp);
	if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK){
			printf("%s\n", errmsg);
	}
	else
	{
		printf("Insert success.\n");
	}
}

//水果卖出

void delete_data(sqlite3 *db)
{
    char sql[128]={0};
    char name_temp[32]={0};
    char time_temp[32]={0};
    int stock_temp;
    char *errmsg;
	
	printf("请输入卖出的水果种类:");
	scanf("%s", name_temp);
	
	printf("请输入剩余的数量:");
	scanf("%d", &stock_temp);
	
	printf("请输入出货时间:");
	scanf("%s", time_temp);
	
	sprintf(sql,"update fruit set stock=%d,nowtime='%s' where name= '%s';",stock_temp,time_temp,name_temp);
	if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK){
			printf("%s\n", errmsg);
	}
	else
	{
		printf("saleout success.\n");
	}
}
//更新

void updata_data(sqlite3 *db)
{
	char sql[128]={0};
	char name_temp[32]={0};
	float stock_temp;
	char *errmsg;
	
	printf("请输入水果种类:");
	scanf("%s", name_temp);
	printf("请输入最新的价格:");
	scanf("%f", &stock_temp);
 
	printf("%s\n",name_temp);
	sprintf(sql,"update fruit set price=%f where name= '%s';",stock_temp,name_temp);
	if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK){
			printf("%s\n", errmsg);
	}
	else
	{
		printf("updata success.\n");
    }
}

//查询
int callback(void *arg, int f_num, char ** f_value, char ** f_name)
{
	int i=0;
	for(i=0;i< f_num;i++){
		printf("%-13s ", f_value[i]);
	}
	putchar(10);
	puts("-----------------------------------------------");
	return 0;
}

void query_data(sqlite3 *db)
{
	char name_temp[32]={0};
	char sql[128] = {0};
	char *errmsg;
 
	sprintf(sql,"select * from fruit;");
	if(sqlite3_exec(db,sql,callback,NULL,&errmsg) != SQLITE_OK){
		printf("%s\n",errmsg);
	}
}


int main(void){
    int temp;
    sqlite3 *db;
    char  *errmsg;

    //open database
    if(sqlite3_open(DATABASE,&db)!=SQLITE_OK){
			printf("%s",sqlite3_errmsg(db)); 
			return -1;
	 }

    //create fruitshop.db
    if(sqlite3_exec(db, "create table if not exists fruit(id integer primary key autoincrement, name text, price real ,stock integer,nowtime text);",NULL,NULL,&errmsg) != SQLITE_OK){
			printf("%s\n",errmsg); 
			return -1;
	}

    while (1)
    {
        printf("****************************************************\n");
		printf("**1.进货  2.出货  3.更新价格  4.价格查询  5.退出**\n");
		printf("****************************************************\n");
		printf("请输入操作值:");
		scanf("%d", &temp);

        switch(temp){
			case 1:
				insert_data(db);
				break;
			case 2:
				delete_data(db);
				break;
			case 3:
				updata_data(db);
				break;
			case 4:
				query_data(db);
				break;
			case 5:
				sqlite3_close(db);
				exit(0);
				break;
			default:
				printf("请输入正确的选项\n");
				break;
		}
	}
	return 0;


      
    }
    



