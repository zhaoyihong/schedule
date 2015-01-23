/**************************************************************************/
/* This is a simple genetic algorithm implementation where the */
/* evaluation function takes positive values only and the      */
/* fitness of an individual is the same as the value of the    */
/* objective function                                          */
/**************************************************************************/
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

/* Change any of these parameters to match your needs */
#define POPSIZE 4               /* population size */
#define MAXGENS 24            /* max. number of generations */
#define NVARS 4                  /* no. of problem variables */
#define PXOVER 0.8               /* probability of crossover */
#define PMUTATION 0.15           /* probability of mutation */
#define TRUE 1
#define FALSE 0
#define UPP 1
#define LOW 4
int generation;                  /* current generation no. */
int cur_best;                    /* best individual */
FILE *galog;                     /* an output file */
FILE *gadata;                    /* an input file */


struct genotype /* genotype (GT), a member of the population */
{
  int gene[NVARS];        /* a string of variables */
  double fitness;            /* GT\'s fitness */
  double upper[NVARS];       /* GT\'s variables upper bound */
  double lower[NVARS];       /* GT\'s variables lower bound */
  double rfitness;           /* relative fitness */
  double cfitness;           /* cumulative fitness */
};
struct genotype population[POPSIZE+1];    /* population */
struct genotype newpopulation[POPSIZE+1]; /* new population; */
                                          /* replaces the */
                                          /* old generation */


/* Declaration of procedures used by this genetic algorithm */
void initialize(void);
void randval(int *);
void evaluate(void);
void keep_the_best(void);
void elitist(void);
void selectt(void);
void crossover(void);
void Xover(int,int,int);
//void swap(double *, double *);
void swap(int *, int *);
void mutate(void);
void report(void);
void printPop();
double getValue(int loop,int cpu,int app);
/***************************************************************/
/* Initialization function: Initializes the values of genes    */
/* within the variables bounds. It also initializes (to zero)  */
/* all fitness values for each member of the population. It    */
/* reads upper and lower bounds of each variable from the      */
/* input file `gadata.txt\'. It randomly generates values       */ 
/* between these bounds for each gene of each genotype in the  */
/* population. The format of the input file `gadata.txt\' is    */
/* var1_lower_bound var1_upper bound                           */
/* var2_lower_bound var2_upper bound ...                       */
/***************************************************************/ 
void initialize(void)
{
	srand( (unsigned)time( NULL ) );
//	FILE *infile;
	int i, j;
/*	double lbound, ubound;
	if ((infile = fopen("gadata.txt","r"))==NULL)
	{
	      fprintf(galog,"\nCannot open input file!\n");
	      exit(1);
	}
*/

	/* initialize variables within the bounds */
	      //fscanf(infile, "%lf",&lbound);
	      // fscanf(infile, "%lf",&ubound);
	      
	      for (j = 0; j < POPSIZE; j++)
	      {
		   population[j].fitness = 0;
		   population[j].rfitness = 0;
		   population[j].cfitness = 0;
		   randval(population[j].gene);
	           cout << "j:" << j << endl;
		   for (i = 0; i < NVARS; i++)
		   {
		          cout <<  population[j].gene[i] << " ";
		 	 // population[j].lower[i] = LOW;
		 	 // population[j].upper[i]= UPP;
			 // population[j].gene[i] = randval(population[j].lower[i],population[j].upper[i]);
	            }
		    cout << endl;
	     }

//	fclose(infile);
}
/***********************************************************/
/* Random value generator: Generates a value within bounds */
/***********************************************************/
void  randval(int *gene)
{
	int history[NVARS] = {0};
	int var;
	int flag;
	for(int i=0;i<NVARS;i++)
	{
		flag=0;
		while(!flag)
		{
			flag=1;
			var = rand()%4 + 1;
			//cout << var << " " ;
			for(int j=0;j<NVARS;j++)
			{
				if(history[j] == var)
				{
					flag = 0;
				}	
			}
		}
		gene[i]=var;
		history[i] = var;
	}
}


/*************************************************************/
/* Evaluation function: This takes a user defined function.  */
/* Each time this is changed, the code has to be recompiled. */
/* The current function is:  x[1]^2-x[1]*x[2]+x[3]           */
/*************************************************************/
double getValue(int loop,int cpu,int app)
{
	ifstream in("gadata.txt");
	if(!in){ cout << "cannot open gadata.txt!exit!" << endl; exit; }

	int cnt=0;
	string line;
	int endline=loop*5+app+1;
	while(cnt != endline)
	{
		getline(in,line);
		cnt++;
	}

	cnt=0;
	istringstream is(line);
	double value=0;
	while(cnt != cpu)
	{
		is >> value;
		cnt++;
	}

	cout << "getValue(" << loop << "," << cpu << ","  << app << ") : "  << value <<  endl; 	
	return value;	

}

void evaluate(void)
{
	int mem;
	int i;
	double value;
	double x[NVARS+1];
	double result=0;
	for (mem = 0; mem < POPSIZE; mem++)
	{
	      result=0;
	      for (i = 0; i < NVARS; i++)
	      {
		    //cout << "generation:" << generation << " " << i+1 << " " << population[mem].gene[i]  << endl;
		    //value = getValue(generation,i+1,population[mem].gene[i]);
		    value = getValue(0,i+1,population[mem].gene[i]); //只取第一次的
		    //cout << value << " " << endl;
	      	    result += value;
	      }
	      population[mem].fitness = result;
	      cout << "sum fitnesee:" << population[mem].fitness  << endl; 
	      //population[mem].fitness = (x[1]*x[1]) - (x[1]*x[2]) + x[3];
	 }
}
/***************************************************************/
/* Keep_the_best function: This function keeps track of the    */
/* best member of the population. Note that the last entry in  */
/* the array Population holds a copy of the best individual    */
/***************************************************************/
void keep_the_best()
{
	int mem;
	int i;
	cur_best = 0; /* stores the index of the best individual */
	for (mem = 0; mem < POPSIZE; mem++)
	{
		population[POPSIZE].fitness=100;
	}
	for (mem = 0; mem < POPSIZE; mem++)
	{
	      if (population[mem].fitness < population[POPSIZE].fitness) //改成了最小
		{
		    cur_best = mem;
		    population[POPSIZE].fitness = population[mem].fitness;
		 }
	}
	/* once the best member in the population is found, copy the genes */
	for (i = 0; i < NVARS; i++)
	      population[POPSIZE].gene[i] = population[cur_best].gene[i];
}
/****************************************************************/
/* Elitist function: The best member of the previous generation */
/* is stored as the last in the array. If the best member of    */
/* the current generation is worse then the best member of the  */
/* previous generation, the latter one would replace the worst  */ 
/* member of the current population                             */
/****************************************************************/ 
void elitist()
{
int i;
double best, worst;             /* best and worst fitness values */
int best_mem, worst_mem; /* indexes of the best and worst member */
best = population[0].fitness;
worst = population[0].fitness;
for (i = 0; i < POPSIZE - 1; ++i)
      {
      if(population[i].fitness < population[i+1].fitness)
            {      
            if (population[i].fitness <= best)
                  {
                  best = population[i].fitness;
                  best_mem = i;
                  }
            if (population[i+1].fitness >= worst)
                  {
                  worst = population[i+1].fitness;
                  worst_mem = i + 1;
                  }
            }
      else
            {
            if (population[i].fitness >= worst)
                  {
                  worst = population[i].fitness;
                  worst_mem = i;
                  }
            if (population[i+1].fitness <= best)
                  {
                  best = population[i+1].fitness;
                  best_mem = i + 1;
                  }
            }
      }
/* if best individual from the new population is better than */
/* the best individual from the previous population, then    */
/* copy the best from the new population; else replace the   */
/* worst individual from the current population with the     */
/* best one from the previous generation                     */
//cout << "bw "<<  best << " " << worst << endl;
//printPop();
//cout << "best:" << best << " " << population[POPSIZE].fitness << endl;

if (best <= population[POPSIZE].fitness)
    {
    for (i = 0; i < NVARS; i++)
       population[POPSIZE].gene[i] = population[best_mem].gene[i];
    population[POPSIZE].fitness = population[best_mem].fitness;
    }
else
    {
    for (i = 0; i < NVARS; i++)
    	population[worst_mem].gene[i] = population[POPSIZE].gene[i];
    population[worst_mem].fitness = population[POPSIZE].fitness;
    }

//printPop();
 
}

void printPop()
{
	cout << "-------------------" << endl;
	for (int i = 0; i < POPSIZE; ++i)
	{
		cout << "i:" << i << " fitnesss:" << population[i].fitness << endl;
		for (int j = 0; j < NVARS; j++)
		{
			cout << population[i].gene[j] << " ";
		}
		cout << endl;
	}
	cout << "-------------------" << endl;
}


/**************************************************************/
/* Selection function: Standard proportional selection for    */
/* maximization problems incorporating elitist model - makes  */
/* sure that the best member survives                         */
/**************************************************************/
void selectt(void)
{
	int mem, i, j, k; 
	double sum = 0;
	double p; 
	/* find total fitness of the population */
	for (mem = 0; mem < POPSIZE; mem++)
	      {
	      sum += population[mem].fitness;
	      }
	/* calculate relative fitness */
	for (mem = 0; mem < POPSIZE; mem++)
	      {
	      population[mem].rfitness =  population[mem].fitness/sum;
	      }
	population[0].cfitness = population[0].rfitness;
	/* calculate cumulative fitness */
	for (mem = 1; mem < POPSIZE; mem++)
	      {
	      population[mem].cfitness =  population[mem-1].cfitness +       
				  population[mem].rfitness;
	      }
	/* finally select survivors using cumulative fitness. */
	for (i = 0; i < POPSIZE; i++)
	      { 
	      p = rand()%1000/1000.0;
	      if (p < population[0].cfitness)
		    newpopulation[i] = population[0];      
	      else
		    {
		    for (j = 0; j < POPSIZE;j++)      
			  if (p >= population[j].cfitness && 
				      p<population[j+1].cfitness)
				newpopulation[i] = population[j+1];
		    }
	      }
	/* once a new population is created, copy it back */
	for (i = 0; i < POPSIZE; i++)
	      population[i] = newpopulation[i];      
}
/***************************************************************/
/* Crossover selection: selects two parents that take part in  */
/* the crossover. Implements a single point crossover          */
/***************************************************************/
void crossover(void)
{
	int i, mem, one;
	int first  =  0; /* count of the number of members chosen */
	double x;
	
	for (mem = 0; mem < POPSIZE; ++mem)
	{
	   for(i=0;i<NVARS;++i)
	    {
		    x = rand()%1000/1000.0;
		    if (x < PXOVER)
		    {
			    ++first;
			    if (first % 2 == 0)
			    {
				  //cout << "Xover"<< mem << " " << one << " " << i << endl;
				  Xover(mem,one,i);
			    }
			    else
			    one = i;
		    }
	   }
	}
}
/**************************************************************/
/* Crossover: performs crossover of the two selected parents. */
/**************************************************************/
void Xover(int mem,int one, int two)
{
	  swap(&population[mem].gene[one], &population[mem].gene[two]);
}

/*************************************************************/
/* Swap: A swap procedure that helps in swapping 2 variables */
/*************************************************************/
void swap(int *x, int *y)
{
	double temp;
	temp = *x;
	*x = *y;
	*y = temp;
}
/**************************************************************/
/* Mutation: Random uniform mutation. A variable selected for */
/* mutation is replaced by a random value between lower and   */ 
/* upper bounds of this variable                              */
/**************************************************************/ 
void mutate(void)
{
	int i, j;
	double lbound, hbound;
	double x;
	for (i = 0; i < POPSIZE; i++)
	      for (j = 0; j < NVARS; j++)
		    {
		    x = rand()%1000/1000.0;
		    if (x < PMUTATION)
			  {
			 /* find the bounds on the variable to be mutated */
			  lbound = population[i].lower[j];
			  hbound = population[i].upper[j];  
			  //population[i].gene[j] = randval(lbound, hbound);
			  }
		    }
}
/***************************************************************/
/* Report function: Reports progress of the simulation. Data   */
/* dumped into the  output file are separated by commas        */
/***************************************************************/
void report(void)
{
	int i;
	double best_val;            /* best population fitness */
	double avg;                 /* avg population fitness */
	double stddev;              /* std. deviation of population fitness */
	double sum_square;          /* sum of square for std. calc */
	double square_sum;          /* square of sum for std. calc */
	double sum;                 /* total population fitness */
	sum = 0.0;
	sum_square = 0.0;
	for (i = 0; i < POPSIZE; i++)
	{
	      sum += population[i].fitness;
	      sum_square += population[i].fitness * population[i].fitness;
	}
	avg = sum/(double)POPSIZE;
	square_sum = avg * avg * POPSIZE;
	stddev = sqrt((sum_square - square_sum)/(POPSIZE - 1));
	best_val = population[POPSIZE].fitness;
	//fprintf(galog, "\n%5d,      %6.3f, %6.3f, %6.3f \n\n", generation, best_val, avg, stddev);
}
/**************************************************************/
/* Main function: Each generation involves selecting the best */
/* members, performing crossover & mutation and then          */
/* evaluating the resulting population, until the terminating */
/* condition is satisfied                                     */
/**************************************************************/
int main(void)
{
/*
	for(int loop=0;loop<5;loop++)
	{
		cout << "loop : " << loop << endl;
		for(int app=1;app<5;app++)
		{
			for(int cpu=1;cpu<5;cpu++)
			{
				cout << getValue(loop,cpu,app) << "\t" ;
			}
			cout << endl;
		}
	}

	return 0;
*/

	int i,j;
	double value=0,result=0;
	if ((galog = fopen("galog.txt","w"))==NULL)
	{
	      exit(1);
	}
	if ((gadata = fopen("gadata.txt","r"))==NULL)
	{
	      exit(1);
	}
	generation = 0;
	fprintf(galog, "\n generation  best  average  standard \n");
	fprintf(galog, "number        value fitness  deviation \n");
	initialize();
	evaluate();
	keep_the_best();
	while(generation<MAXGENS)
	{
	      generation++;
	      selectt();
	      crossover();
	 //// mutate();
	      report();
	      evaluate();
	      elitist();


	      fprintf(galog,"\nLoop %d\n",generation); 
	      fprintf(galog,"\nAll population\n");
	      for(i=0;i<POPSIZE;i++)
	      {
			 fprintf (galog,"genes:");
			 for(j=0;j<NVARS;j++)
			 {
				fprintf (galog,"%d",population[i].gene[j]);
			  }
			 fprintf (galog,"\tfitness:%3.6f\n",population[i].fitness);
		 } 

	      fprintf(galog,"Best member:\n"); 
	      value=0;
	      for (i = 0; i < NVARS; i++)
	      {
		  fprintf (galog,"%d\t",population[POPSIZE].gene[i]); 
		  value+=getValue(generation,i+1,population[POPSIZE].gene[i]); //计算实际的开销,fitness不是实际的.
	      }
	      fprintf(galog,"Best fitness = %3.6f,Reall cost:%3.6f\n",population[POPSIZE].fitness,value);
	      result+=value; 
	 }

	fprintf(galog,"\n\n Simulation completed\n"); 
	fprintf(galog,"Sum of fitness  = %3.6f\n",result);

	//fprintf(galog,"\n\n Simulation completed\n"); 
	//fprintf(galog,"\n Best member: \n"); 
	//for (i = 0; i < NVARS; i++)
	//{
	//   	fprintf (galog,"\n var(%d) = %d",i,population[POPSIZE].gene[i]);
	//}
	//fprintf(galog,"\n\n Best fitness = %3.3f",population[POPSIZE].fitness);

	fclose(galog);
	printf("Success\n");
	printf("Sum of fitness  = %3.6f\n",result);
}
/***************************************************************/



