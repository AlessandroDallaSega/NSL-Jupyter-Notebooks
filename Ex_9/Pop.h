/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#ifndef __Pop__
#define __Pop__
#include <iostream>
#include <fstream>
#include <vector>
#include "Trip.h"
using namespace std;

class population {
    private:
    vector<trip> _trips;
    Random _rnd;
    int _num_ci;
    //vector<trip> _trips; //contiene tutti i trip della popolazione
    int _num_chrom; //numero di cromosomi (trips) nella population
    float _p_c; //probabilità di crossover
    float _p_m_1; //probabilità di mutazione
    float _p_m_2;
    float _p_m_3;
    float _p_m_4;
    void _mutate_1(trip&); 
    //void _mutate_2(trip);
    void _mutate_3(trip&);
    void _mutate_4(trip&);
    void _crossover(trip&, trip&); //con una certa probabilità fa crossover tra due trips (vedere indicazioni)
    bool _is_same(trip, trip); //dice se due trip sono uguali
    trip _select(); //restituisce il trip migliore (con una certa probabilità) in base alla fitness
    //void _sel_sort(); //ordina i trips in base alla probabilità

    public:
    population(Random&, int, trip, float, float, float, float, float);
    ~population();
    //void initialize(int, trip); //inizializza una popolazione a partire da un trip e il numero di trips che voglio
    int get_num_chrom(); 
    void step(); //performa uno step (vedere le indicaizoni)
    void print();
    trip get_best_trip(); //restituisce il trip più corto
    double half_AV();
};


#endif // __Pop__



/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/