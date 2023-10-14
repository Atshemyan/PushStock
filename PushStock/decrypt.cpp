#include "decrypt.h"

unsigned long decrypt(const std::string &str)
{
    unsigned long hash = 5381; // Initial value
    for (auto c : str)
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}
