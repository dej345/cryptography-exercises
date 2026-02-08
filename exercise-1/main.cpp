#include "cipher.h"
#include <iostream>
#include <algorithm>
#include <string.h>

using namespace std;

void vigenereCipher(string cipherText);
void cipher3(string cipherText);
void cipher4(string cipherText);

int main(){
    // cipher 1-2 solved same way
    //vigenereCipher("ct/ct1.txt");
    //vigenereCipher("ct/ct2.txt");
    //cipher3("ct/ct3.txt");
    cipher4("ct/ct4.txt");

    return 0;
}

void vigenereCipher(string cipherText){
    string ct = readCT(cipherText);
    int keyLength = vigenereAnalysis(ct);
    vector<string> subtext = subtextSplit(ct, keyLength);
    
    vector<vector<pair<char, int>>> freq;
    for (string st : subtext){
        freq.push_back(frequencyAnalysis(st));
    }

    int totalChars = findTotalChars(ct);
    vector<vector<char>> candidateShifts = findCandidateShifts(freq, subtext);
    
    vector<string> keys = generateKeyCombinations(candidateShifts);
    
    vector<pair<string, double>> keyScores;
    for (string &key : keys){
        string pt = decryptWithKey(ct, key);
        double score = englishScore(pt);
        keyScores.push_back(make_pair(key, score));
    }

    sort(keyScores.begin(), keyScores.end(), [](pair<string, double> &a, pair<string, double> &b){
        return a.second < b.second;
    });
    
    cout << "\n=== Key Rankings (best first) ===" << endl;
    for (auto &ks : keyScores){
        string pt = decryptWithKey(ct, ks.first);
        cout << "Key: " << ks.first << "  Score: " << ks.second 
             << "  Preview: " << pt.substr(0, 40) << "..." << endl;
    }

    // Display full decryption of best key
    if (!keyScores.empty()){
        cout << "\n=== Best Key: " << keyScores[0].first << " ===" << endl;
        cout << decryptWithKey(ct, keyScores[0].first) << endl;
    }
}


void cipher3(string cipherText){
    string ct = readCT(cipherText);
    vector<pair<char, int>> freq = frequencyAnalysis(ct);
    string pt = monoFreqSub(ct, freq);
    cout << "Plaintext after swap = " << pt << endl;

    // manual swapping
    vector<pair<char,char>> swaps = {
        {'N','H'}, {'A','I'}, {'S','N'}, {'R','S'}, {'W','F'},
        {'O','A'}, {'U','M'}, {'D','L'}, {'B','Y'}, {'P','V'},
        {'G','P'}, {'B','C'}, {'B','G'}
    };
    
    for (auto &s : swaps){
        pt = manualSwap(pt, s.first, s.second);
        cout << "Swap " << s.first << "<->" << s.second << ": " << pt << endl;
    }
}


void cipher4(string cipherText){
    string ct = readCT(cipherText);
    vector<pair<char, int>> freq = frequencyAnalysis(ct);
    
}