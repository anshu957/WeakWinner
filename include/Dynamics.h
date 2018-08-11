#pragma once
#include <Network.h>
#include <Interface.h>
#include <Folder.h>

class DynamicalSystem
{
  // member variables
public:
  std::vector<std::vector<real_t>> m_par;      // parameter vector for all nodes (nodes X  parameter)
  std::vector<std::vector<real_t>> m_coupling; // coupling vector with similar structure as edge list
  Network m_net1;

  // member functions
public:
  DynamicalSystem(const Network &tmp_net, const std::string fn_coupling, const std::string fn_par) : m_net1(tmp_net)
  {
    ReadFromFile(fn_coupling, m_coupling);
    ReadFromFile(fn_par, m_par);
  }
  void operator()(const state_type &x, state_type &dxdt, const real_t /* t*/);
};

struct push_back_state_and_time
{
  std::vector<state_type> &m_states;
  std::vector<real_t> &m_times;

  push_back_state_and_time(std::vector<state_type> &states, std::vector<real_t> &times)
      : m_states(states), m_times(times) {}

  void operator()(const state_type &x, real_t t)
  {
    m_states.push_back(x);
    m_times.push_back(t);
  }
};

void calphase(const std::vector<state_type> &x_vec, const std::vector<real_t> &times, arr2d &phases);

void unwrap_phase(const arr2d &phases, arr2d &unwrapped_phases);

void ph_msd(const arr2d &unwrapped_phases, std::vector<std::pair<real_t, real_t>> &phasediff_stats);