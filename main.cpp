/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct Point {
    long long x;
    __int128 y;
};


string extractDigits(const string& s) {
    string result;
    for (char c : s) {
        if (isdigit(c)) result += c;
    }
    return result;
}


__int128 baseToDecimal(const string& s, int base) {
    __int128 result = 0;
    for (char c : s) {
        int digit;
        if (isdigit(c)) digit = c - '0';
        else digit = toupper(c) - 'A' + 10;
        result = result * base + digit;
    }
    return result;
}


void print128(__int128 value) {
    if (value == 0) {
        cout << "0";
        return;
    }
    string res;
    bool neg = false;
    if (value < 0) {
        neg = true;
        value = -value;
    }
    while (value > 0) {
        res += '0' + value % 10;
        value /= 10;
    }
    if (neg) cout << "-";
    reverse(res.begin(), res.end());
    cout << res;
}


__int128 lagrangeInterpolation(const vector<Point>& points) {
    __int128 result = 0;
    int k = points.size();

    for (int i = 0; i < k; i++) {
        __int128 xi = points[i].x;
        __int128 yi = points[i].y;

        __int128 num = 1, den = 1;
        for (int j = 0; j < k; j++) {
            if (i == j) continue;
            __int128 xj = points[j].x;
            num *= -xj;
            den *= (xi - xj);
        }

        result += (yi * num) / den;
    }
    return result;
}


vector<Point> parseFile(const string& filename, int& k) {
    ifstream file(filename);
    string line;
    vector<Point> points;
    k = 0;

    while (getline(file, line)) {
        
        line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());

       
        if (line.find("\"k\"") != string::npos) {
            string digits = extractDigits(line);
            if (!digits.empty()) k = stoi(digits);
        }
       
        else if (!line.empty() && line[0] == '"' && isdigit(line[1])) {
            long long x = stoll(line.substr(1, line.find('"', 1) - 1));

            string baseLine, valueLine;
            getline(file, baseLine);
            getline(file, valueLine);

            string baseDigits = extractDigits(baseLine);
            string valueStr = valueLine.substr(valueLine.find(":") + 1);
            valueStr.erase(remove(valueStr.begin(), valueStr.end(), '"'), valueStr.end());
            valueStr.erase(remove(valueStr.begin(), valueStr.end(), ','), valueStr.end());

            int base = baseDigits.empty() ? 10 : stoi(baseDigits);
            __int128 y = baseToDecimal(valueStr, base);

            points.push_back({x, y});
        }
    }

    sort(points.begin(), points.end(), [](Point a, Point b) {
        return a.x < b.x;
    });

    return vector<Point>(points.begin(), points.begin() + k);
}

int main() {
    vector<string> files = {"testcase1.json", "testcase2.json"};

    for (const string& file : files) {
        int k;
        vector<Point> points = parseFile(file, k);
        __int128 secret = lagrangeInterpolation(points);

        cout << "Secret for " << file << ": ";
        print128(secret);
        cout << "\n";
    }

    return 0;
}
