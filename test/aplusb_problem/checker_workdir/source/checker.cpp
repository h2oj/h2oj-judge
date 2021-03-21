#include <string>
#include <vector>
#include <sstream>

#include <testlib.h>

using namespace std;

std::string &trim(std::string &s) {
    if (s.empty())
        return s;
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

int main(int argc, char *argv[]) {
    registerTestlibCmd(argc, argv);
    std::string strAnswer;

    int n = 0;
    while (!ans.eof()) {
        std::string j = trim(ans.readString());
        if (j == "" && ans.eof())
          break;
        strAnswer = j;
        std::string p = trim(ouf.readString());
        n++;
        if (j != p) {
            quitf(_wa, "Wrong Answer");
        }
    }
    if (n == 1) {
        quitf(_ok, "Accept");
    }

    quitf(_ok, "Accept");
}

