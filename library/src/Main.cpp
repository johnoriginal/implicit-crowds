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
*  @file       Main.cpp
*  @brief      Implements the main simulator.
*/

#include "callisto/VisualizerCallisto.h"
#include "util/Draw.h"
#include "ImplicitEngine.h"
#include "conio.h"
using namespace Callisto;

// the max number of frames to simulate
int numFrames = 1000;
// the simulation time step
double dt = 0.2;
// the range of the environment
double xMin, xMax, yMin, yMax;
// the engine
ImplicitEngine * _engine = 0;


string getCmdOption(char ** begin, char ** end, const string & option)
{

	char ** itr = std::find(begin, end, option);
	if (itr != end && ++itr != end) 
	{
		return string(*itr);
	}
	return string();
}

void destroy()
{
	delete _engine;
	_engine = 0x0;
}


void setupScenario(const string &name)
{

	std::ifstream input(name);
	if (input.fail())
	{
		std::cerr << "Cannot read scenario file" << std::endl;
		destroy();
		exit(1);
	}

	try {
		
		input >> xMin;
		input >> xMax;
		input >> yMin;
		input >> yMax;
		
		//initialize the engine, given the dimensions of the environment
		_engine->init(xMax - xMin,yMax - yMin, 10, 10);
	
		// Read the default parameters for the agents	
		int nrAgents;
		input >> nrAgents;
		AgentInitialParameters par;
		par.velocity = Vector2D(0, 0); // assume agents start at rest
		par.goalRadius = 1.; // assume a fixed goal radius for all agents 
		par.maxSpeed = 2.; // assume a fixed maxspeed (actually is not being currently used)

		for (int i = 0; i < nrAgents; ++i)
		{
			input >> par.gid;
			input >> par.position.x();
			input >> par.position.y();
			input >> par.goal.x();
			input >> par.goal.y();
			input >> par.prefSpeed;
			input >> par.radius;
			_engine->addAgent(par);
		}		
	}
	catch (std::exception &e) {
		std::cerr << "Error reading the scenario file \n" << e.what() << "\n";
		destroy();
		exit(1);
	}

	input.close();	

}

void draw()
{
	VisualizerCallisto::resetAnimation();
	double animation_step = dt;
	const vector<ImplicitAgent*>& agents = _engine->getAgents();
	for (unsigned int j = 0; j < agents.size(); ++j)
	{
		const ImplicitAgent* agent = agents[j];
		//Animate the group members
		int charId;
		VisualizerCallisto::createCylinderCharacter(charId, (float)agent->radius(), .5f, "agent", true);
		//set the color based on the color id of the group	
		VisualizerCallisto::setCharacterColor(charId, groupColors[agent->gid() % 7].r, groupColors[agent->gid() % 7].g, groupColors[agent->gid() % 7].b);
		//Animate the character	
		double time = 0.;
		vector<Vector2D> path = agent->path();
		vector<Vector2D> or = agent->orientations();
		vector<Vector2D>::iterator it2 = or.begin();
		vector<Vector2D>::iterator it_end = path.end();
		for (vector<Vector2D>::iterator it = path.begin(); it != it_end; ++it, time += animation_step, ++it2)
		{
			float pos[] = { (float)it->x(), (float)it->y(), 0 };
			float orientation[] = { 0, 0, float(atan2((*it2).y(), (*it2).x()) + M_PI) };
			VisualizerCallisto::addAnimationKey((float)time, pos, charId, orientation, false);
		}
	}

	
	int gpath = VisualizerCallisto::createGroup("paths", VisualizerCallisto::getDrawingID());
	// draw the paths
	for (unsigned int j = 0; j < agents.size(); ++j)
	{

		int pid = VisualizerCallisto::createGroup("path", gpath); //the group to hold the character
		const ImplicitAgent* agent = agents[j];
		vector<Vector2D> path = agent->path();
		vector<Vector2D>::iterator it_end = path.end();
		int nrPts = (int)path.size();
		int np[1] = { nrPts };
		float *points = new float[nrPts * 3];
		float *ptr = points;
		for (int i = 0; i < nrPts; ++i)
		{
			*ptr++ = (float)path[i].x();
			*ptr++ = (float)path[i].y();
			*ptr++ = 0;
		}
		VisualizerCallisto::createLines(pid, 1, np, points, 0, 0, 1);
	}
}


int main(int argc, char **argv)
{	
	//parse command line arguments
	string dtArgs = getCmdOption(argv, argv + argc, "-dt");
	string framesArgs = getCmdOption(argv, argv + argc, "-frames");
	string scenarioFilename = getCmdOption(argv, argv + argc, "-scenario");
	string parFilename = getCmdOption(argv, argv + argc, "-parameters");

	if (!dtArgs.empty())
		dt = atof(dtArgs.c_str());
	if (!framesArgs.empty())
		numFrames = atoi(framesArgs.c_str());
    
	//load the engine and setup the scenario
	_engine = new ImplicitEngine();
	_engine->setTimeStep(dt);
	_engine->setMaxSteps(numFrames);
	setupScenario(scenarioFilename);
	
	//read some parameters
	Parser cParser;
	if (!parFilename.empty())
		cParser.registerParameters(parFilename);
	_engine->readParameters(cParser);

	//set the visualizer
	VisualizerCallisto::init();
	VisualizerCallisto::displayEnvironment(scenarioFilename.substr(scenarioFilename.find_last_of('/') + 1));
	VisualizerCallisto::setBackgroundColour(0, 0.99f, 0.99f, 0.99f);
	VisualizerCallisto::setBoundingBox((float)xMin, (float)xMax, (float)yMin, (float)yMax);
	VisualizerCallisto::resetDrawing();
	VisualizerCallisto::resetAnimation();		

	// Run the scenario
	std::cout << "Computing simulation" << std::endl;
	do
	{
		_engine->updateSimulation();
	} while (!_engine->endSimulation());
	std::cout << "Simulation has ended" << std::endl;

	// animate agents
	draw();

	//press a key to terminate
	while (!_kbhit())
	{
	}

	//finalize the environment
	destroy();
	VisualizerCallisto::destroy();

	return 0;
	
	
}