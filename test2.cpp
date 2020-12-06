#include <chrono>
#include <ctime>
#include <iostream>
#include <random>
using namespace std;

int main()
{
    // your code goes here
    unsigned seed;
    default_random_engine generator;
    uniform_int_distribution<int> distribution;
    for (int i = 0; i < 3; i++)
    {
        seed = std::chrono::system_clock::now().time_since_epoch().count();
        generator = default_random_engine(seed);
        distribution = uniform_int_distribution<int>(0, 100); // 1~49
        cout << distribution(generator) << endl;
    }
    return 0;
}