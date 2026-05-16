//
// Created by cmgm on 16/05/2026.
//

#ifndef FSOFT2026_1DC__CART_H
#define FSOFT2026_1DC__CART_H
#include "Product.h"
#include <vector>

class Cart {
private:
    std::vector<std::pair<Product, int>> items;

public:
    void addProduct(const Product& product, int quantity);
    void showCart() const;
    void removeProductById(int productId);
    float getTotal() const;
    bool isEmpty() const;
    void clear();
    const std::vector<std::pair<Product, int>>& getItems() const;

};
#endif //FSOFT2026_1DC__CART_H
