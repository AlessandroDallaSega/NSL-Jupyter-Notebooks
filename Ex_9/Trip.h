/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#ifndef __Trip__
#define __Trip__
#include <iostream>
#include <fstream>
#include <vector>
#include "city.h"
using namespace std;

class trip {
    private:
    vector<city> _cities; //vettore contenente le città che stanno nel trip
    int _num_ci; //numero di città nel trip

    public:
    trip();
    trip(vector<city>);
    ~trip();
    city operator[](int); //operatore che ritorna l'i-esima città del trip
    int get_num_ci();
    bool check(); //controlla che il trip rispetti le condizioni
    double get_lenght(); //restituisce la lunghezza del trip (metrica L^2)
    void set_cities(vector<city>);
    void print();
    void set_city(int, city);
};

#endif // __Trip__



/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/