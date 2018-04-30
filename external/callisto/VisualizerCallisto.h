/*
*  All rights are retained by the authors, Clemson University, and the University of Minnesota.
*  Please contact ioannis@clemson.edu for licensing inquiries.
*
*/

/*!
*  @file       VisualizerCallisto.h
*  @brief      Contains the class to visualize the agents based on Callisto v5.2.
*			   See http://www.nieuwenhuisen.nl/callisto/ for details about Callisto
*/
#pragma once
#include "windows.h" 
#include <string>
using std::string;

#pragma warning( push )
#pragma warning( disable: 4251 )


namespace Callisto {
	/*!
	* @brief
	*  A collection of useful functions for drawing and animation using callisto.
	* This class uses static functionality.
	*/
	class __declspec(dllexport) VisualizerCallisto
	{
	private:
		static int idEnvironment;
		static int idImagePlane;
		static int idBoundingBox;
		static int idAnimation;
		static int idDrawing;
		static float widthBB;
		static float heightBB;

	public:
		VisualizerCallisto(void);
		~VisualizerCallisto(void);
		static int init(void);
		static int destroy(void);
		static int displayEnvironment(string name);
		static int removeEnvironment();
		static int setBoundingBox(float xMin, float xMax, float yMin, float yMax);
		static int setBackgroundColour(int viewID, float r, float g, float b);
		static int displayImage(string name);
		static int resetAnimation();
		static int getAnimationID();
		static int resetDrawing();
		static int getDrawingID();
		static int getEnvironmentID();
		static int suspendVisualization();
		static int resumeVisualization();
		static int setTime(float time);
		static int createBox(int id, float xw, float yw, float zw, float x, float y, float z, float red = 0, float green = 0, float blue = 0);
		static int createLines(int groupID, int nl, int *np, float *p, float red = 0, float green = 0, float blue = 0);
		static int createDisk(int id, float radius, float x = 0.0f, float y = 0.0f, float z = 0.0f, float red = 0, float green = 0, float blue = 0);
		static int createCylinder(int groupID, float radius, float height, float x = 0.0f, float y = 0.0f, float z = 0.0f, float red = 0, float green = 0, float blue = 0);
	
		static int createGroup(string name, int parent);
		static int addAnimationKey(float time, float *pos, int groupID, float *orientation = 0, bool isQuat = true);
		static int setGroupColor(int gid, float red, float green, float blue, float alpha = 1.0f);

		static int createCharacter(int &id, float radius, char *name = "character", bool orientation = false, int gid = -1, float x = 0.0f, float y = 0.0f, float z = 0.0f);
		static int createCylinderCharacter(int &id, float radius, float height, char *name = "character", bool orientation = false, int gid = -1, float x = 0.0f, float y = 0.0f, float z = 0.0f);
		static int setCharacterColor(int id, float red = 0, float green = 0, float blue = 1.0f, float alpha = 1.0f);
		static int destroyCharacter(int gid);

		static int createGrid(int gid, float width, float height, float gridSize, char *name = "grid", float r = 0, float g = 0, float b = 0);
		static int createFlag(int id, float x = 0.0f, float y = 0.0f, float z = 0.0f, float red = 0, float green = 0, float blue = 0);
	};
}

#pragma warning( pop )
