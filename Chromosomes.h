// 染色體class

#include "myVector.h"
#include <iostream>
#include <random>
#include <climits>
#include <chrono>
#include <ctime>

class Chromosomes
{
private:
    int num_cities;
    int path_len;
    i1d path;
    void initPath();
public:
    int getPath(int i);
    i1d getPath();
    Chromosomes(int num_c);
    ~Chromosomes();

};

Chromosomes::Chromosomes(int num_c)
{
    std::cout << "!!!" << endl;
    num_cities = num_c;
    path.assign(num_cities+1,0);
    initPath();
}

Chromosomes::~Chromosomes()
{

}

int Chromosomes::getPath(int i)
{
    return path[i];
}

i1d Chromosomes::getPath()
{
    return path;
}
// 初始化路徑
void Chromosomes::initPath()
{
    int temp_int;
    vector<int>::iterator iter;
    i1d chose_cities;
    // 亂數產生器
    unsigned seed = std::chrono::system_clock()::now();
    std::default_random_engine gen = std::default_random_engine(rd());
    std::uniform_int_distribution<int> rand_max(0,INT_MAX);

    chose_cities.assign(num_cities,0);
    iter=chose_cities.begin();
    for (int i = 0; i < num_cities; i++)
        chose_cities[i] = i;
        
    // 將路線隨機分配至path中
    temp_int = rand_max(gen)%chose_cities.size();
    path[0] = chose_cities[temp_int];
    path[num_cities] = chose_cities[temp_int];
    chose_cities.erase(iter+ temp_int);
    for(int i = 1; i < num_cities; i++)
    {
        temp_int = rand_max(gen)%chose_cities.size();
        path[i] = chose_cities[temp_int];
        chose_cities.erase(iter+ temp_int);
    }


}