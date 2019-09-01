#include <cstdlib>
#include <iostream>
#include <string>
#include <functional>

using namespace std;

void add_(string s1, string s2, string& res) {

    int l_s1 = s1.length();
    int l_s2 = s2.length();

    // operator
    std::function<int(int, int)> op;
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
    if (n1 + n2 == 1)
        op = std::minus<int>();
    else
        op = std::plus<int>();

    // equalise mantissa
    int dot1 = s1.find(".");
    int dot2 = s2.find(".");
    if (dot1 == -1) {
        s1.push_back('.');
        s1.push_back('0');
        l_s1 += 2;
        dot1 = l_s1 - 2;
    }
    if (dot2 == -1) {
        s2.push_back('.');
        s2.push_back('0');
        l_s2 += 2;
        dot2 = l_s2 - 2;
    }
    string *x;
    int append;
    if (l_s1 - dot1 < l_s2 - dot2) {
        x = &s1;
        append = (l_s2 - dot2) - (l_s1 - dot1);
        l_s1 += append;
    } else if (l_s1 - dot1 > l_s2 - dot2) {
        x = &s2;
        append = (l_s1 - dot1) - (l_s2 - dot2);
        l_s2 += append;
    }
    for (int l = 0; l < append; l++) x->push_back('0');

    // order by magnitude
    int switch_ = 0;
    if (l_s1 == l_s2) {
        for (int l = 0; l < l_s1; l++) {
            if (s1.substr(l, 1) == ".") continue;
            if (stoi(s1.substr(l, 1)) < stoi(s2.substr(l, 1))) {
                switch_ = 1;
                break;
            }
        }
    } else if (l_s2 > l_s1)
        switch_ = 1;

    if (switch_ == 1) {
        string s1_ = s2;
        s2 = s1;
        s1 = s1_;
        int l_s1_ = l_s2;
        l_s2 = l_s1;
        l_s1 = l_s1_;
        int n1_ = n2;
        n2 = n1;
        n1 = n1_;
    }

    // ultimate sign
    string sign = n1 == 1 ? "-" : "";

    int carry = 0;
    int idx = 0;
    int i1 = 0;
    int i2 = 0;
    int sum;
    string sum_;
    string ss1;
    string ss2;
    //cout << "adding s1: " << s1 << ", s2: " << s2 << ", l_s1: " << l_s1 << ", l_s2: " << l_s2 << endl;
    while (idx < l_s1) {
        ss1 = s1.substr(l_s1 - idx - 1, 1);
        if (ss1 == ".") {
            res = "." + res;
            idx++;
            continue;
        }
        i1 = stoi(ss1);
        i2 = 0;
        if (idx < l_s2) {
            ss2 = s2.substr(l_s2 - idx - 1, 1);
            i2 = stoi(ss2);
        }
        sum = op(i1, i2) + carry;
        sum_ = to_string(sum);
        //cout << (n1 + n2 == 1 ? "negated" : "added") << " i1: " << i1 << ", i2: " << i2 << ", sum: " << sum << ", res: " << res << endl;
        if (sum > 9) {
            carry = 1;
            res = sum_.substr(sum_.length() - 1) + res;
        } else if (sum < 0) {
            carry = -1;
            sum = sum + 10;
            sum_ = to_string(sum);
            res = sum_.substr(sum_.length() - 1) + res;
        } else {
            carry = 0;
            res = sum_ + res;
        }
        idx++;
    }
    if (carry != 0)
        res = to_string(carry) + res;
    int offset = 0;
    while (res.substr(offset, 1) == "0") offset++;
    res = sign + res.substr(offset);

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
