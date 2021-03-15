#include"Data-Funcs-Classes.h"

double SubProblem(int** Ys, vector<Cut>& Cuts)
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

#pragma region Create and solve the problem
	IloEnv env;
	IloModel Model(env);
	IloNumVarArray U(env, nS, 0, IloInfinity, ILOFLOAT);
	IloNumVarArray V(env, nD, 0, IloInfinity, ILOFLOAT);
	NumVar2D W(env, nS);
	for (int s = 0; s < nS; s++)
	{
		W[s] = IloNumVarArray(env, nD, 0, IloInfinity, ILOFLOAT);
	}


	// Objective function
	IloExpr exp0(env);
	for (int s = 0; s < nS; s++){ exp0 -= S[s] * U[s]; }
	for (int d = 0; d < nD; d++){ exp0 += D[d] * V[d]; }
	for (int s = 0; s < nS; s++)
	{
		for (int d = 0; d < nD; d++)
		{
			exp0 -= BigM* Ys[s][d] * W[s][d];
			exp0 += F[s][d] * Ys[s][d];
		}
	}
	Model.add(IloMaximize(env, exp0));
	exp0.end();

	// constraints
	for (int s = 0; s < nS; s++)
	{
		for (int d = 0; d < nD; d++)
		{
			Model.add(-U[s] + V[d] - W[s][d] <= C[s][d]);
		}
	}
	IloCplex cplex(Model);

	// turn off the presolve to distinguish between infeasibility and unboundedness
	cplex.setParam(IloCplex::Param::Preprocessing::Presolve, 0);
	//cplex.setParam(IloCplex::TiLim, EF_time);
	//cplex.setParam(IloCplex::EpGap, 0.60);
	cplex.setOut(env.getNullStream());
	cplex.solve();
	std::cout << "\t The Subproblem is: ";
	std::cout << cplex.getStatus() << endl;
#pragma endregion


























	// create and initialize a new cut
	Cut new_cut(true);

#pragma region Get the extreme ray coefficients for the feasibility cut, if the problem is unbounded
	if (cplex.getStatus() == IloAlgorithm::Unbounded)
	{
		//std::cout << cplex.getStatus() << endl;
		new_cut.add_feasibility_cut = true;
		IloNumArray Vals(env);
		IloNumVarArray Vars(env);
		cplex.getRay(Vals, Vars);

	// find the index for U variable
		for (int h = 0; h < Vals.getSize(); h++)
		{
			for (int s = 0; s < nS; s++)
			{
				if (U[s].getId() == Vars[h].getId())
				{
					new_cut.Us_ray[s] = Vals[h]; break;
				}
			}
		}

		// find the index for V variable
		for (int h = 0; h < Vals.getSize(); h++)
		{
			for (int d = 0; d < nD; d++)
			{
				if (V[d].getId() == Vars[h].getId())
				{
					new_cut.Vs_ray[d] = Vals[h];
					//	std::cout << "Vs_ray: " << new_cut.Vs_ray[d] << endl;
					break;
				}
			}
		}

		// find the index for W variable
		for (int h = 0; h < Vals.getSize(); h++)
		{
			for (int s = 0; s < nS; s++)
			{
				for (int d = 0; d < nD; d++)
				{
					if (W[s][d].getId() == Vars[h].getId())
					{
						new_cut.Ws_ray[s][d] = Vals[h];
						//	std::cout << "Ws_ray: " << new_cut.Ws_ray[s][d] << endl;
						break;
					}

				}
			}
		}

		//env.error() << "Failed to optimize the Master Problem!!!" << endl;
		Model.end();
		
		// add the cut
		Cuts.push_back(new_cut);
		return INFINITY; // maximization problem is unbounded
	}
#pragma endregion



#pragma region Get the values for decision variables (duals) for the optimality cut
	new_cut.add_optimality_cut = true;
	double obj = cplex.getObjValue();
	std::cout << "\t SP obj value is: " << obj << endl;
	double gap = cplex.getMIPRelativeGap();


	for (int s = 0; s < nS; s++)
	{
		new_cut.Us[s] = cplex.getValue(U[s]);
		//std::cout << "Us: " << new_cut.Us[s] << endl;
	}

	for (int d = 0; d < nD; d++)
	{
		new_cut.Vs[d] = cplex.getValue(V[d]);
		//std::cout << "Vs: " << new_cut.Vs[d] << endl;
	}

	//new_cut.Ws = new double* [nS];
	for (int s = 0; s < nS; s++)
	{
		//new_cut.Ws[s] = new double[nD]();
		for (int d = 0; d < nD; d++)
		{
			new_cut.Ws[s][d] = cplex.getValue(W[s][d]);
			if (new_cut.Ws[s][d] > 0.2)
			{
				//std::cout << "Ws[" << s << "][" << d << "] = " << new_cut.Ws[s][d] << endl;
			}
		}
	}
#pragma endregion


	// add the cut
	Cuts.push_back(new_cut);
	Model.end();
	return obj;
}


