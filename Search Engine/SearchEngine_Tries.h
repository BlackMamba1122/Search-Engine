#pragma once
#include <unordered_map>
#include "Tries.h"

using namespace std;

class SearchEngine_Tries
{
	Tries trie;
	string path = "review/";
private:
	void crawl();
	void search(const string& path);
	vector<pair<string, vector<int>>> searchWord(const string& word);
	void processString(const string& input, vector<string>& include, vector<string>& exclude);
	void input();
	void displayResult(vector<vector<pair<string, vector<int>>>> res, vector<string> searchTexts);
	int Total_Occurance(vector<pair<string, vector<int>>> res);
	void searchMultipleWords(const std::vector<std::string>& words, vector<vector<pair<string, vector<int>>>>& combinedResults);
	vector<vector<pair<string, vector<int>>>> combineResults(vector<vector<pair<string, vector<int>>>>& results1, vector<pair<string, vector<int>>>& results2);
	vector<vector<pair<string, vector<int>>>> excludeDocuments(vector<vector<pair<string, vector<int>>>>& includeResults, vector<vector<pair<string, vector<int>>>>& excludeResults);
	void dumpSearchEngine(const std::string& filename= "nigga_Tries.txt");
	void loadSearchEngine(const std::string& filename="nigga_Tries.txt");
	std::string getSampleLine(const std::string& fileName, int position);
	void addFile();
	void removeFile();
public:
	void run();
};