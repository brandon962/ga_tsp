#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include "GaTsp.h"

using namespace std;

int main()
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // GaTsp(unsigned seed,int _num_runs,int _num_iters, int _num_chrom, int _num_cities, string filename)
    GaTsp ga = GaTsp(seed,1,10,20,51,"eil51.txt");
    cout << "???";
    return 0;

}