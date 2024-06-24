#include <stdlib.h>
#include <iostream>
#include <vector>
#include <omp.h>
#include <time.h>

#define SIZE 400
#define SIMULATIONS 1000

void simulateTemperature(std::vector<std::vector<double>>& plate, int numThreads, int chunk) {
    int i, j, k;
    std::vector<std::vector<double>> tempPlate(SIZE, std::vector<double>(SIZE));

#pragma omp parallel for private(i, j) num_threads(numThreads) schedule(guided, chunk)
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            if (i == 0 || i == SIZE - 1 || j == 0 || j == SIZE - 1)
                plate[i][j] = 100.0;  // The edges temperature is 100 degrees
            else
                plate[i][j] = 40.0;
            tempPlate[i][j] = 40.0;
        }
    }

    for (k = 0; k < SIMULATIONS; k++) {
#pragma omp parallel for private(i, j) num_threads(numThreads) schedule(guided, chunk)
        for (i = 1; i < SIZE - 1; i++) {
            for (j = 1; j < SIZE - 1; j++) {
                tempPlate[i][j] = (plate[i - 1][j] + plate[i + 1][j] + plate[i][j - 1] + plate[i][j + 1]) / 4.0;
            }
        }

#pragma omp parallel for private(i, j) num_threads(numThreads) schedule(guided, chunk)
        for (i = 1; i < SIZE - 1; i++) {
            for (j = 1; j < SIZE - 1; j++) {
                plate[i][j] = tempPlate[i][j];
            }
        }
    }
}

int main() {
    std::vector<std::vector<double>> plate(SIZE, std::vector<double>(SIZE));
    double maxTemp = 0.0, minTemp = 100.0;

    for (int numThreads = 2; numThreads <= 8; numThreads++) {
        double startTime = omp_get_wtime();

        int chunk = SIZE / numThreads;
        simulateTemperature(plate, numThreads, chunk);

        double endTime = omp_get_wtime();
        double elapsedTime = endTime - startTime;

        double currentMax = 0.0, currentMin = 100.0;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (plate[i][j] > currentMax)
                    currentMax = plate[i][j];
                if (plate[i][j] < currentMin)
                    currentMin = plate[i][j];
            }
        }

        if (currentMax > maxTemp)
            maxTemp = currentMax;
        if (currentMin < minTemp)
            minTemp = currentMin;

        std::cout << "scheduling type: guided" << std::endl;
        std::cout << "Number of Threads: " << numThreads << std::endl;
        std::cout << "Maximum temperature: " << currentMax << std::endl;
        std::cout << "Minimum temperature: " << currentMin << std::endl;
        std::cout << "Elapsed time: " << elapsedTime << " seconds" << std::endl;
        std::cout << "---------------------------------------------------------" << std::endl << std::endl;
    }

    std::cout << "Overall maximum temperature: " << maxTemp << std::endl;
    std::cout << "Overall minimum temperature: " << minTemp << std::endl;

    return 0;
}