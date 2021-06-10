#ifndef UON_TOKEN_HPP_INCLUDED
#define UON_TOKEN_HPP_INCLUDED

#include <string>
#include <algorithm>

namespace UON {
namespace Internal {

using TokenType = enum tt_ {
    Token_Null,
    Token_LSBracket,
    Token_RSBracket,
    Token_LCBracket,
    Token_RCBracket,
    Token_Less,
    Token_Greater,
    Token_Equal,
    Token_Semicolon,
    Token_Identifier,
    Token_String
};

struct Token {
    TokenType type;
    std::string text;

    Token(): type(Token_Null), text("") {}
    Token(TokenType tp, const std::string& tx): type(tp), text(tx) {}
    Token(TokenType tp, std::string&& tx): type(tp), text(tx) {}
    Token(const Token& src) {
        *this = src;
    }
    Token(Token&& src) {
        *this = std::forward<Token>(src);
    }
    ~Token() = default;
    Token& operator=(const Token& src) {
        type = src.type;
        text = src.text;
        return *this;
    }
    Token& operator=(Token&& src) {
        std::swap(type, src.type);
        text.swap(src.text);
        return *this;
    }

    void set(TokenType tp = Token_Null, const std::string& tx = "") {
        type = tp;
        text = tx;
    }
    void set(TokenType tp = Token_Null, std::string&& tx = "") {
        set(tp, tx);
    }
};

}
}

#endif