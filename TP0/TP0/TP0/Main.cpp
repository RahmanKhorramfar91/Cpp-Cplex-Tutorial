#include<iostream> 
#include"ilcplex/ilocplex.h";
#include<chrono>;


using namespace std;
typedef IloArray<IloNumVarArray> NumVar2D; // enables us to defien a 2-D decision varialbe
typedef IloArray<NumVar2D> NumVar3D;


int main()
{
	auto start = chrono::high_resolution_clock::now();

#pragma region Problem Data
	int nS = 4;
	int nD = 3;

	int* S = new int[nS] {10, 30, 40, 20};
	int* D = new int[nD] {20, 50, 30};

	int** C = new int* [nS];
	C[0] = new int[nD] {2, 3, 4};
	C[1] = new int[nD] {3, 2, 1};
	C[2] = new int[nD] {1, 4, 3};
	C[3] = new int[nD] {4, 5, 2};
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

	for (int s = 0; s < nS; s++)
	{
		for (int d = 0; d < nD; d++)
		{
			double Xval = cplex.getValue(X[s][d]);
			if (Xval > 0)
			{
				cout << "\t\t\t X[" << s << "][" << d << "] = " << Xval << endl;
			}

		}
	}


}