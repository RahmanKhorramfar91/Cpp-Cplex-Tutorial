#include"Functions.h";



double SubProblem(double* Dual_Vals, int* NewPattern)
{
#pragma region Fetch Data
	int Log_Length = ProblemData::LL;
	int nLength = ProblemData::nL;
	int* Req_Length = ProblemData::ReqL;
	int* b = ProblemData::b;
#pragma endregion

	IloEnv env;
	IloModel Model(env);

	IloNumVarArray newPat(env, nLength, 0, IloInfinity, ILOINT);

	IloExpr exp0(env);
	for (int i = 0; i < nLength; i++)
	{
		exp0 += Dual_Vals[i] * newPat[i];
	}
	Model.add(IloMaximize(env, exp0));

	IloExpr exp1(env);
	for (int i = 0; i < nLength; i++)
	{
		exp1 += Req_Length[i] * newPat[i];
	}
	Model.add(exp1 <= Log_Length);
	IloCplex cplex(Model);
	cplex.setOut(env.getNullStream());
	if (!cplex.solve()) {
		env.error() << "Failed to optimize the Master Problem!!!" << endl;
		throw(-1);
	}
	//NewPattern = new int[nLength]();
	std::cout << "\t New patttern generated: [";
	for (int i = 0; i < nLength; i++)
	{
		NewPattern[i] = cplex.getValue(newPat[i]);
		std::cout << " " << NewPattern[i];
	}
	std::cout << "]" << endl;
	double obj = cplex.getObjValue();
	std::cout << "\n\t\t SP obj is: " << obj << endl;
	std::cout << "\n\n\n\n";
	return obj;
}

