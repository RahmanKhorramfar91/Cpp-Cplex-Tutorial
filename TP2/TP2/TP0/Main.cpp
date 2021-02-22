#include<iostream> 
#include"ilcplex/ilocplex.h";
#include<chrono>;
#include<random>;
#include"RandGen.h"
#include"TP_Data.h"


using namespace std;
typedef IloArray<IloNumVarArray> NumVar2D; // enables us to defien a 2-D decision varialbe
typedef IloArray<NumVar2D> NumVar3D;


int main()
{
	auto start = chrono::high_resolution_clock::now();
#pragma region Problem Setting
	ProblemData::nS = 50;
	ProblemData::nD = 60;

	ProblemData::seed = 39;
	RandLib RL(ProblemData::seed);
	ProblemData::Populate_Parameters(RL);
#pragma endregion


#pragma region Fetch Data
	int nS = ProblemData::nS;
	int nD = ProblemData::nD;
	int seed = 24;
	// create and object of RandLib
	int* S = ProblemData::S;
	int* D = ProblemData::D;
	int** C = ProblemData::C;
#pragma endregion

	IloEnv env;
	IloModel Model(env);

#pragma region Define decision variable
	NumVar2D X(env, nS);

	for (int s = 0; s < nS; s++)
	{
		X[s] = IloNumVarArray(env, nD, 0, IloInfinity, ILOINT);
	}
#pragma endregion

#pragma region Objective Function

	IloExpr exp0(env);

	for (int s = 0; s < nS; s++)
	{
		for (int d = 0; d < nD; d++)
		{
			exp0 += C[s][d] * X[s][d];
		}
	}

	Model.add(IloMinimize(env, exp0));

#pragma endregion


#pragma region Constraints
	// the total outgoing shipment cannot exceed S[s]
	for (int s = 0; s < nS; s++)
	{
		IloExpr exp1(env);
		for (int d = 0; d < nD; d++)
		{
			exp1 += X[s][d];
		}

		Model.add(exp1 <= S[s]);
	}


	for (int d = 0; d < nD; d++)
	{
		IloExpr exp2(env);
		for (int s = 0; s < nS; s++)
		{
			exp2 += X[s][d];
		}

		Model.add(exp2 >= D[d]);
	}
#pragma endregion



	IloCplex cplex(Model);
	cplex.setOut(env.getNullStream());
	if (!cplex.solve()) {
		env.error() << "Failed to optimize the Master Problem!!!" << endl;
		throw(-1);
	}

	double obj = cplex.getObjValue();

	auto end = chrono::high_resolution_clock::now();
	auto Elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
	cout << "\t Elapsed Time(ms): " << Elapsed.count() << endl;

	cout << "\n\n\t The objective value is: " << obj << endl;

	/*for (int s = 0; s < nS; s++)
	{
		for (int d = 0; d < nD; d++)
		{
			double Xval = cplex.getValue(X[s][d]);
			if (Xval > 0)
			{
				cout << "\t\t\t X[" << s << "][" << d << "] = " << Xval << endl;
			}

		}
	}*/


}