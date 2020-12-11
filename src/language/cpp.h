#pragma once

#include "base.h"

namespace hoj {

class language_cpp : public language {
public:
    language_cpp();
    virtual ~language_cpp();
    virtual int run(
        const std::string& code_file,
        const std::string& input_file,
        const std::string& output_file
    ) override;
};

}
