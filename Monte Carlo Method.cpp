#include <iostream>
#include <random>
#include <mpi.h>
using namespace std;

int main(int argc, char *argv[]) {
    int rank, size;
    const int number_of_tosses = 100000;
    int numberInCircle = 0;
    int localNumberInCircle = 0;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // Seed the random number generator
    random_device rd;
    // Add rank to ensure different seeds for different processes
    mt19937 gen(rd() + rank);
    uniform_real_distribution<double> dis(-1.0, 1.0);
    
    for (int toss = 0; toss < number_of_tosses / size; toss++) {
        double x = dis(gen);
        double y = dis(gen);
        double distanceSquared = x * x + y * y;
        if (distanceSquared <= 1.0) {
            localNumberInCircle++;
        }
    }
    
    // Reduce localNumberInCircle using MPI_Reduce to get the total numberInCircle
    MPI_Reduce(&localNumberInCircle, &numberInCircle, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        double PI_Estimation = 4.0 * numberInCircle / (double)(number_of_tosses);
        cout << "Number of processes: " << size << endl;
        cout << "Estimated value of π: " << PI_Estimation << endl;
    } 
    
    MPI_Finalize();
    return 0;
}