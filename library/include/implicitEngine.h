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



/*
 *  ImplicitEngine.h
 *
 *
 *  All rights are retained by the authors, Clemson University, and the University of Minnesota.
 *  Please contact the authors for licensing inquiries.
 *
 *  Authors: Ioannis Karamouzas, Rahul Narain, and Stephen J. Guy
 *  Contact: ioannis@clemson.edu
 */

/*!
*  @file       ImplicitEngine.h
*  @brief      Contains the ImplicitEngine class.
*/

#pragma once
#include "ImplicitAgent.h"
#include "Parser.h"
template <typename T>
using Vector = Eigen::Matrix<T, Eigen::Dynamic, 1>;

/**
* @brief The engine that performs implicit simulations.
*/
class ImplicitEngine 
{
  public:
	///Default Costructor
	ImplicitEngine();
	/// Destructor
	~ImplicitEngine();
	/// Initialization of the engine given the range of the environment, and the number of cells for the NN database.
	void init(double xRange, double yRange, int xCells, int yCells);
	/// Performs a simulation step
	void updateSimulation();
	/// Determines whether the simulation has to stop i.e. when all characters have reached their goals or the simulation steps have exceeded the maximum allowed number
	bool endSimulation();
	/// Draw/animate functionality using Callisto
	void draw();
	/// Add a new agent to the simulation given its parameters
	void addAgent(AgentInitialParameters& parameters);
	/// Read parameters from the Parser where they have been registered
	void readParameters(const Parser& parser);

	/// @name Get/Set functionality
	//@{
	/// Returns the list of agents in the simulation. 
	const vector<ImplicitAgent*> & getAgents() const{ return _agents; }
	///Returns the corresponding agent given its id
	ImplicitAgent* getAgent(int id) const { return _agents[id]; }
	/// Returns the time step of the simulation. 
	double getTimeStep() const { return _dt; }
	/// Sets the time step of the simulation.
	void setTimeStep(double dt) { _dt = dt; }
	/// Returns the number of simulations steps. 
	int getMaxSteps() const { return _maxSteps; }
	/// Sets the maximum number of simulation steps.
	void setMaxSteps(int steps) { _maxSteps = steps; }
	///  Returns the global time of the simulation. Initially this time is set to zero.  
	double getGlobalTime() const { return _globalTime; }
	/// Returns the number of agents in the simulation. 
	int getNumAgents() const { return _noAgents; }
	/// Returns the current simulation step. 
	int getIterationNumber() const { return _iteration; }
	//@}

protected:
	/// @name Functions to perform an implicit simulation step
	//@{
	/// Initializes the problem for the given current time step. Should be called before anything else
	void initializeProblem();
	/// Should be called after a solution has been found for the current time step
	void finalizeProblem();
	///  Returns the objective value for a given set of velocities. Will be used by linesearch
	double value(const  VectorXd &x);
	/// Returns the objective value and computes the gradient of the objective. Will be used by minimize
	double value(const  VectorXd &x, VectorXd &grad);
	/// The inverse time-to-collision energy. TODO: Use a different approximation than the linear extrapolation mentioned in the paper 
	inline double inverse_ttc_energy(double Pa_x, double Pa_y, double Pb_x, double Pb_y, double Va_x, double Va_y, double Vb_x, double Vb_y, double radius, double* grad = NULL);
	/// The minimum distance energy across a timestep. TODO: Replace this with velocity uncertainty (see ) that will make this obsolete
	inline bool min_distance_energy(double Pa_x, double Pa_y, double Pb_x, double Pb_y, double Va_x, double Va_y, double Vb_x, double Vb_y, double radius, double& energy, double* grad = NULL);
	/// L-BFGS implementation
	inline void minimize(Vector<double> & x0);
	/// Inexact line search using the Armijo condition
	inline double linesearch(const Vector<double> & x0, const Vector<double> & searchDir, const double phi0, const Vector<double>& grad, const double alpha_init = 1.0);
	//@}

protected:
	/// The time step in the simulation.
	double  _dt;
	/// The current time in the simulation.
	double  _globalTime;
	/// The current iteration step.
	int _iteration;
	///The maximum number of simulation steps.
	int _maxSteps;
	///Determine whether all agents have reached their goals
	bool _reachedGoals;
	/// The proximity database
	SpatialProximityDatabase * _spatialDatabase;
	/// The agents in the simulation
	vector<ImplicitAgent* >  _agents;
	/// Max cpu threads
	int _max_threads;
	/// The total number of agents
	unsigned int _noAgents;

	/// @name Parameters that affect a simulation. Can be set via a file.
	//@{
	///the parameters of the power-law
	double _k, _p, _t0, _eps;
	///the relaxation time for the goal potential
	double _ksi;
	///the scaling of the distance potential
	double	_eta;
	/// The maximum distance from the agent at which an object will be considered
	double  _neighborDist;
	/// The maximum number of Newton iterations
	int _newtonIter;
	/// Stopping criteria
	double _eps_x;
	/// L-BFGS window size
	int _window; 
	//@}

	/// @name Auxiliary variables needed for performing an implicit step
	//@{
	VectorXd _pos, _posNew, _vel, _vGoal, _radius, _vNew;
	size_t _noVars;
	int _activeAgents; // The number of active agents
 	vector<vector<ProximityDatabaseItem*>> _nn; // Vector of nearest neighbors per agent
	//@}
};