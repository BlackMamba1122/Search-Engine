#include "WordMap.h"

WordMap::WordMap(string _DocName, streampos _pos) : DocName(_DocName)
{
	pos.push_back(_pos);
}

void WordMap::addPos(streampos _pos)
{
	pos.push_back(_pos);
}

int WordMap::Total_Occurance()
{
	return pos.size();
}

string WordMap::get_DocName()
{
	return DocName;
}
void WordMap::addOccurrences(const std::vector<streampos>& _pos) 
{
	pos.insert(pos.end(), _pos.begin(), _pos.end());
}
