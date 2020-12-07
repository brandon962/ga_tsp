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
    GaTsp ga = GaTsp(seed, 30, 2000, 20, 51, "eil51.txt", 0.7, 0.7, 0.2, 2, 20);
    cout << "???";

    return 0;
}