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
 *  @file       AgentInitialParameters.h
 *  @brief      Contains the AgentInitialParameters struct.
 */

#pragma once
#include <Eigen/Dense>
using namespace Eigen;
typedef Eigen::Matrix<double, 2, 1, Eigen::DontAlign> Vector2D; // do this to avoid alignment issues

/** 
  * @brief The initial parameters for a single agent.
  */
struct AgentInitialParameters {
	/// The start position of the agent. 
	Vector2D position;
	/// The goal position of the agent. 
	Vector2D goal;
	/// The initial velocity of the agent.
	Vector2D velocity;		
	/// The radius of the agent.
	double radius;
	/// The preferred speed of the agent. 
	double prefSpeed;
	/// The max speed of the agent. 
	double maxSpeed;
	/// How close to the goal the agent should be to stop the simulation. 
	double goalRadius;
	/// The group id of the agent
	int gid;
	/// The id of the agent
	int id;
};

