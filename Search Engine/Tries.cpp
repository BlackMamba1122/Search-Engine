#include "Tries.h"
#include "utility.h"
#include "sstream"
#include <iostream>
#include <Windows.h>
Tries::Tries()
{
	root = new TrieNode();
}

void Tries::insert(const std::string& word, const std::string& docname, int position)
{
    TrieNode* node = root;

    for (char ch : word) {
        if (node->child.find(ch) == node->child.end()) 
        {
            node->child[ch] = new TrieNode();
        }
        node = node->child[ch];
    }
    node->DocData[docname].push_back(position);
}

vector<pair<string, vector<int>>> Tries::search(const string& word)
{
    vector<pair<string, vector<int>>> results;
    TrieNode* currentNode = root;

    if (isSingleWord(word)) {
        for (char c : word) {
            if (currentNode->child.find(c) == currentNode->child.end()) {
                return results;
            }
            currentNode = currentNode->child[c];
        }
        if (!currentNode->DocData.empty()) {
            for (auto& i : currentNode->DocData) {
                results.push_back(make_pair(i.first,i.second));
            }
        }

        return results;
    }
    else {
        vector<pair<string, vector<int>>> result2;
        std::stringstream ss(word);
        std::string token;
        int wordCount = -2;
        while (ss >> token) {
            wordCount += 1;
            currentNode = root;
            for (char c : word) {
                if (currentNode->child.find(c) == currentNode->child.end()) {
                    results.clear();
                    break;
                }
                currentNode = currentNode->child[c];
            }
            if (results.empty()) {
                if (!currentNode->DocData.empty()) {
                    for (auto& i : currentNode->DocData) {
                        results.push_back(make_pair(i.first, i.second));
                    }
                }
                continue;
            }
            else {
                result2.clear();
                if (!currentNode->DocData.empty()) {
                    for (auto& i : currentNode->DocData) {
                        result2.push_back(make_pair(i.first, i.second));

                    }
                }

                vector<pair<string, vector<int>>> filtered;
                for (auto& res1 : results) {
                    for (auto& res2 : result2) {
                        if (res1.first == res2.first) {
                            std::vector<int> validPositions;
                            for (auto& pos1 : res1.second) {
                                int nextPos = static_cast<int>(pos1) + 1 + wordCount;
                                if (std::find(res2.second.begin(), res2.second.end(), nextPos) != res2.second.end()) {
                                    validPositions.push_back(pos1);
                                }
                            }
                            if (!validPositions.empty()) {
                                filtered.emplace_back(res1.first, validPositions);
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


void Tries::clear(TrieNode* node) {
    if (!node) return;
    for (auto& childPair : node->child) {
        clear(childPair.second);
    }
    delete node;
}

void Tries::serialize(TrieNode* node, ofstream& outFile) {
    if (!node) return;

    outFile << node->DocData.size() << '\n';
    for (auto& doc : node->DocData) {
        outFile << doc.first << ' ' << doc.second.size();
        for (int pos : doc.second) {
            outFile << ' ' << pos;
        }
        outFile << '\n';
    }

    outFile << node->child.size() << '\n';
    for (auto& child : node->child) {
        outFile << child.first << '\n';
        serialize(child.second, outFile);
    }
}

TrieNode* Tries::deserialize(ifstream& inFile) {
    TrieNode* node = new TrieNode();

    size_t docDataSize;
    inFile >> docDataSize;
    inFile.ignore();
    for (size_t i = 0; i < docDataSize; ++i) {
        string docName;
        int positionCount;
        inFile >> docName >> positionCount;
        vector<int> positions(positionCount);
        for (int& pos : positions) {
            inFile >> pos;
        }
        node->DocData[docName] = positions;
    }

    size_t childCount;
    inFile >> childCount;
    inFile.ignore();
    for (size_t i = 0; i < childCount; ++i) {
        char childChar;
        inFile >> childChar;
        inFile.ignore();
        node->child[childChar] = deserialize(inFile);
    }

    return node;
}

void Tries::remove(const string& filename) {
    removeDoc(root, filename);
}

bool Tries::removeDoc(TrieNode* node, const string& filename) {
    if (!node) return false;

    auto docIt = node->DocData.find(filename);
    if (docIt != node->DocData.end()) {
        node->DocData.erase(docIt);
    }

    for (auto it = node->child.begin(); it != node->child.end();) {
        if (removeDoc(it->second, filename)) {
            delete it->second;
            it = node->child.erase(it);
        }
        else {
            ++it;
        }
    }
    return node->child.empty() && node->DocData.empty();
}


void Tries::dump(const string& filename) {
    ofstream outFile(filename, ios::binary);
    if (!outFile.is_open()) {
        //cerr << "Error: Could not open file for dumping: " << filename << endl;
        return;
    }
    serialize(root, outFile);
    outFile.close();
}

void Tries::load(const string& filename, ifstream& inFile) {
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
    cout << "\n\n\n\t\t\t\tLoading...";
    cout << "\n\n\n\t\t\t\t";
    for (int i = 0; i < barLength; i++)
        cout << (char)bar1;
    cout << "\r";
    cout << "\t\t\t\t";
    /////////////////////////////////////////////////////////
    clear(root);
    root = deserialize(inFile);
    inFile.close();
    for (int i = 0; i < barLength; i++)
    {
        cout << (char)bar2;
        Sleep(30);
    }

}

