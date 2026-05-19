//
// Created by gonca on 19/05/2026.
//

#ifndef FSOFT2026_1DC__SUPPLIERCONTAINER_H
#define FSOFT2026_1DC__SUPPLIERCONTAINER_H

#include "Supplier.h"
#include <stdexcept>

class SupplierContainer {
private:
    Supplier* suppliersArray;
    int capacity;
    int count;

    void expand();

public:
    SupplierContainer();
    ~SupplierContainer();

    void addSupplier(const Supplier& supplier);
    Supplier& findById(int id);
    void removeSupplier(int id);

    int getSize() const;
    Supplier& getSupplier(int index);
    const Supplier& getSupplier(int index) const;
};

#endif //FSOFT2026_1DC__SUPPLIERCONTAINER_H
