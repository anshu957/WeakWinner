#include <Dynamics.h>
#include <iostream>

void DynamicalSystem::operator()(const state_type &x, state_type &dxdt, const real_t /* t */)
{
    real_t cp_x, cp_y;
    real_t curr_x, curr_y;
    size_t idx_curr_node; // index of current node
    for (size_t i = 0; i < m_net1.m_networkSize; i++)
    {
        idx_curr_node = m_net1.m_edgeList[i][0];
        curr_x = x[0 + 2 * idx_curr_node];
        curr_y = x[1 + 2 * idx_curr_node];

        cp_x = 0.0;
        cp_y = 0.0;
        size_t idx_nbr_node; // index of neighbors
        for (size_t j = 1; j < m_net1.m_edgeList[i].size(); j++)
        {
            idx_nbr_node = m_net1.m_edgeList[i][j];
            cp_x += m_coupling[idx_curr_node][idx_nbr_node] * (x[0 + 2 * idx_nbr_node] - curr_x);
            cp_y += m_coupling[idx_curr_node][idx_nbr_node] * (x[1 + 2 * idx_nbr_node] - curr_y);
        }

        // Full system defined by below differential equations
        dxdt[0 + 2 * i] = (curr_x - (m_par[i][0] * m_par[i][1] * curr_y)) * (1.0 - (curr_x * curr_x + curr_y * curr_y)) - m_par[i][1] * curr_y + cp_x;

        dxdt[1 + 2 * i] = (curr_y + (m_par[i][0] * m_par[i][1] * curr_x)) * (1.0 - (curr_x * curr_x + curr_y * curr_y)) + m_par[i][1] * curr_x + cp_y;
    }
}

void calphase(const std::vector<state_type> &x_vec, const std::vector<real_t> &times, arr2d &phases)
{
    // calculating phases for all oscillators in state vector using arctangent method

    for (size_t i = 0; i < times.size(); i++)
    {
        for (size_t j = 0; j < phases.cols(); j++)
        {
            phases(i, j) = atan2(x_vec[i][1 + DIM_OSC * j], x_vec[i][0 + DIM_OSC * j]);
        }
    }
}

void unwrap_phase(const arr2d &phases, arr2d &unwrapped_phases)
{
    // unwrapping all the phases from [-pi,pi] to real line
    // works similar to unwrap function in matlab/python

    // NOTE: k has to be tracked seperately for each oscillator
    std::vector<int> k(phases.cols(), 0);
    size_t i = 0;
    for (i = 0; i < phases.rows() - 1; i++)
    {
        for (size_t j = 0; j < phases.cols(); j++)
        {
            unwrapped_phases(i, j) = phases(i, j) + 2 * M_PI * k[j];
            if (fabs(phases(i + 1, j) - phases(i, j)) > M_PI)
            {
                if (phases(i + 1, j) < phases(i, j))
                    k[j] += 1;
                else
                    k[j] -= 1;
            }
        }
    }
    // last row needs to be updated seperately
    for (size_t j = 0; j < phases.cols(); j++)
        unwrapped_phases(i, j) = phases(i, j) + 2 * M_PI * k[j];
}

void ph_msd(const arr2d &unwrapped_phases, std::vector<std::pair<real_t, real_t>> &phasediff_stats)
{
    // calcuates the mean and standard deviation of pair wise phase differences between
    // oscillators.
    arr2d diff_phases(unwrapped_phases.rows(), unwrapped_phases.cols());
    for (size_t i = 0; i < unwrapped_phases.cols(); i++)
    {
        diff_phases.col(i) = unwrapped_phases.col(i) - unwrapped_phases.col((i + 1) % unwrapped_phases.cols());
    }

    auto m1 = diff_phases.colwise().mean();
    auto std1 = ((diff_phases.rowwise() - m1).square().colwise().sum() / (diff_phases.rows() - 1)).sqrt();

    for (size_t i = 0; i < m1.size(); i++)
    {
        phasediff_stats.push_back(std::make_pair(m1(i), std1(i)));
    }
}

/*
int main(int argc, char *argv[])
{
    Network net(FN_IN_1);

    std::vector<std::vector<real_t>> par;
    std::vector<std::vector<real_t>> coupling;

    Dynamics dyn(net, par, coupling);

    return 0;
}
*/