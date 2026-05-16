//
// Created by cmgm on 16/05/2026.
//

#ifndef FSOFT2026_1DC__CLIENTORDER_H
#define FSOFT2026_1DC__CLIENTORDER_H
#include "Product.h"
#include <vector>

class ClientOrder {
private:
    std::vector<std::pair<Product, int>> items;
    float total;
    bool delivered;
public:
    ClientOrder(const std::vector<std::pair<Product, int>>& items, float total, bool delivered = false);
    bool isDelivered() const;
    void setDelivered(bool value);
    void show() const;
    float getTotal() const;
    const std::vector<std::pair<Product, int>>& getItems() const;
};
#endif //FSOFT2026_1DC__CLIENTORDER_H
