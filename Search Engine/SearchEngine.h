#pragma once
#include <unordered_map>
#include "WordMap.h"
using namespace std;

class SearchEngine
{
	unordered_map<string, vector<WordMap>> Hash;
public:
	void crawl(const string& path);
	void search(const string& phath);
	void add(const string& word, const string& docName, streampos pos);
	std::vector<WordMap> searchWord(const string& word);
	void run();
	void processString(const string& input, vector<string>& include, vector<string>& exclude);
	void input();
	void displayResult(std::vector < std::pair< int, WordMap > > res);
	void searchMultipleWords(const std::vector<std::string>& words);
	std::vector<pair<int, WordMap>> combineResults( std::vector < pair<int, WordMap>>& results1,  std::vector<WordMap>& results2);
	std::vector<WordMap> excludeDocuments( std::vector<WordMap>& includeResults,  std::vector<WordMap>& excludeResults);
};

