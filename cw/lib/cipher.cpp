#include "cipher.h"
#include <iostream>
#include <algorithm>

string readCT(string fileName){
    ifstream inFile;
    inFile.open(fileName);

    if (!inFile) {
        cerr << "Unable to open file " << fileName << endl;
        exit(1);
    }

    string content = "";
    string line;
    while (getline(inFile, line)) {
        content += line + "\n";
    }

    inFile.close();
    
    return content;
}


void cipher1(string cipherText){
    string ct = readCT(cipherText);
    vector<pair<char, int>> freq = frequencyAnalysis(ct);
    cout << "Ciphertext = " << ct << endl;

    // manual swapping
    vector<pair<char,char>> swaps = {
        {'E','W'}, {'u','h'}, {'b','y'}, {'i','s'}, {'h','t'},
        {'m','o'}, {'g','u'}, {'p','l'}, {'r','d'}, {'z','a'},
        {'j','r'}, {'t','i'}, {'c','e'}, {'w','c'},
        {'x','f'}, {'n','n'}, {'v','g'}, {'f','v'}, {'q','k'},
        {'l','p'}, {'o','m'}, {'y','b'}, {'s','j'}, {'a','a'},
        {'d','d'}, {'k','k'}
    };

    string pt = buildKeyFromMappings(ct, swaps);
    cout << pt << endl;
}


void cipher2(string cipherText){
    vector<unsigned char> key1 = {0xd6};
    cout << "Single 0xD6: " << printSafe(xorMultiByte("ct/ct2.hex", key1)) << endl;

    vector<unsigned char> key2 = {0xd6, 0xe5};
    cout << "Two byte D6 E5: " << printSafe(xorMultiByte("ct/ct2.hex", key2)) << endl;
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

    if (!keyScores.empty()){
        cout << "\n=== Best Key: " << keyScores[0].first << " ===" << endl;
        cout << decryptWithKey(ct, keyScores[0].first) << endl;
    }
}