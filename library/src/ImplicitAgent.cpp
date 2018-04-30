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

#include "ImplicitAgent.h"


ImplicitAgent::ImplicitAgent()
{
	_enabled = false;
	_proximityToken = NULL;
}

ImplicitAgent::~ImplicitAgent()
{
	destroy();
}

void ImplicitAgent::destroy()
{
	if (_proximityToken != NULL)
	{
		delete _proximityToken;
		_proximityToken = 0x0;
	}
}

void ImplicitAgent::init(const AgentInitialParameters& initialConditions, SpatialProximityDatabase *const pd)
{
	// initialize the agent based on the initial conditions
	_position = initialConditions.position;
	_radius = initialConditions.radius;
	_prefSpeed = initialConditions.prefSpeed;
	_id = initialConditions.id;
	_activeid = _id;
	_gid = initialConditions.gid;
	_goalRadiusSq = initialConditions.goalRadius*initialConditions.goalRadius;
	_velocity = initialConditions.velocity;
	_goal = initialConditions.goal;
	_orientation = (_goal-_position).normalized();
	_enabled = true;	

	//add to the database
	_proximityToken = pd->allocateToken(this);
	// notify proximity database that our position has changed
	_proximityToken->updateForNewPosition(_position);

	// add initial position, orientation
	_path.push_back(position());
	_orientations.push_back(_orientation);
}


void ImplicitAgent::doStep(double dt)
{
	_vPref = _goal - _position;
	double distSqToGoal = _vPref.squaredNorm();
	if (distSqToGoal < _goalRadiusSq)
	{
			destroy();
			_enabled = false;
			return;
	}

	// compute preferred velocity
	if (_prefSpeed * dt*_prefSpeed * dt > distSqToGoal)
	  _vPref = _vPref/dt;
	else 
	 _vPref *= _prefSpeed / sqrt(distSqToGoal);
}



void ImplicitAgent::update(double dt)
{
	//clamp(_velocity, _maxSpeed);		
	_position += _velocity * dt;
	
	//simple smoothing of the orientation; there are more elaborate approaches
	if (_velocity.x() != 0 || _velocity.y() != 0)
		 _orientation = _orientation + (_velocity.normalized() - _orientation) * 0.4;
	
	// notify proximity database that our position has changed
	_proximityToken->updateForNewPosition(_position);
	// add position and orientation to the list
	_path.push_back(position());
	_orientations.push_back(_orientation);
}

void ImplicitAgent::findNeighbors(double neighborDist, vector<ProximityDatabaseItem*>& nn)
{
	_proximityToken->findNeighbors(_position, neighborDist, nn);
}