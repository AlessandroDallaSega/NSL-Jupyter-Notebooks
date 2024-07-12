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
#include <mpi.h>
#include "random.h"
#include "Pop.h"

using namespace std;

struct PathLengthWithRank {
    double length;
    int rank;
};
 
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

   //ESERCITAZIONE 10
   MPI_Init(&argc, &argv);
   int size, rank;
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   int N_provinces = 110;
   int steps = 10000;
   int N = 200; //numero di cromosomi
   int N_migr = 21;
   
   vector<city> initial_cities;
   ifstream in;
   double x, y;
   int index;
   in.open("cap_prov_ita.dat");
   for(int i=0; i<N_provinces; i++){
      in >> x >> y;
      index = i;
      city c(index, x, y);
      initial_cities.push_back(c);
   }
   in.close();
   trip initial_trip(initial_cities);                                   //siccome quando inizializzo pop uso il metodo shuffle per generare i trip e shuffle
   population pop(rnd, N, initial_trip, 0.8, 0.1, 0.1, 0.1, 0.1);       //genera ogni volta un trip diverso non devo avere seed diversi per i diversi process
   ifstream in_c;
   bool communicate;
   string str;
   in_c.open("communicate.in");
   in_c >> str;
   if(str == "TRUE") communicate = true;
   if(str == "FALSE") communicate = false;
   in_c.close();
   for(int i=0; i<steps; i++){
      pop.step();
      if((i+1)%N_migr == 0 && communicate){
         vector<int> best_trip_int_send = pop.get_best_trip().serialize();
         vector<int> best_trip_int_receive(best_trip_int_send.size());
         int send_to = (rank + 1) % size;
         int receive_from = (rank - 1 + size) % size;
         int send_size = (int)best_trip_int_send.size();
         int receive_size = (int)best_trip_int_receive.size();

         MPI_Sendrecv(best_trip_int_send.data(), send_size, MPI_INT, send_to, 0, 
                      best_trip_int_receive.data(), receive_size, MPI_INT, receive_from, 0,
                      MPI_COMM_WORLD, MPI_STATUS_IGNORE);

         pop.get_best_trip().deserialize(best_trip_int_receive);
      }
      if(rank==0) cout << (double)i/steps << endl;
   }
   PathLengthWithRank my_path_length_with_rank;
   my_path_length_with_rank.length = pop.get_best_trip().get_lenght();
   my_path_length_with_rank.rank = rank;
   PathLengthWithRank best_path_length_with_rank;
   MPI_Reduce(&my_path_length_with_rank, &best_path_length_with_rank, 1, MPI_DOUBLE_INT, MPI_MINLOC, 0, MPI_COMM_WORLD);
   //if (rank == 0) {
      //cout << "Best length reached: " << best_path_length_with_rank.length << " at rank: " << best_path_length_with_rank.rank << endl;
      //cout << "Steps taken: " << steps << endl;
   //}
   if (rank == best_path_length_with_rank.rank) {
      ofstream out_positions;
      //string communicate_str = communicate ? "TRUE" : "FALSE";
      //string filename = "coordinates_" + to_string(size) + "_" + communicate_str + ".out";
      string filename = "coordinates_finalzz.out";
      out_positions.open(filename);
      if (out_positions.is_open()) {
         for (int i = 0; i < N_provinces; i++) {
               out_positions << pop.get_best_trip()[i].index << "\t" << pop.get_best_trip()[i].x << "\t" << pop.get_best_trip()[i].y << endl;
         }
         out_positions << pop.get_best_trip()[0].index << "\t" << pop.get_best_trip()[0].x << "\t" << pop.get_best_trip()[0].y << endl; 
         out_positions << best_path_length_with_rank.length << "    000   000" << endl;
         out_positions.close();
      } else {
         cerr << "PROBLEM: Unable to open " << filename << " for writing" << endl;
      }
   }

   MPI_Finalize();
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