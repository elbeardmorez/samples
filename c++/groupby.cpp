#include <cstdlib>
#include <iostream>
#include <cstring>
#include <vector>
#include <map>

using namespace std;

class obj {
    public:
        string name;
        int size;

        obj() {}
        obj(string name_, int size_ = 10) {
            this->name = name_;
            this->size = size_;
        }
};

template <typename I, typename K, typename V, typename... Mx>
void groupby(I input, map<K, vector<V>, Mx...> &groups, K (*f)(V&) = [](V& x) -> K { return x; }) {
    for (auto itr = input.begin(); itr != input.end(); ++itr) {
        K k = f(*itr);
        auto itr2 = groups.find(k);
        if (itr2 == groups.end()) {
            //cout << "adding new key: '" << k << "'" << endl;
            groups[k] = vector<V> {*itr};
        } else {
            //cout << "searched for k: " << k << ", found: " << itr2->first << endl;
            //cout << "appending to existing key: '" << k << "'" << endl;
            itr2->second.emplace_back(*itr);
        }
    }
}

void tests() {

    // string
    string s = "tiesto";
    map<char, vector<char>> map_;
    groupby(s, map_);
    cout << "testing string: '" << s << "'";
    for (auto itr = map_.begin(); itr != map_.end(); ++itr) cout << "k: " << itr->first << ", [v]: " << itr->second.size() << endl;

    // objects
    string s2[] = {"alpha", "beta", "charlie", "alpha"};
    int i2[] = {1, 2, 3, 10};
    vector<obj> objs(4);
    for (int l = 0; l < objs.size(); l++) objs[l] = obj(s2[l], i2[l]);
    map<string, vector<obj>> map_2;
    string (*f_key)(obj&) = [](obj& x) { return x.name; };
    groupby<vector<obj>, string, obj>(objs, map_2, f_key);
    cout << "testing objects:";
    for (auto itr = objs.begin(); itr != objs.end(); ++itr)
        cout << " " << itr->name << "|" << itr->size;
    cout << endl;
    for (auto itr = map_2.begin(); itr != map_2.end(); ++itr)
        cout << "k: " << itr->first << ", [v]: " << itr->second.size() << endl;

    // pointer keys
    vector<const char*> v{"alpha", "beta", "charlie", "alpha"};
    map<const char *, vector<const char*>, bool (*)(const char*, const char*)> map_3([](const char* x, const char* y) -> bool { return strcmp(x, y) < 0; });
    groupby<vector<const char*>, const char*>(v, map_3);
    //groupby(v, map_3);
    cout << "testing pointer keys:";
    for (auto itr = v.begin(); itr != v.end(); ++itr) cout << " " << *itr; cout << endl;
    for (auto itr = map_3.begin(); itr != map_3.end(); ++itr)
        cout << "k: " << itr->first << ", [v]: " << itr->second.size() << endl;
}

int main(int argc, char** argv) {

    if (argc == 1)
        // run static tests
        tests();
    else if (argc == 2) {
        string s = string(argv[1]);
        map<char, vector<char>> map_;
        groupby(s, map_);
        for (auto itr = map_.begin(); itr != map_.end(); ++itr) cout << "k: " << itr->first << ", [v]: " << itr->second.size() << endl;
    } else {
        cout << "[error] excess args!" << endl;
        return 1;
    }

    return 0;
}

// explicitly instantiate templates for debugging
template class std::vector<char>;
template class std::vector<obj>;
template class std::vector<const char*>;
template class std::map<string, vector<obj>>;
template class std::map<char, vector<obj>>;
template class std::map<const char*, vector<const char*>>;
