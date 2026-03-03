#ifndef POLYA_H
#define POLYA_H

#include <string>
#include <vector>
#include <utility>
#include <unordered_map>

using namespace std;

struct WordData {
    vector<int> positions;
    int gcd;
    WordData() : gcd(0) {}
};

// English letter frequencies (percentage)
extern double englishFreq[26];
extern string englishOrder;

int vigenereAnalysis(string text);
int findGCD(vector<int> &positions);
vector<string> subtextSplit(string text, int keyLength);
vector<vector<char>> findCandidateShifts(vector<vector<pair<char, int>>> &freq, vector<string> &subtexts);
int findTotalChars(string ct);
vector<string> generateKeyCombinations(vector<vector<char>> &candidates);
string decryptWithKey(string ct, string key);
double englishScore(string text);

#endif