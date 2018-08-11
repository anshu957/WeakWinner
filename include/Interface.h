#pragma once
#include <iostream>
#include <math.h>
#include <vector>
#include <string>
#include <boost/array.hpp>
#include <boost/numeric/odeint.hpp>
#include <Eigen/Dense>

using namespace boost::numeric::odeint;
typedef double real_t;

typedef Eigen::ArrayXXd arr2d;
typedef Eigen::ArrayXd arr1d;

// Simulation Parameters
const real_t T_RUN = 5000;    // Run time of the system after transience
const real_t T_TRANS = 50000; // Transient time for the system
const real_t D_STEP = 0.002;  // Coupling Step size
const real_t D_MAX = 0.2;     // Maximum value of Coupling

const std::string FN_IN_NETWORK = "Adj.csv";       // Filename for the adjacency matrix
const std::string FN_OUT_1 = "test.dat";           // Filename for storing the results
const std::string FN_IN_COUPLING = "Coupling.csv"; // Filename for specifying initial coupling list
const std::string FN_IN_PARAM = "Parameters.csv";  // Filename for specifying initial parameter list

const size_t NO_OSCILLATORS = 3; // number of oscillators
const size_t DIM_OSC = 2;        // dimension of each oscillator

using state_type = boost::array<real_t, NO_OSCILLATORS * DIM_OSC>; // State type of the system

const size_t TOTAL_STEPS = size_t(pow(floor(D_MAX / D_STEP), 2)); // Total steps of simulation