

#ifndef FSOFT2026_1DC__CONTROLLER_H
#define FSOFT2026_1DC__CONTROLLER_H
#include <map>
#include <vector>

#include "../model/PescaTudo.h"
#include "../model/Cart.h"
#include "../model/Client.h"
#include "../model/Supplier.h"
#include "../model/SupplierContainer.h"
#include "../model/SupplierOrderContainer.h"
#include "../model/ProductContainer.h"

class Controller {
private:
    PescaTudo &store;
    Client* loggedInClient = nullptr;                // Cliente autenticado
    std::map<std::string, Cart> clientCarts;         // Email → Carrinho
    const Supplier* loggedInSupplier = nullptr;
public:
    Controller(PescaTudo &store);
    void run();

    // Áreas principais
    void runClient();
    void runManager();
    void runClientLoggedMenu();
    void loginManager();
    void runManagerMenu();  // (vai ser chamado após login)
    // Área supplier
    void runSupplier();
    void loginSupplier();
    void runSupplierMenu();
    bool isSupplierAuthenticated();
    void viewSupplierOwnPendingOrders();
    void viewSupplierOwnCompletedOrders();
    void completeSupplierOwnOrder();

    // Ações do cliente
    void loginClient();
    void signUpClient();
    void viewProductsGuest();

    // Carrinho
    void addToCart(Cart& cart);
    void viewCart(Cart& cart);
    void removeFromCart(Cart& cart);
    void completeOrder(Cart& cart);

    //Encomendas
    void showClientOrders();

    // Autenticação
    bool isAuthenticated();

    //Menu manager
    void manageProductsMenu();
    void manageSuppliersMenu();
    void manageClientsMenu();
    void addProduct();
    void editProduct();
    void deleteProduct();
    void listProducts();
    void placeOrderToSupplier();
    void deleteClientByEmail();
    void viewCompletedSupplierOrders();
    void viewSupplierOrders();
    void viewAllClientsOrders();
    void completeClientOrder();

    void addProductToClientCart(const std::string& email, const Product& product, int quantity);

};
#endif //FSOFT2026_1DC__CONTROLLER_H
