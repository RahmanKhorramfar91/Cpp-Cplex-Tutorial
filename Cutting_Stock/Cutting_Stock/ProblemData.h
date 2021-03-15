#pragma once
#include <iostream>;  // for  cout cin
#include<random>; // for random number generation
#include <fstream>; // to read and write from/on files
#include<chrono>; // for high resolution timing (elapsed time in microseconds and so on)
#include"ilcplex/ilocplex.h";
typedef IloArray<IloNumVarArray>  NumVar2D;
typedef IloArray< IloRangeArray> Range2D;
typedef IloArray<NumVar2D> NumVar3D;
using namespace std;


class ProblemData
{
public:
	static int LL;
	static int nL;
	static  int* ReqL;
	static int* b;


	static void PopulateParamts();
};