#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <vector>
#include <chrono>
#include <thread>

const int size = 40;
const int steps = 1000;

void initialize(std::vector<std::vector<int>>& grid, int size) {
   for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
         grid[i][j] = rand() % 2;
      }
   }
}

void print(std::vector<std::vector<int>>& grid, int size) {
   for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
         std::cout << (grid[i][j] ? "#" : " ");
      }
      std::cout << std::endl;
   }
   std::cout << std::endl;
   //std::cout << "--------------------------------------------------------------" << std::endl;

}

void simulate(std::vector<std::vector<int>>& grid, int size) {
   std::vector<std::vector<int>> temp(size, std::vector<int>(size));
#pragma omp parallel for
   for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
         int neighbors = 0;
         for (int x = -1; x <= 1; x++) {
            for (int y = -1; y <= 1; y++) {
               int newX = i + x;
               int newY = j + y;
               if (newX >= 0 && newX < size && newY >= 0 && newY < size && !(x == 0 && y == 0)) {
                  neighbors += grid[newX][newY];
               }
            }
         }
         if (grid[i][j] == 1) {
            if (neighbors < 2 || neighbors > 3) {
               temp[i][j] = 0;
            }
            else {
               temp[i][j] = 1;
            }
         }
         else {
            if (neighbors == 3) {
               temp[i][j] = 1;
            }
            else {
               temp[i][j] = 0;
            }
         }
      }
   }
#pragma omp parallel for
   for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
         grid[i][j] = temp[i][j];
      }
   }
}

int main() {
   std::vector<std::vector<int>> grid(size, std::vector<int>(size));
   initialize(grid, size);
   for (int i = 0; i < steps; i++) {
      if (system("CLS")) system("clear");
      std::cout << "Step " << i + 1 << ":" << std::endl;
      print(grid, size);
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      simulate(grid, size);
   }
   return 0;
}