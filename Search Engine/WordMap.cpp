#include "WordMap.h"

WordMap::WordMap(string _DocName, streampos _pos) : DocName(_DocName)
{
	pos.push_back(_pos);
}

WordMap::WordMap(string _DocName, vector<streampos> _pos) : DocName(_DocName)
{
	pos = _pos;
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

void WordMap::serialize(std::ostream& os) const
{
	size_t docNameSize = DocName.size();
	os.write(reinterpret_cast<const char*>(&docNameSize), sizeof(docNameSize));
	os.write(DocName.data(), docNameSize);

	size_t posSize = pos.size();
	os.write(reinterpret_cast<const char*>(&posSize), sizeof(posSize));
	os.write(reinterpret_cast<const char*>(pos.data()), posSize * sizeof(std::streampos));
}

void WordMap::deserialize(std::istream& is)
{
	size_t docNameSize;
	is.read(reinterpret_cast<char*>(&docNameSize), sizeof(docNameSize));
	DocName.resize(docNameSize);
	is.read(&DocName[0], docNameSize);

	size_t posSize;
	is.read(reinterpret_cast<char*>(&posSize), sizeof(posSize));
	pos.resize(posSize);
	is.read(reinterpret_cast<char*>(pos.data()), posSize * sizeof(std::streampos));
}
