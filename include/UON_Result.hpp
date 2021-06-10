#ifndef UON_RESULT_HPP_INCLUDED
#define UON_RESULT_HPP_INCLUDED

#include <string>

#include "UON_Node.hpp"

namespace UON {

class Result {
    public:
        static bool success;
        static Node root;
        static std::string error;

    private:
        Result() = default;
        Result(const Result&) = delete;
        ~Result() = default;
        Result& operator=(const Result&) = delete;
};

bool Result::success;
Node Result::root;
std::string Result::error;

}

#endif