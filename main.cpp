#include "GaTsp.h"
#include <chrono>
#include <ctime>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    if (argc > 3)
    {
        GaTsp gatsp = GaTsp(seed, atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), argv[5], atof(argv[6]), atof(argv[7]), atof(argv[8]), atoi(argv[9]), atoi(argv[10]));
    }
    else
    {
        GaTsp gaCX = GaTsp(seed, 30, 200, 1000, 51, "eil51.txt", 0.8, 0.8, 0.2, 2, 100);
        GaTsp gaOX = GaTsp(seed, 30, 200, 1000, 51, "eil51.txt", 0.8, 0.8, 0.2, 1, 100);
        GaTsp gaPMX = GaTsp(seed, 30, 200, 1000, 51, "eil51.txt", 0.8, 0.8, 0.2, 0, 100);
    }
    return 0;
}