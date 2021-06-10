#ifndef UON_HPP_INCLUDED
#define UON_HPP_INCLUDED

#include "UON_Result.hpp"
#include "UON_Scanner.hpp"

namespace {

template<class T>
T exception(const std::string& reason, UON::Internal::Scanner& sc) {
    UON::Result::success = false;
    UON::Result::error = reason + " -> " + sc.getPosition();
    return T();
}
template<class T>
T exception(std::string&& reason, UON::Internal::Scanner& sc) {
    return exception<T>(reason, sc);
}

using Pair = std::pair<std::string, std::string>;

Pair getPair(UON::Internal::Scanner& sc) {
    // ========================================
    // Get indentifier
    if (sc.getToken().type != UON::Internal::Token_Identifier)
        return exception<Pair>("Syntax error, invalid identifier", sc);
    std::string k_ = sc.getToken().text;
    sc.nextToken();
    // Skip equal
    if (sc.getToken().type != UON::Internal::Token_Equal) 
        return exception<Pair>("Syntax error, need \'=\'", sc);
    sc.nextToken();
    // Get string
    if (sc.getToken().type != UON::Internal::Token_String)
        return exception<Pair>("Syntax error, invalid string", sc);
    std::string v_ = sc.getToken().text;
    sc.nextToken();
    return {k_, v_};
}

UON::Node getBlock(UON::Internal::Scanner&);
UON::Node getBlockBody (UON::Internal::Scanner& sc, UON::Node& nd) {
    bool f_ = true;
    nd.body = true;
    do {
        switch (sc.getToken().type) {
            // Check right cirly bracket
            case (UON::Internal::Token_RCBracket):
                return nd;
            // Get block
            case (UON::Internal::Token_LSBracket): {
                auto b_ = getBlock(sc);
                if (!UON::Result::success)
                    return UON::Node();
                nd.children.emplace_back(b_);
                if (!b_.body)
                    return nd;
                break;
            }
            // Get attribute
            case (UON::Internal::Token_Less): {
                sc.nextToken();
                // Get indentifier
                if (sc.getToken().type != UON::Internal::Token_Identifier) 
                    return exception<UON::Node>("Syntax error, invalid identifier", sc);
                std::string t_ = sc.getToken().text;
                sc.nextToken();
                // Check greater
                if (sc.getToken().type != UON::Internal::Token_Greater)
                    return exception<UON::Node>("Sytax error, invalid attribute", sc);
                sc.nextToken();
                // Check left cirly bracket
                if (sc.getToken().type != UON::Internal::Token_LCBracket)
                    return exception<UON::Node>("Sytax error, invalid attribute", sc);
                sc.nextToken();
                // Get attribute
                while (sc.getToken().type != UON::Internal::Token_RCBracket) {
                    auto p_ = getPair(sc);
                    if (!UON::Result::success || sc.getToken().type != UON::Internal::Token_Semicolon)
                        return UON::Node();
                    nd[t_][p_.first] = p_.second;
                    sc.nextToken();
                }
                break;
            }
            // Exception
            default:
                exception<int>("Syntax error, invalid block", sc);
                f_ = false;
        }
        sc.nextToken();
    } while (f_);
    return UON::Node();
}

UON::Node getBlock(UON::Internal::Scanner& sc) {
    UON::Node r_;
    // Skip left square bracket
    if (sc.getToken().type != UON::Internal::Token_LSBracket)
        return exception<UON::Node>("Syntax error, invalid block", sc);
    sc.nextToken();
    // Get indentifier
    if (sc.getToken().type != UON::Internal::Token_Identifier)
        return exception<UON::Node>("Syntax error, invalid tag", sc);
    r_.tag = sc.getToken().text;
    sc.nextToken();
    // Get attribute
    while (sc.getToken().type != UON::Internal::Token_RSBracket) {
        // Get kvp
        auto p_ = getPair(sc);
        if (!UON::Result::success)
            return UON::Node();
        r_["attr"][p_.first] = p_.second;
    }
    sc.nextToken();
    // Check left cirly bracket
    if (sc.getToken().type != UON::Internal::Token_LCBracket) {
        if (sc.getToken().type != UON::Internal::Token_LSBracket && 
            sc.getToken().type != UON::Internal::Token_RCBracket)
            return exception<UON::Node> ("Syntax error, invalid block", sc);
        return r_;
    }
    sc.nextToken();
    // Get body
    return getBlockBody(sc, r_);
}

}

namespace UON {

using ParseType = enum pt_ {
    Parse_File,
    Parse_String
};

bool parse(const std::string& str, ParseType tp) {
    Internal::Scanner sc_;
    if (!sc_.open(str, tp == Parse_File ? Internal::Input_File : Internal::Input_String)) {
        Result::error = "Cannot open file";
        return false;
    }
    UON::Result::success = true;
    sc_.nextToken();
    Result::root = ::getBlock(sc_);
    return Result::success;
}
bool parse(std::string&& str, ParseType tp) {
    return parse(str, tp);
}

}

#endif