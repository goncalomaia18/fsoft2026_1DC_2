//
// Created by gonca on 20/05/2026.
//

#ifndef FSOFT2026_1DC__SUPPLIERORDERCONTAINER_H
#define FSOFT2026_1DC__SUPPLIERORDERCONTAINER_H

#include "SupplierOrder.h"
#include <stdexcept>

class SupplierOrderContainer {
private:
    SupplierOrder* ordersArray;
    int capacity;
    int count;

    void expand();

public:
    SupplierOrderContainer();
    ~SupplierOrderContainer();

    void addOrder(const SupplierOrder& order);
    SupplierOrder& findByOrderNumber(int orderNumber);
    void removeOrder(int orderNumber); // Apagar pelo número
    void removeByIndex(int index);     // Apagar pela posição (útil no teu Controller)

    int getSize() const;
    SupplierOrder& getOrder(int index);
    const SupplierOrder& getOrder(int index) const;
};

#endif //FSOFT2026_1DC__SUPPLIERORDERCONTAINER_H
