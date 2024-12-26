#pragma once
#include "vector"
#include "string"

using namespace std;

class WordMap
{
	string DocName;
public:
	vector<streampos> pos;
	WordMap() {}
	WordMap(string _DocName,streampos _pos);
	void addPos(streampos _pos);
	int Total_Occurance();
	string get_DocName();
	void addOccurrences(const std::vector<streampos>& _pos);
};

