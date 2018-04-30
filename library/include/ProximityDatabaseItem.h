/*
 *  ProximityDatabaseItem.h
 *  
 *  
 *  All rights are retained by the authors and the University of Minnesota.
 *  Please contact sjguy@cs.umn.edu for licensing inquiries.
 *  
 *  Authors: Ioannis Karamouzas, Brian Skinner, and Stephen J. Guy
 *  Contact: ioannis@cs.umn.edu
 */

/*!
 *  @file       ProximityDatabaseItem.h
 *  @brief      Declares the virtual interface inherited by the objects of the proximity database.
 */

#pragma once
#include "util/Definitions.h"

namespace TTC {

	/**
	 * @brief The virtual interface used by objects in the spatial database.
	 *
	 */
	class ProximityDatabaseItem {
	public:
		/// Destructor.
		virtual ~ProximityDatabaseItem() {}
		/// Returns true if the object is an agent, false if not.
		virtual bool isAgent() = 0;
		
	};

	/// Pointer to a ProximityDatabaseItem
	typedef ProximityDatabaseItem* ProximityDatabaseItemPtr;

} // end namespace 

