# Ant colony optimization algorithnm

The implementation here is essentially a max min algorithm with an added mechanism of elitism.

main takes in a lot of arguments, all of them are single letters followed by a value, numerical or otherwise, here's what they mean:

	-f [path to file which contains the graph data, as shown in matrix.txt]
	-s [seed for the RNG, optional]
	-n [number of ants in the colony]
	-e [number of elite ants, aka how many best ants will leave pheromones]
	-T [t_max, starting pheromone value on all edges]
	-a [a hyperparameter used to calculate t_min, it represents the probability of an ant choosing a node that's not on the main path once the algorithm converges, a good value for it would be 0.05]
	-p [a hyperparameter used to update t_max after 1 epoch, used in updateT_max method]
	-i [number of iterations per epoch]
	-h [number of epochs]

Finding the right values for all of the hyperparameters can be tricky and heavily depends on the problem at hand. I might experiment with this algorithm further and improve it gradually, fixing any bugs and adding more features, but as of right now, it only serves as a good template for when I actually need it in the future.
