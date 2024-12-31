#include "SearchEngine_Tries.h"
#include "utility.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include<map>
#include<set>
#include<conio.h>
#include<Windows.h>

using namespace std;
void SearchEngine_Tries::crawl()
{
    SetClr(YELLOW, BLACK);
    system("cls");
    cout << "\n\n\n\n";
    Title();
    hideCursor();
    SetConsoleCP(437);
    SetConsoleOutputCP(437);
    int barLength = 40;
    int bar1 = 177, bar2 = 219;
    cout << "\n\n\n\t\t\t\tLoading...";
    cout << "\n\n\n\t\t\t\t";
    for (int i = 0; i < barLength; i++)
        cout << (char)bar1;
    cout << "\r";
    cout << "\t\t\t\t";
    int t = 0;
    ///////////////////////////////////////////////
    int filesTotal = 5000;
    for (int i = 1; i <= filesTotal; i++)
    {
        string fileName = path + "review_" + to_string(i) + ".txt";
        search(fileName);
        if (t < ((barLength * i) / filesTotal))
        {
            t++;
            cout << (char)bar2;
        }
    }
    ///////////////////////////////////////////////////
    cout << "\n\t\t\t\t" << (char)1 << "!\n\t\t\t\t";
    showCursor();
    system("Pause");
}

void SearchEngine_Tries::search(const string& path) {
std::ifstream file(path);
if (!file.is_open()) {
    //std::cerr << "Error: Could not open file " << path << std::endl;
    return;
}

std::string line;
int currentLineStart = 0;

while (std::getline(file, line)) {
    std::istringstream lineStream(line);
    std::string word;

    while (lineStream >> word) {
        trie.insert(word, path, currentLineStart);
        currentLineStart += 1;
    }
}
    file.close();
}

vector<pair<string, vector<int>>> SearchEngine_Tries::searchWord(const string& word)
{
    return trie.search(word);
}


void SearchEngine_Tries::processString(const string& input, vector<string>& include, vector<string>& exclude)
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

void SearchEngine_Tries::input()
{
    string input;
    //input = "apple + hello world + nigga";
    //cin.ignore();
    cout << "Search:       ";
    getline(cin, input);
    if (input == "~N" || input == "~n")
    {
        addFile();
        return;
    }
    if (input == "~R" || input == "~r")
    {
        removeFile();
        return;
    }
    vector<string> include;
    vector<string> exclude;

    processString(input, include, exclude);
    vector<vector<pair<string, vector<int>>>> combinedResults;
    vector<vector<pair<string,vector<int>>>> combinedResults2;
    searchMultipleWords(include, combinedResults);
    if (!exclude.empty()) {
        searchMultipleWords(exclude, combinedResults2);
        combinedResults = excludeDocuments(combinedResults, combinedResults2);
    }
    displayResult(combinedResults, include);
}

void SearchEngine_Tries::displayResult(vector<vector<pair<string, vector<int>>>> res, vector<string> searchTexts)
{
    if (res.empty()) {
        std::cout << "No results found." << std::endl;
        return;
    }
    SetClr(BLACK, WHITE);
    std::cout << "\nSearch Results\n";
    cout << res.size() << " found\n";
    SetClr(WHITE, BLACK);
    std::cout << std::string(50, '=') << "\n";

    for (auto& result : res) {
        string docName = result[0].first;
        const int totalOccurrences = Total_Occurance(result);
        const int relevanceScore = result.size();

        SetClr(RED, BLACK);
        std::cout << "Document: " << docName << "\n";

        SetClr(GREEN, BLACK);
        std::cout << "  Relevance Score: " << relevanceScore << "\n";
        std::cout << "       Total Occurrences: " << totalOccurrences << "\n";
        SetClr(WHITE, BLACK);
        std::cout << "  Sample Data:\n";
        for (auto& word : result)
        {
            for (const auto& position : word.second) {
                std::string sampleLine = getSampleLine(docName, position);
                std::cout << "    ...";
                highlightWord(sampleLine, searchTexts);
                std::cout << "...\n";
            }
        }

        SetClr(YELLOW, BLACK);
        std::cout << std::string(50, '-') << "\n";
    }
}

int SearchEngine_Tries::Total_Occurance(vector<pair<string, vector<int>>> res)
{
    int count = 0;
    for (auto& i : res)
        count += i.second.size();
    return count;
}

void SearchEngine_Tries::searchMultipleWords(const std::vector<std::string>& words, vector<vector<pair<string, vector<int>>>>& combinedResults)
{
    for (const auto& word : words)
    {
        auto results = searchWord(word);
        combinedResults = combineResults(combinedResults, results);
    }
}

vector<vector<pair<string, vector<int>>>> SearchEngine_Tries::combineResults(vector<vector<pair<string, vector<int>>>>& results1, vector<pair<string, vector<int>>>& results2)
{
    map<string, vector<pair<string, vector<int>>>> temp;
    for (auto i : results1) {
        for (auto j : i) {
            temp[j.first].push_back(j);
        }
    }
    for (auto i : results2) {
        temp[i.first].push_back(i);
    }
    vector<vector<pair<string, vector<int>>>> result;
    for (auto i : temp) {
        result.push_back(i.second);
    }
    std::sort(result.begin(), result.end(), [](vector<pair<string, vector<int>>>& a, vector<pair<string, vector<int>>>& b) {
        auto totalOccurrencesA = [](vector<pair<string, vector<int>>>& vec) {
            int total = 0;
            for (auto& wm : vec) {
                total += wm.second.size();
            }
            return total;
            };

        auto totalOccurrencesB = [](vector<pair<string, vector<int>>>& vec) {
            int total = 0;
            for (auto& wm : vec) {
                total += wm.second.size();
            }
            return total;
            };

        if (a.size() != b.size()) {
            return a.size() > b.size();
        }

        return totalOccurrencesA(a) > totalOccurrencesB(b);
        });

    return result;
}

vector<vector<pair<string, vector<int>>>> SearchEngine_Tries::excludeDocuments(vector<vector<pair<string, vector<int>>>>& includeResults, vector<vector<pair<string, vector<int>>>>& excludeResults)
{
    std::set<std::string> excludeSet;
    for (auto& res : excludeResults) {
        excludeSet.insert(res[0].first);
    }
    vector<vector<pair<string, vector<int>>>> filteredResults;
    for (auto& res : includeResults) {
        if (excludeSet.find(res[0].first) == excludeSet.end()) {
            filteredResults.push_back(res);
        }
    }
    return filteredResults;
}

void SearchEngine_Tries::dumpSearchEngine(const std::string& filename)
{
    trie.dump(filename);
}

void SearchEngine_Tries::loadSearchEngine(const std::string& filename)
{
    ifstream inFile(filename, ios::binary);
    if (!inFile.is_open()) {
        //cerr << "Error: Could not open file for loading: " << filename << endl;
        crawl();
        dumpSearchEngine(filename);
        return;
    }
    trie.load(filename,inFile);

}

std::string SearchEngine_Tries::getSampleLine(const std::string& fileName, int position)
{
    std::ifstream file(fileName);
    if (!file.is_open()) {
        //std::cerr << "Error: Unable to open file: " << fileName << std::endl;
        return "";
    }

    int currentWordIndex = 0;
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        std::string word;
        string s1;
        string s2;
        int localOccurrence = 0;
        bool ret = false;
        while (lineStream >> word) {
            if (currentWordIndex + localOccurrence >= position) {
                ret = true;
                s2 = s2 + word + " ";
            }
            localOccurrence++;
            if (!ret)
                s1 = s1 + word + " ";
        }
        currentWordIndex += localOccurrence;
        if (ret)
        {
            s1 = s1.size() > 10 ? s1.substr(s1.size() - 10) : s1;

            s2 = s2.size() > 20 ? s2.substr(0, 30) : s2;
            return s1 + s2;
        }
    }
    return "";
}

void SearchEngine_Tries::addFile()
{
    std::string filename;
    std::cout << "Enter the filename: ";
    std::cin >> filename;
    filename = path + filename + ".txt";
    std::ofstream file(filename);

    if (!file) {
        //std::cerr << "Error opening file!" << std::endl;
        return;
    }

    std::cout << "Start typing (Press ESC to save and exit):\n";


    char ch;
    while (true) {
        ch = _getch();

        if (ch == 27) {  // ESC key
            break;
        }

        file.put(ch);
        if (ch == '\r')
            cout << "\n";
        std::cout << ch;
    }

    file.close();
    //    std::cout << "\nFile saved successfully!" << std::endl;
    search(filename);
    dumpSearchEngine();
}

void SearchEngine_Tries::removeFile()
{
    std::string filename;
    std::cout << "Enter the filename: ";
    std::cin >> filename;
    filename = path + filename + ".txt";
    trie.remove(filename);
    //for (auto it = Hash.begin(); it != Hash.end();)
    //{
    //    auto docIt = it->second.find(filename);

    //    if (docIt != it->second.end())
    //        it->second.erase(docIt);

    //    if (it->second.empty())
    //        it = Hash.erase(it);
    //    else
    //        ++it;
    //}
    dumpSearchEngine();
}

void SearchEngine_Tries::run()
{
    //    setConsoleWindowSize(100, 50);
    loadSearchEngine();
    while (1)
    {
        system("cls");
        Title();
        cout << "Write ~N for New File\t\t~R for Remove File\n";
        if (_kbhit()) {
            char key = _getch();
            if (key == 27)
                break;
        }
        input();
        system("Pause");
    }
}