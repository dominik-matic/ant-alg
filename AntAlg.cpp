#include <cmath>
#include <string>
#include <vector>
#include <random>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>


struct AntSolution {
		std::vector<size_t> solution;
		double cost;
		double value;
};

struct custom_comparator {
	inline bool operator() (const AntSolution& struct1, const AntSolution& struct2)
	{
		return (struct1.cost < struct2.cost);
	}
};

class AntAlg {
	private:

	double t_max = 1.0;
	double t_min;

	size_t n_ants;
	size_t n_elite;
	double a;
	double p;
	std::vector<std::vector<double>> pheromones;
	std::vector<double> pheromoneSums;
	std::vector<std::vector<double>> edges;

	std::random_device rd;
	std::mt19937 rng;
	std::uniform_int_distribution<size_t> edge_dist;

	/**
	 * Adjust according to the problem
	*/
	void loadEdges(std::string filepath) {
		std::fstream file(filepath);
		std::string line;
		double number;
		while(std::getline(file, line)) {
			std::istringstream iss(line);
			std::vector<double> vect;
			while(iss >> number) {
				vect.push_back(number);
			}
			edges.push_back(vect);
		}
	}

	/**
	 * Adjust according to the problem
	*/
	AntSolution createSolution() {
		AntSolution ant;
		size_t startingNode = this->getRandomNode();
		ant.solution.push_back(startingNode);
		ant.value = 1.0;
		for(size_t i = 0; i < 100; ++i) {
			int nextNode;
			do {
				nextNode = this->getRandomNode();
			} while(nextNode == ant.solution.back());

			ant.solution.push_back(nextNode);
			ant.value *= this->edges[*(ant.solution.end() - 2)][*(ant.solution.end() - 1)];
		}
		ant.cost = 1.0 / ant.value;
		return ant;
	}

	/**
	 * Adjust according to the problem
	*/
	void updatePheromones(AntSolution ant) {
		for(size_t i = 0; i < ant.solution.size() - 1; ++i) {
			this->pheromones[ant.solution[i]][ant.solution[i + 1]] += ant.value;
		}
	}

	void printVector(std::vector<std::vector<double>> vector) {
		for(size_t i = 0; i < vector.size(); ++i) {
			std::cout << vector[i][0];
			for(size_t j = 1; j < vector[i].size(); ++j) {
				std::cout << " " << vector[i][j];
			}
			std::cout << "\n";
		}
	}

	void updateSums() {
		for(size_t i = 0; i < this->pheromoneSums.size(); ++i) {
			this->pheromoneSums[i] = -this->pheromones[i][i];
			for(size_t j = 0; j < this->pheromones[i].size(); ++j) {
				this->pheromoneSums[i] += this->pheromones[i][j];
			}
		}
	}

	void initPheromones() {
		int dim = this->edges.size();
		this->pheromones = std::vector<std::vector<double>>(dim, std::vector<double>(dim, this->t_max));
		this->pheromoneSums = std::vector<double>(dim, 0);
		this->updateSums();
	}

	void updateT_min() {
		double dim = (double) this->edges.size();
		this->t_min = (this->t_max * (1 - std::pow(this->a, 1.0/dim)))
					/ (((dim/2) - 1) * std::pow(this->a, 1.0/dim));
	}

	void updateT_max(double bestScore) {
		this->t_max = 1 / (this->p * bestScore);
	}

	void evaporatePheromones() {
		int dim = this->edges.size();
		for(size_t i = 0; i < dim; ++i) {
			for(size_t j = 0; j < dim; ++j) {
				this->pheromones[i][j] *= this->p;
			}
		}
	}

	int getRandomNode() {
		return this->edge_dist(this->rng);
	}

	public:
	AntAlg(std::string filepath, size_t n, size_t e, double t_max, double a, double p, int seed) {
		this->n_ants = n;
		this->n_elite = std::min(e, n);
		this->t_max = t_max;
		this->a = a;
		this->p = p;
		this->loadEdges(filepath);
		this->rng = std::mt19937(this->rd());
		this->edge_dist = std::uniform_int_distribution<size_t>(0, this->edges.size() - 1);
		this->rng.seed(seed);
	}

	/*
	 * Returns a vector of indices which represent the path taken,
	 * for example, [0, 4, 5] represents starting from 0, going to
	 * 4, then going to 5, finishing the trip in a total of 2 steps
	*/
	AntSolution execute(size_t nEpochs, size_t nIters) {
		AntSolution bestAnt;
		bestAnt.value = 0;
		
		std::vector<AntSolution> ants(this->n_ants, AntSolution());
		for(size_t epoch = 0; epoch < nEpochs; ++epoch) {
			std::cout << "Epoch " << epoch << "\n";
			this->initPheromones();
			this->updateT_min();
			for(size_t iter = 0; iter < nIters; ++iter) {

				for(size_t i = 0; i < this->n_ants; ++i) {
					ants[i] = this->createSolution();
				}
				
				/*
					Possible optimization here:
					change sorting based on n_elite:
					1. if n_elite == ants.size()
						don't sort
					2. if n_elite <= ants.size()
						partial_sort first n_elite
					3. if n_elite > ants.size()
						partial_sort ants.size() - n_elite with inverted comparator, then reverse the array
				*/
				std::partial_sort(ants.begin(), ants.begin() + this->n_elite, ants.end(), custom_comparator());

				for(size_t i = 0; i < this->n_elite; ++i) {
					this->updatePheromones(ants[i]);
				}

				this->evaporatePheromones();
				this->updateSums();

				// save best ant
				if(ants[0].value > bestAnt.value) {
					bestAnt = ants[0];
				}

				if((iter + 1) % 1000 == 0 ) {
					std::cout << "Iter " << iter + 1 << " Best fitness = " << bestAnt.value << "\n";
				}
			}
			this->updateT_max(ants[0].cost);
		}
		return bestAnt;
	}
};