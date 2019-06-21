#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

template <typename T>
double elapsed(chrono::time_point<T> tp_last) {
    chrono::duration<double> tp_diff = T::now() - tp_last;
    return chrono::duration_cast<chrono::milliseconds>(tp_diff).count();
}

int main(int argc, char** argv) {
    int argc_required = 1;
    if (argc != 1 + argc_required) {
        cout << "[error] " << argc_required << " arg" << (argc_required != 1 ? "s" : "") << "required" << endl;
        exit(1);
    }
    int delay = atoi(argv[1]);
    //auto tp_last = chrono::high_resolution_clock::now();
    //auto tp_last = chrono::system_clock::now();
    auto tp_last = chrono::steady_clock::now();
    this_thread::sleep_for(chrono::seconds(delay));
    cout << "[debug|" << elapsed(tp_last) << "ms] elapsed" << endl;

    return 0;
}
