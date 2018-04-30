// Copyright (c) 2002-2003, Sony Computer Entertainment America
// Original author: Craig Reynolds <craig_reynolds@playstation.sony.com>
// Copyright (c) 2018, Ioannis Karamouzas 
// This is a modified version of OpenSteer's <http://opensteer.sourceforge.net/> locality query code. 
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


#ifndef	_lq2D_h
#define	_lq2D_h
/* ------------------------------------------------------------------ */
/*                                                                    */
/*                       Data types use by LQ                         */
/*                                                                    */
/* ------------------------------------------------------------------ */
/* ------------------------------------------------------------------ */
/* This structure is a proxy for (and contains a pointer to) a client
   (application) object in the spatial database.  One of these exists
   for each client object.  This might be included within the
   structure of a client object, or could be allocated separately.  */

typedef struct lqClientProxy2D
{
    /* previous object in this bin, or NULL */
    lqClientProxy2D*  prev;

    /* next object in this bin, or NULL */
    lqClientProxy2D*  next;

    /* bin ID (pointer to pointer to bin contents list) */
    lqClientProxy2D** bin;

    /* pointer to client object */
	void* object;

    /* the object's location ("key point") used for spatial sorting */
    double x;
    double y;
} lqClientProxy2D;


/* ------------------------------------------------------------------ */
/* This structure represents the spatial database.  Typically one of
   these would be created, by a call to lqCreateDatabase, for a given
   application.  */

typedef struct lqInternalDB2D
{
    /* the origin is the super-brick corner minimum coordinates */
    double originx, originy;

    /* length of the edges of the super-brick */
    double sizex, sizey;

    /* number of sub-brick divisions in each direction */
    int divx, divy;
	
    /* pointer to an array of pointers, one for each bin */
    lqClientProxy2D** bins;

    /* extra bin for "everything else" (points outside super-brick) */
    lqClientProxy2D* other;

} lqInternalDB2D;

/* ------------------------------------------------------------------ */
/*                                                                    */
/*                            Basic API                               */
/*                                                                    */
/* ------------------------------------------------------------------ */
/* Allocate and initialize an LQ database, returns a pointer to it.
   The application needs to call this before using the LQ facility.
   The nine parameters define the properties of the "super-brick":
      (1) origin: coordinates of one corner of the super-brick, its
          minimum x, and y extent.
      (2) size: the width, height and depth of the super-brick.
      (3) the number of subdivisions (sub-bricks) along each axis.
   This routine also allocates the bin array, and initialize its
   contents. */


lqInternalDB2D* lqCreateDatabase2D (double originx, double originy,
			double sizex,   double sizey,
			int   divx,    int   divy);


/* ------------------------------------------------------------------ */
/* Deallocates the LQ database */


void lqDeleteDatabase2D (lqInternalDB2D*);


/* ------------------------------------------------------------------ */
/* The application needs to call this once on each lqClientProxy at
   setup time to initialize its list pointers and associate the proxy
   with its client object. */ 


void lqInitClientProxy2D (lqClientProxy2D* proxy, void* clientObject);


/* ------------------------------------------------------------------ */
/* Call for each client object every time its location changes.  For
   example, in an animation application, this would be called each
   frame for every moving object.  */


void lqUpdateForNewLocation (lqInternalDB2D* lq, 
			     lqClientProxy2D* object, 
			     double x, double y);


/* ------------------------------------------------------------------ */
/* Apply an application-specific function to all objects in a certain
   locality.  The locality is specified as a disk with a given
   center and radius.  All objects whose location (key-point) is
   within this disk are identified and the function is applied to
   them.  The application-supplied function takes three arguments:

     (1) a void* pointer to an lqClientProxy's "object".
     (2) the square of the distance from the center of the search
         locality disc (x,y) to object's key-point.
     (3) a void* pointer to the caller-supplied "client query state"
         object -- typically NULL, but can be used to store state
         between calls to the lqCallBackFunction.

   This routine uses the LQ database to quickly reject any objects in
   bins which do not overlap with the disk of interest.  Incremental
   calculation of index values is used to efficiently traverse the
   bins of interest. */


/* type for a pointer to a function used to map over client objects */
typedef void (* lqCallBackFunction2D)  (void* clientObject,
				      double distanceSquared,
				      void* clientQueryState);


void lqMapOverAllObjectsInLocality (lqInternalDB2D* lq, 
				    double x, double y,
				    double radius,
				    lqCallBackFunction2D func,
				    void* clientQueryState);

/* Given a bin's list of client proxies, traverse the list and invoke
   the given lqCallBackFunction on each object that falls within the
   search radius.  */
void lqTraverseBinClientObjectList(lqClientProxy2D* object, 
								   double x, double y, 
								   double radiusSquared,
								   lqCallBackFunction2D func, 
								   void* clientQueryState);


/* ------------------------------------------------------------------ */
/*                                                                    */
/*                            Other API                               */
/*                                                                    */
/* ------------------------------------------------------------------ */
/* Adds a given client object to a given bin, linking it into the bin
   contents list. */


void lqAddToBin (lqClientProxy2D* object, lqClientProxy2D** bin);


/* ------------------------------------------------------------------ */
/* Removes a given client object from its current bin, unlinking it
   from the bin contents list. */


void lqRemoveFromBin (lqClientProxy2D* object);


/* ------------------------------------------------------------------ */
/* Given an LQ database object and the nine basic parameters: fill in
   the object's slots, allocate the bin array, and initialize its
   contents.  Normally the application does NOT call this directly, it
   is called by lqCreateDatabase.  */


void lqInitDatabase2D (lqInternalDB2D* lq,
		     double originx, double originy,
		     double sizex, double sizey,
		     int divx, int divy);


/* ------------------------------------------------------------------ */
/* Find the bin ID for a location in space.  The location is given in
   terms of its XYZ coordinates.  The bin ID is a pointer to a pointer
   to the bin contents list.  */


lqClientProxy2D** lqBinForLocation2D (lqInternalDB2D* lq, double x, double y);

/* ------------------------------------------------------------------ */
/* Apply a user-supplied function to all objects in the database,
   regardless of locality (cf lqMapOverAllObjectsInLocality) */


void lqMapOverAllObjects (lqInternalDB2D* lq, 
			  lqCallBackFunction2D func,
			  void* clientQueryState);


/* ------------------------------------------------------------------ */
/* Removes (all proxies for) all objects from all bins */


void lqRemoveAllObjects (lqInternalDB2D* lq);
/* ------------------------------------------------------------------ */


/* Removes (all proxies for) all objects from a bin */

void lqRemoveAllObjectsInBin(lqClientProxy2D* bin);

/* ------------------------------------------------------------------ */

#endif /* _lq_h */
