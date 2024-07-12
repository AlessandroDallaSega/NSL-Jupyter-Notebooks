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

   //punto 3.1
   int throws = 10000;
   int N = 100;
   int L = throws/N;
   double avC[N];
   double avP[N];
   double sumavC = 0;
   double sumavP = 0;
   double s = 0;
   double maxc = 0;
   double maxp = 0;
   for(int j=0; j<N; j++){
        sumavC = 0;
        sumavP = 0;
        for (int i=0; i<L; i++){
            s = 100*exp((0.1 - pow(0.25,2)/2)*1 + 0.25*rnd.Gauss(0, 1));
            if(s - 100 >= 0){
                maxc = s - 100;
            } else maxc = 0;
            if(100 - s >= 0){
                maxp = 100 - s;
            } else maxp = 0;
            sumavC = sumavC + exp(-0.1*1)*maxc;
            sumavP = sumavP + exp(-0.1*1)*maxp;
        }
        avC[j] = sumavC/L;
        avP[j] = sumavP/L;
   }
   ofstream Write1;
   ofstream Write2;
   Write1.open("punto31Call.out");
   Write2.open("punto31Put.out");
   rnd.BlockAV(avC, N, Write1);
   rnd.BlockAV(avP, N, Write2);

    //punto 3.2
    double avCdisc[N];
    double avPdisc[N];
    double sumavCdisc = 0;
    double sumavPdisc = 0;
    for(int j=0; j<N; j++){
            sumavCdisc = 0;
            sumavPdisc = 0;
            for (int i=0; i<L; i++){
                s = 100;
                for (int k=0; k<100; k++){
                    s = s*exp((0.1 - 0.5*pow(0.25, 2))*0.01 + 0.25*rnd.Gauss(0, 1)*sqrt(0.01));
                }
                if(s - 100 >= 0){
                    maxc = s - 100;
                } else maxc = 0;
                if(100 - s >= 0){
                    maxp = 100 - s;
                } else maxp = 0;
                sumavCdisc = sumavCdisc + exp(-0.1*1)*maxc;
                sumavPdisc = sumavPdisc + exp(-0.1*1)*maxp;
            }
            avCdisc[j] = sumavCdisc/L;
            avPdisc[j] = sumavPdisc/L;
    }
    ofstream Write3;
    ofstream Write4;
    Write3.open("punto31Calldisc.out");
    Write4.open("punto31Putdisc.out");
    rnd.BlockAV(avCdisc, N, Write3);
    rnd.BlockAV(avPdisc, N, Write4);

    

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