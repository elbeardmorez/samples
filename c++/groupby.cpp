#include <cstdlib>
#include <iostream>
#include <cstring>
#include <vector>
#include <map>

using namespace std;

template <typename X, typename K, typename V>
void groupby(X input, map<K, vector<V>>& groups, K (*f)(V) = [](K x){return x;}) {
    for (auto itr = input.begin(); itr != input.end(); ++itr) {
        K k = f(*itr);
        if (groups.find(k) == groups.end())
            groups[k].reserve(1);
        groups[k].push_back(*itr);
    }
}

int main(int argc, char** argv) {

    int argc_required = 1;
    if (argc != 1 + argc_required) {
        cout << "[error] missing arg" << (argc - argc_required != 1 ? "s, " : ", ") << argc_required << " required" << endl;
        return 1;
    }

    string s = string(argv[1]);
    map<char, vector<char>> map_;
    groupby(s, map_);
    for (auto itr = map_.begin(); itr != map_.end(); ++itr) cout << "k: " << itr->first << ", [v]: " << itr->second.size() << endl;

    return 0;
}

// explicitly instantiate templates for debugging
template class std::vector<char>;
template class std::map<char, vector<char>>;
