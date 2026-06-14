#include "../../headers/model/SupplierOrder.h"

SupplierOrder::SupplierOrder()
        : orderNumber(0), date(""), supplier(), products(),
          status(SupplierOrderStatus::Pending), completed(false) {}

SupplierOrder::SupplierOrder(int number, const std::string& date,
                             const Supplier& supplier)
        : orderNumber(number), date(date), supplier(supplier), products(),
          status(SupplierOrderStatus::Pending), completed(false) {}

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

SupplierOrderStatus SupplierOrder::getOrderStatus() const {
    return status;
}

void SupplierOrder::setOrderStatus(SupplierOrderStatus newStatus) {
    status = newStatus;
}

std::string SupplierOrder::getStatusText() const {
    switch (status) {
        case SupplierOrderStatus::Pending:
            return "Pending";
        case SupplierOrderStatus::Confirmed:
            return "Confirmed";
        case SupplierOrderStatus::PartiallyConfirmed:
            return "Partially Confirmed";
        case SupplierOrderStatus::Rejected:
            return "Rejected";
    }

    return "Unknown";
}

bool SupplierOrder::isPending() const {
    return status == SupplierOrderStatus::Pending;
}

void SupplierOrder::markCompleted() {
    if (status == SupplierOrderStatus::Pending) {
        status = SupplierOrderStatus::Confirmed;
    }

    completed = true;
}

bool SupplierOrder::getStatus() const {
    return completed;
}
