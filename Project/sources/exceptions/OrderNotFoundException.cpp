#include "../../headers/exceptions/OrderNotFoundException.h"

const char* OrderNotFoundException::what() const noexcept {
    return "Order not found.";
}