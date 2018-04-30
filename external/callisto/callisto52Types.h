#pragma once

/*! CAL_scalar value used. */
typedef float CAL_scalar;

/*! CAL_matrix4 value used.
Position can be found as:
m[0][3] = x;
m[1][3] = y;
m[2][3] = z;
*/
typedef CAL_scalar CAL_matrix4[4][4];
/*! CAL_matrix3 value used. */
typedef CAL_scalar CAL_matrix3[3][3];

/*! Callback function type for key press. */
typedef void (* CAL_KeypressCallback) (int viewID, char key, bool pressed);

/*! Callback function type for object selection. */
typedef void (* CAL_ObjectSelectCallback) (int viewID, int objID, CAL_scalar hitPoint[3]);

/*! CAL_NULL value. */
#define CAL_NULL 0

// ****** return values ******
/*! The function was succesful. \ingroup Errors */
#define CAL_SUCCESS                        0
/*! Callisto is not yet initialized, use CAL_Initialisation. \ingroup Errors */
#define CAL_NOT_INIT                      -1
/*! Callisto is already initialized. \ingroup Errors */
#define CAL_ALREADY_INIT                  -2
/*! The group does not exist. \ingroup Errors */
#define CAL_NO_SUCH_GROUP                 -3
/*! The parent group does not exist. \ingroup Errors */
#define CAL_NO_SUCH_PARENT_GROUP          -4
/*! The root group is a system group and cannot be manipulated. \ingroup Errors */
#define CAL_CANNOT_MANIPULATE_ROOT_GROUP  -5
/*! The object does not exist. \ingroup Errors */
#define CAL_NO_SUCH_OBJECT                -6
/*! One of the parameters has an illegal value. \ingroup Errors */
#define CAL_ILLEGAL_VALUE                 -7
/*! Cannot collision check because both groups are part of the same tree-branch. \ingroup Errors */
#define CAL_GROUPS_IN_SAME_SUBTREE        -8
/*! This name does not exist. \ingroup Errors */
#define CAL_NAME_NOT_FOUND                -9
/*! This name already exists. \ingroup Errors */
#define CAL_NAME_ALREADY_EXISTS          -10
/*! The file you try to read/write is not valid or could not be found. \ingroup Errors */
#define CAL_FILE_ERROR                   -11
/*! You try to do something with the visualiser while it is not running or it is suspended. \ingroup Errors */
#define CAL_VIS_NOT_RUNNING              -12
/*! The clone you are trying to make cannot be put in the source's subtree. \ingroup Errors */
#define CAL_CLONE_IN_SUBGROUP            -13
/*! The view you try to show is a already visible (use another viewID. \ingroup Errors */
#define CAL_VIEW_ALREADY_VISIBLE         -14
/*! You try to do something with a view, while it is not visible. \ingroup Errors */
#define CAL_VIEW_NOT_VISIBLE             -15
/*! Cannot calculate penetration depth, because objects do not overlap. \ingroup Errors */
#define CAL_GROUPS_DO_NOT_OVERLAP        -16
/*! Group not capable of collision checks. \ingroup Errors */
#define CAL_GROUP_NOT_COL_CAPABLE        -17
/*! Cannot load texture, check filename/path and fileformat, did you add a texture resource first by using CAL_AddTextureResource?. \ingroup Errors */
#define CAL_TEXTURE_ERROR                -18
/*! Key state already defined. \ingroup Errors */
#define CAL_ILLEGAL_KEY_STATE            -19
/*! You try to set properties of a group/object that has a motion defined. \ingroup Errors */
#define CAL_IS_DYNAMIC                   -20
/*! You try to use a file with the incorrect extension. \ingroup Errors */
#define CAL_INVALID_EXTENSION            -21
/*! Either provide visibility for all keystates of a group, or none. \ingroup Errors */
#define CAL_CANNOT_SET_VISIBILITY        -22
/*! Statistical information is not enabled. \ingroup Errors */
#define CAL_STATISTICSNOTENABLED         -23
/*! Cannot set texture to object without texture coordinates (applies to triangles). \ingroup Errors */
#define CAL_OBJ_HAS_NO_TEXTURE_COORDINATES - 24

// ****** view options ******
/*! Show the navigation compass. \ingroup ViewOptions */
#define CAL_SHOWCOMPASS              (1<<1)
/*! Hide the navigation compass. \ingroup ViewOptions */
#define CAL_HIDECOMPASS              (1<<2)
/*! Show the grid. \ingroup ViewOptions */
#define CAL_SHOWGRID                 (1<<3)
/*! Hide the grid. \ingroup ViewOptions */
#define CAL_HIDEGRID                 (1<<4)
/*! Show the grid permanent. \ingroup ViewOptions */
#define CAL_SHOWGRIDPERMANENT        (1<<5)
/*! Show the grid only when mouse pressed (must be combined with CAL_SHOWGRID). \ingroup ViewOptions */
#define CAL_SHOWGRIDONMOUSE          (1<<6)
/*! Show the position of the camera in the bottom right corner of the screen. \ingroup ViewOptions */
#define CAL_SHOWSTATUSTEXT           (1<<7)
/*! Hide the position of the camera in the bottom right corner of the screen. \ingroup ViewOptions */
#define CAL_HIDESTATUSTEXT           (1<<8)
/*! Prevent the user from navigating using the mouse and/or keyboard. \ingroup ViewOptions */
#define CAL_LOCKNAVIGATION           (1<<9)
/*! Allow the user to navigate. \ingroup ViewOptions */
#define CAL_UNLOCKNAVIGATION         (1<<10)
/*! Disables keyboard response of Callisto navigation. \ingroup ViewOptions */
#define CAL_LOCKKEYBOARDNAVIGATION   (1<<11)
/*! Enables keyboard response of Callisto navigation. \ingroup ViewOptions */
#define CAL_UNLOCKKEYBOARDNAVIGATION (1<<12)
/*! Show the ground plane (can be used to receive shadows on the ground). \ingroup ViewOptions */
#define CAL_SHOWGROUNDPLANE          (1<<13)
/*! Hide the ground plane. \ingroup ViewOptions */
#define CAL_HIDEGROUNDPLANE          (1<<14)
/*! Show all objects as wire-frames (can be used for debugging). \ingroup ViewOptions */
#define CAL_WIREFRAMERENDER          (1<<15)
/*! Show all objects as solids. \ingroup ViewOptions */
#define CAL_SOLIDRENDER              (1<<16)
/*! Show all object labels. \ingroup ViewOptions */
#define CAL_SHOWLABELS               (1<<17)
/*! Hide all object labels. \ingroup ViewOptions */
#define CAL_HIDELABELS               (1<<18)
/*! Use automatic navigation speed. \ingroup ViewOptions */
#define CAL_AUTONAVSPEED             (1<<19)
/*! Use manual navigation speed. \ingroup ViewOptions */
#define CAL_MANUALNAVSPEED           (1<<20)
/*! Use perspective projection. \ingroup ViewOptions */
#define CAL_PERSPPROJECTION          (1<<21)
/*! Use orthogonal projection. \ingroup ViewOptions */
#define CAL_ORTHOPROJECTION          (1<<22)
/*! Show shadows. \ingroup ViewOptions */
#define CAL_SHOWSHADOWS              (1<<23)
/*! Hide shadows. \ingroup ViewOptions */
#define CAL_HIDESHADOWS              (1<<24)

/*! Enable statistical information of collision checks. \ingroup StatisticsOptions */
#define CAL_ENABLESTATISTICS  1
/*! Disable statistical information of collision checks. \ingroup StatisticsOptions */
#define CAL_DISABLESTATISTICS 2

/*! Repeats position animation infinitely. Use with CAL_SetCameraOptions. \ingroup CameraOptions */
#define CAL_CAMPOSITIONCYCLIC          (1<<1)
/*! Do not repeat position animation. Use with CAL_SetCameraOptions. \ingroup CameraOptions */
#define CAL_CAMPOSITIONNONCYCLIC       (1<<2)
/*! Repeats look at animation infinitely. Use with CAL_SetCameraOptions. \ingroup CameraOptions */
#define CAL_CAMLOOKATCYCLIC            (1<<3)
/*! Do not repeat look at animation. Use with CAL_SetCameraOptions. \ingroup CameraOptions */
#define CAL_CAMLOOKATNONCYCLIC         (1<<4)
/*! Repeats roll animation infinitely. Use with CAL_SetCameraOptions. \ingroup CameraOptions */
#define CAL_CAMROLLCYCLIC              (1<<5)
/*! Do not repeat roll animation. Use with CAL_SetCameraOptions. \ingroup CameraOptions */
#define CAL_CAMROLLNONCYCLIC           (1<<6)
/*! Interpolate camera position animation. Use with CAL_SetCameraOptions. \ingroup CameraOptions */
#define CAL_CAMPOSITIONINTERPOLATION   (1<<7)
/*! Do not interpolate camera position animation. Use with CAL_SetCameraOptions. \ingroup CameraOptions */
#define CAL_CAMNOPOSITIONINTERPOLATION (1<<8)
/*! Interpolate camera look at animation. Use with CAL_SetCameraOptions. \ingroup CameraOptions */
#define CAL_CAMLOOKATINTERPOLATION     (1<<9)
/*! Do not interpolate camera look at animation. Use with CAL_SetCameraOptions. \ingroup CameraOptions */
#define CAL_CAMNOLOOKATINTERPOLATION   (1<<10)
/*! Interpolate camera roll animation. Use with CAL_SetCameraOptions. \ingroup CameraOptions */
#define CAL_CAMROLLINTERPOLATION       (1<<11)
/*! Do not interpolate camera roll animation. Use with CAL_SetCameraOptions. \ingroup CameraOptions */
#define CAL_CAMNOROLLINTERPOLATION     (1<<12)

/*! Repeats position animation infinitely. Use with CAL_SetGroupStateOptions or CAL_SetGroupOptions. \ingroup StateOptions */
#define CAL_POSITIONCYCLIC        (1<<1)
/*! Do not repeat position animation. Use with CAL_SetGroupStateOptions or CAL_SetGroupOptions. \ingroup StateOptions */
#define CAL_POSITIONNONCYCLIC     (1<<2)
/*! Repeats orientation animation infinitely. Use with CAL_SetGroupStateOptions or CAL_SetGroupOptions. \ingroup StateOptions */
#define CAL_ORIENTATIONCYCLIC     (1<<3)
/*! Do not repeat orientation animation. Use with CAL_SetGroupStateOptions or CAL_SetGroupOptions. \ingroup StateOptions */
#define CAL_ORIENTATIONNONCYCLIC  (1<<4)
/*! Repeats scaling animation infinitely. Use with CAL_SetGroupStateOptions or CAL_SetGroupOptions. \ingroup StateOptions */
#define CAL_SCALINGCYCLIC         (1<<5)
/*! Do not repeat scaling animation. Use with CAL_SetGroupStateOptions or CAL_SetGroupOptions. \ingroup StateOptions */
#define CAL_SCALINGNONCYCLIC      (1<<6)
/*! Repeats visibility animation infinitely. Use with CAL_SetGroupStateOptions or CAL_SetGroupOptions. \ingroup StateOptions */
#define CAL_VISIBILITYCYCLIC      (1<<7)
/*! Does not repeat visibility animation. Use with CAL_SetGroupStateOptions or CAL_SetGroupOptions. \ingroup StateOptions */
#define CAL_VISIBILITYNONCYCLIC   (1<<8)
/*! Interpolates between keystates (using linear interpolation for translation and SLERP for quaternion interpolation. Use with CAL_SetGroupStateOptions or CAL_SetGroupOptions. \ingroup StateOptions */
#define CAL_POSITIONINTERPOLATION (1<<9)
/*! Does not interpolate keystates (group/object jumps). Use with CAL_SetGroupStateOptions or CAL_SetGroupOptions. \ingroup StateOptions */
#define CAL_NOPOSITIONINTERPOLATION    (1<<10)
/*! Interpolates between keystates (using linear interpolation for translation and SLERP for quaternion interpolation. Use with CAL_SetGroupStateOptions or CAL_SetGroupOptions. \ingroup StateOptions */
#define CAL_ORIENTATIONINTERPOLATION   (1<<11)
/*! Does not interpolate keystates (group/object jumps). Use with CAL_SetGroupStateOptions or CAL_SetGroupOptions. \ingroup StateOptions */
#define CAL_NOORIENTATIONINTERPOLATION (1<<12)
/*! Interpolates between keystates (using linear interpolation for translation and SLERP for quaternion interpolation. Use with CAL_SetGroupStateOptions or CAL_SetGroupOptions. \ingroup StateOptions */
#define CAL_SCALINGINTERPOLATION       (1<<13)
/*! Does not interpolate keystates (group/object jumps). Use with CAL_SetGroupStateOptions or CAL_SetGroupOptions. \ingroup StateOptions */
#define CAL_NOSCALINGINTERPOLATION     (1<<14)
/*! The state inherits the scaling parameters from its parent. \ingroup StateOptions */
#define CAL_INHERITSCALING             (1<<15)
/*! The state does not inherit the scaling parameters from its parent. \ingroup StateOptions */
#define CAL_NOTINHERITSCALING          (1<<16)

/*! Use with CreateGroup/AddKeystate to set visibility. \ingroup Group */
#define CAL_FALSE 0
/*! Use with CreateGroup/AddKeystate to set visibility. \ingroup Group */
#define CAL_TRUE 1
/*! Use with CreateGroup/AddKeystate to set visibility. \ingroup Group */
#define CAL_USEPARENT 2
/*! Use with CreateGroup/AddKeystate to set visibility. \ingroup Group */
#define CAL_NOTSET 3

/*! The result of #CAL_GetObjectType if the object is a box. \ingroup Retrieval */
#define CAL_BOX                        1
/*! The result of #CAL_GetObjectType if the object is a sphere. \ingroup Retrieval */
#define CAL_SPHERE                     2
/*! The result of #CAL_GetObjectType if the object is a cylinder. \ingroup Retrieval */
#define CAL_CYLINDER                   3
/*! The result of #CAL_GetObjectType if the object is a cone. \ingroup Retrieval */
#define CAL_CONE                       4
/*! The result of #CAL_GetObjectType if the object is a plane. \ingroup Retrieval */
#define CAL_PLANE                      5
/*! The result of #CAL_GetObjectType if the object is a triangle group. \ingroup Retrieval */
#define CAL_TRIANGLES                  6
/*! The result of #CAL_GetObjectType if the object is a polyline. \ingroup Retrieval */
#define CAL_POLYLINE                   7
/*! The result of #CAL_GetObjectType if the object is a tetrahedron. \ingroup Retrieval */
#define CAL_TETRAHEDRON                8
/*! The result of #CAL_GetObjectType if the object is an elevation grid. \ingroup Retrieval */
#define CAL_ELEVATIONGRID              9
/*! The result of #CAL_GetObjectType if the object is a point cloud. \ingroup Retrieval */
#define CAL_POINTCLOUD                10
/*! The result of #CAL_GetObjectType if the object is a user drawn object. \ingroup Retrieval */
#define CAL_USERDRAWN                 11

/*! Used for overlays: glue overlay to top left of visualisation window. \ingroup Visualisation */
#define CAL_TOPLEFT     0
/*! Used for overlays: glue overlay to top right of visualisation window. \ingroup Visualisation */
#define CAL_TOPRIGHT    1
/*! Used for overlays: glue overlay to bottom right of visualisation window. \ingroup Visualisation */
#define CAL_BOTTOMRIGHT 2
/*! Used for overlays: glue overlay to bottom left of visualisation window. \ingroup Visualisation */
#define CAL_BOTTOMLEFT  3

/*! Retrieve information about the last collision check/closest pair/penetration depth. 
\brief Structure to receive collision info.
\ingroup CollisionDetection
*/

struct SCALResult
{
  /*! The first resulting object. */
  int objID0;
  /*! The second resulting object. */
  int objID1;
  /*! The group ID of the first resulting object. */
  int groupID0;
  /*! The group ID of the second resulting object. */
  int groupID1;
  /*! The first point involved in the check. Only applicable in case of closest pair and penetration depth. */
  CAL_scalar vector0[3];
  /*! The second point involved in the check. Only applicable in case of closest pair and penetration depth. */
  CAL_scalar vector1[3];
  /*! The penetration distance or distance between the groups, depending on the type of check. */
  CAL_scalar distance;
};

/*! The structure used with CAL_GetGroup to retrieve group information. 
\brief Structure to retrieve a group.
\ingroup Retrieval 
*/
struct SCALGroup
{
  /*! The number of direct subgroups this group has. */
  int nrChildren;
  /*! The number of objects this group contains (not including the potential subgroups). */
  int nrObjects;
  /*! The visibility settings for all of the 4 views. */
  bool visible[4];
  /*! Is the group collision capable (is it in the collision checker)? */
  bool colCapable;
  /*! The current world matrix of the group. */
  CAL_matrix4 matrix;
  /*! The name of the group. */
  char* name;
};

/*! A structure used with CAL_GetObject to retrieve information about a box. 
\brief Structure to retrieve a box.
\ingroup Retrieval 
*/
struct SCALBox
{
  /*! The ID of the group the object is in. */
  int groupID;
  /*! The current world matrix of the object. */
  CAL_matrix4 matrix;
  /*! The width of the box. */
  CAL_scalar width;
  /*! The height of the box. */
  CAL_scalar height;
  /*! The depth of the box. */
  CAL_scalar depth;
};

/*! A structure used with CAL_GetObject to retrieve information about a sphere.
\brief Structure to retrieve a sphere.
\ingroup Retrieval 
*/
struct SCALSphere
{
  /*! The ID of the group the object is in. */
  int groupID;
  /*! The current world matrix of the object. */
  CAL_matrix4 matrix;
  /*! The radius of the sphere. */
  CAL_scalar radius;
};

/*! A structure used with CAL_GetObject to retrieve information about a cylinder.
\brief Structure to retrieve a cylinder.
\ingroup Retrieval 
*/
struct SCALCylinder
{
  /*! The ID of the group the object is in. */
  int groupID;
  /*! The current world matrix of the object. */
  CAL_matrix4 matrix;
  /*! The radius of the cylinder. */
  CAL_scalar radius;
  /*! The height of the cylinder. */
  CAL_scalar height;
};

/*! A structure used with CAL_GetObject to retrieve information about a cone.
\brief Structure to retrieve a cone.
\ingroup Retrieval 
*/
struct SCALCone
{
  /*! The ID of the group the object is in. */
  int groupID;
  /*! The current world matrix of the object. */
  CAL_matrix4 matrix;
  /*! The radius of the cone. */
  CAL_scalar radius;
  /*! The height of the cone. */
  CAL_scalar height;
};

/*! A structure used with CAL_GetObject to retrieve information about a group of triangles.
\brief Structure to retrieve a plane.
\ingroup Retrieval 
*/
struct SCALPlane
{
  /*! The ID of the group the object is in. */
  int groupID;
  /*! The current world matrix of the object. */
  CAL_matrix4 matrix;
  /*! The points of the plane, 12 values in total define the 4 corners. */
  CAL_scalar *points;
};

/*! A structure used with CAL_GetObject to retrieve information about a group of triangles.
\brief Structure to retrieve a triangle mesh.
\ingroup Retrieval 
*/
struct SCALTriangles
{
  /*! The ID of the group the object is in. */
  int groupID;
  /*! The current world matrix of the object. */
  CAL_matrix4 matrix;
  /*! The number of triangles in this object. */
  int nrTriangles;
  /*! The points of the triangles, 9 values for every triangle. */
  CAL_scalar *points;
};

/*! A structure used with CAL_GetObject to retrieve information about a poly line.
\brief Structure to retrieve a polyline.
\ingroup Retrieval 
*/
struct SCALPolyline
{
  /*! The ID of the group the object is in. */
  int groupID;
  /*! The current world matrix of the object. */
  CAL_matrix4 matrix;
  /*! The number of lines. */
  int nrLines;
  /*! The number of points per line. The length of this list is determined by nrLines. */
  int *nrPoints;
  /*! The coordinates of the lines, 3 values per point. */
  CAL_scalar *points;
};

/*! A structure used with CAL_GetObject to retrieve information about a tetrahedron.
\brief Structure to retrieve a tetrahedron.
\ingroup Retrieval 
*/
struct SCALTetrahedron
{
  /*! The ID of the group the object is in. */
  int groupID;
  /*! The current world matrix of the object. */
  CAL_matrix4 matrix;
  /*! The coordinates of the tetrahedron. The length of this list is 3*4=12 values. */
  CAL_scalar *points;
};

/*! A structure used with CAL_GetObject to retrieve information about a group of triangles.
\brief Structure to retrieve a triangle mesh.
\ingroup Retrieval 
*/
struct SCALElevationGrid
{
  /*! The ID of the group the object is in. */
  int groupID;
  /*! The current world matrix of the object. */
  CAL_matrix4 matrix;
  /*! The number of x coordinates. */
  int xDim;
  /*! The number of z coordinates. */
  int zDim;
  /*! The stepsize along the x-axis. */
  CAL_scalar xStep;
  /*! The stepsize along the z-axis. */
  CAL_scalar zStep;
  /*! The height parameters. The number of elemets is (xDim+1)*(zDim+1). */
  CAL_scalar *heights;
  
};

/*! \file callisto52Types.h
\brief Structures to retrieve data from Callisto.
*/
