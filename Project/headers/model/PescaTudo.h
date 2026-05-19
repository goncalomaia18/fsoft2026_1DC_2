//
// Created by cmgm on 16/05/2026.
//

#ifndef FSOFT2026_1DC__PESCATUDO_H
#define FSOFT2026_1DC__PESCATUDO_H
#include <vector>

#include "ProductContainer.h"
#include "ClientContainer.h"
#include "SupplierContainer.h"
#include "SupplierOrderContainer.h"
#include "Product.h"
#include "Client.h"
#include "Manager.h"
#include "Supplier.h"
#include "ClientOrder.h"
#include "SupplierOrder.h"

class PescaTudo {
private:
    ProductContainer products;
    ClientContainer clients;
    SupplierContainer suppliers;
    std::vector<ClientOrder> clientOrders;
    SupplierOrderContainer supplierOrders;
    Manager manager;

public:
    PescaTudo();

    ProductContainer& getProducts();
    ClientContainer& getClients();
    SupplierContainer& getSuppliers();
    std::vector<ClientOrder>& getClientOrders();
    SupplierOrderContainer& getSupplierOrders();
    Manager& getManager();
    void setManager(const Manager& manager);

    Product& findProductById(int id);
    Supplier& findSupplierById(int id);

    void addProduct(const Product& product);
    void addSupplier(const Supplier& supplier);
    void addSupplierOrder(const SupplierOrder& order);
};

#endif //FSOFT2026_1DC__PESCATUDO_H
