#include "../../headers/model/SupplierOrder.h"

SupplierOrder::SupplierOrder() {
    this->orderNumber = 0;
    this->date = "";
    this->isCompleted = false;
}

SupplierOrder::SupplierOrder(int number, const std::string& date, const Supplier& supplier)
        : orderNumber(number), date(date), supplier(supplier), isCompleted(false) {}

int SupplierOrder::getOrderNumber() const {
    return orderNumber;
}

std::string SupplierOrder::getDate() const {
    return date;
}

Supplier SupplierOrder::getSupplier() const {
    return supplier;
}

ProductContainer& SupplierOrder::getProducts() {
    return products;
}

const ProductContainer& SupplierOrder::getProducts() const {
    return products;
}

void SupplierOrder::addProduct(const Product& product) {
    products.addProduct(product);
}

void SupplierOrder::markCompleted() {
    isCompleted = true;
}

bool SupplierOrder::getStatus() const {
    return isCompleted;
}