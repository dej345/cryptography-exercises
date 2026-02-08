#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_map>

using namespace std;

void cipher1();
string readCT(string fileName);
vector<pair<char, int>> frequencyAnalysis(string text);
int vigenereAnalysis(string text);
int findGCD(vector<int> &positions);
vector<string> subtextSplit(string text, int keyLength);
vector<vector<char>> findCandidateShifts(vector<vector<pair<char, int>>> &freq, vector<string> &subtexts);
int findTotalChars(string ct);

void cipher2();

struct WordData {
    vector<int> positions;
    int gcd;
    WordData() : gcd(0) {}
};


int main(){

    cipher1();
    //cipher2();


    return 0;
}

void cipher1(){
    string ct = readCT("ct/ct1.txt");
    int keyLength = vigenereAnalysis(ct);
    vector<string> subtext = subtextSplit(ct, keyLength);
    
    vector<vector<pair<char, int>>> freq;
    for (string st : subtext){
        freq.push_back(frequencyAnalysis(st));
    }

    int totalChars = findTotalChars(ct);
    vector<vector<char>> CandidateShifts = findCandidateShifts(freq, subtext);
}


string readCT(string fileName){
    ifstream inFile;
    inFile.open(fileName);

    if (!inFile) {
        cerr << "Unable to open file datafile.txt";
        exit(1);   // call system to stop
    }

    string content = "";
    string line;
    while (getline(inFile, line)) {
        content += line + "\n";
    }

    inFile.close();
    
    return content;
}


vector<pair<char, int>> frequencyAnalysis(string text){
    // Initialize vector with all 26 letters, count = 0
    vector<pair<char, int>> freq;
    for (char c = 'A'; c <= 'Z'; c++) {
        freq.push_back(make_pair(c, 0));
    }
    
    int totalLetters = 0;
    
    // Count each character
    for (char c : text) {
        if (isalpha(c)) {
            c = toupper(c);
            int index = c - 'A';  // 'A'=0, 'B'=1, etc.
            freq[index].second++;
            totalLetters++;
        }
    }
    
    // Sort by frequency (descending order)
    sort(freq.begin(), freq.end(), [](pair<char,int> &a, pair<char,int> &b){
        return a.second > b.second;
    });
    
    // Print results
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


int vigenereAnalysis(string text){
    unordered_map<string, WordData> table;

    int position = 0;
    int wordStartPosition = 0;
    string currentWord = "";
    
    for (int i = 0; i <= text.length(); i++) {
        char c = (i < text.length()) ? text[i] : ' ';  // treat end as whitespace
        
        if (isalpha(c)) {
            if (currentWord.empty()) {
                // Starting a new word, record position
                wordStartPosition = position;
            }
            currentWord += toupper(c);
            position++;
        } 
        else if (!currentWord.empty()) {
            // Word ended, store it
            table[currentWord].positions.push_back(wordStartPosition);
            currentWord = "";
        }
    }

    // Convert to vector for sorting
    vector<pair<string, WordData>> results(table.begin(), table.end());
    
    // Sort by number of occurrences (descending)
    sort(results.begin(), results.end(), [](pair<string, WordData> &a, pair<string, WordData> &b){
            return a.second.positions.size() > b.second.positions.size();
        });

    // Calculate GCD
    cout << "Repeated Sequences:" << endl;
    cout << "-------------------" << endl;
    
    int smallestGCD = -1;
    for (auto &entry : results) {
        if (entry.second.positions.size() > 1) {  // only show repeated sequences
            entry.second.gcd = findGCD(entry.second.positions);
            if (smallestGCD == -1 || entry.second.gcd < smallestGCD) {
                smallestGCD = entry.second.gcd;
            }
            
            cout << entry.first << ":";
            for (int i = 9 - entry.first.length(); i >= 0; i--)
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

    // calc difference between consecutive positions
    vector <int> differences;
    for (int i = 1; i < positions.size(); i++){
        differences.push_back(positions[i] - positions[i-1]);
    }

    // gcd of all differences

    int result = differences[0];
    for (int i = 1; i < differences.size() - 1; i++){
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
    
    for (int i = 0; i < freq.size(); i++){
        vector<char> candidates;
        
        // Calculate total for this subtext
        int totalLetters = 0;
        for (auto &p : freq[i]) {
            totalLetters += p.second;
        }
        
        cout << "\n=== Subtext " << i + 1 << " ===" << endl;
        cout << "Total letters: " << totalLetters << endl;
        
        // freq is already sorted by frequency descending
        for (auto &p : freq[i]) {
            double percentage = (p.second * 100.0) / totalLetters;
            
            // Debug: show top letters
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

void cipher2(){
    string ct = readCT("ct/ct2.txt");
    frequencyAnalysis(ct);
}