#include <stdio.h>
#include <iostream>
using namespace std;

double getHungryResult(const char *cmd)
{
	FILE *fp = popen(cmd, "r");
	if(fp == NULL){	return 0;}
	char line[100];
	fgets(line, 100, fp);
	pclose(fp);
	double result;
	sscanf(line,"%lf",&result);
	return result;
}

int main()
{
	cout << getHungryResult("sh /home/yihong/schedule/shell/hungryol.sh 4 /home/yihong/schedule/shell/data/m4-1 ") << endl;
}
