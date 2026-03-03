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
        {'h','t'}, {'u','h'}, {'c','e'}, // huc -> the
        {'b','y'}, {'m','o'}, {'g','u'}, // bmg -> you
        {'x','f'}, // mx -> of
        {'z','a'}, // huzh -> that
        {'t','i'}, {'i','s'}, // ti -> is
        {'n','n'}, {'r','d'}, // znr -> and
        {'E','W'}, // Eub -> Why
        {'w','c'}, {'p','l'}, // wpmgr -> cloud
        {'j','r'}, // jcimpghtmni -> resolutions
        {'q','k'}, // qccl -> keep
        {'v','g'}, // uznvtnv -> hanging
        {'f','v'}, // mfcj -> over
        {'l','p'}, // pmwzp -> local
        {'o','m'}, // ozb -> may
        {'y','b'}, // ygh -> but
        {'s','j'} // smv -> jog
        
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


void cipher3(string cipherText){
    string ct = readCT(cipherText);
    cout << "Ciphertext = " << ct << endl;
    vector<pair<char, int>> freq = frequencyAnalysis(ct);
}


void vigenereCipher(string cipherText){
    string ct = readCT(cipherText);
    int keyLength = vigenereAnalysis(ct);
    vector<string> subtext = subtextSplit(ct, keyLength);
    
    vector<vector<pair<char, int>>> freq;
    for (string st : subtext){
        freq.push_back(frequencyAnalysis(st)); // freq analysis per subtexts
    }

    int totalChars = findTotalChars(ct);
    vector<vector<char>> candidateShifts = findCandidateShifts(freq, subtext); // find candidates with freq > 10%
    
    // score for all key combinations
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