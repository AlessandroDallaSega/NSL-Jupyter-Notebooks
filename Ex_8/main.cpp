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
#include "functions.h"

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

   //esercizitazione 8
    int ntot = 0;
    int nacc = 0;
    double delta= 1;
    ofstream out_parametri;
    out_parametri.open("parametri.out");
    double mu = 1;
    double sigma = 1;
    out_parametri << mu << "\t" << sigma << endl;
    double T = 1.0;
    int nacc_T;
    double acceptance_rate_T;
    double sigma_new, mu_new;
    double delta_T = 0.1;
    double beta;
    double prob;
    const int N = 100; //numero blocchi
    int m = 100; //lunghezza blocco
    double* AV = new double[N];
    auto[H, H_err] = calculate_hamilton(mu, sigma, rnd, ntot, nacc, delta, N, m, AV);
    int step_SA = 0;
    ofstream out_H;
    out_H.open("Hamiltonian_SA");
    //vector<double> H_av;
    //double hamilton_prog = H;
    while (T > 0.001){
        nacc_T = 0;
        beta = 1/T;
        //hamilton_prog = 0;
        for(int k=0; k<100; k++){
            sigma_new = sigma + delta_T * (rnd.Rannyu()*2-1);
            mu_new = mu + delta_T * (rnd.Rannyu()*2-1);
            auto[H_new, H_new_err] = calculate_hamilton(mu_new, sigma_new, rnd, ntot, nacc, delta, N, m, AV);
            prob = exp(-beta*(H_new - H));
            if (H_new < H || rnd.Rannyu() < prob){
                sigma = sigma_new;
                mu = mu_new;
                H = H_new;
                H_err = H_new_err;
                nacc_T ++;
                out_parametri << mu << "\t" << sigma << endl;
            }
            //hamilton_prog += H;
        }
        //H_av.push_back(hamilton_prog/100);
        out_H << step_SA + 1 << "\t" << H << "\t" << H_err << endl;
        acceptance_rate_T = (double)nacc_T/100;
        if (acceptance_rate_T < 0.3){
            delta_T *= 0.81;
        } else if(acceptance_rate_T > 0.7){
            delta_T *= 1.21;
        }
        cout << "acceptance for T = " << T << ": " << acceptance_rate_T << endl;
        T *= 0.95;  
        step_SA ++;
    }
    //rnd.BlockAV_file_vector(H_av, step_SA, out_H);
    out_parametri.close();
    out_H.close();
    cout << "SIGMA: " << sigma << endl;
    cout << "MU: " << mu << endl;
    cout << "GROUND STATE ENERGY: " << H << "    WITH AN ERROR: " << H_err << endl;


    ofstream out_x;
    ofstream out;
    out.open("Hamiltonian.out");
    out_x.open("Positions.out");
    int m_plus = 10000;
    calculate_hamilton_file(mu, sigma, rnd, ntot, nacc, delta, N, m_plus, AV, out, out_x);
    out.close();
    out_x.close();

    
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