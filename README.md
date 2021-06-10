# UI-Object-Notation
A data structure for UI object description. (UON)

# BNF
```
<UON> ::= {<block>}
<block> ::= <opt-whitespace> "[" <opt-whitespace> <identifier> {<whitespace> <block-attribute>} <opt-whitespace> "]" [<block-statement>]
<opt-whitespace> ::= Whitespace_characters <opt-whitespace> | ""
<identifier> ::= Letter {Letter | Digit | "-"}
<whitespace> ::= Whitespace_characters <opt-whitespace>
<block-attribute> ::= <identifier> <opt-whitespace> "=" <opt-whitespace> double_quote <attribute> double_quote
<block-statement> ::= <opt-whitespace> "{" {<block> | <append>} "}"
<attribute> ::= {Unicode_expected_control_characters}
<append> ::= <opt-whitespace> "<" <opt-whitespace> <identifier> <opt-whitespace> ">" <append-statement>
<append-statement> ::= <opt-whitespace> "{" {<opt-whitespace> <identifier> "=" double_quote <attribute> double_quote ";"} "}"
```

# Sample .uon file
```
[UI] {
    [assets] {
        <font> {
            list = "msyh,song";
            default = "msyh";
        }
    }
    [main] {
        [layer id="L1" w="300px" h="500px" x="centered" y="centered"] {
            <attr> {
                background-color = "EFEFEFFF";
            }
            [text x="centered" y="10%" size="5px"] {
                <attr> {
                    color = "FF0000FF";
                    value = "Hello World\nThis is a title";
                }
            }
            [button id="B1" x="centered" y="30%"] {
                [text val="Click it"]
            }
        }
        [layer id="L2"]
        [layer id="L3"]
    }
}
```

# Sample cpp
```cpp
#include <cstdio>

#include "include/UON.hpp"

// Output node tree
void rr(const UON::Node& nd, std::string ls) {
    printf("%s\n", (ls + nd.tag).c_str());
    ls += nd.tag + " > ";
    for (auto &i : nd.children)
        rr(i, ls);
}

int main() {
    if (!UON::parse("sample.txt", UON::Parse_File)) {
        printf("%s\n", UON::Result::error.c_str());
        return 0;
    }
    auto r = UON::Result::root;
    rr(r, "");
    return 0;
}
```
