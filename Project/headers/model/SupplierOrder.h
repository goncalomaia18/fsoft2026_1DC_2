//
// Created by cmgm on 16/05/2026.
//

#ifndef FSOFT2026_1DC__SUPPLIERORDER_H
#define FSOFT2026_1DC__SUPPLIERORDER_H
#include <string>
#include <vector>
#include "Supplier.h"
#include "Product.h"
#include "ProductContainer.h"

class SupplierOrder {
private:
    int orderNumber;
    std::string date;
    Supplier supplier;

    ProductContainer products;

public:
    SupplierOrder();
    SupplierOrder(int number, const std::string& date, const Supplier& supplier);

    int getOrderNumber() const;
    std::string getDate() const;
    Supplier getSupplier() const;
    ProductContainer& getProducts();
    const ProductContainer& getProducts() const;
    bool isCompleted = false;
    void markCompleted();
    bool getStatus() const;

    void addProduct(const Product& product);
};

#endif //FSOFT2026_1DC__SUPPLIERORDER_H
