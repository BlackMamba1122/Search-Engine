#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>

using namespace std;
struct TrieNode {
	unordered_map<char, TrieNode*> child;
	unordered_map<string, vector<int>> DocData;
};
class Tries
{
	TrieNode* root;
public:
	Tries();
	void insert(const std::string& word, const std::string& docname, int position);
	vector<pair<string, vector<int>>> search(const string& word);
	void clear(TrieNode* node);
	void dump(const string& filename);
	void load(const string& filename,ifstream& inFile);
	void serialize(TrieNode* node, ofstream& outFile);
	TrieNode* deserialize(ifstream& inFile);
	void remove(const string& filename);
	bool removeDoc(TrieNode* node, const string& filename);

};

