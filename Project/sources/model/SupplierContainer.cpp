#include "../../headers/model/SupplierContainer.h"

SupplierContainer::SupplierContainer() {
    capacity = 5;
    count = 0;
    suppliersArray = new Supplier[capacity];
}

SupplierContainer::~SupplierContainer() {
    delete[] suppliersArray;
}

void SupplierContainer::expand() {
    capacity *= 2;
    Supplier* newArray = new Supplier[capacity];

    for (int i = 0; i < count; i++) {
        newArray[i] = suppliersArray[i];
    }

    delete[] suppliersArray;
    suppliersArray = newArray;
}

void SupplierContainer::addSupplier(const Supplier& supplier) {
    if (count == capacity) {
        expand();
    }
    suppliersArray[count] = supplier;
    count++;
}

Supplier& SupplierContainer::findById(int id) {
    for (int i = 0; i < count; i++) {
        if (suppliersArray[i].getId() == id) {
            return suppliersArray[i];
        }
    }
    throw std::runtime_error("Supplier not found.");
}

void SupplierContainer::removeSupplier(int id) {
    int indexToRemove = -1;
    for (int i = 0; i < count; i++) {
        if (suppliersArray[i].getId() == id) {
            indexToRemove = i;
            break;
        }
    }

    if (indexToRemove == -1) {
        throw std::runtime_error("Supplier not found to delete.");
    }

    for (int i = indexToRemove; i < count - 1; i++) {
        suppliersArray[i] = suppliersArray[i + 1];
    }

    count--;
}

int SupplierContainer::getSize() const {
    return count;
}

Supplier& SupplierContainer::getSupplier(int index) {
    if (index < 0 || index >= count) {
        throw std::out_of_range("Index invalido.");
    }
    return suppliersArray[index];
}

const Supplier& SupplierContainer::getSupplier(int index) const {
    if (index < 0 || index >= count) {
        throw std::out_of_range("Index invalido.");
    }
    return suppliersArray[index];
}
