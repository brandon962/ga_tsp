#include "GaTsp.h"
#include <chrono>
#include <ctime>
#include <iostream>
#include <string>

using namespace std;

int main()
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    // GaTsp(unsigned seed,int _num_runs,int _num_iters, int _num_chrom, int _num_cities, string filename)
    // GaTsp ga = GaTsp(seed, 30, 500, 500, 51, "eil51.txt", 0.8, 0.8, 0.2, 2, 50);
    // GaTsp ga = GaTsp(seed, 30, 500, 500, 51, "eil51.txt", 0.8, 0.8, 0.2, 1, 50);
    GaTsp ga = GaTsp(seed, 30, 500, 500, 51, "eil51.txt", 0.8, 0.8, 0.2, 0, 50);

    return 0;
}