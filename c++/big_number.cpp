#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>

using namespace std;

string add_(string s1, string s2) {

    string res;

    string s1_orig = string(s1);
    string s2_orig = string(s2);

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
    int append = 0;
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

    cout << "added s1: " << s1_orig << ", s2: " << s2_orig << ", res: " << res << "\n" << endl;
    return res;
}

string subtract_(string s1, string s2) {

    string res;

    string s1_orig = string(s1);
    string s2_orig = string(s2);

    if (s2.substr(0, 1) == "-")
        s2 = s2.substr(1);
    else
        s2 = "-" + s2;
    res = add_(s1, s2);

    cout << "subtracted s1: " << s1_orig << ", s2: " << s2_orig << ", res: " << res << "\n" << endl;
    return res;
}

string multiply_(string s1, string s2) {

    string res;

    string s1_orig = string(s1);
    string s2_orig = string(s2);

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
    int dot = res.find(".");
    if (dot > -1) {
        l = res.length() - 1;
        string ss;
        while (l > 1) {
            ss = res.substr(l, 1);
            if (ss == ".") {
               l--;
               break;
            } else if (ss != "0")
                break;
            l--;
        }
        res = res.substr(0, l);
    }

    // replace any sign
    res = sign + res;

    cout << "multiplied s1: " << s1_orig << ", s2: " << s2_orig << ", res: " << res << "\n" << endl;
    return res;
}

void test_() {
    map<string, int> results{{"pass", 0}, {"fail", 0}};
    auto test = [&](string op, string (*fn)(string, string), string n1, string n2, string expected) {
        string res = fn(n1, n2);
        string sres = res == expected ? "pass" : "fail";
        cerr << "test | " << n1 << " " << op << " " << n2 << " | [" << sres << "] expected: " << expected << ", res: " << res << endl;
        return sres;
    };
    results[test("+", &add_, "40", "10", "50")]++;
    results[test("+", &add_, "40", "-10", "30")]++;
    results[test("+", &add_, "-40", "10", "-30")]++;
    results[test("+", &add_, "40", "-51.2", "-11.2")]++;
    results[test("-", &add_, "40", "50", "-10")]++;
    results[test("-", &add_, "-1", "-0.5", "-0.5")]++;
    results[test("-", &add_, "40", "51.2", "-11.2")]++;
    results[test("*", &multiply_, "-10", "-10", "100")]++;
    results[test("*", &multiply_, "-10", "10", "-100")]++;
    results[test("*", &multiply_, "10", "-10", "-100")]++;
    results[test("*", &multiply_, "10", "10", "100")]++;

    int res_total = results["pass"] + results["fail"];
    char buf[50];
    sprintf(buf, "%.02f", results["pass"] / (double)res_total);
    string res_success = buf;
    cerr << results["pass"] << " [/" << res_total << "] passes - " << res_success << "%" << endl;
}

int main(int argc, char** argv) {

    if (argc >= 2 && string(argv[1]) == "test") {
        cerr << "running tests:" << endl;
        test_();
        return 0;
    }

    int argc_required = 3;
    if (argc != 1 + argc_required) {
        cout << "[error] missing arg" << (argc - argc_required != 1 ? "s, " : ", ") << argc_required << " required" << endl;
        cout << "[error] syntax: bignumber OPERATOR number1 number2" << endl;
        exit(1);
    }

    string op = string(argv[1]);
    string s1 = string(argv[2]);
    string s2 = string(argv[3]);

    string res;
    if (op == "+" ) {
        cout << "[info] adding | " << s1 << " + " << s2 << endl;
        res = add_(s1, s2);
    } else if (op == "-" ) {
        cout << "[info] subtracting | " << s1 << " - " << s2 << endl;
        res = subtract_(s1, s2);
    } else if (op == "*" ) {
        cout << "[info] multiplying | " << s1 << " * " << s2 << endl;
        res = multiply_(s1, s2);
    }

    cout << res << endl;
    return 0;
}
