#include "../../headers/exceptions/InvalidLoginException.h"

const char* InvalidLoginException::what() const noexcept {
    return "Invalid email or password.";
}