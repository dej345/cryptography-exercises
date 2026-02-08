#include "cipher.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <cmath>


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


vector<string> generateKeyCombinations(vector<vector<char>> &candidates){
    vector<string> combos;
    int numColumns = candidates.size();
    
    // Row pointer for each column, all start at 0
    vector<int> row(numColumns, 0);
    
    while (true){
        // Build current key from pointers
        string key = "";
        for (int col = 0; col < numColumns; col++){
            key += candidates[col][row[col]];
        }
        combos.push_back(key);
        cout << "  " << key << endl;
        
        // Increment rightmost pointer, carry over if needed
        int col = numColumns - 1;
        while (col >= 0){
            row[col]++;
            if (row[col] < candidates[col].size()){
                break;  // no carry needed
            }
            row[col] = 0;  // reset and carry left
            col--;
        }
        
        // All pointers wrapped around = done
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
    
    // Chi-squared statistic (lower = more likely English)
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


string monoFreqSub(string ct, vector<pair<char, int>> freq){
    // English letters sorted by frequency (most to least)
    string englishOrder = "ETAOINSRHDLUCMFYWGPBVKXQJZ";
    
    // Build substitution map: ciphertext letter -> plaintext letter
    char sub[27];
    sub[26] = '\0';
    for (int i = 0; i < 26 && i < freq.size(); i++){
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


string hillClimbColumnar(string ct, int cols, int iterations){
    int len = ct.length();
    int rows = (len + cols - 1) / cols;
    int extraCols = len % cols;
    if (extraCols == 0) extraCols = cols;
    
    // Start with random column order
    vector<int> order(cols);
    iota(order.begin(), order.end(), 0);
    
    // Seed randomness
    srand(time(0));
    
    // Shuffle initial order
    for (int i = cols - 1; i > 0; i--){
        int j = rand() % (i + 1);
        swap(order[i], order[j]);
    }
    
    string bestPt = columnarDecryptOrder(ct, order, 0);
    double bestScore = bigramScore(bestPt);
    vector<int> bestOrder = order;
    
    for (int iter = 0; iter < iterations; iter++){
        // Pick two random columns and swap them
        int a = rand() % cols;
        int b = rand() % cols;
        while (b == a) b = rand() % cols;
        
        swap(order[a], order[b]);
        
        string pt = columnarDecryptOrder(ct, order, 0);
        double score = bigramScore(pt);
        
        if (score > bestScore){
            bestScore = score;
            bestPt = pt;
            bestOrder = order;
        } else {
            // Undo the swap
            swap(order[a], order[b]);
        }
    }
    
    cout << "Best score: " << bestScore << endl;
    cout << "Column order: ";
    for (int i : bestOrder) cout << i << " ";
    cout << endl;
    cout << "Plaintext: " << bestPt << endl;
    
    return bestPt;
}


void solveColumnar(string ct){
    double globalBest = 0.0;
    string globalPt = "";
    
    for (int cols = 5; cols <= 5; cols++){
        cout << "\n=== " << cols << " columns ===" << endl;
        
        if (cols <= 8){
            bruteForceColumnarDebug(ct, cols);
        } else {
            for (int attempt = 0; attempt < 20; attempt++){
                solveColumnarSA(ct, cols);
            }
        }
    }
    
    cout << "\n====== BEST OVERALL ======" << endl;
    cout << "Score: " << globalBest << endl;
    cout << "Plaintext: " << globalPt << endl;
}


double bigramScore(string text){
    // English bigram log frequencies (higher = more English-like)
    string bigrams[] = {
        "TH","HE","IN","ER","AN","RE","ON","AT","EN","ND",
        "TI","ES","OR","TE","OF","ED","IS","IT","AL","AR",
        "ST","TO","NT","NG","SE","HA","OU","IO","LE","VE",
        "CO","ME","DE","HI","RI","RO","IC","NE","EA","RA",
        "CE","LI","CH","LL","BE","MA","SI","OM","UR","CA",
        "EL","TA","LA","NS","GE","HO","PO","SS","PE","OO",
        "YO","OS","NO","SO","US","WH","AS"
    };
    double scores[] = {
        3.56, 3.07, 2.43, 2.05, 1.99, 1.85, 1.76, 1.49, 1.45, 1.35,
        1.34, 1.34, 1.28, 1.27, 1.17, 1.17, 1.13, 1.12, 1.09, 1.07,
        1.05, 1.05, 1.04, 0.95, 0.93, 0.93, 0.87, 0.83, 0.83, 0.83,
        0.79, 0.79, 0.76, 0.76, 0.73, 0.73, 0.70, 0.69, 0.69, 0.62,
        0.65, 0.62, 0.60, 0.58, 0.58, 0.57, 0.55, 0.55, 0.54, 0.53,
        0.53, 0.53, 0.53, 0.51, 0.51, 0.51, 0.51, 0.49, 0.49, 0.46,
        0.45, 0.43, 0.42, 0.42, 0.42, 0.42, 0.41
    };
    
    int numBigrams = 67;
    unordered_map<string, double> bigramMap;
    for (int i = 0; i < numBigrams; i++){
        bigramMap[bigrams[i]] = scores[i];
    }
    
    double score = 0.0;
    for (int i = 0; i < (int)text.length() - 1; i++){
        char a = toupper(text[i]);
        char b = toupper(text[i + 1]);
        if (!isalpha(a) || !isalpha(b)) continue;
        
        string pair = "";
        pair += a;
        pair += b;
        
        if (bigramMap.count(pair)){
            score += bigramMap[pair];
        }
    }
    
    return score;
}


string columnarDecryptOrder(string ct, vector<int> &order, int extraMode){
    int cols = order.size();
    int len = ct.length();
    int rows = (len + cols - 1) / cols;
    int extraCols = len % cols;
    if (extraCols == 0) extraCols = cols;
    
    vector<bool> isLong(cols, false);
    
    if (extraMode < cols){
        // Mode 0 to cols-1: give extra to specific grid column
        // For extraCols=1, just that one column is long
        // For extraCols>1, start from that position and wrap
        for (int i = 0; i < extraCols; i++){
            isLong[(extraMode + i) % cols] = true;
        }
    } else if (extraMode == cols){
        // First columns READ are long
        for (int i = 0; i < extraCols; i++)
            isLong[order[i]] = true;
    } else {
        // Last columns READ are long
        for (int i = cols - extraCols; i < cols; i++)
            isLong[order[i]] = true;
    }
    
    vector<int> colLen(cols);
    for (int c = 0; c < cols; c++)
        colLen[c] = isLong[c] ? rows : rows - 1;
    
    vector<string> grid(cols);
    int pos = 0;
    for (int i = 0; i < cols; i++){
        int c = order[i];
        grid[c] = ct.substr(pos, colLen[c]);
        pos += colLen[c];
    }
    
    string pt = "";
    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            if (r < (int)grid[c].length())
                pt += grid[c][r];
        }
    }
    
    return pt;
}


double improvedScore(string text){
    // Common English words/fragments to look for
    string commonWords[] = {
        "THE","AND","ING","HER","HAT","HIS","THA","ERE","FOR",
        "ENT","ION","TER","WAS","YOU","ITH","VER","ALL","WIT",
        "THI","TIO","TION","THAT","THIS","WITH","HAVE","FROM",
        "THEY","BEEN","SAID","EACH","WHICH","THEIR","WILL",
        "OTHER","ABOUT","MANY","THEN","THEM","SOME","COULD",
        "MAKE","LIKE","OVER","SUCH","AFTER","ALSO","MOST",
        "ONCE","HOPE","CHOOSE","ANYTHING","POSSIBLE","NOT",
        "BUT","WHAT","CAN","HAD","ARE","ONE","OUR","OUT"
    };
    
    string upper = "";
    for (char c : text){
        upper += toupper(c);
    }
    
    double score = 0.0;
    
    // Bigram scoring
    score += bigramScore(text);
    
    // Bonus for common word fragments found
    for (auto &word : commonWords){
        size_t pos = 0;
        while ((pos = upper.find(word, pos)) != string::npos){
            score += word.length() * 2.0;
            pos++;
        }
    }
    
    // Bonus for space followed by common starting letters
    for (int i = 0; i < (int)text.length() - 1; i++){
        if (text[i] == ' ' && isalpha(text[i + 1])){
            score += 0.5;  // spaces between words is good
        }
    }
    
    // Penalize unlikely patterns
    for (int i = 0; i < (int)text.length() - 2; i++){
        if (text[i] == ' ' && text[i+1] == ' ' && text[i+2] == ' '){
            score -= 5.0;  // triple spaces unlikely
        }
    }

    
    return score;
}


string solveColumnarSA(string ct, int cols, int iterations){
    srand(time(0));
    
    vector<int> order(cols);
    iota(order.begin(), order.end(), 0);
    for (int i = cols - 1; i > 0; i--){
        int j = rand() % (i + 1);
        swap(order[i], order[j]);
    }
    
    // Try both conventions, pick whichever starts better
    bool useReadOrder;
    string pt1 = columnarDecryptOrder(ct, order, 1);
    string pt2 = columnarDecryptOrder(ct, order, 0);
    if (improvedScore(pt1) >= improvedScore(pt2)){
        useReadOrder = true;
    } else {
        useReadOrder = false;
    }
    
    string bestPt = columnarDecryptOrder(ct, order, useReadOrder ? 1 : 0);
    double bestScore = improvedScore(bestPt);
    vector<int> bestOrder = order;
    
    vector<int> currentOrder = order;
    double currentScore = bestScore;
    
    double tempStart = 10.0;
    double tempEnd = 0.01;
    
    for (int iter = 0; iter < iterations; iter++){
        double temp = tempStart * pow(tempEnd / tempStart, (double)iter / iterations);
        
        vector<int> newOrder = currentOrder;
        int a = rand() % cols;
        int b = rand() % cols;
        while (b == a) b = rand() % cols;
        swap(newOrder[a], newOrder[b]);
        
        // Try both conventions, pick the better one
        string ptR = columnarDecryptOrder(ct, newOrder, 1);
        string ptG = columnarDecryptOrder(ct, newOrder, 0);
        double scoreR = improvedScore(ptR);
        double scoreG = improvedScore(ptG);
        
        string pt = (scoreR >= scoreG) ? ptR : ptG;
        double score = max(scoreR, scoreG);
        
        double delta = score - currentScore;
        if (delta > 0 || ((double)rand() / RAND_MAX) < exp(delta / temp)){
            currentOrder = newOrder;
            currentScore = score;
        }
        
        if (score > bestScore){
            bestScore = score;
            bestPt = pt;
            bestOrder = newOrder;
        }
    }
    
    cout << "Score: " << bestScore << endl;
    cout << "Order: ";
    for (int i : bestOrder) cout << i << " ";
    cout << endl;
    cout << "Plaintext: " << bestPt << endl;
    
    return bestPt;
}


void bruteForceColumnarDebug(string ct, int cols){
    vector<int> order(cols);
    iota(order.begin(), order.end(), 0);
    
    double bestScore = 0.0;
    string bestPt = "";
    vector<int> bestOrder;
    int bestMode = 0;
    
    int totalModes = cols + 2;  // modes 0..cols-1, cols, cols+1
    
    do {
        for (int mode = 0; mode < totalModes; mode++){
            string pt = columnarDecryptOrder(ct, order, mode);
            double score = improvedScore(pt);
            
            if (score > bestScore){
                bestScore = score;
                bestPt = pt;
                bestOrder = order;
                bestMode = mode;
            }
        }
    } while (next_permutation(order.begin(), order.end()));
    
    cout << "Best Mode: " << bestMode << " Order: ";
    for (int i : bestOrder) cout << i << " ";
    cout << endl;
    cout << "Score: " << bestScore << endl;
    cout << "Plaintext: " << bestPt << endl;
}