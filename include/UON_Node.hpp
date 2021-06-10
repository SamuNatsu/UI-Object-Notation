#ifndef UON_NODE_HPP_INCLUDED
#define UON_NODE_HPP_INCLUDED

#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

namespace UON {

using Attribute = std::unordered_map<std::string, std::string>;

class Node {
    public:
        std::string tag;
        std::vector<Node> children;
        bool body = false;

    public:
        Node() = default;
        Node(const Node& src) {
            *this = src;
        }
        Node(Node&& src) {
            *this = std::forward<Node>(src);
        }
        ~Node() = default;
        Node& operator=(const Node& src) {
            tag = src.tag;
            children = src.children;
            p_attr = src.p_attr;
            body = src.body;
            return *this;
        }
        Node& operator=(Node&& src) {
            tag.swap(src.tag);
            children.swap(src.children);
            p_attr.swap(src.p_attr);
            std::swap(body, src.body);
            return *this;
        }
        Attribute& operator[](const std::string& str) {
            return p_attr[str];
        }
        Attribute& operator[](std::string&& str) {
            return (*this)[str];
        }

    private:
        std::unordered_map<std::string, Attribute> p_attr;
};

};

#endif