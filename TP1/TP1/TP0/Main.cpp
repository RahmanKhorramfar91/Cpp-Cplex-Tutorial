#include<iostream> 
#include"ilcplex/ilocplex.h";
#include<chrono>;
#include<random>;
using namespace std;
typedef IloArray<IloNumVarArray> NumVar2D; // enables us to defien a 2-D decision varialbe
typedef IloArray<NumVar2D> NumVar3D;

class RandLib
{
public:
	int Seed;
	std::default_random_engine Gen;

	double rand(); // continuous uniform distribution in [0,1]
	int randint(int, int);  // discrete uniform distribution in [a,b]
	double unifrnd(double, double);  // continuous uniform distribution in [a,b]

	RandLib(int sd)
	{
		this->Seed = sd;
		this->Gen.seed(sd);
	}
	RandLib() {};
	~RandLib() {};
};

double RandLib::rand()
{
	static std::uniform_real_distribution<double> rand2(0, 1);
	return rand2(Gen);
}

int RandLib::randint(int a, int b) {
	//static std::uniform_int_distribution<int> randint2(a, b);
	int ub = a + std::round((b - a) * rand());
	return std::min(b, ub);
}
double RandLib::unifrnd(double a, double b)
{
	//static std::uniform_real_distribution<double> unifrnd(a, b);
	//return unifrnd(Gen);
	return a + (b - a) * rand();
}

int main()
{
	auto start = chrono::high_resolution_clock::now();

#pragma region Problem Data

	int nS = 100;
	int nD = 95;
	int seed = 24;
	// create and object of RandLib
	RandLib RL(seed);



	int* S = new int[nS];
	int* D = new int[nD];

	int** C = new int* [nS];

	int Dsum = 0;
	for (int d = 0; d < nD; d++)
	{
		D[d] = RL.randint(20, 150);
		Dsum += D[d];
		//cout <<"D["<<d<<"] = "<< D[d] << endl;
	}

	// Assumption: Supply ~= Demand
	int Ssum = 0;
	for (int s = 0; s < nS; s++)
	{
		S[s] = RL.randint(150, 190);
		Ssum += S[s];
	}
	int gg = 0;
	for (int s = 0; s < nS; s++)
	{
		S[s] = std::round((S[s] * Dsum) / Ssum) + 1;
		//cout << "S[" << s << "] = " << S[s] << endl;
		gg += S[s];
	}

	// populate C matrix
	for (int s = 0; s < nS; s++)
	{
		C[s] = new int[nD];
		for (int d = 0; d < nD; d++)
		{
			C[s][d] = RL.randint(1, 15);
		}
	}

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