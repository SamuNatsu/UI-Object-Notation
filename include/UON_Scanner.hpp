#ifndef UON_SCANNER_HPP_INCLUDED
#define UON_SCANNER_HPP_INCLUDED

#include <string>

#include "UON_Input.hpp"
#include "UON_Token.hpp"

namespace UON {
namespace Internal {

class Scanner {
    public:
        Scanner() = default;
        Scanner(const Scanner&) = delete;
        ~Scanner() = default;
        Scanner& operator=(const Scanner&) = delete;

        bool open(const std::string& str, InputType tp) {
            return p_in.open(str, tp);
        }
        bool open(std::string&& str, InputType tp) {
            return p_in.open(str, tp);
        }
        void close() {
            p_in.close();
        }

        void nextToken() {
            if (!p_in.isReady())
                return;
            skipWhite();
            char c_ = p_in.peek();
            switch (c_) {
                case ('['):
                    p_token.set(Token_LSBracket, "[");
                    break;
                case (']'):
                    p_token.set(Token_RSBracket, "]");
                    break;
                case ('{'):
                    p_token.set(Token_LCBracket, "{");
                    break;
                case ('}'):
                    p_token.set(Token_RCBracket, "}");
                    break;
                case ('<'):
                    p_token.set(Token_Less, "<");
                    break;
                case ('>'):
                    p_token.set(Token_Greater, ">");
                    break;
                case ('='):
                    p_token.set(Token_Equal, "=");
                    break;
                case (';'):
                    p_token.set(Token_Semicolon, ";");
                    break;
                case ('"'):
                    p_token = getString();
                    break;
                default:
                    if (isalpha(c_))
                        p_token.set(Token_Identifier, getIdentifier());
                    else {
                        if (c_ == EOF)
                            p_token.set(Token_Null, "EOF");
                        else
                            p_token.set(Token_Null, std::string("Invalid character -> ") + getPosition());
                    }
            }
            if (p_token.type != Token_Identifier)
                p_in.get();
        }
        const Token& getToken() const {
            return p_token;
        }
        std::string getPosition() const {
            return std::string("Row: ") + std::to_string(p_in.getRow()) + 
                std::string(" Column: ") + std::to_string(p_in.getColumn() - 1);
        }

    private:
        Input p_in;
        Token p_token;

    private:
        void skipWhite() {
            char c_ = p_in.peek();
            while ((c_ == ' ' || c_ == '\n' || c_ == '\r' || c_ == '\t') && c_ != EOF)
                c_ = (p_in.get(), p_in.peek());
        }
        uint32_t hex2dec(char c) {
            if (isdigit(c))
                return c - '0';
            else if (isupper(c))
                return c - 'A' + 10;
            else
                return c - 'a' + 10;
        }
        std::string getUnicode() {
            std::string r_;
            bool v_ = false;
            char c_ = (p_in.get(), p_in.peek()), u_[4];
            for (int i = 0; i < 4; ++i) {
                if (!isxdigit(c_))
                    v_ = true;
                u_[i] = c_;
                c_ = (p_in.get(), p_in.peek());
            }
            if (v_) {
                r_ = "\\u";
                for (int i = 0; i < 4; ++i)
                    r_ += u_[i];
                return r_;
            }
            uint32_t uc_ = 0;
            for (int i = 0; i < 4; ++i)
                uc_ = uc_ << 8 | hex2dec(u_[i]);
            if (uc_ <= 0x7F)
                r_ += uc_ & 0x7F;
            else if (uc_ <= 0x7FF) {
                r_ += 0xC0 | ((uc_ >> 6) & 0x1F);
                r_ += 0x80 | (uc_ & 0x3F);
            }
            else {
                r_ += 0xE0 | ((uc_ >> 10) & 0xF);
                r_ += 0x80 | ((uc_ >> 6) & 0x3F);
                r_ += 0x80 | (uc_ & 0x3F);
            }
            return r_;
        }
        Token getString() {
            std::string r_;
            char c_ = (p_in.get(), p_in.peek());
            while (1) {
                if (iscntrl(c_))
                    break;
                if (c_ == '"')
                    return Token(Token_String, r_);
                else if (c_ != '\\')
                    r_ += c_;
                else {
                    char c__ = (p_in.get(), p_in.peek());
                    if (iscntrl(c__))
                        break;
                    switch (c__) {
                        case ('n'):
                            r_ += '\n';
                            break;
                        case ('r'):
                            r_ += '\r';
                            break;
                        case ('t'):
                            r_ += '\t';
                            break;
                        case ('u'):
                            r_ += getUnicode();
                            break;
                        default:
                            r_ += '\\';
                            r_ += c__;
                    }
                }
                c_ = (p_in.get(), p_in.peek());
            }
            if (c_ == EOF)
                return Token(Token_Null, "EOF");
            else
                return Token(Token_Null, std::string("Invalid character -> ") + std::to_string(p_in.peek()));
        }
        std::string getIdentifier() {
            std::string r_;
            char c_ = p_in.peek();
            while (isalpha(c_) || isdigit(c_) || c_ == '-') {
                r_ += c_;
                c_ = (p_in.get(), p_in.peek());
            }
            return r_;
        }
};

}
}

#endif