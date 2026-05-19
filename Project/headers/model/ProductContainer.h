//
// Created by gonca on 19/05/2026.
//

#ifndef FSOFT2026_1DC__PRODUCTCONTAINER_H
#define FSOFT2026_1DC__PRODUCTCONTAINER_H

#include "Product.h"
#include <stdexcept>

class ProductContainer {
private:
    Product* productsArray;
    int capacity;
    int count;

    void expand();

public:
    ProductContainer();
    ~ProductContainer();

    void addProduct(const Product& product);
    Product& findById(int id);

    void removeProduct(int id);

    // Métodos para aceder aos dados, dependendo de como a tua View itera
    int getSize() const;
    Product& getProduct(int index);
    const Product& getProduct(int index) const;
};

#endif
