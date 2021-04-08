#include <iostream>
#include <fstream>
#include <string>

#define ACCEPTED 0
#define WRONG_ANSWER 1

bool is_empty(char a) {
    switch (a) {
        case ' ':
        [[fallthrough]]; case '\n':
        [[fallthrough]]; case '\r':
        [[fallthrough]]; case '\t':
            return true;
        default:
            return false;
    }
}

std::string read_line(std::istream &in) {
    if (in.eof()) return "";
    std::string result;
    getline(in, result);
    std::size_t beg = 0, end = result.size() - 1;
    while (is_empty(result[beg]) && beg <= end) ++beg;
    while (is_empty(result[end]) && end >= beg) --end;
    if (beg > end) return "";
    return result.substr(beg, end - beg + 1);
}

// path/to/checker input output answer
int main(int argc, char *argv[]) {
    if (argc < 4) return -1;

    std::ifstream out, ans;
    out.open(argv[2]);
    ans.open(argv[3]);

    if (!out.is_open() || !ans.is_open()) return -2;

    std::string str_out, str_ans;
    int line = 1;
    while (!out.eof() || !ans.eof()) {
        str_out = read_line(out);
        str_ans = read_line(ans);
        if (str_out != str_ans) {
            std::clog << "Wrong answer on line " << line << "." << std::endl;
            return WRONG_ANSWER;
        }
        ++line;
    }

    std::clog << "Accepted." << std::endl;
    return ACCEPTED;
}
