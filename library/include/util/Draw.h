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

#pragma once
	class Color {
	public:
		float r, g, b;
		/// Default constructor initializes to blue.
		Color() : r(.0f), g(.0f), b(1.0f) { }
		/// Initializes to a user-defined color
		Color( float newr, float newg, float newb ) : r(newr), g(newg), b(newb) { }
		Color(const Color& col) : r(col.r), g(col.g), b(col.b) { }
	};

	static const Color gBlack(0.0f, 0.0f, 0.0f);
	static const Color gWhite(1.0f, 1.0f, 1.0f);
	static const Color gRed(1.0f, 0.0f, 0.0f); 
	static const Color gGreen(0.0f, 1.0f, 0.0f);
	static const Color gBlue(0.0f, 0.55f, 1.0f);
	static const Color gYellow(1.0f, 1.0f, 0.0f);
	static const Color gCyan(0.0f, 1.0f, 1.0f);
	static const Color gMagenta(1.0f, 0.0f, 1.0f);
	static const Color gOrange(1.0f, 0.5f, 0.0f);
	
	static Color groupColors [] =
	{
		gRed, gGreen, gBlue, gYellow,gCyan,gMagenta, gOrange
	};
	
