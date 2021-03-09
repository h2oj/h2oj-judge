#include <iostream>

using namespace std;

int tt[1024 * 1024 * 100];

int main() {
    long long a, b;
    cin >> a >> b;
    cout << a + b << endl;

    for (int i = 0; i < sizeof(tt) / sizeof(int); ++i) {
        tt[i] = i;
    }
    cout << "Finished" << endl;
    return 0;
}
