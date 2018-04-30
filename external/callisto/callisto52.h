#pragma once

// include the types
#include "callisto52Types.h"

// the resulting manual of this file can be found at http://www.nieuwenhuisen.nl/callisto/docs/html/index.html

/*! \mainpage Callisto 5.2
 *
 * \section intro Callisto Visualizer and Collision Checker
Callisto is a library designed for visualization and collision checking of 3D environments. It is designed to take away the hassle of setting up a 3D engine and collision checker from the user. In just a few lines of codes, a 3D environment can be created. Callisto is originally created to allow researchers/developers to concentrate on their primary task instead of spending days to learn libraries and creating a framework. Callisto allows users to set-up scenes using primitives and/or VRML/XML. Its visualization environment is based on Ogre. For collision checking SOLID is used.
 *
 * More information about Callisto can be found at http://www.nieuwenhuisen.nl/callisto/callisto.php
 */

/*!
  \defgroup Global Global functions
  \defgroup ObjGroup Object/group functions
    \defgroup Primitives Functions to create primitives 
    \ingroup ObjGroup
    \defgroup Animation Animation functions 
    \ingroup ObjGroup
    \defgroup Retrieval Retrieve objects/groups
    \ingroup ObjGroup
  \defgroup CollisionDetection Collision detection
  \defgroup Visualisation Visualisation
  \defgroup Statistics Statistical information
  \defgroup Codes Callisto codes
    \defgroup Errors Error return codes in Callisto
    \ingroup Codes
    \defgroup ViewOptions Options used for CAL_SetViewOptions
    \ingroup Codes
    \defgroup LightOptions Options used for CAL_SetLightOptions
    \ingroup Codes
    \defgroup CameraOptions Options used for CAL_SetCameraOptions
    \ingroup Codes
    \defgroup Options Options used for CAL_SetGroupStateOptions and CAL_SetGroupOptions
    \ingroup Codes
*/

/*! This function initializes Callisto, starts the output window and GUI.
	\param visualisation Set to false if you dont want a visualisation window, default is TRUE.
  \param logFile Provide a filename to create a Callisto logging file (WARNING: this may become large!). Provide CAL_NULL for no logging.
  \param visLogFile Provide a filename for the visualisation library log file. Provide CAL_NULL for no logging.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Global
*/
int CAL_Initialisation (bool visualisation=true, char *logFile=CAL_NULL, char *visLogFile=CAL_NULL);

/*! This function ends Callisto and cleans up memory.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Global
*/
int CAL_End ();

/*! This function stops the visualisation until CAL_ResumeVisualisation is called.
  This function can be used to temporarily stop visualisation, for example when you need all processor power to do some calculations.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Visualisation
*/
int CAL_SuspendVisualisation ();

/*! This function resumes the visualisation after CAL_SuspendVisualisation.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Visualisation
*/
int CAL_ResumeVisualisation ();

/*! Shows a view. Every view has its own unique ID.
    There are at most 4 views (0..3). 0 being the main view. This view cannot be switched on/off.
  \param viewID The ID of the view.
  \param caption The caption of the window of the view, default is no caption.
  \param maximized Maximizes the view window.
  \returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Visualisation
*/
int CAL_ShowView (int viewID, char* caption="", bool maximized=false);

/*! Hides a view. Every view has its own unique ID.
    There are at most 4 views (0..3). 0 being the main view. This view cannot be switched on/off.
  \param viewID The ID of the view.
  \returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Visualisation
*/
int CAL_HideView (int viewID);

/*! Tiles the visible views.
  \returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Visualisation
*/
int CAL_TileViews ();

/*! Change the current view parameters.
  \param viewID The ID of the view.
  \param options The option you want to change. Multiple parameters can be changed at once by using the | operator. Legal values can be found \ref ViewOptions "here".
  \returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Visualisation
*/
int CAL_SetViewOptions (int viewID, long options);

/*! Change light related parameters. These are view independent.
  \param options The option you want to change. Multiple parameters can be changed at once by using the | operator. Legal values can be found \ref LightOptions "here".
  \ingroup Visualisation
  */
int CAL_SetLightOptions (long options);

/*! Set the fog distance. This value is inpendent of the view.
  \param dist The new fog distance.
  \returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Visualisation
*/
int CAL_SetViewFogDistance (CAL_scalar dist);

/*! Set the navigation speed. This is the speed with the keyboard navigation and middle mouse button work. Be sure to use CAL_SetViewOptions to set the CAL_MANUALNAVSPEED first!
  \param viewID The ID of the view.
  \param dist The new navigation speed.
  \returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Visualisation
*/
int CAL_SetViewNavigationSpeed (int viewID, CAL_scalar dist);

/*! Set the near clipping distance.
  \param viewID The ID of the view.
  \param dist The new near clipping distance, legal values are between 0 and 100.
  \returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Visualisation
*/
int CAL_SetViewNearClippingDistance (int viewID, CAL_scalar dist);

/*! Set size of major grid lines.
  \param viewID The ID of the view.
  \param size The distance between the major grid lines
  \returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Visualisation
*/
int CAL_SetViewGridSize (int viewID, CAL_scalar size);

/*! Sets the position of the camera.
  \param viewID The ID of the view.
  \param x The x position of the camera.
  \param y The y position of the camera.
  \param z The z position of the camera.
  \param time The time the camera should be at the position. This is used to define camera motions, do not use if you want a fixed camera.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Visualisation
*/
int CAL_SetCameraPosition (int viewID, CAL_scalar x, CAL_scalar y, CAL_scalar z, CAL_scalar time=-1);

/*! Sets the position the camera looks at. The look at position is also the center of navigation for the mouse.
  \param viewID The ID of the view.
  \param x The x position of the look at point.
  \param y The y position of the look at point.
  \param z The z position of the look at point.
  \param time The time the camera should look at the position. This is used to define camera motions, do not use if you want a fixed camera.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Visualisation
*/
int CAL_SetCameraLookAt (int viewID, CAL_scalar x, CAL_scalar y, CAL_scalar z, CAL_scalar time=-1);

/*! Sets the rotation of the camera around the axis defined by position and look at point. When using the CAL_SetCamera functions, the camera orientation is always parallel to the XZ plane, except when you define a roll (in that case, the camera rotates about its directional axis). When looking straight down (e.g. parallel with the y-axis) there are infinitely many camera orientations that are parallel to the XZ plane. In that case, the camera right vector is equal to the x-axis (and the up vector equal to the –z-axis).
  \param viewID The ID of the view.
  \param roll The roll of the camera around the axis. A value of 0 means the up-vector is parallel with the y-axis.
  \param time The time the camera should be at the specified roll. This is used to define camera motions, do not use if you want a fixed camera.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Visualisation
*/
int CAL_SetCameraRoll (int viewID, CAL_scalar roll, CAL_scalar time=-1);

/*! Alternative method to set camera state by providing position lookAt point and up vector. The angle between the vector formed by pos-lookAt and up should be 90 degrees, if not an CAL_ILLEGALVALUE is thrown.
  \param viewID The ID of the view.
  \param pos The position of the camera.
  \param lookAt The point the camera looks at (is directed to).
  \param up The camera up vector.
  \param time The time the camera should be at the specified roll. This is used to define camera motions, do not use if you want a fixed camera.
  \ingroup Visualisation
*/
int CAL_SetCameraState (int viewID, CAL_scalar pos[3], CAL_scalar lookAt[3], CAL_scalar up[3], CAL_scalar time=-1);

/*! Gets the position of the camera.
  \param viewID The ID of the view.
  \param x The x position of the camera.
  \param y The y position of the camera.
  \param z The z position of the camera.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Visualisation
*/
int CAL_GetCameraPosition (int viewID, CAL_scalar *x, CAL_scalar *y, CAL_scalar *z);

/*! Gets the position the camera looks at. The look at position is also the navigation center.
  \param viewID The ID of the view.
  \param x The x position of the look at point.
  \param y The y position of the look at point.
  \param z The z position of the look at point.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Visualisation
*/
int CAL_GetCameraLookAt (int viewID, CAL_scalar *x, CAL_scalar *y, CAL_scalar *z);

/*! Gets the rotation of the camera around the axis defined by position and look at point.
  \param viewID The ID of the view.
  \param roll The roll of the camera around the axis. A value of 0 means the up-vector is parallel with the y-axis.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Visualisation
*/
int CAL_GetCameraRoll (int viewID, CAL_scalar *roll);

/*! Deletes all defined camera animations and set camera to default position.
  \param viewID The ID of the view.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Visualisation
*/
int CAL_ClearCameraStates (int viewID);

/*! Set options for the camera of a specific view.
  \param viewID The ID of the view.
  \param options The option you want to change. Multiple parameters can be changed at once by using the | operator. Legal values can be found \ref CameraOptions "here".
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Visualisation
*/
int CAL_SetCameraOptions (int viewID, long options);

/*! Sets the background color of the visualisation window.
  \param viewID The ID of the view get the view parameters from, set to 0 if you don't know what this is.
  \param red The red component of the color (0...1).
  \param green The green component of the color (0...1).
  \param blue The blue component of the color (0...1).
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Visualisation
*/
int CAL_SetBackgroundColor (int viewID, CAL_scalar red, CAL_scalar green, CAL_scalar blue);

/*! Adds a resource (i.e. a directory) to the list of available resources.
  \param pathName The name of the path to add to the list of resources.
  \ingroup Global
*/
int CAL_AddTextureResource (char* pathName);

/*! Loads a texture to memory. The texture must be in the .ppm or the .png format. Do not forget to add a texture resource path first by using CAL_AddTextureResource.
  \param textureID The ID you want to give the texture, there is room for 500 textures numbered (0..499).
  \param fileName The filename of the texture. Do not add the filepath (this should be added through CAL_AddTextureResource). The texture has to be in .ppm, .png or .jpg format.
  \returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Global
*/
int CAL_LoadTexture (int textureID, char* fileName);

/*! Add a texture from memory.
  \param textureID The ID you want to give the texture.
  \param width The width in pixels of the texture. Must be a power of 2.
  \param height The height in pixels of the texture. Must be a power of 2.
  \param tex The textures in RGB format. The format is 8 bits red, 8 bits green and 8 bits blue (3 bytes per pixel). Length is width*height*3.
  \returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Global
*/
int CAL_SetTextureFromMem (int textureID, int width, int height, unsigned char *tex);

/*! Saves the content of a view in .bmp format to disk.
  \param viewID The ID of the view to save, set to 0 if you don't know what this is.
  \param fileName The file name of the .bmp.
  \returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Visualisation
*/
int CAL_ScreenCapture (int viewID, char* fileName);

/*! Load a scene in XML or VRML format from disk.
  \param *fileName The name of the file to load.
  \param parentID The parent group to put the loaded scene in, use 0 for no parent.
  \param eraseCurrent Erase the current scene (objects and groups).
  \param **error String with possible error string. Can be omitted.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Global
*/
int CAL_LoadScene (char* fileName, int parentID, bool eraseCurrent, char *&error);

/*! Save a (part of a) scene in XML or VRML format from disk.
  \param *fileName The name of the file to load.
  \param groupID The group id of the group that needs to be saved
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Global
*/
int CAL_SaveScene (char* fileName, int groupID);

/*! Change the time, all dynamic groups and objects will adapt to this time.
  \param time The time;
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Animation
*/
int CAL_SetTime (CAL_scalar time);

/*! Creates an object group.
  \param groupID This is set to the group ID the group gets.
  \param parentID The parent group of this group, use 0 if this group does not need a parent.
  \param colCheck True if this group is used for collision checks, false if not.
  \param name The name of the group as shown in the GUI, default is no name.
  \param collapsed The item will initially not be collapsed in the interface.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_CreateGroup (int* groupID, int parentID, bool colCheck, char* name="", bool collapsed=false);

/*! Delets a group, its child groups and all its objects.
  \param groupID The ID of the group to delete.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_DestroyGroup (int groupID);

/*! Deletes a groups child groups and all its objects.
  \param groupID The ID of the group to empty.
  \param subGroups Set to TRUE if subgroups of group should also be removed, default is FALSE.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_EmptyGroup (int groupID, bool subGroups=false);

/*! Move a group to a new parent.
  \param groupID The ID of the group to move.
  \param parentID The ID of the new parent.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_MoveGroup (int groupID, int parentID);

/*! Place a group and all its objects at a new position. The order in which Callisto processes is scale, orientation, position. Therefore position is influenced by scale and orientation (and also those of parent groups).
  \param groupID The ID of the group to translate.
  \param x The x component of the new position.
  \param y The y component of the new position.
  \param z The z component of the new position.
  \param time The time the group should be at the position. If multiple positions are defined (for different times), Callisto uses linear interpolation to determine the position of the group given the current time. A single position at time 0 defines the group to be static regardless of the time.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_SetGroupPosition (int groupID, CAL_scalar x, CAL_scalar y, CAL_scalar z, CAL_scalar time=-1);

/*! Set the group orientation using Euler angles (in radians) for a specific time. If time is specified, the spherical linear interpolation (SLERP) algorithm is used for interpolation.
  \param groupID The ID of the group to translate.
  \param xRot The orientation with respect to the x-axis;
  \param yRot The orientation with respect to the y-axis;
  \param zRot The orientation with respect to the z-axis;
  \param time The time at which the group should have the orientation. If multiple orientations are defined (for different times), Callisto uses the SLERP algorithm to determine the orientation of the group given the current time. A single orientation at time 0 defines the group to be static regardless of the time.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
*/
int CAL_SetGroupOrientationEuler (int groupID, CAL_scalar xRot, CAL_scalar yRot, CAL_scalar zRot, CAL_scalar time=-1);

/*! Set the group orientation using a quaternion for a specific time. If time is specified, the spherical linear interpolation (SLERP) algorithm is used for interpolation.
  \param groupID The ID of the group to rotate.
  \param x The x component of the quaternion.
  \param y The y component of the quaternion.
  \param z The z component of the quaternion.
  \param w The w component of the quaternion.
  \param time The time at which the group should have the orientation. If multiple orientations are defined (for different times), Callisto uses the SLERP algorithm to determine the orientation of the group given the current time. A single orientation at time 0 defines the group to be static regardless of the time.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_SetGroupOrientationQuaternion (int groupID, CAL_scalar x, CAL_scalar y, CAL_scalar z, CAL_scalar w, CAL_scalar time=-1);

/*! Set the group orientation using a rotation matrix for a specific time. If time is specified, the spherical linear interpolation (SLERP) algorithm is used for interpolation.
  \param groupID The ID of the group to rotate.
  \param mat The 3x3 matrix to set the orientation.
  \param time The time at which the group should have the orientation. If multiple orientations are defined (for different times), Callisto uses the SLERP algorithm to determine the orientation of the group given the current time. A single orientation at time 0 defines the group to be static regardless of the time.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_SetGroupOrientationMatrix (int groupID, CAL_matrix3 mat, CAL_scalar time=-1);

/*! Scale a group and all its objects.
  \param groupID The ID of the group to scale.
  \param xScale The scaling factor in the x direction.
  \param yScale The scaling factor in the y direction.
  \param zScale The scaling factor in the z direction.
  \param time The time at which the group should have the scaling. If multiple scalings are defined (for different times), Callisto uses linear interpolation to determine the scaling of the group given the current time. A single scaling at time 0 defines the group to be static regardless of the time.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_SetGroupScaling (int groupID, CAL_scalar xScale, CAL_scalar yScale, CAL_scalar zScale, CAL_scalar time=-1);

/*! Set the group visibility for a specific view and time. Whether the group is actually visible is also determined by the visibilty of the parent group. Parent visibility takes priority over group visibility.
  \param groupID The ID of the group.
  \param viewID The ID of the view.
  \param visible Boolean that states whether the group should be visibile or not.
  \param time The time at which the group should be (in)visible. A single visibility at time 0 defines the group to be always visible or invisible regardless of the time.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_SetGroupVisibility (int groupID, int viewID, bool visible, CAL_scalar time=-1);

/*! Remove all positions and keep group at current position.
  \param groupID The ID of the group.
  \param subGroups Set to TRUE if positions of subgroups of group should also be removed, default is FALSE.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_ClearGroupPositions (int groupID, bool subGroups=false);

/*! Remove all orientations and keep group at current orientation.
  \param groupID The ID of the group.
  \param subGroups Set to TRUE if orientations of subgroups of group should also be removed, default is FALSE.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_ClearGroupOrientations (int groupID, bool subGroups=false);

/*! Remove all scanlings and keep group at current scaling.
  \param groupID The ID of the group.
  \param subGroups Set to TRUE if scalings of subgroups of group should also be removed, default is FALSE.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_ClearGroupScalings (int groupID, bool subGroups=false);

/*! Remove all visibilities and keep group at current visibility.
  \param groupID The ID of the group.
  \param subGroups Set to TRUE if visibilities of subgroups of group should also be removed, default is FALSE.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_ClearGroupVisibilities (int groupID, bool subGroups=false);

/*! Spherically expand a group. Collision will occur with the set of points whose distance to the objects is at most the clearance.
    This function overrules individual group clearance.
  \param groupID The ID of the group to translate.
  \param c The clearance.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_SetGroupClearance (int groupID, CAL_scalar c);

/*! Sets the color of a group.
  \param groupID The ID of the group.
  \param red The red component of the color (0...1).
  \param green The green component of the color (0...1).
  \param blue The blue component of the color (0...1).
  \param alpha The alpha value of the color (0...1), 0 is fully transparant, 1 is fully opaque. Default is 1.
  \param receiveShadows Set when the objects need to receive shadows from other objects.
  \param subGroups Set to TRUE if subgroups of group should also get new color, default is FALSE.
  \param sID The ID of the material which gets this color, default is material 0.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_SetGroupColor (int groupID, CAL_scalar red, CAL_scalar green, CAL_scalar blue, CAL_scalar alpha=1, bool receiveShadows=true, bool subGroups=false, int sID=0);

/*! Set the texture for a group.
  \param groupID The ID of the group.
  \param textureID The ID of the texture.
  \param xtile The number of times to repeat the texture in the x-direction.
  \param ytile The number of times to repeat the texture in the y-direction.
  \param alpha The alpha value of the color (0...1), 0 is fully transparant, 1 is fully opaque. Default is 1.
  \param receiveShadows Set when the objects need to receive shadows from other objects.
  \param subGroups Set to TRUE if subgroups of group should also get new color. Default is FALSE.
  \param sID The ID of the material which gets this texture, default is material 0.
  \returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_SetGroupTexture (int groupID, int textureID, CAL_scalar xtile, CAL_scalar ytile, CAL_scalar alpha=1, bool receiveShadows=true, bool subGroups=false, int sID=0);

/*! Set whether the objects of the group cast shadows. The default value is true.
    Casting is shadow is independent of the color/texture the object has.
  \param groupID The ID of the group.
  \param castShadows Boolean the states whether the objects should cast shadows or not.
  \param subGroups Set to TRUE if casting shadows of subgroups should also be set. Default is FALSE.
  \returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_SetGroupCastShadows (int groupID, bool castShadows, bool subGroups=false);

/*! Sets the active material of the group.
  \param groupID The ID of the group
  \param subGroups Set to true if subgroups also have to change the active material.
  \param sID The ID of the new active material.
  \ingroup ObjGroup
*/
int CAL_SetGroupActiveMaterial (int groupID, bool subGroups, int sID);

/*! Sets the collision check capability of a group.
  \param groupID The ID of the group.
  \param colCapable True if the group can be collision checked.
  \param subGroups Set to true if subgroups also have to change their collision check capability recursively.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_SetGroupCollisionCheckCapability (int groupID, bool colCapable, bool subGroups=false);

/*! Change the name of a group.
  \param groupID The ID of the group.
  \param name The new name
  \ingroup ObjGroup
*/
int CAL_SetGroupName (int groupID, char *name);

/*! Change the group options parameters.
  \param groupID The ID of the group.
  \param options The option you want to change. Multiple parameters can be changed at once by using the | operator. Legal values can be found \ref StateOptions "here".
  \returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Animation
*/
int CAL_SetGroupOptions (int groupID, long options);

/*! Clones a group including all objects.
  \param groupIDNew Set to the group ID of the clone.
  \param groupID The ID of the group to clone.
  \param parentID The parent group of the clone (0 for no parent).
  \param colCheck Set to false if clone group does not need collision checks.
  \param name The name of the group, default is no name.
  \param cloneObjs Optional flag that indicates whether to clone the objects in the group as well.
  \returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_CloneGroup (int* groupIDNew, int groupID, int parentID, bool colCheck, char* name="", bool cloneObjs=true);

/*! Clones a group including all objects and child groups.
  \param newgroupIDs List of new group IDs, can be NULL.
  \param groupID The ID of the group to clone.
  \param parentID The parent group of the clone (0 for no parent).
  \param nr The size of the ids and names lists, can be 0.
  \param ids List of id's corresponding to the names list, can be NULL.
  \param names Optional list of new names for the clone, defaults to NULL.
  \param cloneObjs Optional flag that indicates whether to clone the objects in the group as well, defaults to true.
  \param keepColCap Optional flag that indicates whether the collision capabilities should be preserved. Default clones do not have collision check capabilities.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_CloneGroupRecursive (int* newgroupIDs, int groupID, int parentID, int nr, int* ids, char** names=CAL_NULL, bool cloneObjs=true, bool keepColCap=false);

/*! Move an object to another group.
  \param objID The ID of the object to destroy.
  \param groupID The ID of the new group.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_MoveObject (int objID, int groupID);

/*! Destroys an object.
  \param objID The ID of the object to destroy.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_DestroyObject (int objID);

/*! Set the object at a new position for a specific time.
  \param objID The ID of the object to translate.
  \param x The x component of the new position.
  \param y The y component of the new position.
  \param z The z component of the new position.
  \param time The time the object should be at the position. If multiple positions are defined (for different times), Callisto uses linear interpolation to determine the position of the object given the current time. A single position at time 0 defines the object to be static regardless of the time.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_SetObjectPosition (int objID, CAL_scalar x, CAL_scalar y, CAL_scalar z, CAL_scalar time=-1);

/*! Set the object orientation using Euler angles (in radians) for a specific time. If time is specified, the spherical linear interpolation (SLERP) algorithm is used for interpolation.
  \param objID The ID of the object to translate.
  \param xRot The orientation with respect to the x-axis;
  \param yRot The orientation with respect to the y-axis;
  \param zRot The orientation with respect to the z-axis;
  \param time The time at which the object should have the orientation. If multiple orientations are defined (for different times), Callisto uses the SLERP algorithm to determine the orientation of the object given the current time. A single orientation at time 0 defines the object to be static regardless of the time.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_SetObjectOrientationEuler (int objID, CAL_scalar xRot, CAL_scalar yRot, CAL_scalar zRot, CAL_scalar time=-1);

/*! Set the object orientation using a quaternion for a specific time. If time is specified, the spherical linear interpolation (SLERP) algorithm is used for interpolation.
  \param objID The ID of the object to rotate.
  \param x The x component of the quaternion.
  \param y The y component of the quaternion.
  \param z The z component of the quaternion.
  \param w The w component of the quaternion.
  \param time The time at which the object should have the orientation. If multiple orientations are defined (for different times), Callisto uses the SLERP algorithm to determine the orientation of the object given the current time. A single orientation at time 0 defines the object to be static regardless of the time.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_SetObjectOrientationQuaternion (int objID, CAL_scalar x, CAL_scalar y, CAL_scalar z, CAL_scalar w, CAL_scalar time=-1);

/*! Set the object orientation using a rotation matrix for a specific time. If time is specified, the spherical linear interpolation (SLERP) algorithm is used for interpolation.
  \param objID The ID of the object to rotate.
  \param mat The 3x3 matrix to set the orientation.
  \param time The time at which the object should have the orientation. If multiple orientations are defined (for different times), Callisto uses the SLERP algorithm to determine the orientation of the object given the current time. A single orientation at time 0 defines the object to be static regardless of the time.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_SetObjectOrientationMatrix (int objID, CAL_matrix3 mat, CAL_scalar time=-1);

/*! Set the object scaling for a specific time.
  \param objID The ID of the object to scale.
  \param xScale The scaling factor in the x direction.
  \param yScale The scaling factor in the y direction.
  \param zScale The scaling factor in the z direction.
  \param time The time at which the object should have the scaling. If multiple scalings are defined (for different times), Callisto uses linear interpolation to determine the scaling of the object given the current time. A single scaling at time 0 defines the object to be static regardless of the time.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_SetObjectScaling (int objID, CAL_scalar xScale, CAL_scalar yScale, CAL_scalar zScale, CAL_scalar time=-1);

/*! Set the object visibility for a specific time. Whether the object is actually visible is also determined by the visibilty of its group. Group visibility takes priority over object visibility.
  \param objID The ID of the object to scale.
  \param viewID The ID of the view.
  \param visible Boolean that states whether the object should be visibile or not.
  \param time The time at which the object should be (in)visible. A single visibility at time 0 defines the object to be always visible or invisible regardless of the time.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_SetObjectVisibility (int objID, int viewID, bool visible, CAL_scalar time=-1);

/*! Remove all positions and keep object at current position.
  \param objID The ID of the object.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_ClearObjectPositions (int objID);

/*! Remove all orientations and keep object at current orientation.
  \param objID The ID of the object.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_ClearObjectOrientations (int objID);

/*! Remove all scanlings and keep object at current scaling.
  \param objID The ID of the object.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_ClearObjectScalings (int objID);

/*! Remove all visibilities and keep object at current visibility.
  \param objID The ID of the object.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_ClearObjectVisibilities (int objID);

/*! Set the WORLD matrix of an object. This works even when group matrix is set.
  \param objID The ID of the object.
  \param matrix Should be of type CAL_matrix;
  \param time The time at which the object should have the world matrix.
  \returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_SetObjectWorldMatrix (int objID, CAL_matrix4 *matrix, CAL_scalar time);

/*! Spherically expand an object. Collision will occur with the set of points whose distance to the object is at most the clearance.
    If CAL_SetGroupClearance is called later, individual object clearance will be overruled.
  \param objID The ID of the object to translate.
  \param clearance The clearance.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_SetObjectClearance (int objID, CAL_scalar clearance);

/*! Sets the color of an object.
  \param objID The ID of the object.
  \param red The red component of the color (0...1).
  \param green The green component of the color (0...1).
  \param blue The blue component of the color (0...1).
  \param alpha The alpha value of the color (0...1), 0 is fully transparant, 1 is fully opaque. Default is 1.
  \param receiveShadows Set when the object needs to receive shadows from other objects.
  \param sID The ID of the material which gets this color. Default value is material 0.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_SetObjectColor (int objID, CAL_scalar red, CAL_scalar green, CAL_scalar blue, CAL_scalar alpha=1, bool receiveShadows=true, int sID=0);

/*! Set the texture for an object.
  \param objID The ID of the object.
  \param textureID The ID of the texture.
  \param xtile The number of times to repeat the texture in the x-direction.
  \param ytile The number of times to repeat the texture in the y-direction.
  \param alpha The alpha value of the color (0...1), 0 is fully transparant, 1 is fully opaque. Default is 1.
  \param receiveShadows Set when the object needs to receive shadows from other objects.
  \param sID The ID of the material which gets this texture, default is material 0.
  \returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_SetObjectTexture (int objID, int textureID, CAL_scalar xtile, CAL_scalar ytile, CAL_scalar alpha=1, bool receiveShadows=true, int sID=0);

/*! Set whether the object casts shadow. The default value is true.
    Casting is shadow is independent of the color/texture the object has.
  \param objID The ID of the object.
  \param castShadows Boolean the states whether the object should cast shadows or not.
  \returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjGroup
*/
int CAL_SetObjectCastShadows (int objID, bool castShadows);

/*! Sets the active material of the group.
  \param objID The ID of the object.
  \param sID The ID of the new active material.
  \ingroup ObjGroup
*/
int CAL_SetObjectActiveMaterial (int objID, int sID);

/*! Change the objects options parameters.
  \param objID The ID of the object.
  \param options The option you want to change. Multiple parameters can be changed at once by using the | operator. Legal values can be found \ref StateOptions "here".
  \returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Animation
*/
int CAL_SetObjectOptions (int objID, long options);

/*! Returnes the ID of a group or object with a certain name.
  \param ID This is set to the ID.
  \param name The name of the group/object.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Retrieval
*/
int CAL_GetID (int* ID, char* name);

/*! Get the properties of a group.
  \param groupID The ID of the group.
  \param CALGroup Pointer to an SCALGroup-structure.
  \returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Retrieval
*/
int CAL_GetGroup (int groupID, void *CALGroup);

/*! Returnes the ID of the nr'th childgroup.
  \param groupID The ID of the group.
  \param nr The nr of the childgroup.
  \param childGroupID This value is set to the nr'th childgroup.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Retrieval
*/
int CAL_GetGroupChildID (int groupID, int nr, int *childGroupID);

/*! Returnes the ID of the nr'th object.
  \param groupID The ID of the group.
  \param nr The nr of the object.
  \param objectID This value is set to the nr'th object.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Retrieval
*/
int CAL_GetGroupObjectID (int groupID, int nr, int *objectID);

/*! Get the type of an object (#CAL_BOX, #CAL_CYLINDER etc.).
  \param objID The ID of the object.
  \param objType This will be set to the object type according to the values in callistoTypes.h.
  \returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Retrieval
*/
int CAL_GetObjectType (int objID, int* objType);

/*! Get the WORLD matrix of an object.
  \param objID The ID of the object.
  \param matrix Should be of type CAL_matrix;
  \returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Retrieval
*/
int CAL_GetObjectWorldMatrix (int objID, CAL_matrix4 *matrix);

/*! Get the properties of an object.
  \param objID The ID of the object.
  \param SCALObj Pointer to an SCAL-object (SCALBox, SCALSphere etc.). This has to be of the right type (the type can be retrieved by using CAL_GetObjectType).
         Note that to retrieve a CAL_ELEVATIONGRID object, SCALPolygonGroup needs to be used.
  \returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Retrieval
*/
int CAL_GetObject (int objID, void* SCALObj);

/*! Set a callback function, this is called when the user pressed a key in a view.
  \param cb The adress of the callback function.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Global
*/
int CAL_SetKeypressCallback (CAL_KeypressCallback cb);

/*! Set a callback function, this is called when the user selects an object by clicking the left mouse button while pressing SHIFT.
  \param cb The adress of the callback function.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Global
*/
int CAL_SetObjectSelectCallback (CAL_ObjectSelectCallback cb);

/*! Create a box.
  \param groupID The group ID to put the object in.
  \param xw The width of the object.
  \param yw The height of the object.
  \param zw The depth of the object.
  \param objID Set to the object ID.
  \param name Name of the object. Default is no name.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Primitives
*/
int CAL_CreateBox (int groupID, CAL_scalar xw, CAL_scalar yw, CAL_scalar zw, int* objID=CAL_NULL, char* name="");

/*! Create a sphere.
  \param groupID The group ID to put the object in.
  \param radius The radius of the object.
  \param objID Set to the object ID.
  \param name Name of the object. Default is no name.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Primitives
*/
int CAL_CreateSphere (int groupID, CAL_scalar radius, int* objID=CAL_NULL, char* name="");

/*! Create a cylinder.
  \param groupID The group ID to put the object in.
  \param radius The radius of the object.
  \param height The height of the object.
  \param objID Set to the object ID.
  \param name Name of the object. Default is no name.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Primitives
*/
int CAL_CreateCylinder (int groupID, CAL_scalar radius, CAL_scalar height, int* objID=CAL_NULL, char* name="");

/*! Create a cylinder.
  \param groupID The group ID to put the object in.
  \param from The position of the center of the top of the cylinder.
  \param to The position of the center of the bottom of the cylinder.
  \param radius The radius of the object.
  \param objID Set to the object ID.
  \param name Name of the object. Default is no name.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Primitives
*/
int CAL_CreateCylinderFromTo (int groupID, CAL_scalar *from, CAL_scalar *to, CAL_scalar radius, int* objID=CAL_NULL, char* name="");

/*! Create a cone.
  \param groupID The group ID to put the object in.
  \param radius The radius of the bottom of the object.
  \param height The height of the object.
  \param objID Set to the object ID.
  \param name Name of the object. Default is no name.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Primitives
*/
int CAL_CreateCone (int groupID, CAL_scalar radius, CAL_scalar height, int* objID=CAL_NULL, char* name="");

/*! Create a cone.
  \param groupID The group ID to put the object in.
  \param from The position of the top of the cone.
  \param to The position of the center of the bottom of the cone.
  \param radius The radius of the bottom of the object.
  \param objID Set to the object ID.
  \param name Name of the object. Default is no name.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Primitives
*/
int CAL_CreateConeFromTo (int groupID, CAL_scalar *from, CAL_scalar *to, CAL_scalar radius, int* objID=CAL_NULL, char* name="");

/*! Create a triangle mesh.
  \param groupID The group ID to put the object in.
  \param nrTriangles The number of triangles the object consists of.
  \param *p List of coordinates of the object. The size of the list must be 9*nrTriangles.
  \param *texCoords List of texture coordinates. Each texture coordinate consists of two values betwee 0 and1. 
          There should be 2 coordinates for each point in the list. Thus its size should be 6*nrTriangles.
  \param objID Set to the object ID.
  \param name Name of the object. Default is no name.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Primitives
*/
int CAL_CreateTriangles (int groupID, int nrTriangles, CAL_scalar* p, CAL_scalar* texCoords=CAL_NULL, int* objID=CAL_NULL, char* name="");

/*! Create a polyline. This is for the visualisation only.
  \param groupID The group ID to put the object in.
  \param nl The number of lines.
  \param *np The number of points each line consists of.
  \param *p List of coordinates of the lines. Its size should be is 3 * np * nl.
  \param objID Set to the object ID.
  \param name Name of the object. Default is no name.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Primitives
*/
int CAL_CreatePolyline (int groupID, int nl, int *np, CAL_scalar *p, int* objID=CAL_NULL, char* name="");

/*! Create a tetrahedron.
  \param groupID The group ID to put the object in.
  \param *p List of coordinates of the object. The size of the list must be 3*4 since a tetrahydron consists of 4 points. 
    The first 3 points define the ground plane, the fourth is the top. The ground plane should be defined in a counter-clockwise order.
  \param objID Set to the object ID.
  \param name Name of the object. Default is no name.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Primitives
*/
int CAL_CreateTetrahedron (int groupID, CAL_scalar* p, int* objID=CAL_NULL, char* name="");

/*! Create an elevation grid on the XZ plane.
  \param groupID The group ID to put the object in.
  \param xDim The number of x coordinates.
  \param zDim The number of z coordinates.
  \param xStep The stepsize along the x-axis.
  \param zStep The stepsize along the z-axis.
  \param heights The height parameters. The number of elements should be (xDim+1)*(zDim+1).
  \param objID Set to the object ID.
  \param name Name of the object. Default is no name.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Primitives
*/
int CAL_CreateElevationGrid (int groupID, int xDim, int zDim, CAL_scalar xStep, CAL_scalar zStep, CAL_scalar* heights, int *objID=CAL_NULL, char* name="");

/*! Create a point cloud.
  \param groupID The group ID to put the object in.
  \param nrPoints The number of points.
  \param points The points with x, y and z coordinates. Its size should be 3 * nrPoints.
  \param colors Color parameters per point with red, green and blue values. Its size should be 3 * nrPoints.
  \param objID Set to the object ID.
  \param name Name of the object. Default is no name.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup Primitives
*/
int CAL_CreatePointCloud (int groupID, int nrPoints, CAL_scalar *points, CAL_scalar *colors=CAL_NULL, int *objID=CAL_NULL, char* name="");

/*! Create a label attached to an object. The label consists of a line and a text. The text is always faced towards the viewer. A label moves with the object it is attached to, but does not rotate or scale.
  \param objID The ID of the object.
  \param caption The caption of the label (use \n to start a new line).
  \param charHeight Size of the label text (between 0 and 100).
  \param x The x position of the label w.r.t. the object.
  \param y The y position of the label w.r.t. the object.
  \param z The z position of the label w.r.t. the object.
  \param showLine Indicates whether to show a line connecting the label to the object.
  \param fixedSize The label has a fixed size, independent of distance of object to camera (i.e. when zooming, label remains same size on screen).
  \returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjectDef
*/
int CAL_CreateLabel (int objID, char *caption, CAL_scalar charHeight, CAL_scalar x, CAL_scalar y, CAL_scalar z, bool showLine=true, bool fixedSize=false);

/*! Change the caption of the label.
  \param objID The ID of the object.
  \param caption The caption of the label.
  \returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjectDef
*/
int CAL_SetLabelCaption (int objID, char *caption);

/*! Change the color of the label.
  \param objID The ID of the object.
  \param red The red component of the color.
  \param green The green component of the color.
  \param blue The blue component of the color.
  \param alpha The alpha transparency component of the color.
  \returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjectDef
*/
int CAL_SetLabelColor (int objID, CAL_scalar red, CAL_scalar green, CAL_scalar blue, CAL_scalar alpha);

/*! Destroy a label.
  \param objID The ID of the object which label should be desroyed.
  \returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup ObjectDef
*/
int CAL_DestroyLabel (int objID);

/*! Create an overlay. An overlay is a box that appears at at a static position in the visualisation window. It can be used to display user defined information.
    An example of an overlay is the status overlay.
    \param overlayID The ID of the overlay.
    \param viewID The ID of the view the overlay should appear in.
    \param xPos The x coordinate of the screen position the overlay should appear.
    \param yPos The y coordinate of the screen position the overlay should appear.
    \param xDim The width of the overlay.
    \param yDim The height of the overlay.
    \param referencePos The reference from which xPos and yPos are calculated. The value can be CAL_TOPLEFT, CAL_TOPRIGHT, CAL_BOTTOMRIGHT or CAL_BOTTOMLEFT.
    \param nrLines The number of lines the overlay consists of.
    \param background Shows a partially transparent background behind the text.
    \ingroup Visualisation
*/
int CAL_CreateOverlay (int *overlayID, int viewID, int xPos, int yPos, int xDim, int yDim, int referencePos=CAL_TOPLEFT, int nrLines=1, bool background=true);

/*! Destroy an overlay.
  \param overlayID The ID of the overlay that needs to be destroyed.
  \ingroup Visualisation
*/
int CAL_DestroyOverlay (int overlayID);

/*! Change the overlay text.
  \param overlayID The ID of the overlay.
  \param lineNr The number of the line to change, the first line has number 1.
  \param text The line of text.
  \param charHeight The character height of the text.
  \param red The red component of the text color (0...1).
  \param green The green component of the text color (0...1).
  \param blue The blue component of the text color (0...1).
  \param alpha The alpha value of the text color (0...1), 0 is fully transparant, 1 is fully opaque. Default is 1.
  \param text An array of text lines. The length of the array should be equal as the number of lines used in CAL_CreateOverlay.
  \ingroup Visualisation
*/
int CAL_SetOverlayTextLine (int overlayID, int lineNr, char *text, float charHeight=10.f, float red=1.0f, float green=1.0f, float blue=1.0f, float alpha=1.0f);

/*! Change the visibility of an overlay.
  \param overlayID The ID of the overlay.
  \param visibility Boolean indicating visibility.
  \ingroup Visualisation
*/
int CAL_SetOverlayVisibility (int overlayID, bool visibility);

/*! Check whether a point collides with a group.
  \param groupID The group ID to check with.
  \param x The x coordinate of the point to check.
  \param y The y coordinate of the point to check.
  \param z The z coordinate of the point to check.
  \param multiple Flag whether to find all results, or just the first (faster).
  \param *nrCols The number of collisions.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup CollisionDetection
*/
int CAL_CheckPointCollision (int groupID, CAL_scalar x, CAL_scalar y, CAL_scalar z, bool multiple, int *nrCols);

/*! Check whether a line collides with a group.
  \param groupID The group ID to check with.
  \param x0 The x coordinate of the first point to check.
  \param y0 The y coordinate of the first point to check.
  \param z0 The z coordinate of the first point to check.
  \param x1 The x coordinate of the second point to check.
  \param y1 The y coordinate of the second point to check.
  \param z1 The z coordinate of the second point to check.
  \param multiple Flag whether to find all results, or just the first (faster).
  \param *nrCols The number of collisions.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup CollisionDetection
*/
int CAL_CheckLineCollision (int groupID, CAL_scalar x0, CAL_scalar y0, CAL_scalar z0, CAL_scalar x1, CAL_scalar y1, CAL_scalar z1, bool multiple, int *nrCols);

/*! Check whether two groups collide. Groups cannot be each others subgroups.
  \param group0 The ID of the first group.
  \param group1 The ID of the second group.
  \param multiple Flag whether to find all results, or just the first (faster).
  \param *nrCols The number of collisions.
	\returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup CollisionDetection
*/
int CAL_CheckGroupCollision (int group0, int group1, bool multiple, int *nrCols);

/*! Get the two positions where the distance between two groups is smallest.
  \param groupID0 The ID of the first group.
  \param groupID1 The ID of the second group.
  \param nrPairs The number of results.
  \returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup CollisionDetection
*/
int CAL_GetClosestPairs (int groupID0, int groupID1, int *nrPairs);

/*! Get the two positions where the penetration of two groups is largest.
  \param groupID0 The ID of the first group.
  \param groupID1 The ID of the second group.
  \param nrPairs The number of results.
  \returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup CollisionDetection
*/
int CAL_GetPenetrationDepths (int groupID0, int groupID1, int *nrPairs);

/*! Get the results of the objects involved in the last collision check/penetration depth/closest pair.
  \param userResults A list of type SCALResult which contains the results. The client is reponsible of creating the list with the right size (size == count). The results for closest pairs and penetration depths are sorted on distance.
  \returns The function returns #CAL_SUCCESS on success, and an \ref Errors "errorcode" on failure.
  \ingroup CollisionDetection
*/
int CAL_GetResults (void* userResults);

/*! Enable statistics. Statistical information will be gathered for collision checks, closest pairs and penetration depth. Both the number of calls and the total time spent in these functions will be administered. Beware that to gather the statistics themselved a small amount of time is spent.
  \param enable This value has to be either equal to either CAL_ENABLESTATISTICS or CAL_DISABLESTATISTICS.
  \ingroup Statistics
*/
int CAL_GatherStatistics (int enable);

/*! Reset all statistics to 0.
  \ingroup Statistics
*/
int CAL_ResetStatistics ();

/*! Save the statistical information to a comma delimited file.
  \param groupID The ID of the group.
  \param fileName The path and name of the file to write the information to.
  \ingroup Statistics
*/
int CAL_SaveGroupStatistics (int groupID, char *fileName);

/*! Add an entry to the Callisto log file.
  \param entry The text of the entry.
  \ingroup Global
*/
int CAL_AddLogEntry (char *entry);

/*! \file callisto52.h
\brief All functions and constants of Callisto.
*/