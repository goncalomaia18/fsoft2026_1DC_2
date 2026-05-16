#include "../../headers/views/Utils.h"
#include <cctype>

bool Utils::isValidEmail(const std::string& email) {
    size_t atPos = email.find('@');
    size_t dotPos = email.find('.', atPos);

    if (atPos == std::string::npos || dotPos == std::string::npos)
        return false;

    if (atPos == 0 || dotPos <= atPos + 1 || dotPos == email.length() - 1)
        return false;

    return true;
}