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
#include <random>
#include <algorithm>
#include "random.h"
#include "Pop.h"

using namespace std;

population :: ~population(){}

population :: population(Random& rnd, int n, trip t, float pc, float pm1, float pm2, float pm3, float pm4){
    _rnd = rnd; 
    _num_chrom = n;
    _p_c = pc;
    _p_m_1 = pm1;
    _p_m_2 = pm2;
    _p_m_3 = pm3;
    _p_m_4 = pm4;
    _num_ci = t.get_num_ci();

    random_device rd;
    trip tr(t);
    vector<int> indexes;
    vector<city> temp_trip;
    for(int i=1; i<t.get_num_ci(); i++){
        indexes.push_back(i);
    }
    _trips.push_back(t);
    for(int i=1; i<_num_chrom; i++){
        temp_trip.push_back(t[0]);
        mt19937 g(rd());
        shuffle(indexes.begin(), indexes.end(), g);
        for(int j=0; j < t.get_num_ci()-1; j++){
            temp_trip.push_back(t[indexes[j]]);
        }
        tr.set_cities(temp_trip);
        tr.check();
        _trips.push_back(tr);
        temp_trip.clear();
    }
}

int population :: get_num_chrom(){
    return _num_chrom;
}

void population :: print(){
    for(int i=0; i<_num_chrom; i++){
        for(int j=0; j<_num_ci; j++){
            cout << _trips[i][j].index << " ";
        }
        cout << endl;
    }
}

trip& population :: get_best_trip(){
    int index = 0;
    double L = _trips[0].get_lenght();
    for(int i=1; i<_num_chrom; i++){
        if (_trips[i].get_lenght() < L){
            L = _trips[i].get_lenght();
            index = i;
        }
    }
    return _trips[index];
}

bool population :: _is_same(trip t1, trip t2){
    for(int i=0; i<_num_ci; i++){
        if(t1[i].index != t2[i].index) return false;
    }
    return true;
}

trip population :: _select(){
    float P[_num_chrom];
    double beta = 0.08;
    double total_lenght = 0;
    double L;
    for(int i=0; i<_num_chrom; i++){
        L = _trips[i].get_lenght();
        P[i] = exp(-beta*L);
        total_lenght += P[i];
    }
    for(int i=0; i<_num_chrom; i++){
        P[i] /= total_lenght;
    }
    double s = P[0];
    int i = 0;
    double prob = _rnd.Rannyu();
    while(s < prob){
        i++;
        s += P[i];
    }
    return _trips[i];
}

//funziona per un numero pari di cromosomi
void population :: step(){
    vector<trip> offspring;
    for(int i=0; i<_num_chrom/2; i++){
        offspring.push_back(this->_select());
        offspring.push_back(this->_select());
        while(this->_is_same(offspring[2*i], offspring[2*i+1])){
            offspring.pop_back();
            offspring.push_back(this->_select());
        }
        if(_rnd.Rannyu() < _p_c) this->_crossover(offspring[2*i], offspring[2*i+1]);
        if(_rnd.Rannyu() < _p_m_1) this->_mutate_1(offspring[2*i]);
        if(_rnd.Rannyu() < _p_m_1) this->_mutate_1(offspring[2*i+1]);
        //if(_rnd.Rannyu() < _p_m_2) this->_mutate_2(offspring[2*i]);
        //if(_rnd.Rannyu() < _p_m_2) this->_mutate_2(offspring[2*i+1]);
        if(_rnd.Rannyu() < _p_m_3) this->_mutate_3(offspring[2*i]);
        if(_rnd.Rannyu() < _p_m_3) this->_mutate_3(offspring[2*i+1]);
        if(_rnd.Rannyu() < _p_m_4) this->_mutate_4(offspring[2*i]);
        if(_rnd.Rannyu() < _p_m_4) this->_mutate_4(offspring[2*i+1]);
    }
    _trips = offspring;
}


void population :: _mutate_1(trip& t){
    int index_1, index_2;
    do {
        index_1 = floor(_rnd.Rannyu()*(t.get_num_ci()-1)) + 1;
        index_2 = floor(_rnd.Rannyu()*(t.get_num_ci()-1)) + 1;
    } while (index_1 == 0 || index_2 == 0 || index_1 == index_2);
    city c = t[index_1];
    t.set_city(index_1, t[index_2]);
    t.set_city(index_2, c);
    t.check();
}

void population :: _mutate_3(trip& t){
    int L_max, L;
    L_max = floor((t.get_num_ci()-1)/2);
    L = floor(_rnd.Rannyu()*(L_max-1)) + 1; 
    int a_start = floor(_rnd.Rannyu()*(t.get_num_ci()-1)); 
    int a_end = floor(_rnd.Rannyu(a_start + L, a_start + t.get_num_ci() - L));
    city c;
    for(int i=0; i<L; i++){
        c = t[(a_start + i)%(t.get_num_ci()-1) + 1];
        t.set_city((a_start + i)%(t.get_num_ci()-1) + 1, t[(a_end + i)%(t.get_num_ci()-1) + 1]);
        t.set_city((a_end + i)%(t.get_num_ci()-1) + 1, c);
    }
    t.check();
}

void population :: _mutate_4(trip& t){
    int a_start = floor(_rnd.Rannyu()*(t.get_num_ci()-1));
    int L = floor(_rnd.Rannyu(1, t.get_num_ci()-1));
    city c;
    for(int i=0; i<floor(L/2); i++){
        c = t[(a_start + i)%(t.get_num_ci()-1) + 1];
        t.set_city((a_start + i)%(t.get_num_ci()-1) + 1, t[(a_start + L - 1 - i)%(t.get_num_ci()-1) + 1]);
        t.set_city((a_start + L - 1 - i)%(t.get_num_ci()-1) + 1, c);
    }
    t.check();
}

void population :: _crossover(trip& t1, trip& t2){
    int num_ci = t1.get_num_ci();
    vector<city> copy_t1;
    for(int i=0; i<num_ci; i++){
        copy_t1.push_back(t1[i]);
    }
    int a = floor(_rnd.Rannyu(1, num_ci));
    vector<int> index_t1;
    for(int i=a; i<num_ci; i++){
        for(int j=1; j<num_ci; j++){
            if(t2[j].index == t1[i].index) index_t1.push_back(j);
        }
    }
    sort(index_t1.begin(), index_t1.end());
    for(int i=a; i<num_ci; i++){
        t1.set_city(i, t2[index_t1[i - a]]);
    }
    vector<int> index_t2;
    for(int i=a; i<num_ci; i++){
        for(int j=1; j<num_ci; j++){
            if(copy_t1[j].index == t2[i].index) index_t2.push_back(j);
        }
    }
    sort(index_t2.begin(), index_t2.end());
    for(int i=a; i<num_ci; i++){
        t2.set_city(i, copy_t1[index_t2[i - a]]);
    }
    t1.check();
    t2.check();
}

double population :: half_AV(){
    vector<double> lenghts_half;
    for(int i=0; i<_num_chrom; i++){
        lenghts_half.push_back(_trips[i].get_lenght());
        sort(lenghts_half.begin(), lenghts_half.end());
    }
    double sum = 0;
    for(int i=0; i<_num_chrom/2; i++){
        sum += lenghts_half[i];
    }
    return sum / (_num_chrom/2);
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