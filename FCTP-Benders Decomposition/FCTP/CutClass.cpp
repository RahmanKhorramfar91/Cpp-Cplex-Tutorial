#include"Data-Funcs-Classes.h"


Cut::Cut(bool is_init)
{
	add_optimality_cut = false;
	add_feasibility_cut = false;
	int nS = 4;
	int nD = 3;
	Us = new double[nS]();
	Vs = new double[nD]();
	Ws = new double* [nS];
	for (int s = 0; s < nS; s++)
	{
		Ws[s] = new double[nD]();
	}

	Us_ray = new double[nS]();
	Vs_ray = new double[nD]();
	Ws_ray = new double* [nS];
	for (int s = 0; s < nS; s++)
	{
		Ws_ray[s] = new double[nD]();
	}
}

