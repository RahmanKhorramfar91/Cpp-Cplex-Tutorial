#include"Data-Funcs-Classes.h"

int main(int argc, char* argv[])
{

#pragma region Set Problem Params
	// assume that the problem 
	if (argc >= 2)
	{
		ProblemData::nS = atoi(argv[1]);
		ProblemData::nD = atoi(argv[2]);
		ProblemData::seed = atoi(argv[3]);
	}
	else
	{
		ProblemData::nS = 4;
		ProblemData::nD = 3;
		ProblemData::seed = 39;
	}
#pragma endregion
	
	RandLib RL(ProblemData::seed);
	ProblemData::Populate_Parameters(RL);



#pragma region Fetch Data
	int nS = ProblemData::nS;
	int nD = ProblemData::nD;
	int* S = ProblemData::S;
	int* D = ProblemData::D;
	int** C = ProblemData::C;
	int** F = ProblemData::f;
	int BigM = ProblemData::BigM;
#pragma endregion

#pragma region initialization (get a feasible solution)
	// create and initialize the y vairable
	int** Ys = new int* [nS];
	for (int s = 0; s < nS; s++)
	{
		Ys[s] = new int[nD]();
		for (int d = 0; d < nD; d++) { Ys[s][d] = 1; }
	}

	// set bounds
	double UB = 10e6;
	double LB = -10e6;
#pragma endregion

#pragma region Main Loop

	vector<Cut> Cuts;

	double SP_obj = 0; double MP_obj = 0; int iter_count = 0;
	
	while (std::abs(UB - LB) > 10e-3)
	{
		iter_count++;
		cout << "\n\n \t\t Iteration: " << iter_count << "\t UB: " << UB << "\t LB: " << LB << endl;


		// solve the subproblem
		SP_obj = SubProblem(Ys, Cuts);
		MP_obj = MasterProblem(Ys, Cuts);
		
		// update bounds
		UB = std::min(UB, SP_obj);
		LB = MP_obj;
		
	}
#pragma endregion



	return 0;
}