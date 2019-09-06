#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <sstream>

using namespace std;

void clean_(string &);
string round_(string &, int = 10);

static int debug = 0;

string add_(string s1, string s2) {

    // subtraction is not commutitive
    string res;

    clean_(s1);
    clean_(s2);

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
    if (n1 + n2 == 1) {
        if (debug >= 2) cout << "negating" << endl;
        op = std::minus<int>();
    } else {
        if (debug >= 2) cout << "adding" << endl;
        op = std::plus<int>();
    }
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
            if (stoi(s1.substr(l, 1)) > stoi(s2.substr(l, 1)))
                break;
            else if (stoi(s1.substr(l, 1)) < stoi(s2.substr(l, 1))) {
                switch_ = 1;
                break;
            }
        }
    } else if (l_s2 > l_s1)
        switch_ = 1;

    // ultimate sign
    string sign = "";
    int xover = 0;
    if (n1 == 1 && n2 == 1)
        sign = "-";
    else if (n1 == 0 && n2 == 1 && switch_) {
        // s1 is positive, but s2 is negative and is bigger in magnitude
        sign = "-";
        xover = 1;
    } else if (n1 == 1 && n2 == 0 && switch_) {
        // s1 is negative, but s2 is positive and is bigger in magnitude
        xover = 1;
    } else if (n1 == 1 && n2 == 0 && !switch_) {
        // s1 is negative, s2 is positive but is smaller in magnitude
        sign = "-";
    }
    if (debug >= 1) cout << "n1: " << n1 << ", n2: " << n2 << ", switch: " << switch_ << ", xover: " << xover << endl;

    int l;
    int carry = 0;
    int idx = 0;
    int i1 = 0;
    int i2 = 0;
    int sum;
    string sum_;
    string ss1;
    string ss2;
    if (debug >= 1) cout << "adding s1: " << s1 << ", s2: " << s2 << ", l_s1: " << l_s1 << ", l_s2: " << l_s2 << ", xover: " << xover << endl;
    while (idx < max(l_s1, l_s2)) {
        i1 = 0;
        if (idx < l_s1) {
            ss1 = s1.substr(l_s1 - idx - 1, 1);
            if (ss1 == ".") {
                res = "." + res;
                idx++;
                continue;
            }
            i1 = stoi(ss1);
        }
        i2 = 0;
        if (idx < l_s2) {
            ss2 = s2.substr(l_s2 - idx - 1, 1);
            if (ss2 == ".") {
                res = "." + res;
                idx++;
                continue;
            }
            i2 = stoi(ss2);
        }
        sum = op(i1, i2) + carry;
        sum_ = to_string(sum);
        if (debug >= 3) cout << (n1 + n2 == 1 ? "negated" : "added") << " i1: " << i1 << ", i2: " << i2 << ", carry: " << carry << ", sum: " << sum << ", res: " << res << endl;
        carry = 0;
        if (sum == 0) {
            sum_ = to_string(sum);
            res = sum_ + res;
            if (debug >= 2) cout << "[0] res: " << res << endl;
        } else if (sum > 9) {
            carry = 1;
            sum_ = to_string(sum);
            res = sum_.substr(sum_.length() - 1) + res;
            if (debug >= 2) cout << "[1] res: " << res << endl;
        } else if (sum < -9) {
            carry = -1;
            sum_ = to_string(sum);
            res = sum_.substr(sum_.length() - 1) + res;
            if (debug >= 2) cout << "[2] res: " << res << endl;
        } else if (sum < 0) {
            carry = 0;
            if (xover && n1 != n2) {
                sum = sum - 10;
                sum_ = to_string(sum);
                res = sum_.substr(sum_.length() - 1) + res;
                if (debug >= 2) cout << "[3] res: " << res << endl;
            } else if (!xover && n1 != n2) {
                sum = sum + 10;
                carry = -1;
                sum_ = to_string(sum);
                res = sum_ + res;
                if (debug >= 2) cout << "[5] res: " << res << endl;
            } else {
                sum_ = to_string(sum);
                res = sum_.substr(1) + res;
                if (debug >= 2) cout << "[7] res: " << res << endl;
            }
        } else if (sum > 0) {
            carry = 0;
            if (xover && n1 != n2) {
                carry = 1;
                sum = sum - 10;
                sum_ = to_string(sum);
                res = sum_.substr(sum_.length() - 1) + res;
                if (debug >= 2) cout << "[8] res: " << res << endl;
            } else {
                sum_ = to_string(sum);
                res = sum_ + res;
                if (debug >= 2) cout << "[10] res: " << res << endl;
            }
        }
        idx++;
    }

    clean_(res);

    // replace any sign
    if (res != "0")
        res = sign + res;

    if (debug >= 1) cout << "added s1: " << s1_orig << ", s2: " << s2_orig << ", res: " << res << endl;
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

    if (debug >= 1) cout << "subtracted s1: " << s1_orig << ", s2: " << s2_orig << ", res: " << res << endl;
    return res;
}

string multiply_(string s1, string s2) {

    int debug_ = debug;
    string res = "";

    clean_(s1);
    clean_(s2);

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

    int dot1 = s1.find(".");
    int dot2 = s2.find(".");
    if (dot1 > -1) {
        s1.erase(dot1, 1);
        l_s1--;
    } else
        dot1 = l_s1;

    if (dot2 > -1) {
        s2.erase(dot2, 1);
        l_s2--;
    } else
        dot2 = l_s2;

    if (debug > 5) cout << "multiply_: s1 " << s1 << ", s2: " << s2 << endl;

    int idx = 0;
    int v;

    int digits = (l_s1 - 1) + (l_s2 - 1) + 1;
    vector<int> res_(digits, 0);
    int l_s = res_.size();

    if (debug > 5) cout << digits << endl;

    int i1;
    int i2;
    for (int l = l_s1 - 1; l >= 0; l--) {
        i1 = stoi(s1.substr(l, 1));
        if (i1 == 0) continue;
        for (int l2 = l_s2 - 1; l2 >= 0; l2--) {
            i2 = stoi(s2.substr(l2, 1));
            if (i2 == 0) continue;
            idx = l_s - (l_s1 - l + l_s2 - l2) + 1;
            v = i1 * i2;
            res_[idx] += v;
            if (debug > 5) cout << "l: " << l << ", l2: " << l2 << ", idx: " << idx << ", v: " << v << endl;
        }
    }
    for (int l = l_s - 1; l >= 0; l--) {
        if (res_[l] > 9 && l > 0) {
            if (debug >= 6) cout << "idx: " << l << ", v: " << res_[l - 1] << ", added: " << res_[l] / 10 << endl;
            res_[l - 1] += (res_[l] / 10);
            res_[l] = res_[l] % 10;
        }
    }
    stringstream ss;
    for (auto itr = res_.begin(); itr != res_.end(); itr++)
        ss << *itr;
    res = ss.str();

    int dot = dot1 - 1 + dot2 - 1 + 1;
    if (dot < l_s)
        res = res.substr(0, dot) + "." + res.substr(dot);

    res = round_(res);

    return sign + res;
}

string divide_(string s1, string s2) {

    int debug_ = debug;
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

    // strip leading 0s
    int l = 0;
    while (l < l_s1 && s1.substr(l, 1) == "0")
        l++;
    s1 = s1.substr(l);
    if (s1.substr(0, 1) == ".") s1 = "0" + s1;
    l = 0;
    while (l < l_s2 && s2.substr(l, 1) == "0")
        l++;
    s2 = s2.substr(l);
    if (s2.substr(0, 1) == ".") s2 = "0" + s2;

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
    if (debug >= 3) cout << "s1: " << s1 << ", s2: " << s2 << endl;

    // ultimate sign
    string sign = n1 == 1 || n2 == 1 ? "-" : "";

    // find reciprocal
    string s2_ = string(s2);

    clean_(s2_);

    /*
    shift most significant to 1x10-1 digit
    e.g. 0.05 -> 0.5 shift 1
       . 100 -> 1 shift -3
    */
    int shift = 0;
    int dot = s2_.find(".");
    if (dot == -1)
        shift = -(s2_.length());
    else {
        if (s2_.substr(0, 1) == "0") {
            shift = 0;
            s2_ = s2_.substr(2);
            while (s2_.substr(0, 1) == "0") {
                s2_ = s2_.substr(1);
                shift++;
            }
        } else {
            shift = -dot;
            s2_ = s2_.substr(0, dot) + s2_.substr(dot + 1);
        }
    }
    s2_ = "0" + s2_;
    if (debug >= 2) cerr << "s2_: " << s2_ << ", shift: " << shift << endl;

    int l_s2_ = s2_.length();

    string one = "1";
    string reciprocal = "";
    string divisor = s2_;
    int l_divisor = s2_.length();
    string divisor_ = "";
    int l_divisor_;
    string dividend = one;
    int l_dividend = dividend.length();
    while (l_dividend < l_divisor) {
        dividend += "0";
        l_dividend++;
    }
    string remainder = dividend;
    int l_remainder = remainder.length();
    string remainder_ = "";
    int l_remainder_;
    int idx = 0; //remainder.length() - 1;
    int idx2 = 0; // divisor / remainder substrings
    int ip;
    string buf;
    string buf2;
    int max_digits = 150;
    if (debug >= 2) cout << "reciprocal: 1 / s2_, dividend: " << dividend << ", divisor: " << divisor << ", shift: " << shift << endl;

    // idx is fixed to the dividend!
    while (idx <= l_dividend) {
        if (debug >= 3) cout << "reciprocal: " << reciprocal << endl;

        // work on next dividend digit

        if (reciprocal.length() - shift == max_digits)
            break;

        if (debug >= 3) cout << "loop, remainder: " << remainder << endl;

        // get head estimate
        if (l_remainder < l_dividend) {
            // extend dividend for further precision
            if (debug >= 4) cout << "extending dividend" << endl;
            dividend += "0";
            l_dividend++;
            // bring down next dividend digit to exend remainder
            remainder += dividend.substr(idx2, 1);
            l_remainder++;
        }
        idx++;
        idx2 = 0;
        remainder_ = "";
        l_remainder_ = 0;
        divisor_ = "";
        l_divisor_ = 0;
        while (idx2 < min(5, l_remainder)) {
            // add to remainder 'head' until valid for estimate
            if (debug >= 3) cout << "l_remainder: " << l_remainder << ", idx2: " << idx2 << endl;
            remainder_ += remainder.substr(idx2, 1);
            l_remainder_++;
            if (idx2 < l_divisor) {
                divisor_ += divisor.substr(idx2, 1);
                l_divisor_++;
            }
            idx2++;
        }
        if (debug >= 3) cout << "remainder: " << remainder <<  ", divisor_: " << divisor_ << ", remainder_: " << remainder_ << endl;
        if (divisor_ == "0") {
            continue;
            idx2++;
        }
        ip = stoi(remainder_) / stoi(divisor_);  // estimate
        debug = 0;
        buf = subtract_(remainder, multiply_(to_string(ip), divisor));  // estimate result, full length
        debug = debug_;
        if (debug >= 4) cout << "ip: " << ip << ", buf: " << buf << endl;
        if (buf.substr(0, 1) != "-") {
            if (debug >= 4) cout << "initial estimate ip: " << ip << " is under, closing gap" << endl;
            while (buf.substr(0, 1) != "-") {
                ip++;
                if (debug >= 3) cout << "subtracting buf: " << buf << ", divisor: " << divisor << endl;
                debug = 0;
                buf = subtract_(buf, divisor);
                debug = debug_;
                if (debug >= 3) cout << "estimate ip: " << ip << ", buf: " << buf << endl;
            }
            if (ip > 0) {
                debug = 0;
                buf = add_(buf, divisor);
                debug = debug_;
                ip--;
            }
        } else if (buf.substr(0, 1) == "-") {
            if (debug >= 4) cout << "initial estimate ip: " << ip << " is over, closing gap" << endl;
            while (buf.substr(0, 1) == "-") {
                ip--;
                if (debug >= 3) cout << "adding buf: " << buf << ", divisor: " << divisor << endl;
                debug = 0;
                buf = add_(buf, divisor);
                debug = debug_;
                if (debug >= 3) cout << "estimate ip: " << ip << ", buf: " << buf << endl;
            }
        }
        reciprocal += to_string(ip);
        if (ip > 9) shift++;

        remainder = string(buf);
        l_remainder = remainder.length();
        if (remainder == "0")
            break;

        if (debug >= 3) cout << "integer part: " << ip << ", remainder: " << remainder << endl;
    }
    if (debug >= 2) cerr << "reciprocal: " << reciprocal << endl;
    if (shift != 0) {
        int l_reciprocal = reciprocal.length();
        if (debug >= 1) cout << "unshifting reciprocal: " << reciprocal << " by "  << shift << " d.p." << endl;
        int shift_ = abs(shift);
        for (l = 0; l < shift_; l++) {
            if (shift > 0)
                reciprocal += "0";
            else
                reciprocal = "0" + reciprocal;
            if (debug >= 3) cout << "reciprocal: " << reciprocal << endl;
        }
        if (shift < 0) {
            reciprocal = reciprocal.substr(0, 1) + "." + reciprocal.substr(1);
        } else
            reciprocal = reciprocal.substr(0, shift) + "." + reciprocal.substr(shift);
        if (debug >= 3) cout << "reciprocal: " << reciprocal << endl;
    }
    if (debug >= 2) cerr << "reciprocal shifted: " << reciprocal << endl;

    if (debug >= 1) cout << "multiplying s1: " << s1 << " by 1/s2: " << reciprocal << endl;
    debug = 0;
    res = multiply_(s1, reciprocal);
    debug = debug_;

    // replace any sign
    if (res != "0")
        res = sign + res;

    res = round_(res);
    clean_(res);

    if (debug >= 1) cout << "divided s1: " << s1_orig << ", s2: " << s2_orig << ", res: " << res << endl;
    return res;
}

string round_(string &s, int sf) {
    string rounded = "";
    bool round = false;

    clean_(s);
    string sign = "";
    if (s.substr(0, 1) == "-") {
        sign = "-";
        s = s.substr(1);
    }
    int dot = s.find(".");
    if (dot == -1)
        return sign + s;
    if (s.length() < sf)
        return sign + s;
    int l = 0;
    string ss;
    while (true) {
        ss = s.substr(l, 1);
        if (ss != "0" && ss != ".")
            break;
        l++;
    }
    if (s.length() < sf)
        return sign + s;
    s = s.substr(0, l + sf);

    if (debug >= 4) cout << "round: " << s << ", sf: " << sf << endl;

    for (auto itr = s.rbegin(); itr < s.rend(); itr++) {
        string c(1, *itr);
        if (debug >= 6) cout << "c: " << c << endl;
        if (&(*itr) == &(s.back())) {
            if (stoi(c) >= 5)
                round = true;
            rounded = "0";
        } else if (*itr == '.' || *itr == '-' || !round)
            rounded = c + rounded;
        else if (stoi(c) == 9)
            rounded = "0" + rounded;
        else {
            rounded = to_string(stoi(c) + 1) + rounded;
            round = false;
        }
        if (debug >= 6) cout << "rounded: " << rounded << endl;
    }
    if (round) rounded = "1" + rounded;
    clean_(rounded);
    return sign + rounded;
}

void clean_(string &s) {
    // strip leading / trailing 0s
    int l = 0;
    string sign = "";
    if (s.substr(0, 1) == "-") {
        sign = "-";
        s = s.substr(1);
    }

    while (l < s.length() - 1 && s.substr(l, 1) == "0")
        l++;
    s = s.substr(l);
    if (s.substr(0, 1) == ".") s = "0" + s;
    int dot = s.find(".");
    if (dot > -1) {
        l = s.length() - 1;
        string ss;
        if (debug >= 4) cout << "clean_ testing [" << l << "] " << ss << endl;
        while (l > 0) {
            ss = s.substr(l, 1);
            if (ss == ".") {
                l--;
                break;
            } else if (ss != "0")
                break;
            l--;
        }
        s = s.substr(0, l + 1);
    }
    s = sign + s;
    if (debug >= 4) cout << "cleaned: '" << s << "'" << endl;
}

void test_() {
    map<string, int> results{{"pass", 0}, {"fail", 0}};
    auto test = [&](string op, string (*fn)(string, string), string n1, string n2, string expected) {
        string res = fn(n1, n2);
        string sres = res == expected ? "pass" : "fail";
        cerr << "test | " << n1 << " " << op << " " << n2 << " | [" << sres << "] expected: " << expected << ", res: " << res << "\n" << endl;
        return sres;
    };
    results[test("+", &add_, "40", "10", "50")]++;
    results[test("+", &add_, "40", "-10", "30")]++;
    results[test("+", &add_, "-40", "10", "-30")]++;
    results[test("+", &add_, "40", "-51.2", "-11.2")]++;
    results[test("+", &add_, "-2", "12", "10")]++;
    results[test("-", &subtract_, "40", "50", "-10")]++;
    results[test("-", &subtract_, "-1", "-0.5", "-0.5")]++;
    results[test("-", &subtract_, "-0.5", "-1", "0.5")]++;
    results[test("-", &subtract_, "40", "51.2", "-11.2")]++;
    results[test("-", &subtract_, "100", "12", "88")]++;
    results[test("-", &subtract_, "-100", "-12", "-88")]++;
    results[test("*", &multiply_, "-10", "-10", "100")]++;
    results[test("*", &multiply_, "-10", "10", "-100")]++;
    results[test("*", &multiply_, "10", "-10", "-100")]++;
    results[test("*", &multiply_, "10", "10", "100")]++;
    results[test("*", &multiply_, "10.2", "10", "102")]++;
    results[test("*", &multiply_, "0.2", "0.2", "0.04")]++;
    results[test("*", &multiply_, "0.2", "-0.2", "-0.04")]++;
    results[test("/", &divide_, "81", "9", "9")]++;
    results[test("/", &divide_, "0.2", "-0.2", "-1")]++;
    results[test("/", &divide_, "5", "2.5", "2")]++;
    results[test("/", &divide_, "-105", "10", "-10.5")]++;
    results[test("/", &divide_, "-150", "15", "-10")]++;
    results[test("/", &divide_, "24", "4", "6")]++;

    int res_total = results["pass"] + results["fail"];
    char buf[50];
    sprintf(buf, "%0.02f", 100 * results["pass"] / (double)res_total);
    string res_success = buf;
    cerr << results["pass"] << " [/" << res_total << "] passes - " << res_success << "%" << endl;
}

int main(int argc, char** argv) {

    auto _ = getenv("DEBUG");
    if (_ != NULL) debug = stoi(_);

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
    } else if (op == "/" ) {
        cout << "[info] dividing | " << s1 << " / " << s2 << endl;
        res = divide_(s1, s2);
    }

    cout << res << endl;
    return 0;
}
