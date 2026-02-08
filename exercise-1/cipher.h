#ifndef CIPHER_H
#define CIPHER_H

#include <string>
#include <vector>
#include <utility>

using namespace std;

struct WordData {
    vector<int> positions;
    int gcd;
    WordData() : gcd(0) {}
};

// made to solve c1-2
string readCT(string fileName);
vector<pair<char, int>> frequencyAnalysis(string text);
int vigenereAnalysis(string text);
int findGCD(vector<int> &positions);
vector<string> subtextSplit(string text, int keyLength);
vector<vector<char>> findCandidateShifts(vector<vector<pair<char, int>>> &freq, vector<string> &subtexts);
int findTotalChars(string ct);
vector<string> generateKeyCombinations(vector<vector<char>> &candidates);
string decryptWithKey(string ct, string key);
double englishScore(string text);


#endif