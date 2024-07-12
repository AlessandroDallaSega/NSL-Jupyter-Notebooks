#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <utility>
#include <sstream>
#include "random.h"

using namespace std;


double psi(double x, double mu, double sigma) {
    return exp(-pow((x - mu), 2) / (2 * pow(sigma, 2))) + exp(-pow((x + mu), 2) / (2 * pow(sigma, 2)));
}
double hamilton(double x, double mu, double sigma){
    return (((pow(x+mu,2)*exp(-pow(x+mu,2)/(2*pow(sigma,2))))/pow(sigma,4) - exp(-pow(x+mu,2)/(2*pow(sigma,2)))/pow(sigma,2)
           + (pow(x-mu,2)*exp(-pow(x-mu,2)/(2*pow(sigma,2))))/pow(sigma,4) - exp(-pow(x-mu,2)/(2*pow(sigma,2)))/pow(sigma,2))*(-0.5))/psi(x, mu, sigma)
           + pow(x,4) - pow(x,2)*5.0/2;
}

bool step(double &x, Random &rnd, double sigma, double mu, int ntot, int nacc, double delta){
    ntot ++;
    double x_new = rnd.Rannyu(x - delta, x + delta);
    double prob_old = pow(psi(x, mu, sigma), 2);
    double prob_new = pow(psi(x_new, mu, sigma), 2);
    if(prob_new > prob_old){
        x = x_new;
        nacc ++;
        return true;
    } else {
        if(rnd.Rannyu() < prob_new/prob_old) {
        x = x_new;
        nacc ++;
        return true;
        } else return false;
    }
}

pair <double, double> calculate_hamilton(double mu, double sigma, Random &rnd, int ntot, int nacc, double delta, int N, int m, double*AV){
    double x = rnd.Rannyu(-1, 1);
    double current;
    for(int i=0; i<N; i++){
        current = 0;
        for(int j=0; j<m; j++){
            current += hamilton(x, mu, sigma);
            step(x, rnd, sigma, mu, ntot, nacc, delta);
        }
        AV[i] = current/m;   
    }
    return rnd.BlockAV(AV, N);
}

void calculate_hamilton_file(double mu, double sigma, Random &rnd, int ntot, int nacc, double delta, int N, int m, double*AV, std:: ofstream& file, std:: ofstream& file_position){
    double x = rnd.Rannyu(-1, 1);
    file_position << x << endl;
    double current;
    for(int i=0; i<N; i++){
        current = 0;
        for(int j=0; j<m; j++){
            current += hamilton(x, mu, sigma);
            step(x, rnd, sigma, mu, ntot, nacc, delta);
            file_position << x << endl;
        }
        AV[i] = current/m;   
    }
    rnd.BlockAV_file(AV, N, file);
}
