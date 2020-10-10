#include "solving.h"
#include "object.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include "time.h"
#include "armadillo"

solving::solving(){
    total_planets = 0;
    radius = 100;
    total_mass = 0;
    G = 4*M_PI*M_PI;
    totalKinetic = 0;
    totalPotential = 0;
}

solving::solving(double radi){
    total_planets = 0;
    radius = radi;
    total_mass = 0;
    G = 4*M_PI*M_PI;
    totalKinetic = 0;
    totalPotential = 0;
}

vec solving::stable_orbiter(vec pos){
  vec hyp = -pos;
  vec tanvec(2);
  tanvec[0] = -hyp[1]/sqrt(hyp[0]*hyp[0] + hyp[1]*hyp[1]);
  tanvec[1] = hyp[0]/sqrt(hyp[0]*hyp[0] + hyp[1]*hyp[1]);
  double v_stable = sqrt(GMstar/sqrt(hyp[0]*hyp[0] + hyp[1]*hyp[1]));
  vec stable_orbit(2);
  stable_orbit[0] = v_stable*tanvec[0]; stable_orbit[1] = v_stable*tanvec[1];
  return stable_orbit;

}

void add(object newplanet){
  total_planets += 1;
  total_mass += newplanet.mass;
  all_planets.push_back(newplanet);
}

mat setup_Matrix(int height, int width){
  mat matric(height,width,fill::zeros);
  return matric;
}

void VelocityVerlet(int dimension, int integration_points, double final_time, int print_number, double epsilon){

  // Create files for data storage
  char *filename = new char[1000];
      sprintf(filename, "PlanetsVV_%d_%.3f.txt",total_planets,time_step);

  std::ofstream output_file(filename);

  mat acceleration = setup_Matrix(total_planets, 3);
  mat acceleration_next = setup_Matrix(total_planets, 3);
  double t = 0;
  double Fx, Fy, Fz, Fxnew, Fynew, Fznew;
  double h = final_time/((double) integration_points);
  t+= h;

  while (t < final_time){



    //Loop over all planets
    for (int nr = 0; nr < total_planets; nr++){

      object &current = all_planets[nr];

      Fx = Fy = Fz = Fxnew = Fynew = Fznew = 0;

      //Find forces on other planets
      for (int nr2 = nr1 + 1; nr2 < total_planets; nr2++){
        object &other = all_planets[nr2];
        GravitationalForce(current, other, Fx, Fy, Fz, epsilon);
      }

      acceleration[nr][0] = Fx/current.mass;
      acceleration[nr][1] = Fy/current.mass;
      acceleration[nr][2] = Fz/current.mass;

      //Calculate position for each planet
      for (int k = 0; k<dimension; k++){
        current.position[k] += current.velocity[k] + h*h/2*acceleration[nr1][k];
      }

      //Loop again over all other planets
      for (int nr2 = nr1 + 1; nr2 < total_planets; nr2++){
        object &other = all_planets[nr2]
        GravitationalForce(current, other, Fxnew, Fynew, Fznew, epsilon);
      }

      acceleration_next[nr][0] = Fxnew/current.mass;
      acceleration_next[nr][1] = Fynew/current.mass;
      acceleration_next[nr][2] = Fznew/current.mass;

      //calculate velocity for planets
      for (int y = 0; y < dimension; y++){
        current.velocity[y] += h/2*(acceleration[nr][y] + acceleration_next[nr][y]);
      }
    }
  }






}
void print_to_file(mat planets){
  ofstream ofile;
  outfilename = "Planets_pos.txt";
  ofile.open(outfilename);
  ofile << setprecision(5) << total_planets <<" "<<integration_points<<endl;
  for(int i=0; i<total_planets; i++){
    for(int j=0; j<integration_points;j++){
      if(i==total_planets-1){
      ofile << setprecision(5) << planets(i,j,0) << " " << planets(i,j,1) << planets(i,j,2) <<;
    }
    else{
      ofile << setprecision(5) << planets(i,j,0) << " " << planets(i,j,1) << planets(i,j,2) <<endl;
    }
  }
}
}
