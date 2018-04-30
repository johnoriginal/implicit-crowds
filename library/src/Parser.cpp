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


#include "Parser.h"

Parser::Parser()
{
}

Parser::~Parser(void)
{
}

int Parser::findKey(const std::string& key) const
{
	int idx = -1;
	for (unsigned int i = 0; i < args.size(); ++i)
	{
		if (args[i].key == key)
		{
			idx = i;
			return idx;
		}
	}
	return idx;
}


bool Parser::getStringValue(const string& key, string& value) const
{
	int idx = findKey(key);
	if (idx < 0)
		return false;
	value = args[idx].value;
	return true;
}

bool Parser::getIntValue(const string& key, int& value) const
{
	int idx = findKey(key);
	if (idx < 0)
		return false;
	value = atoi(args[idx].value.c_str());
	return true;
}

bool Parser::getDoubleValue(const string& key, double& value) const
{
	int idx = findKey(key);
	if (idx < 0)
		return false;
	value = atof(args[idx].value.c_str());
	return true;
}

bool Parser::getBoolValue(const string& key, bool& value) const
{
	int idx = findKey(key);
	if (idx < 0)
		return false;
	string str = args[idx].value;
	value = str == "1" || str == "true";
	return true;
}


void Parser::trimspaces(std::string& value)
{
	// could also be done with lambdas in C++11
	char const* delims = " \t\r\n";
	size_t pos = value.find_first_not_of(delims);
	if (pos == string::npos)
	{
		value.erase();
		return;
	}
	value = value.substr(pos, value.size() - pos);
	value = value.substr(0, value.find_last_not_of(delims) + 1);
}


bool Parser::registerParameters(const string& fileName)
{
	string line;
	ifstream input(fileName);
	if (input.fail())
	{
		std::cerr << "File not found" << std::endl;
		return false;
	}

	//parse the file and read the values
	while (getline(input, line) && input.good())
	{
		//getRecord(line);
		trimspaces(line);
		if (!line.empty())
		{
			size_t pos = line.find('=');
			if (pos != string::npos)
			{
				Parameter par;
				par.key = line.substr(0, pos);
				par.value = line.substr(pos + 1);
				args.push_back(par);
			}
		}
	}
	input.close();
	return true;
}