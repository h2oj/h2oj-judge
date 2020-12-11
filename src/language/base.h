#pragma once

#include <string>
#include <cstdint>

namespace hoj {

class language {
protected:
    int _result;
public:
    language() {}
    virtual ~language() {};

    virtual int compile(
        const std::string& code_file
    ) = 0;
    virtual int run(
        const std::string& code_file,
        const std::string& input_file,
        const std::string& output_file
    ) = 0;
    int get_result() const { return this->_result; }
};

}
