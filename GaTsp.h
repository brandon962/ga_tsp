// GA class
#define _CRT_SECURE_NO_WARNINGS
#include "myVector.h"
#include <climits>
#include <ctime>
#include <iostream>
#include <random>
#include <string>

class GaTsp
{
private:
    /* data */
    time_t start, end;
    int num_runs;
    int num_iters;
    int num_cities;
    int num_chromos;

    double crossover_rate;
    double mutation_rate;
    double mutation_decrease;
    int crossover_choice;
    int num_players;

    d2d run_result;
    d1d run_avg;

    i2d place_table;
    d2d distance_table;
    i2d chromo_set;
    d1d chromo_fit_set;

    // CX
    i1d cx_mark;

    // select
    i2d select_chromos;
    int select_fit;

    //  random
    std::default_random_engine gen;
    std::uniform_int_distribution<int> rand_max;
    std::uniform_int_distribution<int> rand_cities;
    std::uniform_int_distribution<int> rand_allchromos;
    std::uniform_real_distribution<double> rand_01;

public:
    GaTsp(unsigned, int, int, int, int, string, double, double, double, int, int);
    ~GaTsp();
    void assignVector();
    void initChromo();
    void printAllChromos();
    void readDistance(string);
    void countChromoFit();
    void run();
    double findBest();
    int find(i1d, int);
    int find(i1d, int, int, int);
    void crossoverPMX();
    void crossoverCX();
    void crossoverOX();
    void select();
    void wirteFile();
    void printMessage();
};

GaTsp::GaTsp(unsigned seed, int _num_runs, int _num_iters, int _num_chroms, int _num_cities, string _input_filename, double _crossover_rate, double _mutation_rate, double _mutation_decrease, int _cossover_choice, int _players)
{
    start = time(NULL);
    num_runs = _num_runs;
    num_iters = _num_iters;
    num_chromos = _num_chroms;
    num_cities = _num_cities;
    crossover_rate = _crossover_rate;
    mutation_rate = _mutation_rate;
    mutation_decrease = _mutation_decrease;
    crossover_choice = _cossover_choice;
    num_players = _players;

    gen = default_random_engine(seed);
    rand_max = uniform_int_distribution<int>(0, INT_MAX);
    rand_cities = uniform_int_distribution<int>(0, num_cities - 1);
    rand_01 = uniform_real_distribution<double>(0, 1);
    rand_allchromos = uniform_int_distribution<int>(0, (num_chromos * 2) - 1);

    printMessage();
    assignVector();
    readDistance(_input_filename);
    run();
    end = time(NULL);
    cout << "cost time : " << end - start << endl;
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
    run_result.clear();
    run_result.shrink_to_fit();
    run_avg.clear();
    run_avg.shrink_to_fit();
    cx_mark.clear();
    cx_mark.shrink_to_fit();
    select_chromos.clear();
    select_chromos.shrink_to_fit();
}

void GaTsp::assignVector()
{
    chromo_set.assign(num_chromos * 2, i1d(num_cities, 0));
    chromo_fit_set.assign(num_chromos * 2, 0);
    distance_table.assign(num_cities, d1d(num_cities, 0));
    place_table.assign(num_cities, i1d(2, 0));
    run_result.assign(num_runs, d1d(num_iters, 0.0));
    run_avg.assign(num_iters, 0.0);
    if (crossover_choice == 2)
    {
        cx_mark.assign(num_cities, 0);
    }
    select_chromos.assign(num_chromos, i1d(num_cities, 0));
}

void GaTsp::initChromo()
{
    int itemp;
    vector<int>::iterator cities_iterator;
    for (int chromos = 0; chromos < num_chromos * 2; chromos++)
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

void GaTsp::printAllChromos()
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
    for (int chromos = 0; chromos < num_chromos * 2; chromos++)
    {
        distance = 0.0;
        for (int cities = 0; cities < num_cities - 1; cities++)
        {
            distance += distance_table[chromo_set[chromos][cities]][chromo_set[chromos][cities + 1]];
        }
        distance += distance_table[chromo_set[chromos][num_cities - 1]][chromo_set[chromos][0]];
        chromo_fit_set[chromos] = distance;
    }
}

void GaTsp::run()
{
    for (int runs = 0; runs < num_runs; runs++)
    {
        double best = __DBL_MAX__;
        initChromo();
        countChromoFit();
        for (int iters = 0; iters < num_iters; iters++)
        {
            switch (crossover_choice)
            {
            case 0:
                crossoverPMX();
                break;
            case 1:
                crossoverOX();
                break;
            case 2:
                crossoverCX();
                break;
            }
            countChromoFit();
            select();
            countChromoFit();
            if (best > findBest())
                best = findBest();
            run_result[runs][iters] = best;
        }
    }

    for (int iters = 0; iters < num_iters; iters++)
    {
        for (int runs = 0; runs < num_runs; runs++)
        {
            run_avg[iters] += run_result[runs][iters];
        }
        run_avg[iters] /= num_runs;
    }
    cout << "avg : " << run_avg[num_iters - 1] << endl;
    wirteFile();
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

int GaTsp::find(i1d chromo, int start, int end, int target)
{
    for (int cities = start; cities <= end; cities++)
    {
        if (chromo[cities] == target)
            return cities;
    }
    return -1;
}

double GaTsp::findBest()
{
    double best = __DBL_MAX__;
    for (int chromos = 0; chromos < num_chromos; chromos++)
    {
        if (chromo_fit_set[chromos] < best)
            best = chromo_fit_set[chromos];
    }
    return best;
}

void GaTsp::crossoverPMX()
{
    for (int chromos = 0; chromos < num_chromos; chromos += 2)
    {
        int point_start = 0, point_end = 0;
        int itemp;
        while (point_start == point_end)
        {
            point_start = rand_cities(gen);
            point_end = rand_cities(gen);
        }
        point_start > point_end ? swap(point_start, point_end) : swap(point_start, point_start);

        for (int cities = 0; cities < num_cities; cities++)
        {
            chromo_set[chromos + num_chromos][cities] = -1;
            chromo_set[chromos + 1 + num_chromos][cities] = -1;
        }

        for (int cities = point_start; cities <= point_end; cities++)
        {
            chromo_set[chromos + num_chromos][cities] = chromo_set[chromos + 1][cities];
            chromo_set[chromos + num_chromos + 1][cities] = chromo_set[chromos][cities];
        }

        for (int cities = 0; cities < num_cities; cities++)
        {
            if (cities >= point_start && cities <= point_end)
                continue;
            if (find(chromo_set[chromos + 1], point_start, point_end, chromo_set[chromos][cities]) == -1)
                chromo_set[chromos + num_chromos][cities] = chromo_set[chromos][cities];
            if (find(chromo_set[chromos], point_start, point_end, chromo_set[chromos + 1][cities]) == -1)
                chromo_set[chromos + 1 + num_chromos][cities] = chromo_set[chromos + 1][cities];
        }

        for (int cities = 0; cities < num_cities; cities++)
        {
            if (chromo_set[chromos + num_chromos][cities] == -1)
            {
                itemp = find(chromo_set[chromos + 1], point_start, point_end, chromo_set[chromos][cities]);
                while (itemp != -1)
                {
                    chromo_set[chromos + num_chromos][cities] = chromo_set[chromos][itemp];
                    itemp = find(chromo_set[chromos + 1], point_start, point_end, chromo_set[chromos + num_chromos][cities]);
                }
            }
            if (chromo_set[chromos + 1 + num_chromos][cities] == -1)
            {
                itemp = find(chromo_set[chromos], point_start, point_end, chromo_set[chromos + 1][cities]);
                while (itemp != -1)
                {
                    chromo_set[chromos + 1 + num_chromos][cities] = chromo_set[chromos + 1][itemp];
                    itemp = find(chromo_set[chromos], point_start, point_end, chromo_set[chromos + 1 + num_chromos][cities]);
                }
            }
        }

        double m1 = mutation_rate;
        while (rand_01(gen) < m1)
        {
            m1 *= mutation_decrease;
            swap(chromo_set[chromos + num_chromos][rand_cities(gen)], chromo_set[chromos + num_chromos][rand_cities(gen)]);
        }
        m1 = mutation_rate;
        while (rand_01(gen) < m1)
        {
            m1 *= mutation_decrease;
            swap(chromo_set[chromos + num_chromos + 1][rand_cities(gen)], chromo_set[chromos + num_chromos + 1][rand_cities(gen)]);
        }
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
        // cout << point_start << endl;

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
                chromo_set[chromos + num_chromos][cities] = chromo_set[chromos][cities];
                chromo_set[chromos + num_chromos + 1][cities] = chromo_set[chromos + 1][cities];
            }
            else
            {
                chromo_set[chromos + num_chromos + 1][cities] = chromo_set[chromos][cities];
                chromo_set[chromos + num_chromos][cities] = chromo_set[chromos + 1][cities];
            }
        }
        double m1 = mutation_rate;
        while (rand_01(gen) < m1)
        {
            m1 *= mutation_decrease;
            swap(chromo_set[chromos + num_chromos][rand_cities(gen)], chromo_set[chromos + num_chromos][rand_cities(gen)]);
        }
        m1 = mutation_rate;
        while (rand_01(gen) < m1)
        {
            m1 *= mutation_decrease;
            swap(chromo_set[chromos + num_chromos + 1][rand_cities(gen)], chromo_set[chromos + num_chromos + 1][rand_cities(gen)]);
        }
    }
}

void GaTsp::crossoverOX()
{
    for (int chromos = 0; chromos < num_chromos; chromos += 2)
    {
        int point_start = 0, point_end = 0;
        int point_next0, point_next1;
        bool flag = true;
        while (point_start == point_end)
        {
            point_start = rand_cities(gen);
            point_end = rand_cities(gen);
        }
        point_start > point_end ? swap(point_start, point_end) : swap(point_start, point_start);

        // cout << endl
        //      << point_start << " " << point_end << endl;

        for (int cities = 0; cities < num_cities; cities++)
        {
            chromo_set[chromos + num_chromos][cities] = -1;
            chromo_set[chromos + 1 + num_chromos][cities] = -1;
        }

        for (int cities = point_start; cities <= point_end; cities++)
        {
            chromo_set[chromos + num_chromos][cities] = chromo_set[chromos + 1][cities];
            chromo_set[chromos + num_chromos + 1][cities] = chromo_set[chromos][cities];
        }

        for (int cities = 0; cities < num_cities; cities++)
        {
            flag = true;
            for (int i = point_start; i <= point_end; i++)
                if (chromo_set[chromos][cities] == chromo_set[chromos + 1][i])
                    flag = false;
            if (flag)
            {
                point_next0 = cities;
                break;
            }
        }
        for (int cities = 0; cities < num_cities; cities++)
        {
            flag = true;
            for (int i = point_start; i <= point_end; i++)
                if (chromo_set[chromos + 1][cities] == chromo_set[chromos][i])
                    flag = false;
            if (flag)
            {
                point_next1 = cities;
                break;
            }
        }

        for (int cities = 0; cities < num_cities; cities++)
        {
            if (cities >= point_start && cities <= point_end)
                continue;
            chromo_set[chromos + num_chromos][cities] = chromo_set[chromos][point_next0];
            chromo_set[chromos + 1 + num_chromos][cities] = chromo_set[chromos + 1][point_next1];
            for (int cities = point_next0 + 1; cities < num_cities; cities++)
            {
                flag = true;
                for (int i = point_start; i <= point_end; i++)
                    if (chromo_set[chromos][cities] == chromo_set[chromos + 1][i])
                        flag = false;
                if (flag)
                {
                    point_next0 = cities;
                    break;
                }
            }
            for (int cities = point_next1 + 1; cities < num_cities; cities++)
            {
                flag = true;
                for (int i = point_start; i <= point_end; i++)
                    if (chromo_set[chromos + 1][cities] == chromo_set[chromos][i])
                        flag = false;
                if (flag)
                {
                    point_next1 = cities;
                    break;
                }
            }
        }

        // for (int citits = 0; citits < num_cities; citits++)
        //     cout << chromo_set[chromos + num_chromos][citits] << " ";
        // cout << endl;

        // for (int citits = 0; citits < num_cities; citits++)
        //     cout << chromo_set[chromos + 1 + num_chromos][citits] << " ";
        // cout << endl;

        double m1 = mutation_rate;
        while (rand_01(gen) < m1)
        {
            m1 *= mutation_decrease;
            swap(chromo_set[chromos + num_chromos][rand_cities(gen)], chromo_set[chromos + num_chromos][rand_cities(gen)]);
        }
        m1 = mutation_rate;
        while (rand_01(gen) < m1)
        {
            m1 *= mutation_decrease;
            swap(chromo_set[chromos + num_chromos + 1][rand_cities(gen)], chromo_set[chromos + num_chromos + 1][rand_cities(gen)]);
        }
    }
}

void GaTsp::select()
{
    int selected;
    int itemp;
    for (int chromos = 0; chromos < num_chromos; chromos++)
    {
        selected = rand_allchromos(gen);
        select_fit = chromo_fit_set[selected];
        for (int players = 1; players < num_players; players++)
        {
            itemp = rand_allchromos(gen);
            if (select_fit > chromo_fit_set[itemp])
            {
                select_fit = chromo_fit_set[selected];
                selected = itemp;
            }
        }
        select_chromos[chromos] = chromo_set[selected];
    }

    for (int chromos = 0; chromos < num_chromos; chromos++)
        chromo_set[chromos] = select_chromos[chromos];
}

void GaTsp::wirteFile()
{
    FILE *fp;
    string filename = "data\\output";
    switch (crossover_choice)
    {
    case 0:
        filename += "PMX";
        break;
    case 1:
        filename += "OX";
        break;
    case 2:
        filename += "CX";
        break;
    }
    filename += ".txt";
    fp = fopen(filename.c_str(), "w");
    char *ctemp;
    ctemp = (char *)calloc(10, sizeof(char));
    for (int iters = 0; iters < num_iters; iters++)
    {
        // sprintf(ctemp, "%f", run_avg[iters]);
        // cout << ctemp << endl;
        fprintf(fp, "%f\n", run_avg[iters]);
    }
    fclose(fp);
}

void GaTsp::printMessage()
{
    cout << "runs          \t" << num_runs << endl
         << "iters         \t" << num_iters << endl
         << "pop size      \t" << num_chromos << endl
         << "cities        \t" << num_cities << endl
         << "crossover rate\t" << crossover_rate << endl
         << "mutation rate \t" << mutation_rate << endl;
    switch (crossover_choice)
    {
    case 0:
        cout << "crossover type\tPMX" << endl;
        break;
    case 1:
        cout << "crossover type\tOX" << endl;
        break;
    case 2:
        cout << "crossover type\tCX" << endl;
        break;
    }
}