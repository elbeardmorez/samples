#include <cstdlib>
#include <iostream>
#include <sstream>
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

int test(string s) {

    if (s.find(",") == -1) {
        // string chars
        // s: tiesto
        map<char, vector<char>> map_;
        groupby(s, map_);
        cout << "testing string: '" << s << "'" << endl;
        for (auto itr = map_.begin(); itr != map_.end(); ++itr) cout << "k: " << itr->first << ", [v]: " << itr->second.size() << endl;
    } else {
        if (s.find('|') != -1) {
            // objects
            // s: alpha|1,beta|2,beta|5,charlie|10,alpha|100
            vector<obj> objs;
            stringstream ss(s);
            string s_, n, v;
            int idx;
            while (getline(ss, s_, ',')) {
                if ((idx = s_.find('|')) == -1) {
                    cout << "skipping invalid 'x|y' token '" << s_ << "'" << endl;
                    continue;
                }
                n = s_.substr(0, idx);
                v = s_.substr(idx + 1);
                objs.push_back(obj(n, stoi(v)));
            }
            if (objs.size() == 0) {
                cout << "[error] no valid objs" << endl;
                return 1;
            }
            map<string, vector<obj>> map_2;
            string (*f_key)(obj&) = [](obj& x) { return x.name; };
            groupby<vector<obj>, string, obj>(objs, map_2, f_key);
            cout << "testing objects:";
            for (auto itr = objs.begin(); itr != objs.end(); ++itr)
                cout << " " << itr->name << "|" << itr->size;
            cout << endl;
            for (auto itr = map_2.begin(); itr != map_2.end(); ++itr)
                cout << "k: " << itr->first << ", [v]: " << itr->second.size() << endl;
        } else {
            // pointer keys
            // s: alpha,beta,charlie,alpha
            stringstream ss(s);
            string s_;
            vector<string> v;
            while (getline(ss, s_, ','))
                v.push_back(string(s_));
            map<const char *, vector<string>, bool (*)(const char*, const char*)> map_3([](const char* x, const char* y) -> bool { return strcmp(x, y) < 0; });
            groupby(v, map_3, (const char* (*)(string&))([](string& x) -> const char* { return x.data(); }));
            cout << "testing pointer keys:";
            for (auto itr = v.begin(); itr != v.end(); ++itr) cout << " " << *itr; cout << endl;
            for (auto itr = map_3.begin(); itr != map_3.end(); ++itr)
                cout << "k: " << itr->first << ", [v]: " << itr->second.size() << endl;
        }
    }
    return 0;
}

int main(int argc, char** argv) {

    if (argc == 1) {
        // run static tests
        int res;
        res = test("tiesto");
        if (res != 0) return res;
        res = test("alpha|1,beta|2,beta|5,charlie|10,alpha|100");
        if (res != 0) return res;
        res = test("alpha,beta,charlie,alpha");
        if (res != 0) return res;
    } else if (argc == 2) {
        return test(string(argv[1]));
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
