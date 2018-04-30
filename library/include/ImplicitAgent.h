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


/*!
*  @file       ImplicitAgent.h
*  @brief      Contains the Agent class.
*/

#pragma once
#include "AgentInitialParameters.h"
#include "proximitydatabase/Proximity2D.h"

/*!
@class	ImplicitAgent
@brief	A simple agent class
*/
class ImplicitAgent : public ProximityDatabaseItem
{
public:
	ImplicitAgent();
	~ImplicitAgent();
	void init(const AgentInitialParameters& initialConditions, SpatialProximityDatabase *const);
	void update(double dt);
	void doStep(double dt);

	/// @name AbstractAgent functionality
	//@{
	/// Overides the isAgent() from ProximityDatabaseItem
	bool isAgent() { return true; }
	/// Returns true if the agent is active.
	bool enabled() const { return _enabled; }
	/// Returns the position of the agent.  
	Vector2D position() const { return _position; }
	/// Returns the velocity of the agent.  
	Vector2D velocity() const { return _velocity; }
	/// Returns the goals of the agent.  
	Vector2D goal() const { return _goal; }
	/// Returns the preferred velocity of the agent.  
	Vector2D vPref() const { return _vPref; }
	/// Returns the orientation of the agent.  
	Vector2D orientation() const { return _orientation; }
	/// Returns the preferred speed of the agent.  
	double prefSpeed() const { return _prefSpeed; }
	/// Returns the radius of the agent.  
	double radius() const { return _radius; }
	/// Returns the id of the agent.  
	int id() const { return _id; }
	/// Returns the active id of the agent.  
	int activeID() const { return _activeid; }
	/// Returns the group id of the agent.  
	int gid() const { return _gid; }
	/// Sets the preferred velocity of the agent to a specific value.	
	void setPreferredVelocity(const Vector2D& v){_vPref = v;}
	/// Sets the  velocity of the agent to a specific value.	
	void setVelocity(const Vector2D& v) { _velocity = v; }
	/// Sets the active id of the agent to a specific value.	
	void setActiveID(const int& id) { _activeid = id; }	
	/// Returns the path of the agent
	vector<Vector2D> path(void) const {return _path;}
	/// Returns the orientations of the agent across its trajectory
	vector<Vector2D> orientations(void) const {return _orientations;}
	/// Finds the neighbors of the agent given a sensing radius
	void findNeighbors(double neighborDist, vector<ProximityDatabaseItem*>& nn);
	//@}

protected:
	inline void destroy();
			
protected:
	/// the preferred velocity of the character
	Vector2D _vPref;
	/// Determine whether the charater is enabled;
	bool _enabled;
	/// The position of the character. 
	Vector2D _position;
	/// The goal of the character. 
	Vector2D _goal;
	/// The orientation of the character
	Vector2D _orientation;
	/// The velocity of the character
	Vector2D _velocity;
	/// The radius of the character.
	double _radius;
	/// The id of the character. 
	int _id;
	/// The active id of the character. Workaround to account for the fact that the crowd size can dynamically change
	int _activeid;
	/// The group id of the character
	int _gid;
	/// The maximum speed of the character. Not used here 
	double _maxSpeed;
	/// The preferred speed of the character. 
	double _prefSpeed;
	/// The goal radius of the character
	double _goalRadiusSq;
	/// a pointer to this interface object for the proximity database
	ProximityToken* _proximityToken;	
	/// path and orientations
	vector<Vector2D> _path;
	vector<Vector2D> _orientations;
};