#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

void add_(string s1, string s2, string& res) {
    int l_s1 = s1.length();
    int l_s2 = s2.length();
    if (l_s2 > l_s1) {
        string s1_ = s2;
        s2 = s1;
        s1 = s1_;
        int l_s1_ = l_s2;
        l_s2 = l_s1;
        l_s1 = l_s1_;
    }
    int carry = 0;
    int idx = 0;
    int i1 = 0;
    int i2 = 0;
    string sum;
    //cout << "adding s1: " << s1 << ", s2: " << s2 << ", l_s1: " << l_s1 << ", l_s2: " << l_s2 << endl;
    while (idx < l_s1) {
        i1 = stoi(s1.substr(l_s1 - idx - 1, 1));
        i2 = 0;
        if (idx < l_s2)
            i2 = stoi(s2.substr(l_s2 - idx - 1, 1));
        //cout << "adding i1: " << i1 << ", i2: " << i2 << endl;
        sum = to_string(i1 + i2 + carry);
        if (sum.length() == 1) {
            carry = 0;
            res = sum + res;
        } else {
            res = sum.substr(sum.length() - 1) + res;
            carry = stoi(sum.substr(0, sum.length() - 1));
        }
        idx++;
    }
    if (carry != 0)
        res = to_string(carry) + res;
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
    cout << "[info] adding '" << s1 << "' and '" << s2 << "'" << endl;
    string added;
    add_(s1, s2, added);
    cout << "added: " << added << endl;

    return 0;
}
