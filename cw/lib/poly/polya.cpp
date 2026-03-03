#include "polya.h"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <cmath>

double englishFreq[26] = {
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

string englishOrder = "ETAOINSRHDLCUMFPGWYBVKXJQZ";


int vigenereAnalysis(string text){
    unordered_map<string, WordData> table;

    int position = 0;
    int wordStartPosition = 0;
    string currentWord = "";
    
    for (int i = 0; i <= (int)text.length(); i++) {
        char c = (i < (int)text.length()) ? text[i] : ' ';
        
        if (isalpha(c)) {
            if (currentWord.empty()) {
                wordStartPosition = position;
            }
            currentWord += toupper(c);
            position++;
        } 
        else if (!currentWord.empty()) {
            table[currentWord].positions.push_back(wordStartPosition);
            currentWord = "";
        }
    }

    vector<pair<string, WordData>> results(table.begin(), table.end());
    
    sort(results.begin(), results.end(), [](pair<string, WordData> &a, pair<string, WordData> &b){
            return a.second.positions.size() > b.second.positions.size();
        });

    cout << "Repeated Sequences:" << endl;
    cout << "-------------------" << endl;
    
    int smallestGCD = -1;
    for (auto &entry : results) {
        if (entry.second.positions.size() > 1) {
            entry.second.gcd = findGCD(entry.second.positions);
            if (smallestGCD == -1 || entry.second.gcd < smallestGCD) {
                smallestGCD = entry.second.gcd;
            }
            
            cout << entry.first << ":";
            for (int i = 9 - (int)entry.first.length(); i >= 0; i--)
                cout << " ";
            cout << "  Occurrences: " << entry.second.positions.size();
            cout << "  GCD: " << entry.second.gcd;
            if (entry.second.gcd < 100)
                cout << " ";
            if (entry.second.gcd < 10)
                cout << " ";
            cout << "  Positions: ";
            for (int pos : entry.second.positions) {
                cout << pos << " ";
            }
            cout << endl;
        }
    }

    cout << "Smallest GCD: " << smallestGCD << endl;
    return smallestGCD;
}


int findGCD(vector<int> &positions){
    if (positions.size() < 2) return 0;

    vector<int> differences;
    for (int i = 1; i < (int)positions.size(); i++){
        differences.push_back(positions[i] - positions[i-1]);
    }

    int result = differences[0];
    for (int i = 1; i < (int)differences.size(); i++){
        result = __gcd(result, differences[i]);
    }

    return result;
}


vector<string> subtextSplit(string text, int keyLength){
    vector<string> subtexts(keyLength, "");
    
    int position = 0;
    for (char c : text) {
        if (isalpha(c)) {
            int index = position % keyLength;
            subtexts[index] += toupper(c);
            position++;
        }
    }
    
    return subtexts;
}


vector<vector<char>> findCandidateShifts(vector<vector<pair<char, int>>> &freq, vector<string> &subtexts){
    vector<vector<char>> allCandidates;
    
    for (int i = 0; i < (int)freq.size(); i++){
        vector<char> candidates;
        
        int totalLetters = 0;
        for (auto &p : freq[i]) {
            totalLetters += p.second;
        }
        
        cout << "\n=== Subtext " << i + 1 << " ===" << endl;
        cout << "Total letters: " << totalLetters << endl;
        
        for (auto &p : freq[i]) {
            double percentage = (p.second * 100.0) / totalLetters;
            
            if (percentage > 5.0) {
                cout << p.first << ": " << percentage << "%" << endl;
            }
            
            if (percentage > 10.0) {
                int shiftValue = (p.first - 'E' + 26) % 26;
                char shiftChar = shiftValue + 'A';
                cout << "  -> If " << p.first << " is E, key letter = " << shiftChar << endl;
                candidates.push_back(shiftChar);
            }
        }
        
        allCandidates.push_back(candidates);
        
        cout << "Candidates: ";
        for (char c : candidates){
            cout << c << " ";
        }
        cout << endl;
    }

    return allCandidates;
}


int findTotalChars(string ct){
    int count = 0;
    for (char c : ct){
        if (isalpha(c))
            count++;
    }
    return count;
}


vector<string> generateKeyCombinations(vector<vector<char>> &candidates){
    vector<string> combos;
    int numColumns = candidates.size();
    
    vector<int> row(numColumns, 0);
    
    while (true){
        string key = "";
        for (int col = 0; col < numColumns; col++){
            key += candidates[col][row[col]];
        }
        combos.push_back(key);
        cout << "  " << key << endl;
        
        int col = numColumns - 1;
        while (col >= 0){
            row[col]++;
            if (row[col] < (int)candidates[col].size()){
                break;
            }
            row[col] = 0;
            col--;
        }
        
        if (col < 0) break;
    }
    
    cout << "Total key combinations: " << combos.size() << endl;
    return combos;
}


string decryptWithKey(string ct, string key){
    string pt = "";
    int keyLen = key.length();
    int position = 0;
    
    for (char c : ct){
        if (isalpha(c)){
            int shift = key[position % keyLen] - 'A';
            char decrypted = ((toupper(c) - 'A' - shift + 26) % 26) + 'A';
            pt += decrypted;
            position++;
        }
    }
    
    return pt;
}


double englishScore(string text){    
    int counts[26] = {0};
    int totalLetters = 0;
    
    for (char c : text){
        if (isalpha(c)){
            counts[toupper(c) - 'A']++;
            totalLetters++;
        }
    }
    
    if (totalLetters == 0) return 0.0;
    
    double chiSquared = 0.0;
    for (int i = 0; i < 26; i++){
        double expected = (englishFreq[i] / 100.0) * totalLetters;
        double observed = counts[i];
        if (expected > 0){
            chiSquared += ((observed - expected) * (observed - expected)) / expected;
        }
    }
    
    return chiSquared;
}