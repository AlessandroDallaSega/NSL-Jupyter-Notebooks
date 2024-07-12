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
#include "random.h"

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

   //punto 2.1 distribuzione uniforme
   int throws = 10000;
   int N = 100;
   int L = throws/N;
   double avintegral[N];
   double sumav = 0;
   for(int j=0; j<N; j++){
        sumav = 0;
        for (int i=0; i<L; i++){
            sumav = sumav + (M_PI/2)*cos((M_PI*rnd.Rannyu())/2);
        }
        avintegral[j] = sumav/L;
   }
   ofstream Write;
   Write.open("punto21unif.out");
   rnd.BlockAV(avintegral, N, Write);
   Write.close();

   //punto 2.1 importance sampling
   double t = 0;
   double avintegral1[N];
   for(int j=0; j<N; j++){
        sumav = 0;
        for (int i=0; i<L; i++){
            t = rnd.Ranretta();
            sumav = sumav + (M_PI/2)*cos((M_PI*t)/2)/(2-2*t);
        }
    avintegral1[j] = sumav/L;
   }

   ofstream Write1;
   Write1.open("punto21impsampling.out");
   rnd.BlockAV(avintegral1, N, Write1);
   Write1.close();

   //punto 2.2
   int A = 10000;
   int B = 100;
   int C = int(A/B);

   double RW[B][100]={0}; //prima [] indica il blocco, seconda [] indica il passo
   double RW_con[B][100]={0}; //stessa cosa ma per il continuo

   int x, y, z;
   double x_con, y_con, z_con;
   double prob, a1, b1, a2, b2;

   for(int j=0; j<B; j++){
      for (int i=0; i<C; i++){
         x=0;
         y=0;
         z=0;
         x_con=0;
         y_con=0;
         z_con=0;
         for (int k=0; k<100; k++){
            prob = rnd.Rannyu();
            do {   
               a1 = rnd.Rannyu(-1, 1);
               b1 = rnd.Rannyu(0, 1);
            } while (sqrt(pow(a1, 2) + pow(b1, 2)) > 1);
            do {
               a2 = rnd.Rannyu(-1, 1);
               b2 = rnd.Rannyu(-1, 1);  
            } while (sqrt(pow(a2, 2) + pow(b2, 2)) > 1);
            x_con += b1*a2;
            y_con += b1*b2;
            z_con += a1;      
            if (prob <= 1.0 / 6) x += 1;
            else if (prob > 1.0 / 6 && prob <= 2.0 / 6) x -= 1;
            else if (prob > 2.0 / 6 && prob <= 3.0 / 6) y += 1;
            else if (prob > 3.0 / 6 && prob <= 4.0 / 6) y -= 1;
            else if (prob > 4.0 / 6 && prob <= 5.0 / 6) z += 1;
            else z -= 1; 
            RW[j][k] += pow(x, 2) + pow(y, 2) + pow(z, 2);
            RW_con[j][k] += pow(x_con, 2) + pow(y_con, 2) + pow(z_con, 2);
         } 
      }
      for(int s=0; s<100; s++){
         RW[j][s] = sqrt(RW[j][s]/C);
         RW_con[j][s] = sqrt(RW_con[j][s]/C);
      }   
   }
   ofstream Write2;
   Write2.open("RandomWalkDisc.out");
   ofstream Write3;
   Write3.open("RandomWalkCon.out");

   double sum_prog[B][100] = {0}; // medie progressive
   double sum2_prog[B][100] = {0}; // quadrato delle medie progressive
   double sum_prog_con[B][100] = {0}; // medie progressive per RW continuo
   double sum2_prog_con[B][100] = {0}; // quadrato delle medie progressive per RW continuo

   for (int i = 0; i < 100; i++) {
      for (int k = 0; k < B; k++) {
         sum_prog[k][i] = 0;
         sum2_prog[k][i] = 0;
         sum_prog_con[k][i] = 0;
         sum2_prog_con[k][i] = 0;
         for (int j = 0; j <= k; j++) {
               sum_prog[k][i] += RW[j][i];
               sum2_prog[k][i] += RW[j][i] * RW[j][i];
               sum_prog_con[k][i] += RW_con[j][i];
               sum2_prog_con[k][i] += RW_con[j][i] * RW_con[j][i];
         }
         sum_prog[k][i] /= (k + 1);
         sum2_prog[k][i] /= (k + 1);
         sum_prog_con[k][i] /= (k + 1);
         sum2_prog_con[k][i] /= (k + 1);
      }
   }

   double average, average_con, devstd, devstd_con;
   for (int i = 0; i < 100; i++) {
      for (int k = 0; k < B; k++) {
         average = sum_prog[k][i];
         average_con = sum_prog_con[k][i];
         if (k == 0) {
            devstd = 0;
            devstd_con = 0;
         } else {
            devstd = sqrt((sum2_prog[k][i] - sum_prog[k][i] * sum_prog[k][i]) / k);
            devstd_con = sqrt((sum2_prog_con[k][i] - sum_prog_con[k][i] * sum_prog_con[k][i]) / k);
         }
      }
      Write2 << i + 1 << "\t" << average << "\t" << devstd << endl;
      Write3 << i + 1 << "\t" << average_con << "\t" << devstd_con << endl;
   }
   Write2.close();
   Write3.close();

   return 0;
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
