
// ----------------------------------------------------------------------------
// Copyright (c) 2002-2003, Sony Computer Entertainment America
// Original author: Craig Reynolds <craig_reynolds@playstation.sony.com>
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
// ----------------------------------------------------------------------------

/*!
 *  @file       Proximity2D.h
 *  @brief      This is a modified version of opensteer's data structure for accelerating proximity queries. 
 */

#pragma once

#include <vector>
#include "lq2D.h"
#include "ProximityDatabaseItem.h" 


namespace TTC {

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
            const Vector2D halfsize (dimensions * 0.5f);
            const Vector2D origin (center - halfsize);
			_origin = origin;
			_dimensions = dimensions;
			_divisions = divisions;

            lq = lqCreateDatabase2D (origin.x, origin.y, 
                                   dimensions.x, dimensions.y,  
								   (int) floor(0.5f + divisions.x),
								   (int) floor(0.5f + divisions.y));
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
                lqUpdateForNewLocation (lq, &proxy, p.x, p.y);
            }

            // find all neighbors within the given sphere (as center and radius)
            void findNeighbors (const Vector2D& center,
								const Vector2D& facingDirection,
                                const float radius,
                                std::vector<ProximityDatabaseItem*>& results, 
								bool restrictedView=false)
            {
                lqMapOverAllObjectsInLocality (lq, 
                                               center.x, center.y,
											   facingDirection.x, facingDirection.y, 
                                               radius,
											   restrictedView, 
                                               perNeighborCallBackFunction,
                                               (void*)&results);
            }

            // called by LQ for each clientObject in the specified neighborhood:
            // push that clientObject onto the ContentType vector in void*
            // clientQueryState
            // (parameter names commented out to prevent compiler warning from "-W")
            static void perNeighborCallBackFunction  (void* clientObject,
                                                      float /*distanceSquared*/,
                                                      void* clientQueryState)
            {
                typedef std::vector<ProximityDatabaseItem*> ctv;
                ctv& results = *((ctv*) clientQueryState);
                results.push_back ((ProximityDatabaseItem*) clientObject);
            }

        private:
            lqClientProxy2D proxy;
            lqInternalDB2D* lq;
        };

	    // allocate a token to represent a given client object in this database
        tokenType* allocateToken (ProximityDatabaseItem* parentObject)
        {
            return new tokenType (parentObject, *this);
        }

        // count the number of tokens currently in the database
        int getPopulation (void)
        {
            int count = 0;
            lqMapOverAllObjects (lq, counterCallBackFunction, &count);
            return count;
        }
        
        // (parameter names commented out to prevent compiler warning from "-W")
        static void counterCallBackFunction  (void* /*clientObject*/,
                                              float /*distanceSquared*/,
                                              void* clientQueryState)
        {
            int& counter = *(int*)clientQueryState;
            counter++;
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

}
