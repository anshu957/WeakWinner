
// The MIT License (MIT)

// Copyright (c) 2018 Anshul

//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.

/* code */

#include <Interface.h>
#include <Dynamics.h>
#include <Folder.h>
#include <random>
#include <fstream>

int main(int argc, char *argv[])
{
    // ++++++++++++++++ Start of [setting up RNG] ++++++++++++++++++++++++++++

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<real_t> dis(-2, 2);

    // +++++++++++++++++++ End of [setting up RNG] ++++++++++++++++++++++++++++

    // ++++++++++++++++  Start of [Folder management ]++++++++++++++++++++++++++++

    // Folder name for collecting all the data with current date as the subfolder
    Folder f("data");
    // filename to be written with correct folder path
    std::string fout_1 = f.m_path + FN_OUT_1;
    std::ofstream out_1(fout_1.c_str(), std::ios_base::out);

    // ++++++++++++++++++++ End of [Folder management ]++++++++++++++++++++++++++++

    // ++++++++++++++++  Start of [setting up the problem] ++++++++++++++++++++++++++++

    // Creating a network object
    Network net1(FN_IN_NETWORK);

    DynamicalSystem sys1(net1, FN_IN_COUPLING, FN_IN_PARAM);

    // +++++++++++++++++++  End of [setting up the problem] ++++++++++++++++++++++++++++

    // @@@@@@@@@@@@@@@@  Start of [ INTEGRATING the system] @@@@@@@@@@@@@@@@@@@@@@@@@@@@

    for (real_t d1 = 0.0; d1 <= D_MAX; d1 += D_STEP)
    {
        for (real_t d2 = 0.0; d2 <= D_MAX; d2 += D_STEP)
        {
            sys1.m_coupling[0][1] = d1;
            sys1.m_coupling[1][0] = d1;

            sys1.m_coupling[2][1] = d2;
            sys1.m_coupling[1][2] = d2;

            // Initialize the state of the system
            state_type x;
            for (size_t i = 0; i < net1.m_networkSize * DIM_OSC; i++)
                x[i] = dis(gen);

            size_t steps1 = integrate_adaptive(make_dense_output(1.0e-06, 1.0e-06, runge_kutta_dopri5<state_type>()), sys1, x, 0.0, T_TRANS, 0.01);

            for (size_t i = 0; i < x.size(); i++)
                assert(isfinite(x[i]));

            // time vector to sample data at specific points -- for observer
            std::vector<real_t> times_it;
            times_it.reserve(size_t(T_RUN));
            for (size_t i = 0; i < size_t(T_RUN); i++)
                times_it.push_back(1.0 * i);

            std::vector<state_type> x_vec;
            std::vector<real_t> times;
            size_t steps2 = integrate_times(make_dense_output(1.0e-06, 1.0e-06, runge_kutta_dopri5<state_type>()), sys1, x,
                                            times_it.begin(), times_it.end(), 0.01, push_back_state_and_time(x_vec, times));
            // @@@@@@@@@@@@@  End of [ INTEGRATING the system] @@@@@@@@@@@@@@@@@@@@@@@@

            // @@@@@@@@@@@@@  Start of [ generating SECONDARY DATA ] @@@@@@@@@@@@@@@@@@

            // calculating phases and unwrapping them
            arr2d phases(times.size(), net1.m_networkSize);
            arr2d unwrapped_phases(times.size(), net1.m_networkSize);

            calphase(x_vec, times, phases);
            unwrap_phase(phases, unwrapped_phases);

            std::vector<std::pair<real_t, real_t>> phasediff_stats;
            phasediff_stats.reserve(net1.m_networkSize);
            ph_msd(unwrapped_phases, phasediff_stats);

            out_1 << d1 << "\t" << d2;
            for (size_t i = 0; i < net1.m_networkSize; i++)
            {
                out_1 << "\t" << phasediff_stats[i].first << "\t" << phasediff_stats[i].second;
            }
            out_1 << "\n";

            // @@@@@@@@@@@@@  End of [ generating SECONDARY DATA ] @@@@@@@@@@@@@@@@@@@@@
        }
    }

    out_1.close();
    return 0;
}