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

   /*for(int i=0; i<20; i++){
      //cout << rnd.Rannyu() << endl;
   //}

   rnd.SaveSeed();*/


   //esercizio 1.1

   //punto 1 - punto 2
   int throws = 100000;
   int nblocchi = 100;
   int blocco = int(throws/nblocchi);
   double AVE0[nblocchi];
   double AVE1[nblocchi];
   double sum, sum1, x;
   for (int i=0; i<nblocchi; i++){
      sum = 0;
      sum1 = 0;
      for(int j=0; j<blocco; j++){
         x = rnd.Rannyu();
         sum += x;
         sum1 += pow((x - 0.5),2);
      }
      AVE0[i] = sum/blocco;
      AVE1[i] = sum1/blocco;
   }
   
   ofstream Write1;
   Write1.open("punto1.out");
   rnd.BlockAV(AVE0, nblocchi, Write1);

   ofstream Write2;
   Write2.open("punto2.out");
   rnd.BlockAV(AVE1, nblocchi, Write2);


//punto 3
int throws1 = 10000;
int nintervalli = 100;
int arrayintervalli[nintervalli];
double arrchi[100];
double t;
int index=0;
ofstream Write3;
Write3.open("punto3.out");
double chiquadro = 0;
for(int y=0; y<100; y++){
   for(int i=0; i<nintervalli; i++){
   arrayintervalli[i] = 0;
}
   for(int i=0; i<throws1; i++){
      t = rnd.Rannyu();
      index=(t*100)/1;
      arrayintervalli[index]+=1;
   }
   chiquadro=0;
   for(int i=0; i<nintervalli; i++){
     chiquadro = chiquadro + (pow((arrayintervalli[i] - double(throws1)/nintervalli),2))/(double(throws1)/nintervalli);
   }
   arrchi[y]=chiquadro;
}
if(Write3.is_open()){
   for(int i=0; i<100; i++){
   Write3 << i+1 << "\t" << arrchi[i] << endl;
   }
} else cerr << "PROBLEM: Unable to open punto3.out" << endl;

//esercizio 1.2

//distribuzione uniforme
ofstream Write12unif;
Write12unif.open("punto12uniform.out");
double k=0;
for(int i=0; i<10000; i++){
   k=0;
   for(int j=0; j<10; j++){
      k = k + rnd.Rannyu();
   }
   Write12unif << rnd.Rannyu() << "\t" << (rnd.Rannyu() + rnd.Rannyu())/2 << "\t" << k/10 << "\t";
   k=0;
   for(int j=0; j<100; j++){
      k = k + rnd.Rannyu();
   }
   Write12unif << k/100 << endl;
}
//distribuzione esponenziale
ofstream Write12exp;
Write12exp.open("punto12exp.out");
double lambda=1;
for(int i=0; i<10000; i++){
   k=0;
   for(int j=0; j<10; j++){
      k += rnd.Ranexp(lambda);
   }
   Write12exp << rnd.Ranexp(lambda) << "\t" << (rnd.Ranexp(lambda) + rnd.Ranexp(lambda))/2 << "\t" << k/10 << "\t";
   k=0;
   for(int j=0; j<100; j++){
      k += rnd.Ranexp(lambda);
   }
   Write12exp << k/100 << endl;
}

//distribuzione Cauchy-Lorentz
ofstream Write12Cau;
Write12Cau.open("punto12Cau.out");
double gamma=1;
double mu=0;
for(int i=0; i<10000; i++){
   k=0;
   for(int j=0; j<10; j++){
      k += rnd.Rancauchy(gamma, mu);
   }
   Write12Cau << rnd.Rancauchy(gamma, mu) << "\t" << (rnd.Rancauchy(gamma, mu) + rnd.Rancauchy(gamma, mu))/2 << "\t" << k/10 << "\t";
   k=0;
   for(int j=0; j<100; j++){
      k += rnd.Rancauchy(gamma, mu);
   }
   Write12Cau << k/100 << endl;
}

//esercizio1.3
//d = 3; L = 1

double d = 3;
double L = 1;
double pigs[100];
double xp = 0;
int nhit = 0;
double a,b = 0;
double m = 0;
for(int i=0; i<100; i++){
   nhit = 0;
   for(int j=0; j<100000; j++){
      xp = rnd.Rannyu(1.5, 3);
      if (rnd.Rannyu() < 0.5){
         do {
         a = rnd.Rannyu(-1, 1);
         b = rnd.Rannyu(-1, 1);
         } while (sqrt(pow(a,2)+pow(b,2)) > 1 || a == 0);
         m = b/a;
         if ((xp + sqrt(pow(m,2)/(1+pow(m,2)))) > 3) {
            nhit++;
         }
      }
   }
   pigs[i] = (2*L*100000)/(nhit*d);
}

ofstream Write13;
Write1.open("punto13.out");
rnd.BlockAV(pigs, 100, Write13);

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
