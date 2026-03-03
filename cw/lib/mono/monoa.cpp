#include "monoa.h"
#include <iostream>
#include <algorithm>
#include <string.h>

vector<pair<char, int>> frequencyAnalysis(string text){
    vector<pair<char, int>> freq;
    for (char c = 'A'; c <= 'Z'; c++) {
        freq.push_back(make_pair(c, 0));
    }
    
    int totalLetters = 0;
    
    for (char c : text) {
        if (isalpha(c)) {
            c = toupper(c);
            int index = c - 'A';
            freq[index].second++;
            totalLetters++;
        }
    }
    
    sort(freq.begin(), freq.end(), [](pair<char,int> &a, pair<char,int> &b){
        return a.second > b.second;
    });
    
    cout << "Character Frequencies (sorted):" << endl;
    cout << "--------------------------------" << endl;
    
    for (auto &p : freq) {
        if (p.second > 0) {
            double percentage = (p.second * 100.0) / totalLetters;
            cout << p.first << ": " << p.second << " (" << percentage << "%)" << endl;
        }
    }

    cout << "Total Chars: " << totalLetters << endl;

    return freq;
}


string monoFreqSub(string ct, vector<pair<char, int>> freq){
    string englishOrder = "ETAOINSRHDLUCMFYWGPBVKXQJZ";
    
    char sub[27];
    sub[26] = '\0';
    for (int i = 0; i < 26 && i < (int)freq.size(); i++){
        sub[freq[i].first - 'A'] = englishOrder[i];
    }
    
    cout << "Key = "<< sub << endl;

    string pt = "";
    for (char c : ct){
        if (isalpha(c)){
            pt += sub[toupper(c) - 'A'];
        }
    }
    
    return pt;
}


string manualSwap(string pt, char a, char b){
    for (char &c : pt){
        if (c == a) c = b;
        else if (c == b) c = a;
    }
    return pt;
}


string buildKeyFromMappings(string ct, vector<pair<char,char>> mappings){
    char sub[26];
    memset(sub, '?', 26);
    
    for (auto &m : mappings){
        sub[toupper(m.first) - 'A'] = toupper(m.second);
    }
    
    cout << "Key: ";
    for (int i = 0; i < 26; i++) cout << sub[i];
    cout << endl;
    
    string pt = "";
    for (char c : ct){
        if (isalpha(c)){
            char decoded = sub[toupper(c) - 'A'];
            if (decoded != '?')
                pt += islower(c) ? tolower(decoded) : decoded;
            else
                pt += decoded;
        } else {
            pt += c;
        }
    }
    
    return pt;
}