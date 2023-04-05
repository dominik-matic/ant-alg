#include <iostream>
#include <getopt.h>
#include <string>
#include "AntAlg.cpp"

int main(int argc, char* argv[]) {
	
	struct {
		std::string file_path;
		int seed;
		int n_ants;
		int e_ants;
		int iters;
		int epochs;
		double T;
		double a;
		double p;
	} params;

	for(;;) {
		switch(getopt(argc, argv, "f:n:e:T:a:p:s:i:h:")) {
			case -1:
				break;
			case 'f':
				params.file_path = optarg;
				continue;
			case 's':
				params.seed = std::atoi(optarg);
				continue;
			case 'n':
				params.n_ants = std::atoi(optarg);
				continue;
			case 'e':
				params.e_ants = std::atoi(optarg);
				continue;
			case 'T':
				params.T = std::atof(optarg);
				continue;
			case 'a':
				params.a = std::atof(optarg);
				continue;
			case 'p':
				params.p = std::atof(optarg);
				continue;
			case 'i':
				params.iters = std::atof(optarg);
				continue;
			case 'h':
				params.epochs = std::atof(optarg);
				continue;
			
		}
		break;
	}
	std::cout << "Loading " << params.file_path <<
				"...\nn_ants=" << params.n_ants <<
				"\ne_ants= " << params.e_ants <<
				"\nt_max=" << params.T <<
				"\na=" << params.a <<
				"\np=" << params.p <<
				"\nseed=" << params.seed <<
				"\niters=" << params.iters <<
				"\nepochs=" << params.epochs << "\n";
	AntAlg alg(params.file_path, params.n_ants, params.e_ants, params.T, params.a, params.p, params.seed);

	AntSolution ant = alg.execute(params.epochs, params.iters);
	std::cout << "Best ant value = " << ant.value << "\n";
	for(size_t i = 0; i < ant.solution.size(); ++i) {
		std::cout << ant.solution[i] << " ";
	}
	std::cout << "\n";
}