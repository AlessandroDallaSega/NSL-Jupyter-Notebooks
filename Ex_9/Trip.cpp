/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <utility>
#include <sstream>
#include <vector>
#include "random.h"
#include "Trip.h"

using namespace std;

trip :: trip(){
    
}

trip :: ~trip(){}

trip :: trip(vector<city> c){
    _cities = c;
    _num_ci = c.size();
}

city trip :: operator[](int i){
    return _cities[i];
}

int trip :: get_num_ci(){
    return _num_ci;
}

bool trip :: check(){
    bool flag;

    //controllo che la prima città sia la partenza
    if(_cities[0].index != 0){
        cerr << "Il trip parte dalla città sbagliata" << endl;
        return false;
    }

    //controllo che passi da tutte le città
    for(int i=0; i<_num_ci; i++){
        flag = false;
        for(int j=0; j<_num_ci; j++){
            if(_cities[j].index == i) flag = true;
        }
        if(!flag){
            cerr << "Il trip non visita tutte le città" << endl;
            return false;
        }
    }
    
    return false;
}

double trip :: get_lenght(){
    double L = 0.0;
    for(int i=0; i<_num_ci-1; i++){
        L += sqrt(pow(_cities[i].x - _cities[i+1].x, 2) + pow(_cities[i].y - _cities[i+1].y, 2));
    }
    return L + sqrt(pow(_cities[_num_ci-1].x - _cities[0].x, 2) + pow(_cities[_num_ci-1].y - _cities[0].y, 2));
}

void trip :: set_cities(vector<city> c){
    _cities = c;
}

void trip :: print(){
    for(int i=0; i<_num_ci; i++){
        cout << _cities[i].index << "  ";
    }
    cout << endl;
}

void trip :: set_city(int n, city c){
    _cities[n] = c;
}



  

/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/