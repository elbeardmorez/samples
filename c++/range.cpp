#include <cstdlib>
#include <iostream>
#include <sstream>
#include <cstring>
#include <vector>
#include <map>

using namespace std;

template <typename T>
vector<T> range(T n1, T n2, T x) {
    vector<T> res;
    res.reserve(T(n2 - n1 / x));
    for (T l = n1; l < n2; l += x) res.push_back(l);
    return res;
}

template <typename T>
vector<T> range(T n1, T n2) {
    return range(n1, n2, T(1));
}

template <typename T>
vector<T> range(T n2) {
    return range(T(0), n2, T(1));
}

void help() {
    cout << "SYNTAX:\n\n  range N  <- [0 .. N-1]\n  range N1 N2 <- [N1 .. N2-1]\n  range N1 N2 x <- [N1 .. N2-1] ∀ n; N1 <= n < N2; n % x = 0\n" << endl;
}

int main(int argc, char** argv) {

    vector<long> res;
    switch (argc) {
        case 1: help(); cout << "[error] insufficient args" << endl; return 1;
        case 2: res = range(stol(argv[1])); break;
        case 3: res = range(stol(argv[1]), stol(argv[2])); break;
        case 4: res = range(stol(argv[1]), stol(argv[2]), stol(argv[3])); break;
        default: help(); cout << "[error] too many arg(s)" << endl; return 1;
    }
    auto itr = res.begin();
    auto itr2 = prev(res.end());
    cout << "["; while (itr < itr2) cout << *(itr++) << " "; cout << *itr << "]" << endl;

    return 0;
}
