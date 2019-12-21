#include <boost/regex.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include "GlobalHeader.h"

namespace bo = ::boost;

namespace testCrap {
    void boostTestFunc() {
        auto const text{ STR("To: George Shmidlap\n") +
            STR("From: Rita Marlowe\n") +
            STR("Subject: Will Success Spoil Rock Hunter ?\n") +
            STR("---\n") +
            STR("See subject.") };
        std::basic_istringstream<fa::Char> istringstream{ text };
        bo::basic_regex<fa::Char> pat{ STR(R"~(^Subject: (Re: |Aw: )*(.*))~") };
        fa::String line{ };

        while (std::getline(istringstream, line)) {
            bo::match_results<fa::String::const_iterator> matches{ };
            if (bo::regex_match(line, matches, pat)) {
                std::cout << matches[2] << std::endl;
            }
        }
    }

    auto add2(int val) -> int {
        static auto constexpr valToAdd{ 2 };
        return val + valToAdd;
    }
}

