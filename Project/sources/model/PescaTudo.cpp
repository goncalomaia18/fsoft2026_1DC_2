#include "../../headers/model/PescaTudo.h"
#include <stdexcept>

PescaTudo::PescaTudo() {
    // vazio por agora
}

ProductContainer& PescaTudo::getProducts() {
    return products;
}


std::vector<Client>& PescaTudo::getClients() {
    return clients;
}

std::vector<Supplier>& PescaTudo::getSuppliers() {
    return suppliers;
}

std::vector<ClientOrder>& PescaTudo::getClientOrders() {
    return clientOrders;
}

std::vector<SupplierOrder>& PescaTudo::getSupplierOrders() {
    return supplierOrders;
}

Manager& PescaTudo::getManager() {
    return manager;
}

void PescaTudo::setManager(const Manager& manager) {
    this->manager = manager;
}

Product& PescaTudo::findProductById(int id) {
    return products.findById(id);
}

Supplier& PescaTudo::findSupplierById(int id) {
    for (auto& s : suppliers) {
        if (s.getId() == id)
            return s;
    }
    throw std::runtime_error("Supplier not found.");
}

void PescaTudo::addProduct(const Product& product) {
    products.addProduct(product);
}

void PescaTudo::addSupplier(const Supplier& supplier) {
    suppliers.push_back(supplier);
}

void PescaTudo::addSupplierOrder(const SupplierOrder& order) {
    supplierOrders.push_back(order);
}