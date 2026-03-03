#ifndef MONOA_H
#define MONOA_H

#include <string>
#include <vector>
#include <utility>

using namespace std;

vector<pair<char, int>> frequencyAnalysis(string text);
string monoFreqSub(string ct, vector<pair<char, int>> freq);
string manualSwap(string pt, char a, char b);
string buildKeyFromMappings(string ct, vector<pair<char,char>> mappings);

#endif