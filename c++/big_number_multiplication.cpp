#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void multiply_(string s1, string s2, string& res) {

    int l_s1 = s1.length();
    int l_s2 = s2.length();

    // trim signs
    int n1 = 0;
    int n2 = 0;
    if (s1.substr(0, 1) == "-") {
        n1 = 1;
        s1 = s1.substr(1);
        l_s1--;
    }
    if (s2.substr(0, 1) == "-") {
        n2 = 1;
        s2 = s2.substr(1);
        l_s2--;
    }
    // ultimate sign
    string sign = n1 + n2 == 1 ? "-" : "";

    // shift any mantissa
    int l_mantissa;
    int dot1 = s1.find(".");
    int shift = 0;
    if (dot1 > -1) {
        l_mantissa = l_s1 - dot1 - 1;
        shift += l_mantissa;
        s1 = s1.erase(dot1, 1);
        l_s1--;
    }
    int dot2 = s2.find(".");
    if (dot2 > -1) {
        l_mantissa = l_s2 - dot2 - 1;
        shift += l_mantissa;
        s2 = s2.erase(dot2, 1);
        l_s2--;
    }

    int l;
    int i1;
    int i2;
    int idx;
    int idx2;
    int mult;
    vector<string> parts;
    string order1 = "";
    for (idx = l_s1 - 1; idx >= 0; idx--) {
        i1 = stoi(s1.substr(idx, 1));
        string order2 = "";
        idx2 = 0;
        for (idx2 = l_s2 - 1; idx2 >= 0; idx2--) {
            i2 = stoi(s2.substr(idx2, 1));
            //cout << "orders: " << order1.length() << "|" << order2.length() << \
            //        ", multiplying i1: " << i1 << ", i2: " << i2 << endl;
            mult = i1 * i2;
            parts.push_back(to_string(mult) + order1 + order2);
            order2 += "0";
        }
        order1 += "0";
    }
    /*
    // sum parts
    cout << "# parts:" << endl;
    for (auto itr = parts.begin(); itr < parts.end(); ++itr) {
        cout << *itr << endl;
    }
    */

    string part;
    idx = 0;
    int carry = 0;
    string s_carry;
    while (parts.size() > 0) {
        for (int l = parts.size() - 1; l >= 0; l--) {
            part = parts[l];
            if (part.length() <= idx)
                parts.erase(parts.begin() + l);
            else {
                carry += stoi(part.substr(part.length() - 1 - idx, 1));
            }
        }
        if (parts.size() == 0)
            break;
        s_carry = to_string(carry);
        //cout << "pre carry: " << carry << endl;
        if (s_carry.length() == 1) {
            res = s_carry + res;
            carry = 0;
        } else {
            res = s_carry.substr(s_carry.length() - 1) + res;
            carry = stoi(s_carry.substr(0, s_carry.length() - 1));
        }
        //cout << "post carry: " << carry << endl;
        idx++;
    }

    if (carry != 0)
        res = to_string(carry) + res;

    if (shift > 0) {
        //cout << "shift: " << shift << endl;
        int l_res = res.length();
        l_res = res.length();
        res = res.substr(0, l_res - shift) + "." + res.substr(l_res - shift);
    }

    // strip leading / trailing 0s
    l = 0;
    while (l < res.length() && res.substr(l, 1) == "0")
        l++;
    res = res.substr(l);
    if (res.substr(0, 1) == ".") res = "0" + res;
    l = res.length() - 1;
    string ss;
    while (l > 0) {
        ss = res.substr(l, 1);
        if (ss != "0" && ss != ".") break;
        l--;
    }
    res = res.substr(0, l + 1);

    // replace any sign
    res = sign + res;

    return;
}

int main(int argc, char** argv) {
    int argc_required = 2;
    if (argc != 1 + argc_required) {
        cout << "[error] " << argc_required << " arg" << (argc_required != 1 ? "s" : "") << "required" << endl;
        exit(1);
    }
    string s1 = string(argv[1]);
    string s2 = string(argv[2]);
    cout << "[info] multiplying '" << s1 << "' and '" << s2 << "'" << endl;
    string multiplied;
    multiply_(s1, s2, multiplied);
    cout << "multiplied: " << multiplied << endl;

    return 0;
}
