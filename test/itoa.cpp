#include <iostream>
#include <cstdlib>
using namespace std;

int main(){
	int num = 100;
	char str[25];
	itoa(num, str, 16);
	cout << str; //输出100 
	return 0;
}
