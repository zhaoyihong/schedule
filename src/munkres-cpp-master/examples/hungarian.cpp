#include "munkres.h"
#include <iostream>
#include <cstdio>
using namespace std;


int sqrt(int num)
{
	int i;
	for(i=1; i*i <num; ++i){}
	return i;
}

int main(int argc,char **argv)
{
		//double costArray[argc-1];
		int size = sqrt(argc-1);
		//cout << argc << " " << size << endl;;
		Matrix<double> matrix(size,size);
		
		for(int i=1; i < argc ; ++i)
		{
			int nrow = (i-1)/size;
			int ncol = (i-1)%size;
			//cout << "r:" << nrow << ",c:" << ncol << endl;
			sscanf(argv[i],"%lf",&matrix(nrow,ncol));
		}
		
		Matrix<double> costMatrix(matrix);
		Munkres m;
		m.solve(matrix);

		int choosen[size];
		double sum = 0;
		for(int i=0; i < size ; ++i)
		{
			for(int j=0; j < size ; ++j)
			{
			//	cout << matrix(i,j) << " ";
				if(matrix(i,j)==0)
				{
					sum += costMatrix(i,j);
					choosen[i] = j;
					break;
				}
			}
			//cout << endl;
		}


	for(int i=0;i < size; ++i)
	{
		cout << choosen[i] << " ";
	}
	cout << endl;
	cout << sum << endl;
	
	return 0;


}
