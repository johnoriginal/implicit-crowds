// Implicit Crowds
// Copyright (c) 2018, Ioannis Karamouzas 
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other materials
//    provided with the distribution.
// THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR  A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
// OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
// IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Original author: Ioannis Karamouzas <http://cs.clemson.edu/~ioannis/>


#include "ImplicitEngine.h"
#include <omp.h>
#include <algorithm>


#define _INFTY 9e9

ImplicitEngine::ImplicitEngine()
{
	_spatialDatabase = NULL;
	_max_threads = omp_get_max_threads();
	_noAgents = 0;
}

ImplicitEngine::~ImplicitEngine()
{

	for (vector<ImplicitAgent*>::iterator it = _agents.begin(); it != _agents.end(); ++it)
	{
		delete *it;
		*it = 0x0;

	}

	if (_spatialDatabase != NULL)
	{
		delete _spatialDatabase;
		_spatialDatabase = 0x0;
	}
}

void ImplicitEngine::init(double xRange, double yRange, int xCells, int yCells)
{
	srand(23); // fixed seed to compare some results 
	_iteration = 0;
	_globalTime = 0;
	_spatialDatabase = new SpatialProximityDatabase(VectorXd::Zero(2, 1), Vector2D(xRange, yRange), Vector2D(xCells, yCells));
	//some defult paramaters, can be easily set via a file and calling readParameters
	_k = 1.5;
	_p = 2.;
	_t0 = 3.;
	_ksi = 2.;
	_eps = 0.2;
	_eta = 0.01;
	_neighborDist = 10.;
	_newtonIter = 100;
	_window = 5;
	_eps_x = 1e-5;

}


void ImplicitEngine::readParameters(const Parser& parser)
{
	parser.getDoubleValue("k", _k);
	parser.getDoubleValue("p", _p);
	parser.getDoubleValue("t0", _t0);
	parser.getDoubleValue("ksi", _ksi);
	parser.getDoubleValue("eps", _eps);
	parser.getDoubleValue("repulsive", _eta);
	parser.getDoubleValue("neighborDist", _neighborDist);
	parser.getIntValue("newtonIter", _newtonIter);
	parser.getIntValue("lbfgsWindow", _window);
	parser.getDoubleValue("eps_x", _eps_x);
}

bool ImplicitEngine::endSimulation()
{
	return _reachedGoals || _iteration >= _maxSteps;
}

void ImplicitEngine::draw()
{
	
}

void ImplicitEngine::addAgent(AgentInitialParameters& agentConditions)
{
	ImplicitAgent* newAgent = new ImplicitAgent();
	if (newAgent != NULL) {
	    agentConditions.id = _noAgents;
		newAgent->init(agentConditions , _spatialDatabase);
		_agents.push_back(newAgent);
		++_noAgents;
	}
}


void ImplicitEngine::updateSimulation()
{
	_reachedGoals = true;
	_activeAgents = 0;

	for (unsigned int i = 0; i < _noAgents; ++i)
	{
		_agents[i]->doStep(_dt);
		if (_agents[i]->enabled())
		{
			_reachedGoals = false;
			++_activeAgents;
		}
	}

	if (_reachedGoals) return;

	this->initializeProblem();
	this->minimize(_vNew);
	this->finalizeProblem();

	for (unsigned int i = 0; i < _noAgents; ++i)
	{
		if (_agents[i]->enabled())
			_agents[i]->update(_dt);
	}

	_globalTime += _dt;
	_iteration++;
}


void ImplicitEngine::initializeProblem()
{
	_noVars = _activeAgents + _activeAgents;
	_pos.resize(_noVars);
	_vel.resize(_noVars);
	_vGoal.resize(_noVars);
	_radius.resize(_activeAgents);
	_nn.resize(_activeAgents);
	//initial optimal velocity is zero to guarantee collision-freeness
	_vNew = VectorXd::Zero(_noVars);

	int counter = 0;
	for (unsigned int i = 0; i < _noAgents; ++i)
	{
		if (_agents[i]->enabled())
		{
			size_t id_y = counter + _activeAgents;
			_pos[counter] = _agents[i]->position().x();
			_pos[id_y] = _agents[i]->position().y();
			_vel[counter] = _agents[i]->velocity().x();
			_vel[id_y] = _agents[i]->velocity().y();
			_vGoal[counter] = _agents[i]->vPref().x();
			_vGoal[id_y] = _agents[i]->vPref().y();
			_radius[counter] = _agents[i]->radius();
			_agents[i]->setActiveID(counter);
			_nn[counter].clear();
			// precompute NN 
			_agents[i]->findNeighbors(_neighborDist, _nn[counter]);
			++counter;
		}
	}
}

void ImplicitEngine::finalizeProblem()
{
	for (unsigned int i = 0; i < _noAgents; ++i)
	{
		if (_agents[i]->enabled())
		{
			_agents[i]->setVelocity(Vector2D(_vNew(_agents[i]->activeID()), _vNew(_agents[i]->activeID() + _activeAgents)));
		}
	}
	
}

double ImplicitEngine::value(const VectorXd &vNew)
{
	//const int n = vNew.rows();
	_posNew = _pos + vNew*_dt;
	// acceleration and goal velocity contributions
	double f = 0.5*_dt*((vNew - _vel).array().square()).sum() + 0.5*_ksi*((vNew - _vGoal).array().square()).sum();

	bool exit = false;
	#pragma omp parallel for shared(exit) reduction(+:f) num_threads(_max_threads)
	for (int i = 0; i < _activeAgents; ++i)
	{
		if (!exit)
		{
			size_t id_y = i + _activeAgents;
			
			for (unsigned int j = 0; j < _nn[i].size() && !exit; ++j)
			{
				const ImplicitAgent* other = static_cast<ImplicitAgent*>(_nn[i][j]);
				int other_id = other->activeID();
				//int other_id = this->_mappedIds[other->id()];
				if (other_id > i)
				{
					size_t other_id_y = other_id + _activeAgents;
					double radius = _radius[i] + _radius[other_id];
					// are we colliding?
					double distance_energy = .0;
					if (min_distance_energy(_pos[i], _pos[id_y], _pos[other_id], _pos[other_id_y],
						vNew[i], vNew[id_y], vNew[other_id], vNew[other_id_y], radius, distance_energy))
						exit = true;
					else
					{
						// compute the ttc energy
						double ttc_energy = inverse_ttc_energy(_posNew[i], _posNew[id_y], _posNew[other_id], _posNew[other_id_y],
							vNew[i], vNew[id_y], vNew[other_id], vNew[other_id_y], radius);
						f += ttc_energy;
						f += distance_energy;

					}
				}

			}

		}
	}
	if (exit)
		f = _INFTY;

	return f;
}

double ImplicitEngine::value(const VectorXd &vNew, VectorXd &grad)
{
	_posNew = _pos + vNew*_dt;
	// acceleration and goal velocity contributions
	VectorXd vNewMinVel = vNew - _vel;
	VectorXd vNewMinVGoal = vNew - _vGoal;
	double f = 0.5*_dt*(vNewMinVel.array().square()).sum() + 0.5*_ksi*(vNewMinVGoal.array().square()).sum();
	grad = _ksi*vNewMinVGoal + (1 / _dt)*vNewMinVel;

	bool exit = false;
	//Agents
	#pragma omp parallel for shared(exit) reduction(+:f) num_threads(_max_threads)
	for (int i = 0; i < _activeAgents; ++i)
	{
		if (!exit)
		{
			size_t id_y = i + _activeAgents;
			for (unsigned int j = 0; j < _nn[i].size() && !exit; ++j)
			{
				const ImplicitAgent* other = static_cast<ImplicitAgent*>(_nn[i][j]);
				int other_id = other->activeID();
				//int other_id = this->_mappedIds[other->id()];
				if (other_id != i)
				{
					size_t other_id_y = other_id + _activeAgents;
					double radius = _radius[i] + _radius[other_id];
					double distance_energy = 0;
					double g[] = { 0, 0 };
					if (min_distance_energy(_pos[i], _pos[id_y], _pos[other_id], _pos[other_id_y],
						vNew[i], vNew[id_y], vNew[other_id], vNew[other_id_y], radius, distance_energy, g))
						exit = true;
					else
					{
						// compute the ttc energy
						double ttc_energy = inverse_ttc_energy(_posNew[i], _posNew[id_y], _posNew[other_id], _posNew[other_id_y],
							vNew[i], vNew[id_y], vNew[other_id], vNew[other_id_y], radius, g);

						if (other_id > i) { // do not add the energy twice!  
							f += ttc_energy;
							f += distance_energy;
						}

						//add the gradients 
						//In theory we could set the gradient of the neihbor to be the opposite of grad, but assuming openmp is used
						//it's faster to recompute the energy and does not lead to any shared violations
						grad[i] += g[0];
						grad[id_y] += g[1];

					}
				}
			}
		}
	}

	if (exit)
		f = _INFTY;

	return f;
}


bool ImplicitEngine::min_distance_energy(double Pa_x, double Pa_y, double Pb_x, double Pb_y, double Va_x, double Va_y, double Vb_x, double Vb_y, double radius, double& energy, double* grad)
{
	energy = 0;
	double Xx = Pb_x - Pa_x;
	double Xy = Pb_y - Pa_y;
	double Vx = Va_x - Vb_x;
	double Vy = Va_y - Vb_y;

	double speed = Vx * Vx + Vy * Vy;
	double rate = Xx*Vx + Xy*Vy;
	double tti = rate / (speed + 1e-4); // add a bit of noise since when speed = 0, tti is not differentiable
	tti = max(min(tti, _dt), 0.);

	double dx = Vx*tti - Xx;
	double dy = Vy*tti - Xy;
	double d = dx*dx + dy*dy;

	if (d <= radius*radius) //tunelling
	{
		return true;
	}

	d = sqrt(d);
	double distance = d - radius;
	energy = min(_eta/distance, _INFTY);

	if (grad != NULL && rate >0)
	{
		double tti_prime_x = 0, tti_prime_y = 0;
		if (tti > 0 && tti < _dt)
		{
			double tti_prime_x = (Xx - 2 * tti*Vx) / speed;
			double tti_prime_y = (Xy - 2 * tti*Vy) / speed;
		}
		double scale = -_eta / (d * distance * distance);
		double distance_prime_x = dx*(tti + Vx*tti_prime_x) + dy*(Vy*tti_prime_x);
		double distance_prime_y = dy*(tti + Vy*tti_prime_y) + dx*(Vx*tti_prime_y);
		grad[0] += scale*distance_prime_x;
		grad[1] += scale*distance_prime_y;
	}

	return false;

}

// here gradients are explicitly computed, though a bit too verbose (autodiff and/or Eigen will slow things down a bit)
double ImplicitEngine::inverse_ttc_energy(double Pa_x, double Pa_y, double Pb_x, double Pb_y, double Va_x, double Va_y, double Vb_x, double Vb_y, double radius, double* grad)
{
	
	double f = 0;

	//relative velocity
	double V_x = Va_x - Vb_x;
	double V_y = Va_y - Vb_y;

	//relative displacement
	double X_x = Pb_x - Pa_x;
	double X_y = Pb_y - Pa_y;
	double x = sqrt(X_x*X_x + X_y*X_y);
	double Xhat_x = X_x;
	double Xhat_y = X_y;
	if (x > 0)
	{
		Xhat_x /= x;
		Xhat_y /= x;
	}

	//parallel component
	double vp = Xhat_x*V_x + Xhat_y*V_y;
	if (vp < 0) //agents are diverging
	{
		return 0;
	}


	//tangential component
	double VT_x = V_x - vp*Xhat_x;
	double VT_y = V_y - vp*Xhat_y;
	double vt = sqrt(VT_x*VT_x + VT_y*VT_y);

	double rSq = radius*radius;
	double xMinR = x*x - rSq;
	double xMinR_sqrt = sqrt(xMinR);
	double nominator = sqrt(1 - _eps*_eps);
	double vtstar = nominator*radius*vp / xMinR_sqrt;

	if (vt < vtstar) // compute inv_ttc as usual
	{
		double discr = sqrt(rSq*vp*vp - xMinR*vt*vt);
		double inv_ttc = (x*vp + discr) / xMinR;
		if (inv_ttc > 0)
		{
			double mult = _k*pow(inv_ttc, _p - 1)*exp(-(1 / inv_ttc) / _t0);
			f = mult*inv_ttc;
			if (grad != NULL)
			{
				double VP_x = vp*Xhat_x;
				double VP_y = vp*Xhat_y;
				double A_x = -X_x + V_x*_dt - vp*_dt*Xhat_x;
				double A_y = -X_y + V_y*_dt - vp*_dt*Xhat_y;
				double B_x = (((_dt*vp + x)*VT_x)*xMinR / x - X_x*_dt*vt*vt + rSq*vp*A_x / x) / discr + _dt*VP_x;
				double B_y = (((_dt*vp + x)*VT_y)*xMinR / x - X_y*_dt*vt*vt + rSq*vp*A_y / x) / discr + _dt*VP_y;
				grad[0] += -mult / xMinR*((A_x + B_x)*(_p + 1 / (_t0*inv_ttc)) - 2 * _dt*(1 / _t0 + _p*inv_ttc)*X_x);
				grad[1] += -mult / xMinR*((A_y + B_y)*(_p + 1 / (_t0*inv_ttc)) - 2 * _dt*(1 / _t0 + _p*inv_ttc)*X_y);
			}

		}
	}
	else //linear extrapolation from vtstar
	{
		double inv_ttc = (x + _eps*radius)*vp / xMinR - nominator / _eps*(vt - vtstar) / xMinR_sqrt;
		if (inv_ttc > 0)
		{
			double mult = _k*exp(-(1 / inv_ttc) / _t0);
			f = mult*pow(inv_ttc, _p);
			if (grad != NULL)
			{
				double A_x = -X_x / x + V_x*_dt / x - vp*_dt*Xhat_x / x;
				double A_y = -X_y / x + V_y*_dt / x - vp*_dt*Xhat_y / x;
				double B_x = ((_eps*radius + x)*A_x) / xMinR + (nominator*((VT_x*_dt*vp / x + VT_x) / vt + radius*nominator / xMinR_sqrt*(A_x - _dt*vp*X_x / (xMinR)))) / (_eps*xMinR_sqrt) - _dt*X_x / xMinR*(vp*(_eps*radius + x) / xMinR - vp / x + inv_ttc);
				double B_y = ((_eps*radius + x)*A_y) / xMinR + (nominator*((VT_y*_dt*vp / x + VT_y) / vt + radius*nominator / xMinR_sqrt*(A_y - _dt*vp*X_y / (xMinR)))) / (_eps*xMinR_sqrt) - _dt*X_y / xMinR*(vp*(_eps*radius + x) / xMinR - vp / x + inv_ttc);
				mult *= -pow(inv_ttc, _p - 1)*(_p + 1 / (_t0*inv_ttc));
				grad[0] += mult*B_x;
				grad[1] += mult*B_y;
			}

		}
	}

	return  f;
}



double ImplicitEngine::linesearch(const Vector<double> & x0, const Vector<double> & searchDir, const double phi0, const Vector<double>& grad, const double alpha_init)
{
	double phi_prime = searchDir.dot(grad);
	// Minimum step length
	Vector<double> tmp(_noVars);
	for (size_t i = 0; i < _noVars; ++i)
	{
		tmp(i) = max(fabs(x0(i)), 1.);
	}

	double temp = (searchDir.array().abs() / tmp.array()).maxCoeff();
	double alpha_min = 1e-3 / temp;

	Vector<double> x(_noVars);
	double c = 1e-4; // sufficient decrease parameter
	double alpha = alpha_init; //  try a full Newton step first
	double alpha_prev = 0;
	double phi_prev = phi0;
	double alpha_next;

	while (true)
	{
		if (alpha < alpha_min)
			return alpha;// _min;
		x = x0 + alpha*searchDir;
		const double phi = value(x);
		if (phi < phi0 + c*alpha*phi_prime) // Sufficient function decrease
			break;
		else //Backtrack
		{
			if (alpha_prev == 0) // First time, quadratic fit 
			{
				alpha_next = -(phi_prime*alpha*alpha) / (2.0*(phi - phi0 - phi_prime*alpha)); //minimize [phi phi0 phi_prime]alpha^2 + phi_prime*alpha + phi0
			}
			else // Subsequent backtracks, cubic fit
			{
				//minimize a*alpha^3 + b*alpha^2 + phi_prime*alpha + phi0
				double rhs1 = phi - phi0 - alpha*phi_prime;
				double rhs2 = phi_prev - phi0 - alpha_prev*phi_prime;
				double alphaSq = alpha*alpha;
				double alpha2Sq = alpha_prev*alpha_prev;
				double denominator = alpha - alpha_prev;
				double a = (rhs1 / alphaSq - rhs2 / alpha2Sq) / denominator;
				double b = (-alpha_prev*rhs1 / alphaSq + alpha*rhs2 / alpha2Sq) / denominator;
				if (a == 0.0)
					alpha_next = -phi_prime / (2.0*b);
				else {
					const double disc = b*b - 3.0*a*phi_prime;
					if (disc < 0.0)
						alpha_next = 0.5*alpha;
					else if (b <= 0.0)  // minimum of the cubic
						alpha_next = (-b + sqrt(disc)) / (3.0*a);
					else  // minimize roundoff errors
						alpha_next = -phi_prime / (b + sqrt(disc));
				}
				if (alpha_next > 0.5*alpha)
					alpha_next = 0.5*alpha;  // alpha_new <= 0.5*alpha
			}

			alpha_prev = alpha;
			phi_prev = phi;
			alpha = max(alpha_next, 0.1*alpha);

		}
	}
	return alpha;
}

void ImplicitEngine::minimize(Vector<double> & x0)
{

	MatrixXd  s = MatrixXd::Zero(_noVars, _window);
	MatrixXd y = MatrixXd::Zero(_noVars, _window);

	Vector<double> alpha = Vector<double>::Zero(_window);
	Vector<double> rho = Vector<double>::Zero(_window);
	Vector<double> grad(_noVars), q(_noVars), grad_old(_noVars), x_old(_noVars), s_temp(_noVars), y_temp(_noVars);

	double f = value(x0, grad);

	double gamma_k = 1;
	double alpha_init = min(1.0, 1.0 / grad.lpNorm<Eigen::Infinity>());
	int iter;
	int end = 0;
	int j;
	int maxiter = _newtonIter;

	for (int k = 0; k < maxiter; k++)
	{
		x_old = x0;
		grad_old = grad;
		q = grad;

		//L-BFGS first - loop recursion			
		iter = min(_window, k);
		j = end;
		for (int i = 0; i < iter; ++i) {
			if (--j == -1) j = _window - 1;
			rho(j) = 1.0 / ((s.col(j)).dot(y.col(j)));
			alpha(j) = rho(j)*(s.col(j)).dot(q);
			q = q - alpha(j)*y.col(j);
		}

		//L-BFGS second - loop recursion			
		q = gamma_k*q;
		for (int i = 0; i < iter; ++i)
		{
			double beta = rho(j)*q.dot(y.col(j));
			q = q + (alpha(j) - beta)*s.col(j);
			if (++j == _window) j = 0;
		}

		// is there a valid descent?
		double dir = q.dot(grad);
		// not a valid direction due to bad Hessian estimation, restart the optimization 
		if (dir < 1e-4) {
			q = grad;
			maxiter -= k;
			k = 0;
			alpha_init = min(1.0, 1.0 / grad.lpNorm<Eigen::Infinity>());
		}
		const double rate = linesearch(x0, -q, f, grad, alpha_init);
		x0 = x0 - rate * q; //update solution
		s_temp = x0 - x_old;
		if (s_temp.lpNorm<Eigen::Infinity>() < _eps_x) //stop?
			break;

		f = value(x0, grad);
		y_temp = grad - grad_old;
		s.col(end) = s_temp;
		y.col(end) = y_temp;

		// update the history		
		gamma_k = s_temp.dot(y_temp) / y_temp.dot(y_temp);
		alpha_init = 1.0;
		if (++end == _window)
			end = 0;
	}

}


