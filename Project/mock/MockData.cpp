#include "MockData.h"
#include "../headers/model/Product.h"
#include "../headers/model/Client.h"
#include "../headers/model/Manager.h"
#include "../headers/model/Supplier.h"
#include "../headers/model/SupplierOrder.h"

#include <fstream>

void MockData::generateData(PescaTudo& store) {

    // Adicionar suppliers
    Supplier shimano(1, "Shimano", "123456789", "shimano@supply.com");
    Supplier daiwa(2, "Daiwa", "987654321", "daiwa@supply.com");
    Supplier rapala(3, "Rapala", "222333444", "rapala@supply.com");

    store.getSuppliers().push_back(shimano);
    store.getSuppliers().push_back(daiwa);
    store.getSuppliers().push_back(rapala);

    // Adicionar produtos com suppliers associados
    store.getProducts().addProduct(Product(1, "Cana de Pesca Shimano", "Shimano", 12, "Canas",
                                          "Cana resistente para pesca em água doce", 25.0f, 49.99f, shimano));

    store.getProducts().addProduct(Product(2, "Carreto Daiwa Ninja", "Daiwa", 8, "Carretos",
                                          "Carreto leve e preciso para pesca desportiva", 45.0f, 89.90f, daiwa));

    store.getProducts().addProduct(Product(3, "Amostra Artificial Rapala", "Rapala", 15, "Amostras",
                                          "Amostra artificial para pesca de predadores", 7.0f, 14.99f, rapala));

    // Produtos sem supplier real
    store.getProducts().addProduct(Product(4, "Anzol Tamanho 6", "Mustad", 30, "Anzóis",
                                          "Conjunto de anzóis resistentes", 1.5f, 3.99f));

    store.getProducts().addProduct(Product(5, "Isco Fresco Sardinha", "PescaMar", 20, "Iscos",
                                          "Isco fresco ideal para pesca marítima", 2.0f, 5.50f));

    store.getProducts().addProduct(Product(6, "Linha de Pesca 0.30mm", "Berkley", 18, "Linhas",
                                          "Linha resistente para pesca em rio e mar", 4.0f, 9.99f));

    store.getProducts().addProduct(Product(7, "Boia de Pesca Grande", "Cormoran", 10, "Boias",
                                          "Boia resistente para pesca de superfície", 1.0f, 2.99f));

    store.getProducts().addProduct(Product(8, "Chumbadas 50g", "FishingPro", 25, "Chumbadas",
                                          "Conjunto de chumbadas para pesca de fundo", 2.5f, 6.99f));

    store.getProducts().addProduct(Product(9, "Caixa de Pesca", "Plano", 7, "Acessórios",
                                          "Caixa organizadora para material de pesca", 15.0f, 29.99f));

    store.getProducts().addProduct(Product(10, "Rede de Pesca", "SeaNet", 5, "Redes",
                                          "Rede resistente para captura de peixe", 12.0f, 24.99f));

    // Adicionar clientes
    store.getClients().push_back(Client("goncalo@gmail.com", "1234"));
    store.getClients().push_back(Client("raul@gmail.com", "1234"));
    store.getClients().push_back(Client("fernando@gmail.com", "1234"));

    // Adicionar manager
    store.setManager(Manager("admin@pescatudo.com", "admin777"));

    // Encontrar referências aos clientes
    Client& goncalo = store.getClients()[0];
    Client& raul = store.getClients()[1];

    // Produtos para usar nas orders
    Product canaProd = store.findProductById(1);
    Product carretoProd = store.findProductById(2);
    Product amostraProd = store.findProductById(3);
    Product anzolProd = store.findProductById(4);

    // Produtos no carrinho do Pedro
    std::vector<std::pair<Product, int>> cartItems;

    cartItems.push_back(std::make_pair(canaProd, 1));
    cartItems.push_back(std::make_pair(carretoProd, 1));

    float cartTotal = canaProd.getPriceClient() + carretoProd.getPriceClient();

    ClientOrder cartOrder(cartItems, cartTotal, false);

    goncalo.addOrder(cartOrder);

    // Orders para Pedro

    // Order pendente
    std::vector<std::pair<Product, int>> goncaloPending1 = {
            {amostraProd, 1}
    };

    ClientOrder goncaloOrderPending1(goncaloPending1,
                                   amostraProd.getPriceClient(),
                                   false);

    goncalo.addOrder(goncaloOrderPending1);

    // Order confirmada 1
    std::vector<std::pair<Product, int>> goncaloConfirmed1 = {
            {anzolProd, 3}
    };

    float goncaloConfirmed1Total =
            anzolProd.getPriceClient() * 3;

    ClientOrder goncaloOrderConfirmed1(
            goncaloConfirmed1,
            goncaloConfirmed1Total,
            true
    );

    goncalo.addOrder(goncaloOrderConfirmed1);

    // Order confirmada 2
    std::vector<std::pair<Product, int>> goncaloConfirmed2 = {
            {canaProd, 1},
            {carretoProd, 1}
    };

    float goncaloConfirmed2Total =
            canaProd.getPriceClient() +
            carretoProd.getPriceClient();

    ClientOrder goncaloOrderConfirmed2(
            goncaloConfirmed2,
            goncaloConfirmed2Total,
            true
    );

    goncalo.addOrder(goncaloOrderConfirmed2);

    // Orders para Raul

    std::vector<std::pair<Product, int>> raulOrder1 = {
            {carretoProd, 2}
    };

    float raulOrder1Total =
            carretoProd.getPriceClient() * 2;

    ClientOrder raulConfirmed1(
            raulOrder1,
            raulOrder1Total,
            true
    );

    raul.addOrder(raulConfirmed1);

    std::vector<std::pair<Product, int>> raulOrder2 = {
            {amostraProd, 1},
            {anzolProd, 2}
    };

    float raulOrder2Total =
            amostraProd.getPriceClient() +
            anzolProd.getPriceClient() * 2;

    ClientOrder raulConfirmed2(
            raulOrder2,
            raulOrder2Total,
            true
    );

    raul.addOrder(raulConfirmed2);

    // SupplierOrders

    SupplierOrder order1(1, "2026-05-01", shimano);

    order1.addProduct(canaProd);
    order1.addProduct(canaProd);

    store.getSupplierOrders().push_back(order1);

    SupplierOrder order2(2, "2026-05-03", daiwa);

    order2.addProduct(carretoProd);
    order2.addProduct(carretoProd);

    store.getSupplierOrders().push_back(order2);

    SupplierOrder order3(3, "2026-05-05", rapala);

    order3.addProduct(amostraProd);
    order3.addProduct(anzolProd);

    store.getSupplierOrders().push_back(order3);

    // SupplierOrders completed

    SupplierOrder completed1(4, "2026-05-10", shimano);

    completed1.addProduct(canaProd);

    completed1.markCompleted();

    store.getSupplierOrders().push_back(completed1);

    SupplierOrder completed2(5, "2026-05-12", daiwa);

    completed2.addProduct(carretoProd);
    completed2.addProduct(anzolProd);

    completed2.markCompleted();

    store.getSupplierOrders().push_back(completed2);

    SupplierOrder completed3(6, "2026-05-15", rapala);

    completed3.addProduct(amostraProd);
    completed3.addProduct(canaProd);

    completed3.markCompleted();

    store.getSupplierOrders().push_back(completed3);
}