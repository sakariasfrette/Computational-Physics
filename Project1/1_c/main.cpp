//  Project 1
#include <iostream>
#include <new>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <string>
#include "armadillo"
#include "time.h"
#include <algorithm>

using namespace std;
using namespace arma;

ofstream ofile;

inline double f(double x){return 100*exp(-10*x);}
inline double exactfunc(double x){return 1-(1-exp(-10))*x-exp(-10*x);}
inline double relativeerror(double sol, double exac){return fabs((sol-exac)/exac);}

int main(int argc, char const *argv[]) {
  /* code */


  int ex = atof(argv[1]);
  for (int i = 1; i <=ex; i++)
  {
    string outfilename ;

    //Formats name of lists from amount of loops needed
    if (ex < 10){

      char filestuff[9];
      sprintf(filestuff, "valn%d.txt", i );
      outfilename = filestuff;
      ofile.open(outfilename);

    }
    else{
      char filestuff[10];
      sprintf(filestuff, "valn%d.txt", i );
      outfilename = filestuff;
      ofile.open(outfilename);

    }

    int n = (int) pow(10,i);
    double h = 1./(n+2);
    cout << "Time step :" << h << endl;
    cout << "Dimension of vectors:" << n << endl;

    // Define vectors to solve equation Av = b
    vec v(n);
    vec x(n);
    vec g(n);
    vec gtilde(n);
    vec d(n);
    vec dtilde(n);
    vec e(n);
    vec sol(n);
    vec exac(n);
    clock_t start, finish;
    start = clock();
    x = linspace(h,1-h,n);
    for (int i = 0; i<n; i++) e(i) =1;
    for (int i =0; i<n; i++) d(i) = -2;

    for (int i =0; i<n; i++){
      gtilde(i) = h*h*f(x(i));
    }


    //dtilde(0) = d(0);
    //gtilde(0) = g(0);
    //v(0)= 0 ;
    //v(n) = 0;

    //Forward Part
    for (int i = 1; i<n; i++)
    {
      //i+2 to make sure we get x in [0,1]
      //d(i) = d(i)-1./d(i-1);
      gtilde(i) = gtilde(i) + ((i-1.)/i*gtilde(i-1));
    }
    //Backward Part

    v(n-1) = gtilde(n-1)*(n-1)/n;

    //exact solution

    for (int i= 0; i <n; i++){
      sol(i) = exactfunc(i*h);
    }

    for (int i = n-2; i>=0; i--)
    {
      v(i) = i/(i+1.)*(gtilde(i)+v(i+1));
    }
    finish = clock();
    double timeused = (double) (finish - start)/(CLOCKS_PER_SEC );
    cout << setprecision(10) << "N="<< n<< ":  Time used  for computing=" << timeused  << "s"<< endl;

    for (int i = 0; i<n; i++){
      exac(i) = exactfunc(x(i));
      ofile << setprecision(15) << v(i) << " " << x(i) << " " << exac(i) << endl;
      }
    ofile.close();

    }

  return 0;
}
