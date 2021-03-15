#include"RandGen.h"
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
