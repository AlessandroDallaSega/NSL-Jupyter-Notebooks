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
#include <vector>
#include "random.h"
#include "Pop.h"

using namespace std;
 
int main (int argc, char *argv[]){

   Random rnd;
   int seed[4];
   int p1, p2;
   ifstream Primes("Primes");
   if (Primes.is_open()){
      Primes >> p1 >> p2 ;
   } else cerr << "PROBLEM: Unable to open Primes" << endl;
   Primes.close();

   ifstream input("seed.in");
   string property;
   if (input.is_open()){
      while ( !input.eof() ){
         input >> property;
         if( property == "RANDOMSEED" ){
            input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
            rnd.SetRandom(seed,p1,p2);
         }
      }
      input.close();
   } else cerr << "PROBLEM: Unable to open seed.in" << endl;

   //ESERCITAZIONE 9
   int N_cities = 34;
   int steps = 1000;
   int N = 200; //numero di cromosomi
   
   vector<city> initial_cities;
   double angle;
   for(int i=0; i<N_cities; i++){
      angle = rnd.Rannyu(0, 2*M_PI);
      city c(i, cos(angle), sin(angle));
      initial_cities.push_back(c);
   }
   trip initial_trip(initial_cities);
   population pop(rnd, N, initial_trip, 0.7, 0.05, 0.05, 0.05, 0.05);
   ofstream out;
   out.open("averages_C.out");
   for(int i=0; i<steps; i++){
      pop.step();
      out << i+1 << "\t" << pop.get_best_trip().get_lenght() << "\t" << pop.half_AV() << endl;
   }
   out.close();
   ofstream out_positions;
   out_positions.open("coordinates_C.out");
   for(int i=0; i<N_cities; i++){
      out_positions << pop.get_best_trip()[i].index << "\t" << pop.get_best_trip()[i].x << "\t" << pop.get_best_trip()[i].y << endl;
   }
   out_positions << pop.get_best_trip()[0].index << "\t" << pop.get_best_trip()[0].x << "\t" << pop.get_best_trip()[0].y << endl; 
   out_positions.close();

   vector<city> initial_cities_Q;
   double x,y;
   for(int i=0; i<N_cities; i++){
      x = rnd.Rannyu(0, 1);
      y = rnd.Rannyu(0, 1);
      city c(i, x, y);
      initial_cities_Q.push_back(c);
   }
   trip initial_trip_Q(initial_cities_Q);
   population pop_Q(rnd, N, initial_trip_Q, 0.7, 0.05, 0.05, 0.05, 0.05);
   ofstream out_Q;
   out_Q.open("averages_Q.out");
   for(int i=0; i<steps; i++){
      pop_Q.step();
      out_Q << i+1 << "\t" << pop_Q.get_best_trip().get_lenght() << "\t" << pop_Q.half_AV() << endl;
   }
   out_Q.close();
   ofstream out_positions_Q;
   out_positions_Q.open("coordinates_Q.out");
   for(int i=0; i<N_cities; i++){
      out_positions_Q << pop_Q.get_best_trip()[i].index << "\t" << pop_Q.get_best_trip()[i].x << "\t" << pop_Q.get_best_trip()[i].y << endl;
   }
   out_positions_Q << pop_Q.get_best_trip()[0].index << "\t" << pop_Q.get_best_trip()[0].x << "\t" << pop_Q.get_best_trip()[0].y << endl; 
   out_positions_Q.close();

   rnd.SaveSeed();
   return 0;
}

/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************/