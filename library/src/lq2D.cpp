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

/* ------------------------------------------------------------------ */
/*                                                                    */
/* Locality Query facility                                            */
/*                                                                    */
/* (by Craig Reynolds, see lq.h file for documentation)               */
/*                                                                    */
/*  5-17-99: created                                                  */
/*  5-20-99: found elusive "allocate 0 bins" bug                      */
/*  5-28-99: lqMapOverAllObjectsInLocality: clipped, incremental      */
/*  6- 7-99: clean up, split off annotation stuff into debuglq.c      */
/*  6- 8-99: tried screening by sum of coords ("first mean"?) but     */
/*           it was slightly slower, moved unused code to debuglq     */
/* 10-19-99: Change lqClientObject, lqObject from: "struct x {};" to  */
/*           "typedef struct x {} x;" for EE compiler.                */
/* 12- 2-00: Make lqObject "private" using lqInternalDB.              */
/* 12- 5-00: Rename lqObject to lqDB, lqClientObject to lqClientProxy */
/* 12- 6-00: Change lqCallBackFunction from arglist of (void*) to:    */
/*           (void* clientObject, double distanceSquared, void*        */
/*           clientQueryState).  Add void* clientQueryState arg to    */
/*           lqMapOverAllObjectsInLocality and its helper functions   */
/*           lqMapOverAllObjectsInLocalityClipped and                 */
/*           lqMapOverAllOutsideObjects. Change macro                 */
/*           lqTraverseBinClientObjectList to invoke callback         */
/*           function with three arguments, add "state" to its        */
/*           arglist.  Remove extern lqDistanceSquared.               */
/* 12- 7-00: Rename lqInitClientObject to lqInitClientProxy, make     */
/*           "func" be an argument to lqTraverseBinClientObjectList,  */
/*           add comments.                                            */
/* 12- 8-00: Add lqFindNearestNeighborWithinRadius and related        */
/*           definitions: lqFindNearestHelper lqFindNearestState      */
/*           Add lqMapOverAllObjects and lqRemoveAllObjects (plus:    */
/*           lqMapOverAllObjectsInBin and lqRemoveAllObjectsInBin)    */
/*                                                                    */

/* (by Ioannis Karamouzas)               */
/*  5- 30-17: 2D version of the code, cleanup, convert from Cto C++   */ 
/* ------------------------------------------------------------------ */

#include <stdlib.h>
#include <float.h>
#include "proximitydatabase/lq2D.h"

/* ------------------------------------------------------------------ */
/* Allocate and initialize an LQ database, return a pointer to it.
The application needs to call this before using the LQ facility.
The nine parameters define the properties of the "super-brick":
(1) origin: coordinates of one corner of the super-brick, its
minimum x, y and z extent.
(2) size: the width, height and depth of the super-brick.
(3) the number of subdivisions (sub-bricks) along each axis.
This routine also allocates the bin array, and initialize its
contents. */


lqInternalDB2D* lqCreateDatabase2D(double originx, double originy,
	double sizex, double sizey,
	int   divx, int   divy)
{
	lqInternalDB2D* lq = new lqInternalDB2D();

	lqInitDatabase2D(lq,
		originx, originy,
		sizex, sizey,
		divx, divy);
	return lq;
}


/* ------------------------------------------------------------------ */
/* Deallocate the memory used by the LQ database */


void lqDeleteDatabase2D(lqInternalDB2D* lq)
{
	int bincount = lq->divx * lq->divy;
	for (int i = 0; i < bincount; ++i)
		if (lq->bins[i] != NULL)
			delete lq->bins[i];
	if (lq != NULL)
		delete lq;
}


/* ------------------------------------------------------------------ */
/* Given an LQ database object and the nine basic parameters: fill in
the object's slots, allocate the bin array, and initialize its
contents. */


void lqInitDatabase2D(lqInternalDB2D* lq,
	double originx, double originy,
	double sizex, double sizey,
	int divx, int divy)
{
	lq->originx = originx;
	lq->originy = originy;
	lq->sizex = sizex;
	lq->sizey = sizey;
	lq->divx = divx;
	lq->divy = divy;

	int bincount = divx * divy;
	lq->bins = new lqClientProxy2D*[bincount];
	for (int i = 0; i < bincount; i++)
		lq->bins[i] = NULL;
	lq->other = NULL;
}


/* ------------------------------------------------------------------ */
/* Find the bin ID for a location in space.  The location is given in
terms of its XY coordinates.  The bin ID is a pointer to a pointer
to the bin contents list.  */


lqClientProxy2D** lqBinForLocation2D(lqInternalDB2D* lq,
	double x, double y)
{
	int i, ix, iy;

	/* if point outside super-brick, return the "other" bin */
	if (x < lq->originx)              return &(lq->other);
	if (y < lq->originy)              return &(lq->other);
	if (x >= lq->originx + lq->sizex) return &(lq->other);
	if (y >= lq->originy + lq->sizey) return &(lq->other);

	/* if point inside super-brick, compute the bin coordinates */
	ix = (int)(((x - lq->originx) / lq->sizex) * lq->divx);
	iy = (int)(((y - lq->originy) / lq->sizey) * lq->divy);

	/* convert to linear bin number */
	i = ix * (lq)->divy + iy;

	/* return pointer to that bin */
	return &(lq->bins[i]);
}


/* ------------------------------------------------------------------ */
/* The application needs to call this once on each lqClientProxy at
setup time to initialize its list pointers and associate the proxy
with its client object. */


void lqInitClientProxy2D(lqClientProxy2D* proxy, void* clientObject)
{
	proxy->prev = NULL;
	proxy->next = NULL;
	proxy->bin = NULL;
	proxy->object = clientObject;
}


/* ------------------------------------------------------------------ */
/* Adds a given client object to a given bin, linking it into the bin
contents list. */


void lqAddToBin(lqClientProxy2D* object, lqClientProxy2D** bin)
{
	/* if bin is currently empty */
	if (*bin == NULL)
	{
		object->prev = NULL;
		object->next = NULL;
		*bin = object;
	}
	else
	{
		object->prev = NULL;
		object->next = *bin;
		(*bin)->prev = object;
		*bin = object;
	}

	/* record bin ID in proxy object */
	object->bin = bin;
}


/* ------------------------------------------------------------------ */
/* Removes a given client object from its current bin, unlinking it
from the bin contents list. */


void lqRemoveFromBin(lqClientProxy2D* object)
{
	/* adjust pointers if object is currently in a bin */
	if (object->bin != NULL)
	{
		/* If this object is at the head of the list, move the bin
		pointer to the next item in the list (might be NULL). */
		if (*(object->bin) == object) *(object->bin) = object->next;

		/* If there is a prev object, link its "next" pointer to the
		object after this one. */
		if (object->prev != NULL) object->prev->next = object->next;

		/* If there is a next object, link its "prev" pointer to the
		object before this one. */
		if (object->next != NULL) object->next->prev = object->prev;
	}

	/* Null out prev, next and bin pointers of this object. */
	object->prev = NULL;
	object->next = NULL;
	object->bin = NULL;
}


/* ------------------------------------------------------------------ */
/* Call for each client object every time its location changes.  For
example, in an animation application, this would be called each
frame for every moving object.  */


void lqUpdateForNewLocation(lqInternalDB2D* lq,
	lqClientProxy2D* object,
	double x, double y)
{
	/* find bin for new location */
	lqClientProxy2D** newBin = lqBinForLocation2D(lq, x, y);

	/* store location in client object, for future reference */
	object->x = x;
	object->y = y;

	/* has object moved into a new bin? */
	if (newBin != object->bin)
	{
		lqRemoveFromBin(object);
		lqAddToBin(object, newBin);
	}
}


/* ------------------------------------------------------------------ */
/* Given a bin's list of client proxies, traverse the list and invoke
the given lqCallBackFunction on each object that falls within the
search radius.  */

void lqTraverseBinClientObjectList(lqClientProxy2D* co, double x, double y, double radiusSquared,
	lqCallBackFunction2D func, void* state)
{
	while (co != NULL)
	{
		double dx = co->x - x;
		double dy = co->y - y;
		double distanceSquared = (dx*dx) + (dy*dy);
		if (distanceSquared < radiusSquared)
		{
			(*func) (co->object, distanceSquared, state);

		}
		/* consider next client object in bin list */
		co = co->next;
	}
}

/* ------------------------------------------------------------------ */
/* This subroutine of lqMapOverAllObjectsInLocality efficiently
traverses of subset of bins specified by max and min bin
coordinates. */


void lqMapOverAllObjectsInLocalityClipped(lqInternalDB2D* lq,
	double x, double y,
	double radius,
	lqCallBackFunction2D func,
	void* clientQueryState,
	int minBinX,
	int minBinY,
	int maxBinX,
	int maxBinY)
{
	int i, j;
	int iindex, jindex;
	int slab = lq->divy;
	int istart = minBinX * slab;
	int jstart = minBinY;
	lqClientProxy2D* co;
	lqClientProxy2D** bin;
	double radiusSquared = radius * radius;

	/* loop for x bins across diameter of disk */
	iindex = istart;
	for (i = minBinX; i <= maxBinX; i++)
	{
		/* loop for y bins across diameter of disk */
		jindex = jstart;
		for (j = minBinY; j <= maxBinY; j++)
		{
			/* get current bin's client object list */
			bin = &lq->bins[iindex + jindex];
			co = *bin;

			/* traverse current bin's client object list */
			lqTraverseBinClientObjectList(co,
				x, y,
				radiusSquared,
				func,
				clientQueryState);
			jindex += 1;
		}
		iindex += slab;
	}
}


/* ------------------------------------------------------------------ */
/* If the query region extends outside of the "super-brick"
we need to check for objects in the catch-all "other" bin which
holds any object which are not inside the regular sub-bricks  */


void lqMapOverAllOutsideObjects(lqInternalDB2D* lq,
	double x, double y,
	double radius,
	lqCallBackFunction2D func,
	void* clientQueryState)
{
	lqClientProxy2D* co = lq->other;
	double radiusSquared = radius * radius;

	/* traverse the "other" bin's client object list */
	lqTraverseBinClientObjectList(co,
		x, y,
		radiusSquared,
		func,
		clientQueryState);
}


/* ------------------------------------------------------------------ */
/* Apply an application-specific function to all objects in a certain
locality.  The locality is specified as a disk with a given
center and radius.  All objects whose location (key-point) is
within this disk are identified and the function is applied to
them.  The application-supplied function takes three arguments:

(1) a void* pointer to an lqClientProxy's "object".
(2) the square of the distance from the center of the search
locality disk (x,y) to object's key-point.
(3) a void* pointer to the caller-supplied "client query state"
object -- typically NULL, but can be used to store state
between calls to the lqCallBackFunction.

This routine uses the LQ database to quickly reject any objects in
bins which do not overlap with the disk of interest.  Incremental
calculation of index values is used to efficiently traverse the
bins of interest. */


void lqMapOverAllObjectsInLocality(lqInternalDB2D* lq,
	double x, double y,
	double radius,
	lqCallBackFunction2D func,
	void* clientQueryState)
{
	int partlyOut = 0;
	int completelyOutside =
		(((x + radius) < lq->originx) ||
		((y + radius) < lq->originy) ||
			((x - radius) >= lq->originx + lq->sizex) ||
			((y - radius) >= lq->originy + lq->sizey));
	int minBinX, minBinY, maxBinX, maxBinY;

	/* is the disk completely outside the "super brick"? */
	if (completelyOutside)
	{
		lqMapOverAllOutsideObjects(lq, x, y, radius, func,
			clientQueryState);
		return;
	}

	/* compute min and max bin coordinates for each dimension */
	minBinX = (int)((((x - radius) - lq->originx) / lq->sizex) * lq->divx);
	minBinY = (int)((((y - radius) - lq->originy) / lq->sizey) * lq->divy);
	maxBinX = (int)((((x + radius) - lq->originx) / lq->sizex) * lq->divx);
	maxBinY = (int)((((y + radius) - lq->originy) / lq->sizey) * lq->divy);

	/* clip bin coordinates */
	if (minBinX < 0) { partlyOut = 1; minBinX = 0; }
	if (minBinY < 0) { partlyOut = 1; minBinY = 0; }
	if (maxBinX >= lq->divx) { partlyOut = 1; maxBinX = lq->divx - 1; }
	if (maxBinY >= lq->divy) { partlyOut = 1; maxBinY = lq->divy - 1; }

	/* map function over outside objects if necessary (if clipped) */
	if (partlyOut)
		lqMapOverAllOutsideObjects(lq, x, y, radius, func,
			clientQueryState);

	/* map function over objects in bins */
	lqMapOverAllObjectsInLocalityClipped(lq,
		x, y,
		radius,
		func,
		clientQueryState,
		minBinX, minBinY,
		maxBinX, maxBinY);
}


/* ------------------------------------------------------------------ */
/* internal helper function */


typedef struct lqFindNearestState
{
	void* ignoreObject;
	void* nearestObject;
	double minDistanceSquared;

} lqFindNearestState;


void lqFindNearestHelper(void* clientObject,
	double distanceSquared,
	void* clientQueryState)
{
	lqFindNearestState* fns = (lqFindNearestState*)clientQueryState;

	/* do nothing if this is the "ignoreObject" */
	if (fns->ignoreObject != clientObject)
	{
		/* record this object if it is the nearest one so far */
		if (fns->minDistanceSquared > distanceSquared)
		{
			fns->nearestObject = clientObject;
			fns->minDistanceSquared = distanceSquared;
		}
	}
}


/* ------------------------------------------------------------------ */
/* internal helper function */


void lqMapOverAllObjectsInBin(lqClientProxy2D* binProxyList,
	lqCallBackFunction2D func,
	void* clientQueryState)
{
	/* walk down proxy list, applying call-back function to each one */
	while (binProxyList != NULL)
	{
		(*func) (binProxyList->object, 0, clientQueryState);
		binProxyList = binProxyList->next;
	}
}


/* ------------------------------------------------------------------ */
/* Apply a user-supplied function to all objects in the database,
regardless of locality (cf lqMapOverAllObjectsInLocality) */

void lqMapOverAllObjects(lqInternalDB2D* lq,
	lqCallBackFunction2D func,
	void* clientQueryState)
{
	int i;
	int bincount = lq->divx * lq->divy;
	for (i = 0; i<bincount; i++)
	{
		lqMapOverAllObjectsInBin(lq->bins[i], func, clientQueryState);
	}
	lqMapOverAllObjectsInBin(lq->other, func, clientQueryState);
}


/* ------------------------------------------------------------------ */
/* internal helper function */

void lqRemoveAllObjectsInBin(lqClientProxy2D* bin) {
	while ((bin) != NULL) lqRemoveFromBin((bin));
}
    


/* ------------------------------------------------------------------ */
/* Removes (all proxies for) all objects from all bins */


void lqRemoveAllObjects(lqInternalDB2D* lq)
{
	int i;
	int bincount = lq->divx * lq->divy;
	for (i = 0; i<bincount; i++)
	{
		lqRemoveAllObjectsInBin(lq->bins[i]);
	}
	lqRemoveAllObjectsInBin(lq->other);
}


/* ------------------------------------------------------------------ */
