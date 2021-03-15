#include"Data-Funcs-Classes.h"




int ProblemData::nS;
int ProblemData::nD;
int ProblemData::seed;
int ProblemData::BigM;
int* ProblemData::D;
int* ProblemData::S;
int** ProblemData::C;
int** ProblemData::f;

void ProblemData::Populate_Parameters(RandLib RL)
{
	D = new int[nD]();
	S = new int[nS];
	C = new int* [nS]();
	f = new int* [nS]();
	int Dsum = 0;
	for (int d = 0; d < nD; d++)
	{
		D[d] = RL.randint(20, 150);
		Dsum += D[d];
	}

	// Assumption: Supply ~= Demand
	int Ssum = 0;
	for (int s = 0; s < nS; s++)
	{
		S[s] = RL.randint(150, 190);
		Ssum += S[s];
	}

	for (int s = 0; s < nS; s++)
	{
		S[s] = std::round((S[s] * Dsum) / Ssum) + 1;
		//cout << "S[" << s << "] = " << S[s] << endl;
	}

	for (int s = 0; s < nS; s++)
	{
		for (int d = 0; d < nD; d++)
		{
			BigM = std::max(D[d], S[s]);
		}
	}

	// populate C matrix
	for (int s = 0; s < nS; s++)
	{
		C[s] = new int[nD];
		for (int d = 0; d < nD; d++)
		{
			C[s][d] = RL.randint(1,10);
		}
	}


	// populate f matrix
	for (int s = 0; s < nS; s++)
	{
		f[s] = new int[nD];
		for (int d = 0; d < nD; d++)
		{
			f[s][d] = RL.randint(15,30);
		}
	}
}
