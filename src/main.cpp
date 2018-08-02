#pragma warning( disable : 4996 )
#include<iostream>
#include<vector>
#include<fstream>
#include <numeric>
#include<sstream>
#include<chrono>
#include <boost/array.hpp>
#include <boost/numeric/odeint.hpp>

using namespace boost::numeric::odeint;
using namespace std;
using namespace std::chrono;


typedef double real_t;
typedef boost::array<real_t, 6> state_type;

void calphase(const vector<state_type> &x_vec, const vector<real_t> &time, vector<boost::array<real_t, 2> > &phases,
	const vector<real_t> &b);


class sys_dyn
{
	vector<real_t> m_omega;
	vector<real_t> m_q;
	vector<vector<real_t> > m_CouplingMatrix;
public:
	sys_dyn(vector<real_t> omega, vector<real_t> q, vector<vector<real_t> > coup), : m_omega(omega),m_q(q), m_CouplingMatrix(coup) {}

	void operator () (const state_type &x, state_type &dxdt, const real_t /* t */)
	{
		for (size_t i = 0; i < m_b.size(); i++)
		{
			real_t cp_x = 0.0, cp_y = 0.0;

			size_t curr_osc = i;
			size_t next_osc = (i + 1) % 3;
			size_t prev_osc = (i - 1 + 3) % 3;

			cp_x = m_CouplingMatrix[i][0] * (x[0 + 3 * prev_osc] - x[0 + 3 * curr_osc]) +
				m_CouplingMatrix[i][1] * (x[0 + 3 * next_osc] - x[0 + 3 * curr_osc]);
			cp_y = m_CouplingMatrix[i][0] * (x[1 + 3 * prev_osc] - x[1 + 3 * curr_osc]) +
				m_CouplingMatrix[i][1] * (x[1 + 3 * next_osc] - x[1 + 3 * curr_osc]);

            real_t curr_x = x[0+3*i];
            real_t curr_y = x[1+3*i];

			dxdt[curr_x] = (curr_x-m_q[i]*curr_y)*(1-(curr_x*curr_x + curr_y*curr_y))-m_omega[i]*curr_y + cp_x;
			dxdt[curr_y] = (curr_y+m_q[i]*curr_x)*(1-(curr_x*curr_x + curr_y*curr_y))+m_omega[i]*curr_x + cp_y;
		}
	}

};

struct push_back_state_and_time
{
	vector< state_type >& m_states;
	vector< real_t >& m_times;

	push_back_state_and_time(vector< state_type > &states, vector< real_t > &times)
		: m_states(states), m_times(times) { }

	void operator()(const state_type &x, double t)
	{
		m_states.push_back(x);
		m_times.push_back(t);
	}
};



int main(int argc, char *argv[])
{
	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	/*--------------------------------Start of Dynamical Evolution ----------------------------------------------------------------------------*/

	const real_t T_RUN				= 5000;
	const real_t T_TRANS			= 50000;
	const real_t T_RECORD			= 10000;
	const real_t D_STEP				= 0.002;
	const real_t D_MAX				= 0.2;
	const size_t TOTAL_STEPS		= size_t(pow(floor(D_MAX / D_STEP),2));
    const size_t kappa              = 5.0;
	
	bool flag = 0;
	//real_t d12 = D_MAX, d23 = D_MAX;
	
	vector<real_t> omega{ 1.2,0.94,1.2 };
	vector<real_t> q;
    q.reserve(omega.size());
    for(size_t i=0;i<omega.size();i++)
            q.push_back(kappa*omega[i]);

	stringstream ss_d12, ss_d23;


	size_t count = 1;
	for (real_t d12 = 0.0; d12 < D_MAX; d12 += D_STEP)
	{
		for (real_t d23 = 0.0; d23 < D_MAX; d23 += D_STEP)
		{
			cout << "At " << count << " step of " << TOTAL_STEPS << "...\r"<< flush;
			
			count++;

			ss_d12.str("");
			ss_d23.str("");

			ss_d12 << d12;
			ss_d23 << d23;

			vector<vector< real_t> > coup{  { 0.0, d12 },
											{ d12, d23 },
											{ d23, 0.0 } };

			// Initialize the state 
			state_type x{ 1.2, 2.8, 2.0, 3.6, 1.7, 0.5 };

			sys_dyn dyn1(omega,q, coup);


			// runge_kutta4< state_type > stepper; // fixed step size rk4

			// Transience
			//integrate_const(stepper, dyn1, x, 0.0, T_TRANS, 0.01); // fixed step size rk4
			 size_t steps1 = integrate_adaptive(make_dense_output(1.0e-06, 1.0e-06, runge_kutta_dopri5< state_type >()),dyn1, x, 0.0, T_TRANS, 0.01);

			//state_type x_in = x;
			for (size_t i = 0; i < x.size(); i++)
				assert(isfinite(x[i]));


			vector<state_type> x_vec;
			vector<real_t> times;
			
			// time vector to sample data at specific points -- for observer
			vector<real_t> times_it;
			times_it.reserve(size_t(T_RUN));
			for (size_t i = 0; i < size_t(T_RUN); i++)
			{
				times_it.push_back(1.0 * i);
			}


			// fixed step size rk4
			//size_t steps = integrate_const(stepper, dyn1, x, 0.0, T_RUN, 0.01, push_back_state_and_time(x_vec, times)); 

			// Adaptive step size
			size_t steps2 = integrate_times(make_dense_output(1.0e-06, 1.0e-06, runge_kutta_dopri5< state_type >()),dyn1, x,
				times_it.begin(), times_it.end(), 0.01, push_back_state_and_time(x_vec, times));
	

			//cout << "\n No. of steps used in the integration = " << steps2 << endl;
			//cout << "\n No. of steps used by observer : " << times.size() << endl;


			/*  Calculating Phases from the stored data*/
			vector<boost::array<real_t, 3> > phases;
			phases.reserve(times.size());

			calphase(x_vec, times, phases, b);

			ofstream out_1;
			string fn = "data/test_d12" + ss_d12.str() + "_d23" + ss_d23.str() + "_b1.09.dat";
			out_1.open(fn.c_str(), ios_base::out);

			for (size_t i = 0; i < times.size(); i++)
			{
				out_1 << times[i];
				for (size_t j = 0; j < phases[i].size(); j++)
					out_1 << "\t" << phases[i][j];
				out_1 << endl;
			}

			out_1.close();
		}
	}

	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<minutes> (t2 - t1).count();
					
	cout << "\n Program took :  "<< duration <<	" minutes to execute "<< endl;

	cin.get();
	
}


void calphase(const vector<state_type> &x_vec, const vector<real_t> &times, vector<boost::array<real_t, 3> > &phases,
	const vector<real_t> &b)
{
	real_t xcurr, ycurr;
	boost::array<real_t, 3> p;

	for (size_t i = 0; i < times.size(); i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
            xcurr = x_vec[i][0+3*j];
            ycurr = x_vec[i][1+3*j];
			
			p[j] = atan2(ycurr, xcurr);
		}

		phases.push_back(p);
	}

}

