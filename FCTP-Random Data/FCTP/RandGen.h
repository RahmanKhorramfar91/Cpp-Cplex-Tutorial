#pragma once
#include<random>;


class RandLib
{
public:
	int Seed;
	std::default_random_engine Gen;

	double rand(); // continuous uniform distribution in [0,1]
	int randint(int, int);  // discrete uniform distribution in [a,b]
	double unifrnd(double, double);  // continuous uniform distribution in [a,b]


	void Reseed(int);
	RandLib(int sd)
	{
		this->Seed = sd;
		this->Gen.seed(sd);
	}
	//static pcg_extras::seed_seq_from<std::random_device> seed_source;
	RandLib() {};
	~RandLib() {};
private:
};