#ifndef CIPHER_H
#define CIPHER_H

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
inline double englishFreq[26] = {
    8.167,  // A
    1.492,  // B
    2.782,  // C
    4.253,  // D
    12.702, // E
    2.228,  // F
    2.015,  // G
    6.094,  // H
    6.966,  // I
    0.153,  // J
    0.772,  // K
    4.025,  // L
    2.406,  // M
    6.749,  // N
    7.507,  // O
    1.929,  // P
    0.095,  // Q
    5.987,  // R
    6.327,  // S
    9.056,  // T
    2.758,  // U
    0.978,  // V
    2.360,  // W
    0.150,  // X
    1.974,  // Y
    0.074   // Z
};

inline string englishOrder = "ETAOINSRHDLCUMFPGWYBVKXJQZ";
    
// polyalphabetic cipher
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

// monoalphabetic cipher
string monoFreqSub(string ct, vector<pair<char, int>> freq);
string manualSwap(string pt, char a, char b);
string buildKeyFromMappings(string ct, vector<pair<char,char>> mappings);


#endif