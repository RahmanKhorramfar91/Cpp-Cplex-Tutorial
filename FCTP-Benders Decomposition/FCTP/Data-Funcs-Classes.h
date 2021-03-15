#pragma once
#include <iostream>;  // for  cout cin
#include<random>; // for random number generation
#include <fstream>; // to read and write from/on files
#include<chrono>; // for high resolution timing (elapsed time in microseconds and so on)
#include"ilcplex/ilocplex.h";
#include"RandGen.h"
using namespace std;
typedef IloArray<IloNumVarArray>  NumVar2D;

class Cut
{
public:
	int nS;
	int nD;
	double* Us;
	double* Vs;
	double** Ws;

	double* Us_ray;
	double* Vs_ray;
	double** Ws_ray;
	Cut(bool is_initialize);

	bool add_optimality_cut;
	bool add_feasibility_cut;
};


double SubProblem(int** Ys, vector<Cut>& Cuts);

double MasterProblem(int** Ys, vector<Cut> Cuts);

class ProblemData
{
public:
	static int nS;
	static int nD;
	static int seed;

	static int BigM;
	static int* D;
	static int* S;
	static int** C;
	static int** f;
	static void Populate_Parameters(RandLib RL);
};





