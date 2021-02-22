#pragma once

#include"RandGen.h"



class ProblemData
{
public:
	static int nS;
	static int nD;
	static int seed;

	static int* D;
	static int* S;
	static int** C;
	static void Populate_Parameters(RandLib RL);
};