// GA class
#define _CRT_SECURE_NO_WARNINGS
#include "myVector.h"
#include <climits>
#include <iostream>
#include <random>
#include <string>

class GaTsp
{
private:
    /* data */
    int num_runs;
    int num_iters;

    int num_cities;
    int num_chromos;
    i2d place_table;
    d2d distance_table;
    i2d chromo_set;
    d1d chromo_fit_set;

    //  random
    std::default_random_engine gen;
    std::uniform_int_distribution<int> rand_max;

public:
    GaTsp(unsigned seed, int _num_runs, int _num_iters, int _num_chroms, int _num_cities, string _input_filename);
    ~GaTsp();
    void initChromo();
    void printAllChromo();
    void readDistance(string _input_filename);
    void countChromoFit();
    void run();
};

GaTsp::GaTsp(unsigned seed, int _num_runs, int _num_iters, int _num_chroms, int _num_cities, string _input_filename)
{
    num_runs = _num_runs;
    num_iters = _num_iters;
    num_chromos = _num_chroms;
    num_cities = _num_cities;
    rand_max = uniform_int_distribution<int>(0, INT_MAX);
    chromo_set.assign(num_chromos, i1d(num_cities, 0));
    chromo_fit_set.assign(num_chromos, 0);
    distance_table.assign(num_cities, d1d(num_cities, 0));
    place_table.assign(num_cities, i1d(2, 0));

    readDistance(_input_filename);
    initChromo();
    countChromoFit();
    printAllChromo();

    cout << "!!!" << endl;
}

GaTsp::~GaTsp()
{
    distance_table.clear();
    distance_table.shrink_to_fit();
    place_table.clear();
    place_table.shrink_to_fit();
    chromo_set.clear();
    chromo_set.shrink_to_fit();
    chromo_fit_set.clear();
    chromo_fit_set.shrink_to_fit();
}

void GaTsp::initChromo()
{
    int itemp;
    vector<int>::iterator cities_iterator;
    for (int chromos = 0; chromos < num_chromos; chromos++)
    {
        i1d temp_cities;
        temp_cities.assign(num_cities, 0);
        for (int cities = 0; cities < num_cities; cities++)
            temp_cities[cities] = cities;
        cities_iterator = temp_cities.begin();

        itemp = rand_max(gen) % temp_cities.size();
        chromo_set[chromos][0] = temp_cities[itemp];
        temp_cities.erase(cities_iterator + itemp);
        for (int cities = 1; cities < num_cities; cities++)
        {
            itemp = rand_max(gen) % temp_cities.size();
            chromo_set[chromos][cities] = temp_cities[itemp];
            temp_cities.erase(cities_iterator + itemp);
        }
    }
}

void GaTsp::printAllChromo()
{
    for (int chromos = 0; chromos < num_chromos; chromos++)
    {
        cout << chromos << " chromos" << endl;
        for (int j = 0; j < num_cities; j++)
            cout << chromo_set[chromos][j] << " ";
        cout << endl;
        cout << "chromos length : " << chromo_fit_set[chromos]
             << endl
             << endl;
    }
}

void GaTsp::readDistance(string _input_filename)
{
    FILE *fp;
    fp = fopen(_input_filename.c_str(), "r");
    for (int cities = 0; cities < num_cities; cities++)
        fscanf(fp, "%d %d %d", &place_table[cities][0], &place_table[cities][0], &place_table[cities][1]);
    fclose(fp);

    for (int cities_s = 0; cities_s < num_cities; cities_s++)
        for (int cities_e = cities_s; cities_e < num_cities; cities_e++)
            distance_table[cities_s][cities_e] = pow((pow((place_table[cities_s][0] - place_table[cities_e][0]), 2) + pow((place_table[cities_s][1] - place_table[cities_e][1]), 2)), 0.5);

    for (int cities_s = 0; cities_s < num_cities; cities_s++)
        for (int cities_e = 0; cities_e < cities_s; cities_e++)
            distance_table[cities_s][cities_e] = distance_table[cities_e][cities_s];
}

void GaTsp::countChromoFit()
{
    double distance;
    for (int chromos = 0; chromos < num_chromos; chromos++)
    {
        distance = 0.0;
        for (int cities = 0; cities - 1 < num_cities; cities++)
        {
            distance += distance_table[chromo_set[chromos][cities]][chromo_set[chromos][cities + 1]];
        }
        chromo_fit_set[chromos] = distance;
    }
}

void GaTsp::run()
{
}
