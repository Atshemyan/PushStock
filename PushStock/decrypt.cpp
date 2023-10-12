#include "decrypt.h"

std::string decrypt(const std::string& str) {
    std::string res = "bdewkf" + str + "deuw";

    for (auto& i : res)
    {
        if (i > 'Z')
        {
            i /= 2;
        }
        else
        {
            i += 10;
        }
    }
    return res;
}
