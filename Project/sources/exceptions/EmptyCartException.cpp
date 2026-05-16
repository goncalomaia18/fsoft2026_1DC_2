#include "../../headers/exceptions/EmptyCartException.h"

const char* EmptyCartException::what() const noexcept {
    return "Cart is empty. Cannot complete the order.";
}