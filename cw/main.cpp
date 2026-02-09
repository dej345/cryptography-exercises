#include "cipher.h"
#include <iostream>
#include <algorithm>
#include <string.h>

using namespace std;

void vigenereCipher(string cipherText);
void monoSubCipher(string cipherText);

int main(){
    // cipher 1 probably a sub
    vigenereCipher("ct/ct1.txt");
    monoSubCipher("ct/ct1.txt");
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


void monoSubCipher(string cipherText){
    string ct = readCT(cipherText);
    vector<pair<char, int>> freq = frequencyAnalysis(ct);
    cout << "Ciphertext = " << ct << endl;

    // manual swapping
    vector<pair<char,char>> swaps = {
        {'E','W'}, {'u','h'}, {'b','y'}, {'i','s'}, {'h','t'},
        {'m','o'}, {'g','u'}, {'p','l'}, {'r','d'}, {'z','a'},
        {'j','r'}, {'t','i'}, {'c','e'}, {'e','w'}, {'w','c'},
        {'x','f'}, {'n','n'}, {'v','g'}, {'f','v'}, {'q','k'},
        {'l','p'}, {'o','m'}, {'y','b'}, {'s','j'}
    };

    string pt = buildKeyFromMappings(ct, swaps);
    cout << pt << endl;
}