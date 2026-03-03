#include "xorc.h"
#include <fstream>
#include <cstdio>

string xorSingleByte(string cipherText, unsigned char key){
    ifstream inFile(cipherText, ios::binary);
    vector<unsigned char> data((istreambuf_iterator<char>(inFile)),
                               istreambuf_iterator<char>());
    inFile.close();
    
    string result = "";
    for (unsigned char byte : data){
        result += (char)(byte ^ key);
    }
    
    return result;
}


string xorMultiByte(string cipherText, vector<unsigned char> key){
    ifstream inFile(cipherText, ios::binary);
    vector<unsigned char> data((istreambuf_iterator<char>(inFile)),
                               istreambuf_iterator<char>());
    inFile.close();
    
    string result = "";
    for (int i = 0; i < (int)data.size(); i++){
        result += (char)(data[i] ^ key[i % key.size()]);
    }
    
    return result;
}


string printSafe(string text){
    string safe = "";
    for (unsigned char c : text){
        if (c >= 32 && c <= 126) safe += c;
        else {
            char buf[8];
            sprintf(buf, "[%02X]", c);
            safe += buf;
        }
    }
    return safe;
}