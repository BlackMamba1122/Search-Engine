#pragma once
#include <unordered_map>
#include "WordMap.h"
using namespace std;

class SearchEngine
{
	unordered_map<string, unordered_map<string, WordMap>> Hash;
	string path = "review/";
private:
	void crawl();
	void search(const string& path);
	void add(const string& word, const string& docName, streampos pos);
	std::vector<WordMap> searchWord(const string& word);
	void processString(const string& input, vector<string>& include, vector<string>& exclude);
	void input();
	void displayResult(std::vector <std::vector< WordMap>> res, vector<string> searchTexts);
	int Total_Occurance(vector<WordMap>res);
	void searchMultipleWords(const std::vector<std::string>& words, std::vector < std::vector< WordMap > >& combinedResults);
	std::vector<vector<WordMap>> combineResults(std::vector <vector<WordMap>>& results1, std::vector<WordMap>& results2);
	std::vector<vector< WordMap>> excludeDocuments(std::vector < std::vector< WordMap >>& includeResults, std::vector <vector<WordMap >>& excludeResults);
	void dumpSearchEngine(const std::string& filename= "nigga_Hash.bin");
	void loadSearchEngine(const std::string& filename="nigga_Hash.bin");
	std::string getSampleLine(const std::string& fileName, std::streampos position);
	void addFile();
	void removeFile();
public:
	void run();

};