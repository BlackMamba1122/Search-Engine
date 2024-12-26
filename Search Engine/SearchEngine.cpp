#include "SearchEngine.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include<map>
#include<set>

void SearchEngine::crawl(const string& path)
{
	for (int i = 1; i <= 3; i++)
	{
		string fileName = path + "review_" + to_string(i) + ".txt";
		search(fileName);
	}
}

void SearchEngine::search(const string& path)
{
	ifstream file(path);
	string word;
	while (file >> word)
		add(word,path, file.tellg() - static_cast<streamoff>(word.length()));
		//add(word,path,file.tellg());
	file.close();
}

void SearchEngine::add(const string& word, const string& docName, streampos pos)
{
    if (Hash.find(word) == Hash.end()) 
        Hash[word] = { WordMap(docName, pos) };
    else 
    {
        bool found = false;
        for (auto& _DocName : Hash[word]) {
            if (_DocName.get_DocName() == docName)
            {
                _DocName.addPos(pos);
                found = true;
                break;
            }
        }
        if (!found)
            Hash[word].push_back(WordMap(docName, pos));
    }
}
std::vector<WordMap> SearchEngine::searchWord(const string& word) {
    if (Hash.find(word) == Hash.end()) 
    {
        cout << "No results found for word: " << word << endl;
        //return;
    }
    auto& results = Hash[word];
    std::sort(results.begin(), results.end(), [](WordMap& a, WordMap& b) 
    {
        return a.Total_Occurance() > b.Total_Occurance();
    });
    return results;
    cout << "Results for word: " << word << endl;
    for (auto& wid : results)
        cout << "Document: " << wid.get_DocName() << " | Occurrences: " << wid.Total_Occurance() << endl;
}

void SearchEngine::run()
{
    crawl("");
    for (auto& i : Hash)
    {
        cout << i.first<<"  ";
        for (auto& j :i.second)
        {
            cout<<j.get_DocName() << "  " <<j.Total_Occurance() << "  ";
            for (auto& k : j.pos)
            {
                cout << k << "  ";
            }
        }
    cout << endl;
    }
}

void SearchEngine::processString(const string& input, vector<string>& include, vector<string>& exclude)
{
    stringstream ss(input);
    string token;
    bool isInclude = false;
    bool isExclude = false;
    bool last = true;
    while (ss >> token) {
        if (token[0] == '+') {
            last = true;
            isInclude = true;
            if (token.size() > 1) {
                include.push_back(token.substr(1));
                isInclude = false;
            }
            continue;
        }
        if (token[0] == '-') {
            last = false;
            isExclude = true;
            if (token.size() > 1) {
                isExclude = false;
                exclude.push_back(token.substr(1));
            }
            continue;
        }
        if (isInclude) {
            isInclude = false;
            include.push_back(token);
        }
        else if (isExclude) {
            isExclude = false;
            exclude.push_back(token);
        }
        else {
            if (last)
            {
                if (include.size() == 0)
                    include.push_back(token);
                else
                {
                    string temp = include[include.size() - 1];
                    include.pop_back();
                    temp = temp + " " + token;
                    include.push_back(temp);
                }
            }
            else
            {
                if (exclude.size() == 0)
                    exclude.push_back(token);
                else
                {
                    string temp = exclude[exclude.size() - 1];
                    exclude.pop_back();
                    temp = temp + " " + token;
                    exclude.push_back(temp);
                }
            }
        }
    }
}

void SearchEngine::input()
{
    crawl("");
    string input = "apple + hello + nigga";

    vector<string> include;
    vector<string> exclude;

    processString(input, include, exclude);
    searchMultipleWords(include);
    // Output the results
    cout << "\nIncluded words:" << endl;
    for (const auto& word : include) {
        cout << word << endl;
    }

    cout << "\nExcluded words:" << endl;
    for (const auto& word : exclude) {
        cout << word << endl;
    }
}

void SearchEngine::displayResult(std::vector < std::pair< int, WordMap > > res)
{
    for (auto& result : res) {
        std::cout << "Document: " << result.second.get_DocName()
            << ", Occurrences: " << result.second.Total_Occurance()
        << " Total" << result.first<<endl;
    }
}

void SearchEngine::searchMultipleWords(const std::vector<std::string>& words)
{
    std::vector < std::pair< int,WordMap > > combinedResults;
        for (const auto& word : words) {
            auto results = searchWord(word);
            combinedResults = combineResults(combinedResults, results);
        }
     displayResult(combinedResults);
}

std::vector<pair<int,WordMap>> SearchEngine::combineResults(std::vector<pair<int,WordMap>>& results1, std::vector<WordMap>& results2) {
    std::map<std::string, pair<int,WordMap>> combined;

    // Add results from the first vector
    for (auto& res : results1) {
        if (combined.find(res.second.get_DocName()) == combined.end()) {
            combined[res.second.get_DocName()].first = res.first;
            combined[res.second.get_DocName()].second = res.second; // Initialize new entry
        }
        else {
            combined[res.second.get_DocName()].first+=1;
            combined[res.second.get_DocName()].second.addOccurrences(res.second.pos);
        }
    }

    // Add results from the second vector
    for (auto& res : results2) {
        if (combined.find(res.get_DocName()) == combined.end()) {
            combined[res.get_DocName()].first = 1 ;
            combined[res.get_DocName()].second = res; // Initialize new entry
        }
        else {
            combined[res.get_DocName()].first += 1;
            combined[res.get_DocName()].second.addOccurrences(res.pos);
        }
    }

    // Extract WordMap objects into a vector
    std::vector<pair<int,WordMap>> result;
    for (auto& entry : combined) {
        result.push_back(entry.second);
    }
    std::sort(result.begin(), result.end(), []( std::pair<int, WordMap>& a, std::pair<int, WordMap>& b) {
        if (a.first != b.first) {
            return a.first > b.first; // Primary: Sort by count descending
        }
        return a.second.Total_Occurance() > b.second.Total_Occurance(); // Secondary: Sort by positions size descending
        });
    return result;
}



vector<WordMap> SearchEngine::excludeDocuments( std::vector<WordMap>& includeResults,  std::vector<WordMap>& excludeResults) {
    std::set<std::string> excludeSet;
    for ( auto& res : excludeResults) {
        excludeSet.insert(res.get_DocName());
    }
    std::vector<WordMap> filteredResults;
    for ( auto& res : includeResults) {
        if (excludeSet.find(res.get_DocName()) == excludeSet.end()) {
            filteredResults.push_back(res);
        }
    }
    return filteredResults;
}
