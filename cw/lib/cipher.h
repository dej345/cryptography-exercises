#ifndef CIPHER_H
#define CIPHER_H

#include <string>
#include <vector>
#include <fstream>

#include "mono/monoa.h"
#include "poly/polya.h"
#include "xor/xorc.h"

using namespace std;

string readCT(string fileName);
void cipher1(string cipherText);
void cipher2(string cipherText);
void cipher3(string cipherText);
void vigenereCipher(string cipherText);

#endif