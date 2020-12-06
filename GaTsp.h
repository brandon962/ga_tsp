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

    double crossover_rate;
    double mutation_rate;
    int crossover_choice;

    i2d place_table;
    d2d distance_table;
    i2d chromo_set;
    d1d chromo_fit_set;

    // CX
    i1d cx_mark;

    //  random
    std::default_random_engine gen;
    std::uniform_int_distribution<int> rand_max;
    std::uniform_int_distribution<int> rand_cities;
    std::uniform_real_distribution<double> rand_01;

public:
    GaTsp(unsigned seed, int _num_runs, int _num_iters, int _num_chroms, int _num_cities, string _input_filename, double _crossover_rate, double _mutation_rate, int _cossover_choice);
    ~GaTsp();
    void assignVector();
    void initChromo();
    void printAllChromo();
    void readDistance(string _input_filename);
    void countChromoFit();
    void run();
    int find(i1d, int);
    void crossoverPMX();
    void crossoverCX();
};

GaTsp::GaTsp(unsigned seed, int _num_runs, int _num_iters, int _num_chroms, int _num_cities, string _input_filename, double _crossover_rate, double _mutation_rate, int _cossover_choice)
{
    num_runs = _num_runs;
    num_iters = _num_iters;
    num_chromos = _num_chroms;
    num_cities = _num_cities;
    crossover_rate = _crossover_rate;
    mutation_rate = _mutation_rate;
    crossover_choice = _cossover_choice;

    gen = default_random_engine(seed);
    rand_max = uniform_int_distribution<int>(0, INT_MAX);
    rand_cities = uniform_int_distribution<int>(0, num_cities);
    rand_01 = uniform_real_distribution<double>(0, 1);

    assignVector();
    readDistance(_input_filename);
    run();

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

void GaTsp::assignVector()
{
    chromo_set.assign(num_chromos, i1d(num_cities, 0));
    chromo_fit_set.assign(num_chromos, 0);
    distance_table.assign(num_cities, d1d(num_cities, 0));
    place_table.assign(num_cities, i1d(2, 0));
    if (crossover_choice == 2)
    {
        cx_mark.assign(num_cities, 0);
    }
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
        for (int cities = 0; cities < num_cities - 1; cities++)
        {
            distance += distance_table[chromo_set[chromos][cities]][chromo_set[chromos][cities + 1]];
        }
        chromo_fit_set[chromos] = distance;
    }
}

void GaTsp::run()
{
    for (int runs = 0; runs < num_runs; runs++)
    {
        initChromo();
        countChromoFit();
        printAllChromo();
        for (int iters = 0; iters < num_iters; iters++)
        {
            // crossoverPMX();
            // printAllChromo();
            crossoverCX();
            // printAllChromo();
            cout << "-----------------------------------------------"
                 << endl
                 << "-----------------------------------------------"
                 << endl;
        }
    }
}

int GaTsp::find(i1d chromo, int target)
{
    for (int cities = 0; cities < num_cities; cities++)
    {
        if (chromo[cities] == target)
            return cities;
    }
    return -1;
}

void GaTsp::crossoverPMX()
{
    for (int chromos = 0; chromos < num_chromos; chromos += 2)
    {
        int point_start = 0, point_end = 0;
        while (point_start == point_end)
        {
            point_start = rand_cities(gen);
            point_end = rand_cities(gen);
        }
        point_start > point_end ? swap(point_start, point_end) : swap(point_start, point_start);
    }
}

void GaTsp::crossoverCX()
{
    int point_start;
    int point_next;
    for (int chromos = 0; chromos < num_chromos; chromos += 2)
    {
        if (rand_01(gen) > crossover_rate)
            continue;
        for (int cities = 0; cities < num_cities; cities++)
            cx_mark[cities] = 0;
        point_start = rand_cities(gen);
        cx_mark[point_start] = 1;
        cout << point_start << endl;

        point_next = find(chromo_set[chromos], chromo_set[chromos + 1][point_start]);
        while (point_next != point_start)
        {
            cx_mark[point_next] = 1;
            point_next = find(chromo_set[chromos], chromo_set[chromos + 1][point_next]);
        }

        for (int cities = 0; cities < num_cities; cities++)
        {
            if (cx_mark[cities])
            {
            }
            else
                swap(chromo_set[chromos][cities], chromo_set[chromos + 1][cities]);
        }
    }
}
