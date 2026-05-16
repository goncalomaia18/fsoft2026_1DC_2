#include "../headers/model/SupplierOrder.h"

SupplierOrder::SupplierOrder(int number, const std::string& date, const Supplier& supplier)
        : orderNumber(number), date(date), supplier(supplier) {}

int SupplierOrder::getOrderNumber() const {
    return orderNumber;
}

std::string SupplierOrder::getDate() const {
    return date;
}

Supplier SupplierOrder::getSupplier() const {
    return supplier;
}


std::vector<Product> SupplierOrder::getProducts() const {
    return products;
}

void SupplierOrder::addProduct(const Product& product) {
    products.push_back(product);
}
void SupplierOrder::markCompleted() {
    isCompleted = true;
}

bool SupplierOrder::getStatus() const {
    return isCompleted;
}