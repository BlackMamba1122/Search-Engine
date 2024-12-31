#include "SearchEngine.h"
#include "utility.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include<map>
#include<set>
#include<conio.h>
#include<Windows.h>

void SearchEngine::crawl()
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
        if (t<((barLength*i)/filesTotal))
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

void SearchEngine::search(const string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        //std::cerr << "Error: Could not open file " << path << std::endl;
        return;
    }

    std::string line;
    std::streampos currentLineStart = 0;

    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        std::string word;

        while (lineStream >> word) {
            add(word, path, currentLineStart);
            currentLineStart += 1;
        }
    }
    file.close();
}

void SearchEngine::add(const string& word, const string& docName, streampos pos) {
    auto& docMap = Hash[word];
    if (docMap.find(docName) == docMap.end()) {
        docMap.emplace(docName, WordMap(docName, pos));
    }
    else {
        docMap[docName].addPos(pos);
    }

}

std::vector<WordMap> SearchEngine::searchWord(const std::string& word) {
    std::vector<WordMap> results;

    if (isSingleWord(word)) {
        if (Hash.find(word) == Hash.end()) {
            return results;
        }

        for (auto& entry : Hash[word]) {
            results.push_back(entry.second);
        }

        return results;
    }
    else {
        std::vector<WordMap> result2;
        std::stringstream ss(word);
        std::string token;
        int wordCount = -2;
        while (ss >> token) {
            wordCount += 1;
            if (Hash.find(token) == Hash.end()) {
                results.clear();
                break;
            }

            if (results.empty()) {
                for (auto& entry : Hash[token]) {
                    results.push_back(entry.second);
                }
                continue;
            }
            else {
                result2.clear();
                for (auto& entry : Hash[token]) {
                    result2.push_back(entry.second);
                }

                std::vector<WordMap> filtered;
                for (auto& res1 : results) {
                    for (auto& res2 : result2) {
                        if (res1.get_DocName() == res2.get_DocName()) {
                            std::vector<streampos> validPositions;

                            for (auto& pos1 : res1.pos) {
                                int nextPos = static_cast<int>(pos1)+ 1+wordCount;
                                if (std::find(res2.pos.begin(), res2.pos.end(), nextPos) != res2.pos.end()) {
                                    validPositions.push_back(pos1);
                                }
                            }

                            if (!validPositions.empty()) {
                                filtered.emplace_back(res1.get_DocName(), validPositions);
                            }
                        }
                    }
                }
                results = filtered; 
            }
        }
        return results;
    }
}


void SearchEngine::run()
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
    std::vector < std::vector< WordMap >> combinedResults;
    std::vector < std::vector< WordMap >> combinedResults2;
    searchMultipleWords(include, combinedResults);
    if (!exclude.empty()){
        searchMultipleWords(exclude, combinedResults2);
    combinedResults = excludeDocuments(combinedResults, combinedResults2);
    }
    displayResult(combinedResults,include);
}

void SearchEngine::displayResult(std::vector < std::vector<WordMap >> res, vector<string> searchTexts)
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
        string docName = result[0].get_DocName();
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
            for (const auto& position : word.pos) {
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

int SearchEngine::Total_Occurance(vector<WordMap> res)
{
    int count = 0;
    for (auto& i : res)
        count += i.Total_Occurance();
    return count;
}

void SearchEngine::searchMultipleWords(const std::vector<std::string>& words, std::vector<std::vector<WordMap>>& combinedResults)
{
        for (const auto& word : words) 
        {
            auto results = searchWord(word);
            combinedResults = combineResults(combinedResults, results);
        }
}



std::vector<vector<WordMap>> SearchEngine::combineResults(std::vector<vector<WordMap>>& results1, std::vector<WordMap>& results2)
{
    std::map<std::string, vector<WordMap>> combined;

    for (auto& res : results1) {
        for (auto& res1 : res) {
           combined[res1.get_DocName()].push_back(res1);
        }
    }

    for (auto& res : results2) {
       combined[res.get_DocName()].push_back(res);
    }

    std::vector<vector<WordMap>> result;
    for (auto& entry : combined) {
        result.push_back(entry.second);
    }

    std::sort(result.begin(), result.end(), [](std::vector<WordMap>& a, std::vector<WordMap>& b) {
        auto totalOccurrencesA = [](std::vector<WordMap>& vec) {
            int total = 0;
            for (auto& wm : vec) {
                total += wm.Total_Occurance();
            }
            return total;
            };

        auto totalOccurrencesB = [](std::vector<WordMap>& vec) {
            int total = 0;
            for (auto& wm : vec) {
                total += wm.Total_Occurance();
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


vector<vector<WordMap>> SearchEngine::excludeDocuments( std::vector < std::vector<WordMap >>& includeResults,  std::vector < std::vector< WordMap >>& excludeResults) {
    std::set<std::string> excludeSet;
    for ( auto& res : excludeResults) {
        excludeSet.insert(res[0].get_DocName());
    }
    std::vector<vector<WordMap>> filteredResults;
    for ( auto& res : includeResults) {
        if (excludeSet.find(res[0].get_DocName()) == excludeSet.end()) {
            filteredResults.push_back(res);
        }
    }
    return filteredResults;
}

void SearchEngine::dumpSearchEngine(const std::string& filename)
{
    std::ofstream ofs(filename, std::ios::binary | std::ios::trunc);
    if (!ofs) {
       //std::cerr << "Failed to open file for dumping: " << filename << std::endl;
        return;
    }

    size_t hashSize = Hash.size();
    ofs.write(reinterpret_cast<const char*>(&hashSize), sizeof(hashSize));
    for (const auto& docMap : Hash) {
        size_t wordSize = docMap.first.size();
        ofs.write(reinterpret_cast<const char*>(&wordSize), sizeof(wordSize));
        ofs.write(docMap.first.data(), wordSize);

        size_t docMapSize = docMap.second.size();
        ofs.write(reinterpret_cast<const char*>(&docMapSize), sizeof(docMapSize));

        for (const auto& wordMap : docMap.second) {
            wordMap.second.serialize(ofs);
        }
    }
    ofs.close();
//    std::cout << "Search engine dumped successfully to " << filename << std::endl;
}

void SearchEngine::loadSearchEngine(const std::string& filename)
{
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs) {
        //std::cerr << "File does not exist: " << filename << ". A new index will be created." << std::endl;
        crawl();
        dumpSearchEngine(filename);
        return;
    }
    //////////////////////////////////////////////////////
    SetClr(YELLOW, BLACK);
    system("cls");
    cout << "\n\n\n\n";
    Title();
    hideCursor();
    SetConsoleCP(437);
    SetConsoleOutputCP(437);
    int barLength = 40;
    int bar1 = 177, bar2 = 219;
    int t = -1;
    cout << "\n\n\n\t\t\t\tLoading...";
    cout << "\n\n\n\t\t\t\t";
    for (int i = 0; i < barLength; i++)
        cout << (char)bar1;
    cout << "\r";
    cout << "\t\t\t\t";
    /////////////////////////////////////////////////////////
    size_t hashSize;
    ifs.read(reinterpret_cast<char*>(&hashSize), sizeof(hashSize));

    for (size_t i = 0; i < hashSize; ++i) {
        std::string word;
        size_t wordSize;
        ifs.read(reinterpret_cast<char*>(&wordSize), sizeof(wordSize));
        word.resize(wordSize);
        ifs.read(&word[0], wordSize);

        size_t docMapSize;
        ifs.read(reinterpret_cast<char*>(&docMapSize), sizeof(docMapSize));

        std::unordered_map<std::string, WordMap> docMap;
        for (size_t j = 0; j < docMapSize; ++j) {
            WordMap wordMap;
            wordMap.deserialize(ifs);
            docMap[wordMap.get_DocName()] = wordMap;
        }

        Hash[word] = std::move(docMap);
        int s = ((barLength * i) / hashSize);
        if (t < s)
        {
            t+=1;
            cout << (char)bar2;
        }
    }
    ///////////////////////////////////////////////////
    cout << "\n\t\t\t\t" << (char)1 << "!\n\t\t\t\t";
    showCursor();
    system("Pause");
    ifs.close();
//    std::cout << "Search engine loaded successfully from " << filename << std::endl;
}

std::string SearchEngine::getSampleLine(const std::string& fileName, std::streampos position) {
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

void SearchEngine::addFile()
{
    std::string filename;
    std::cout << "Enter the filename: ";
    std::cin >> filename;
    filename =path+filename+".txt";
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

void SearchEngine::removeFile()
{
    std::string filename;
    std::cout << "Enter the filename: ";
    std::cin >> filename;
    filename = path + filename + ".txt";

    for (auto it = Hash.begin(); it != Hash.end();) 
    {
        auto docIt = it->second.find(filename);

        if (docIt != it->second.end())
            it->second.erase(docIt);

        if (it->second.empty())
            it = Hash.erase(it);
        else
            ++it;
    }
    dumpSearchEngine();
}


