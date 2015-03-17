#include <iostream>
#include <vector>
using namespace std;

#include "hungry.h"


int main()
{
	vector<vector<double>> juzhen ;
	vector<int> count(4);
	vector<double> init;
	init.push_back(1.0);
	init.push_back(2.0);
	init.push_back(3.0);
	init.push_back(4.0);
	juzhen.push_back(init);
	juzhen.push_back(init);
	juzhen.push_back(init);
	juzhen.push_back(init);
	double min_cost = hungry(juzhen,count,4);
	cout << min_cost << endl;
}
