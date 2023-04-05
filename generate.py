import numpy as np


"""
	Used for testing. Generates a random MxN matrix
	with 1s on the diagonal, supposed to represent
	a fully connected graph with all weights between
	0.5 and 1.5
"""
def generate(M=30, N=30, filename='matrix.txt'):
	A = np.random.rand(M, N)
	A = A + 0.5
	np.fill_diagonal(A, 1)
	
	with open(filename, "w") as file:
		for i in range(M):
			for j in range(N):
				file.write(str(A[i][j]))
				if j == N - 1 and i != M - 1:
					file.write("\n")
				else:
					file.write(" ")


if __name__ == '__main__':
	generate()