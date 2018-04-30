#pragma once
#include <string>
#include <vector>
using namespace std;

struct Parameter
{
	string Key;
	string Value;
};

class Parser
{

	Parser();
	~Parser(void);
	
	//vector<Parameter> getKeysAndValues();
	bool getStringValue(const string& key, string& value);
	bool getIntValue(const string& key, int& value);
	bool getDoubleValue(const string& key, double& value);
	bool getBoolValue(const string& key, bool& value);
	bool registerParameters(const string& fileName);

private:
	vector<Parameter> args;	

};