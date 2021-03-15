#include"Data-Funcs-Classes.h"


double MasterProblem(int** Ys, vector<Cut> Cuts)
{

#pragma region Fetch Data
	int nS = ProblemData::nS;
	int nD = ProblemData::nD;
	int* S = ProblemData::S;
	int* D = ProblemData::D;
	int** C = ProblemData::C;
	int** F = ProblemData::f;
	int BigM = ProblemData::BigM;
#pragma endregion

	IloEnv env;
	IloModel Model(env);

	IloNumVar Z(env, -IloInfinity, IloInfinity, ILOFLOAT);
	NumVar2D Y(env, nS);
	for (int s = 0; s < nS; s++)
	{
		Y[s] = IloNumVarArray(env, nD, 0, 1, ILOBOOL);
	}

	Model.add(IloMinimize(env, Z));

	// add the feasibility cuts
	int nCuts = Cuts.size();
	for (int c = 0; c < nCuts; c++)
	{
		if (Cuts[c].add_optimality_cut)
		{
			IloExpr exp1(env);
			for (int s = 0; s < nS; s++)
			{
				exp1 += -S[s] * Cuts[c].Us[s];
				for (int d = 0; d < nD; d++)
				{
					exp1 += F[s][d] * Y[s][d];
					exp1 += -BigM * Cuts[c].Ws[s][d] * Y[s][d];
				}
			}
			for (int d = 0; d < nD; d++)
			{
				exp1 += D[d] * Cuts[c].Vs[d];
			}
			Model.add(Z >= exp1);
			exp1.end();
		}
	}


	// add optimality cuts
	for (int c = 0; c < nCuts; c++)
	{
		if (Cuts[c].add_feasibility_cut)
		{
			IloExpr exp2(env);
			for (int s = 0; s < nS; s++)
			{
				exp2 += -S[s] * Cuts[c].Us_ray[s];
				for (int d = 0; d < nD; d++)
				{
					exp2 += -BigM * Cuts[c].Ws_ray[s][d] * Y[s][d];
				}
			}
			for (int d = 0; d < nD; d++)
			{
				exp2 += D[d] * Cuts[c].Vs_ray[d];
			}
			Model.add(exp2 <= 0);
			exp2.end();
		}
	}
#pragma region Solve the problem
	IloCplex cplex(Model);
	//cplex.setParam(IloCplex::TiLim, EF_time);
	//cplex.setParam(IloCplex::EpGap, 0.60);
	cplex.setOut(env.getNullStream());

	if (!cplex.solve()) {
		env.error() << "Failed to optimize the Master Problem!!!" << endl;
		throw(-1);
	}

	double obj = cplex.getObjValue();
	std::cout << "\t MP obj value is: " << obj << endl;
	double gap = cplex.getMIPRelativeGap();
#pragma endregion

	for (int s = 0; s < nS; s++)
	{
		for (int d = 0; d < nD; d++)
		{
			Ys[s][d] = cplex.getValue(Y[s][d]);
			if (Ys[s][d] > 0.2)
			{
				//std::cout << "Y[" << s << "][" << d << "] = " << Ys[s][d] << endl;
			}
		}
	}

	return obj;
}




