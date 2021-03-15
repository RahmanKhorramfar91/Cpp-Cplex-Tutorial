#include"ProblemData.h"
#include"Functions.h";


int main()
{
#pragma region Fetch Data and Initialize the algorithm
	int Log_Length = ProblemData::LL; // total length of a log
	int* Req_Length = ProblemData::ReqL; // order lengths
	int nLength = ProblemData::nL;
	int* b = ProblemData::b; // demand

	// generate initial patterns
	vector<int*> Pattern;
	for (int i = 0; i < nLength; i++)
	{
		int* Pat0 = new int[nLength]();
		Pat0[i] = std::floor(Log_Length / Req_Length[i]);

		Pattern.push_back(Pat0);
	}



	// print initial patterns
	cout << "Initial patterns" << endl;
	for (int p = 0; p < Pattern.size(); p++)
	{
		cout << "Pattern " << p << " [";
		for (int i = 0; i < nLength; i++)
		{
			cout << " " << Pattern[p][i];
		}
		cout << "]" << endl;
	}

#pragma endregion


	int iter_count = 0;

	while (true)
	{
		iter_count++;
		// Solve the restricted master problem (RMP)
		IloEnv env; IloModel Model(env);


		IloNumVarArray X(env, Pattern.size(), 0, IloInfinity, ILOFLOAT);
		IloRangeArray cons_set1(env); // name constraints to get the duals later

		// define the objective function
		IloExpr exp0(env);
		for (int p = 0; p < Pattern.size(); p++) { exp0 += X[p]; }

		Model.add(IloMinimize(env, exp0)); exp0.end();

		// constraint
		for (int i = 0; i < nLength; i++)
		{
			IloExpr exp1(env);
			for (int p = 0; p < Pattern.size(); p++)
			{
				exp1 += Pattern[p][i] * X[p];
			}
			cons_set1.add(exp1 >= b[i]);
//			Model.add(exp1 >= b[i]);
		}
		Model.add(cons_set1);

		IloCplex cplex(Model);
		cplex.setOut(env.getNullStream());
		cplex.solve();
		double obj = cplex.getObjValue();
		std::cout << "\n\t Iteration: " << iter_count << "\t Required Number of logs: " << obj << endl;

		// get the dual variables
		double* Duals = new double[nLength]();
		for (int i = 0; i < nLength; i++)
		{
			Duals[i] = cplex.getDual(cons_set1[i]);
			//cout << Duals[i] << endl;
		}
		int jj = 0;
		// generate a new pattern from the subproblem (knapsack problem)
		int* NewPat = new int[nLength];
		double SP_obj = SubProblem(Duals, NewPat);

		// check the optimality condition/ add the new pattern
		if (1 - SP_obj >= 0) { break; }
		else
		{
			Pattern.push_back(NewPat);
		}
	}




	cout << "Final patterns" << endl;
	for (int p = 0; p < Pattern.size(); p++)
	{
		cout << "Pattern " << p << " [";
		for (int i = 0; i < nLength; i++)
		{
			cout << " " << Pattern[p][i];
		}
		cout << "]" << endl;
	}
	return 0;
}


//Range2D Const2D(env, M); for 2-D dual variables
//for (int i = 0; i < 5; i++)
//{
//	Const2D[i] = IloRangeArray(env,M);
//}

