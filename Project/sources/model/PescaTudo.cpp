#include "../../headers/model/PescaTudo.h"
#include <stdexcept>

PescaTudo::PescaTudo() {
    // vazio por agora
}

ProductContainer& PescaTudo::getProducts() {
    return products;
}


ClientContainer& PescaTudo::getClients() {
    return clients;
}

SupplierContainer& PescaTudo::getSuppliers() {
    return suppliers;
}

std::vector<ClientOrder>& PescaTudo::getClientOrders() {
    return clientOrders;
}

SupplierOrderContainer& PescaTudo::getSupplierOrders() {
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
    return suppliers.findById(id);
}

void PescaTudo::addProduct(const Product& product) {
    products.addProduct(product);
}

void PescaTudo::addSupplier(const Supplier& supplier) {
    suppliers.addSupplier(supplier);
}

void PescaTudo::addSupplierOrder(const SupplierOrder& order) {
    supplierOrders.addOrder(order);
}

EvaluationContainer& PescaTudo::getEvaluations() {
    return evaluations;
}

void PescaTudo::addEvaluation(const Evaluation& evaluation) {
    evaluations.addEvaluation(evaluation);
}