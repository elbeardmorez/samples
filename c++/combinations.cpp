#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

template<class iterator_type>
class combination_generator {
    iterator_type first, last;
    std::vector<bool> use;
    unsigned int r;
    typedef typename std::iterator_traits<iterator_type>::value_type element_type;

    public:
        combination_generator(iterator_type first_, iterator_type last_, unsigned int r_) : first(first_), last(last_), r(r_) {
            use.resize(std::distance(first, last), false);
            int n = use.size();
            if (r > n) {
                r = n;
                cout << "[info] nCr requires r <= n, setting r = " << n << endl;
            }
            std::fill(use.begin(), use.begin() + r, true);
        }
        template<class output_iterator>
        bool operator()(output_iterator result) {
            iterator_type c = first;
            for (unsigned int i = 0; i < use.size(); ++i, ++c) {
                if (use[i])
                    *result++ = *c;
            }
            return std::prev_permutation(use.begin(), use.end());
        }
};
template<class iterator_type>
combination_generator<iterator_type> make_combination_generator(iterator_type first, iterator_type last, unsigned int r) {
    return combination_generator<iterator_type>(first, last, r);
}

int main(int argc, char** argv) {
    int argc_required = 2;
    if (argc <  1 + argc_required) {
        cout << "[error] " << argc_required << " or more arg" << (argc_required != 1 ? "s" : "") << "required" << endl;
        exit(1);
    }
    if (argc < 2) {
        cout << "[error] 2 or more args required" << endl;
        exit(1);
    }
    auto generator = make_combination_generator(argv + 1, argv + argc - 1, stoi(argv[argc - 1]));
    std::ostream_iterator<std::string> it(std::cout, " ");
    while(generator(it))
        std::cout << '\n';
    return 0;
}
