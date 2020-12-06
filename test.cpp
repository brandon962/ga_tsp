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
    GaTsp ga = GaTsp(seed, 1, 3, 2, 10, "eil51.txt", 0.8, 0.2, 2);
    cout << "???";
    cout << endl
         << seed << endl;
    return 0;
}