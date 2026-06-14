//
// Created by cmgm on 16/05/2026.
//

#ifndef FSOFT2026_1DC__SUPPLIERORDER_H
#define FSOFT2026_1DC__SUPPLIERORDER_H

#include <string>
#include "Supplier.h"
#include "Product.h"
#include "ProductContainer.h"

enum class SupplierOrderStatus {
    Pending,
    Confirmed,
    PartiallyConfirmed,
    Rejected
};

class SupplierOrder {
private:
    int orderNumber;
    std::string date;
    Supplier supplier;
    ProductContainer products;
    SupplierOrderStatus status;
    bool completed;

public:
    SupplierOrder();
    SupplierOrder(int number, const std::string& date, const Supplier& supplier);

    int getOrderNumber() const;
    std::string getDate() const;
    Supplier getSupplier() const;

    ProductContainer& getProducts();
    const ProductContainer& getProducts() const;
    void addProduct(const Product& product);

    SupplierOrderStatus getOrderStatus() const;
    void setOrderStatus(SupplierOrderStatus newStatus);
    std::string getStatusText() const;
    bool isPending() const;


    void markCompleted();
    bool getStatus() const;
};

#endif //FSOFT2026_1DC__SUPPLIERORDER_H
