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


/*!
 *  @file       Proximity2D.h
 *  @brief      This is a modified version of opensteer's data structure for accelerating proximity queries. 
 */

#pragma once

#include <vector>
#include "lq2D.h"
#include "ProximityDatabaseItem.h" 
#include <Eigen/Dense>
using namespace Eigen;
using std::vector; 

// ----------------------------------------------------------------------------
// A ProximityDatabase-style wrapper for the LQ bin lattice system
class LQProximityDatabase2D
{
public:

    // constructor
    LQProximityDatabase2D (const Vector2D& center,
                            const Vector2D& dimensions,
                            const Vector2D& divisions)
    {
        const Vector2D halfsize (dimensions * 0.5);
        const Vector2D origin (center - halfsize);
		_origin = origin;
		_dimensions = dimensions;
		_divisions = divisions;

        lq = lqCreateDatabase2D (origin.x(), origin.y(),
                                dimensions.x(), dimensions.y(),  
								(int) floor(0.5 + divisions.x()),
								(int) floor(0.5 + divisions.y()));
    }

    // destructor
    virtual ~LQProximityDatabase2D ()
    {
        lqDeleteDatabase2D (lq);
        lq = NULL;
    }

    // "token" to represent objects stored in the database
    class tokenType
    {
    public:

        // constructor
		tokenType (ProximityDatabaseItem* parentObject, LQProximityDatabase2D& lqsd)
        {
            lqInitClientProxy2D (&proxy, parentObject);
            lq = lqsd.lq;
        }

        // destructor
        virtual ~tokenType (void)
        {
            lqRemoveFromBin (&proxy);
        }

        // the client object calls this each time its position changes
        void updateForNewPosition (const Vector2D& p)
        {
            lqUpdateForNewLocation (lq, &proxy, p.x(), p.y());
        }

        // find all neighbors within the given sphere (as center and radius)
        void findNeighbors (const Vector2D& center,
							const double radius,
                            vector<ProximityDatabaseItem*>& results 
							)
        {
                lqMapOverAllObjectsInLocality (lq, center.x(), center.y(), 	radius, 
					[](void* clientObject, double distanceSquared, void* clientQueryState) 
					{
						vector<ProximityDatabaseItem*>& results = *((vector<ProximityDatabaseItem*>*) clientQueryState);
						results.push_back((ProximityDatabaseItem*)clientObject); 
				    }, (void*)&results);
        }


    private:
        lqClientProxy2D proxy;
        lqInternalDB2D* lq;
    };

	// allocate a token to represent a given client object in this database
    tokenType* allocateToken (ProximityDatabaseItem* item)
    {
        return new tokenType (item, *this);
    }

 	
	Vector2D getOrigin (void) {return _origin;}
	Vector2D getDivisions (void) {return _divisions;}
	Vector2D getDimensions (void) {return _dimensions;}

private:
    lqInternalDB2D* lq;
	Vector2D _origin;
	Vector2D _divisions;
	Vector2D _dimensions;
};

/// The spatial proximity database
typedef LQProximityDatabase2D SpatialProximityDatabase;

/// An object in the proximity database 
typedef SpatialProximityDatabase::tokenType ProximityToken;

