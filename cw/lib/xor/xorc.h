#ifndef XORC_H
#define XORC_H

#include <string>
#include <vector>

using namespace std;

string xorSingleByte(string cipherText, unsigned char key);
string xorMultiByte(string cipherText, vector<unsigned char> key);
string printSafe(string text);

#endif