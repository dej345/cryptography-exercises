#include "cipher.h"
#include <iostream>
#include <algorithm>
#include <string.h>

using namespace std;

void vigenereCipher(string cipherText);
void cipher1(string cipherText);
void cipher2(string cipherText);

int main(){
    // cipher 1 substition cipher - implemented manual swaps
    //cipher1("ct/ct1.txt");
    cipher2("ct/ct2.txt");
    //cipher 3 done by hand in excel
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


void cipher1(string cipherText){
    string ct = readCT(cipherText);
    vector<pair<char, int>> freq = frequencyAnalysis(ct);
    cout << "Ciphertext = " << ct << endl;

    // manual swapping
    vector<pair<char,char>> swaps = {
        {'E','W'}, {'u','h'}, {'b','y'}, {'i','s'}, {'h','t'},
        {'m','o'}, {'g','u'}, {'p','l'}, {'r','d'}, {'z','a'},
        {'j','r'}, {'t','i'}, {'c','e'}, {'e','w'}, {'w','c'},
        {'x','f'}, {'n','n'}, {'v','g'}, {'f','v'}, {'q','k'},
        {'l','p'}, {'o','m'}, {'y','b'}, {'s','j'}, {'a','a'},
        {'b','y'}, {'d','d'}, {'k','k'}
    };

    string pt = buildKeyFromMappings(ct, swaps);
    cout << pt << endl;
}

void cipher2(string cipherText){
    // xor operation
    // key likely to be 1-3 bytes

    /*
    HEX DUMP: hexdump -C ct/ct2.hex
    00000000  82 84 ba 89 b3 96 a2 c5  bb 84 a4 97 bf 80 b2 c5  |................|
    00000010  b5 8a a3 95 ba 80 f6 80  a0 80 a4 c5 a4 80 b5 8a  |................|
    00000020  a4 81 b3 81 f6 92 b7 96  f6 a4 b8 8b b7 c5 9e 84  |................|
    00000030  bf 8b bf 8b b1 c5 85 92  b7 8b f6 84 b8 81 f6 a8  |................|
    00000040  b7 97 a2 8c b8 c5 80 84  b8 c5 94 90 a4 80 b8 c5  |................|
    00000050  94 84 a2 80 a5 c5 f6 e8  dc                       |.........|
    00000059
    
    First key byte:
    (first char is T (0x54), first byte is 0x82)
    0x82 ^ 0x54 = 0xd6

    Second key byte:
    (0xc5 appears the most -> likely to be a space (0x20))
    0xc5 ^ 0x20 = 0xe5
    */

    vector<unsigned char> key1 = {0xd6};
    cout << "Single 0xD6: " << printSafe(xorMultiByte("ct/ct2.hex", key1)) << endl;

    vector<unsigned char> key2 = {0xd6, 0xe5};
    cout << "Two byte D6 E5: " << printSafe(xorMultiByte("ct/ct2.hex", key2)) << endl;
}