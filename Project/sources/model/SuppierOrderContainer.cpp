#include "../../headers/model/SupplierOrderContainer.h"

SupplierOrderContainer::SupplierOrderContainer() {
    capacity = 5;
    count = 0;
    ordersArray = new SupplierOrder[capacity];
}

SupplierOrderContainer::~SupplierOrderContainer() {
    delete[] ordersArray;
}

void SupplierOrderContainer::expand() {
    capacity *= 2;
    SupplierOrder* newArray = new SupplierOrder[capacity];

    for (int i = 0; i < count; i++) {
        newArray[i] = ordersArray[i];
    }

    delete[] ordersArray;
    ordersArray = newArray;
}

void SupplierOrderContainer::addOrder(const SupplierOrder& order) {
    if (count == capacity) {
        expand();
    }
    ordersArray[count] = order;
    count++;
}

SupplierOrder& SupplierOrderContainer::findByOrderNumber(int orderNumber) {
    for (int i = 0; i < count; i++) {
        if (ordersArray[i].getOrderNumber() == orderNumber) {
            return ordersArray[i];
        }
    }
    throw std::runtime_error("Supplier order not found.");
}

void SupplierOrderContainer::removeOrder(int orderNumber) {
    int indexToRemove = -1;
    for (int i = 0; i < count; i++) {
        if (ordersArray[i].getOrderNumber() == orderNumber) {
            indexToRemove = i;
            break;
        }
    }

    if (indexToRemove == -1) {
        throw std::runtime_error("Supplier order not found to delete.");
    }

    for (int i = indexToRemove; i < count - 1; i++) {
        ordersArray[i] = ordersArray[i + 1];
    }
    count--;
}

void SupplierOrderContainer::removeByIndex(int index) {
    if (index < 0 || index >= count) {
        throw std::out_of_range("Index invalido.");
    }
    for (int i = index; i < count - 1; i++) {
        ordersArray[i] = ordersArray[i + 1];
    }
    count--;
}

int SupplierOrderContainer::getSize() const {
    return count;
}

SupplierOrder& SupplierOrderContainer::getOrder(int index) {
    if (index < 0 || index >= count) {
        throw std::out_of_range("Index invalido.");
    }
    return ordersArray[index];
}

const SupplierOrder& SupplierOrderContainer::getOrder(int index) const {
    if (index < 0 || index >= count) {
        throw std::out_of_range("Index invalido.");
    }
    return ordersArray[index];
}
