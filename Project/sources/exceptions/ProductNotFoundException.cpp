#include "../../headers/exceptions/ProductNotFoundException.h"

const char* ProductNotFoundException::what() const noexcept {
    return "Product not found.";
}