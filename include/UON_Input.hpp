#ifndef UON_INPUT_HPP_INCLUDED
#define UON_INPUT_HPP_INCLUDED

#include <string>
#include <fstream>

namespace UON {
namespace Internal {

using InputType = enum it_ {
    Input_File,
    Input_String
};

class Input {
    public:
        Input() = default;
        Input(const Input&) = delete;
        ~Input() {
            close();
        }
        Input& operator=(const Input&) = delete;

        bool open(const std::string& str, InputType tp) {
            close();
            p_type = tp;
            if (tp == Input_File) {
                p_fin.open(str);
                p_isReady = p_fin.is_open();
            }
            else {
                p_pos = 0;
                p_buff = str;
                p_isReady = true;
            }
            return p_isReady;
        }
        bool open(std::string&& str, InputType tp) {
            return open(str, tp);
        }
        void close() {
            if (p_type == Input_File && p_fin.is_open())
                p_fin.close();
            p_row = p_column = 1;
        }

        bool isReady() const {
            return p_isReady;
        }

        bool eof() const {
            return p_isReady ? (p_type == Input_File ? p_fin.eof() : p_pos >= p_buff.length()) : true;
        }
        char get() {
            peek() == '\n' ? (++p_row, p_column = 1) : ++p_column;
            return p_isReady && !eof() ? (p_type == Input_File ? p_fin.get() : p_buff[p_pos++]) : EOF;
        }
        char peek() {
            return p_isReady && !eof() ? (p_type == Input_File ? p_fin.peek() : p_buff[p_pos]) : EOF;
        }

        size_t getRow() const {
            return p_row;
        }
        size_t getColumn() const {
            return p_column;
        }

    private:
        bool p_isReady;
        InputType p_type;
        size_t p_pos, p_row, p_column;
        std::string p_buff;
        std::ifstream p_fin;
};

}
}

#endif