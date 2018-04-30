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
 *  @file       ProximityDatabaseItem.h
 *  @brief      Declares the virtual interface inherited by the objects of the proximity database.
 */

#pragma once
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



