#include <iostream>
#include <thread>
using namespace std;

int main() {
    new int[1024*1024*1024];
    long long a, b;
    cin >> a >> b;
    cout << a + b << endl;
    //while (true);
    //this_thread::sleep_for(2s);
    return 0;
}
