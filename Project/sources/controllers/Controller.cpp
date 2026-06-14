#include <iostream>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <map>
#include <vector>
#include <thread>
#include <chrono>

#include "../../headers/controllers/Controller.h"
#include "../../headers/views/View.h"
#include "../../headers/views/Utils.h"
#include "../../headers/model/Cart.h"
#include "../../headers/model/Product.h"
#include "../../headers/model/ClientOrder.h"
#include "../../headers/exceptions/ProductNotFoundException.h"
#include "../../headers/exceptions/EmptyCartException.h"
#include "../../headers/exceptions/InvalidLoginException.h"
#include "../../headers/exceptions/OrderNotFoundException.h"
#include "../../headers/model/SupplierContainer.h"
#include "../../headers/model/SupplierOrderContainer.h"
#include "../../headers/model/ProductContainer.h"
#include "../../headers/model/Evaluation.h"
#include "../../headers/model/EvaluationContainer.h"



namespace {
Product* findOwnedProduct(ProductContainer& products, int productId, int supplierId) {
    for (int i = 0; i < products.getSize(); i++) {
        Product& product = products.getProduct(i);

        if (product.getId() == productId &&
            product.getSupplier().getId() == supplierId) {
            return &product;
        }
    }

    return nullptr;
}

SupplierOrder* findOwnedSupplierOrder(SupplierOrderContainer& orders,
                                      int orderNumber,
                                      int supplierId) {
    for (int i = 0; i < orders.getSize(); i++) {
        SupplierOrder& order = orders.getOrder(i);

        if (order.getOrderNumber() == orderNumber &&
            order.getSupplier().getId() == supplierId) {
            return &order;
        }
    }

    return nullptr;
}

void showSupplierOrderDetails(const SupplierOrder& order) {
    const ProductContainer& products = order.getProducts();
    std::map<int, int> quantities;

    for (int i = 0; i < products.getSize(); i++) {
        quantities[products.getProduct(i).getId()]++;
    }

    std::cout << "\n--- Restock Request Details ---\n";
    std::cout << "Order number: " << order.getOrderNumber() << "\n";
    std::cout << "Date: " << order.getDate() << "\n";
    std::cout << "Supplier: " << order.getSupplier().getName() << "\n";
    std::cout << "Status: " << order.getStatusText() << "\n";

    float total = 0.0f;

    for (const auto& entry : quantities) {
        const int productId = entry.first;
        const int quantity = entry.second;
        const Product* selectedProduct = nullptr;

        for (int i = 0; i < products.getSize(); i++) {
            const Product& product = products.getProduct(i);

            if (product.getId() == productId) {
                selectedProduct = &product;
                break;
            }
        }

        if (selectedProduct == nullptr) {
            continue;
        }

        const float subtotal = selectedProduct->getPriceSupplier() * quantity;
        total += subtotal;

        std::cout << "Product ID: " << selectedProduct->getId() << "\n";
        std::cout << "Name: " << selectedProduct->getName() << "\n";
        std::cout << "Quantity: " << quantity << "\n";
        std::cout << "Unit price: " << std::fixed << std::setprecision(2)
                  << selectedProduct->getPriceSupplier() << " EUR\n";
        std::cout << "Subtotal: " << subtotal << " EUR\n";
        std::cout << "--------------------------\n";
    }

    std::cout << "Total: " << std::fixed << std::setprecision(2)
              << total << " EUR\n";
}
}

Controller::Controller(PescaTudo &store) : store(store) {}
void Controller::run() {
    int option;
    do {
        View::showMainMenu();
        option = View::askMainOption();

        switch (option) {
            case 1:
                runClient();
                break;

            case 2:
                runManager();
                break;

            case 3:
                runSupplier();
                break;

            case 0:
                std::cout << "Exiting PescaTudo Store...\n";
                break;

            default:
                std::cout << "Invalid option. Try again.\n";
        }

    } while (option != 0);
}
// Client implementation
void Controller::runClient() {
    int option;
    do {
        View::showClientMenu();
        option = View::askClientOption();
        switch (option) {
            case 1:
                viewProductsGuest();
                break;
            case 2:
                try {
                    loginClient();
                } catch (const InvalidLoginException& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
                break;
            case 3:
                signUpClient();
                break;
            case 0:
                std::cout << "Returning...\n";
                break;
            default:
                std::cout << "Invalid option.\n";
        }
    } while (option != 0);
}
void Controller::viewProductsGuest() {
    const ProductContainer& products = store.getProducts();
    int option;

    Cart* cartPtr = nullptr;

    if (isAuthenticated()) {
        cartPtr = &clientCarts[loggedInClient->getEmail()];
    } else {
        static Cart guestCart;
        cartPtr = &guestCart;
    }

    Cart& cart = *cartPtr;

    std::cout << "\n--- Available Products ---\n";

    if (products.getSize() == 0) {
        std::cout << "No products available.\n";
        return;
    }

    for (int i = 0; i < products.getSize(); i++) {
        const Product& p = products.getProduct(i);

        std::cout << "ID: " << p.getId() << "\n";
        std::cout << "Name: " << p.getName() << "\n";
        std::cout << "Brand: " << p.getBrand() << "\n";
        std::cout << "Category: " << p.getCategory() << "\n";
        std::cout << "Description: " << p.getDescription() << "\n";
        std::cout << "Price: " << std::fixed << std::setprecision(2)
                  << p.getPriceClient() << " EUR\n";
        std::cout << "Stock: " << p.getStock() << "\n";
        std::cout << "--------------------------\n";
    }

    do {
        std::cout << "1. Add to cart\n";
        std::cout << "2. View cart\n";
        std::cout << "0. Back to menu\n";
        std::cout << "Option: ";
        std::cin >> option;

        switch (option) {
            case 1:
                try {
                    addToCart(cart);
                } catch (const ProductNotFoundException& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
                break;

            case 2:
                viewCart(cart);
                break;

            case 0:
                std::cout << "Returning to client menu...\n";
                break;

            default:
                std::cout << "Invalid option.\n";
        }

    } while (option != 0);
}
void Controller::loginClient() {
    std::string email, password;
    bool found = false;

    std::cout << "\n--- Client Login ---\n";
    std::cout << "Email: ";
    std::cin >> email;
    std::cout << "Password: ";
    std::cin >> password;

    ClientContainer& clients = store.getClients();

    for (int i = 0; i < clients.getSize(); i++) {
        Client& c = clients.getClient(i);

        if (c.getEmail() == email && c.getPassword() == password) {
            std::cout << "Login successful! Welcome back, " << email << ".\n";
            loggedInClient = &c;
            found = true;
            runClientLoggedMenu();
            break;
        }
    }

    if (!found) {
        throw InvalidLoginException();
    }
}
void Controller::runClientLoggedMenu() {
    if (!isAuthenticated()) {
        std::cout << "Access denied. Please login first.\n";
        return;
    }
    Cart& cart = clientCarts[loggedInClient->getEmail()];  // Carrinho pessoal do cliente
    int option;
    do {
        std::cout << "\n--- Client Menu ---\n";
        std::cout << "1 - View products\n";
        std::cout << "2 - View cart\n";
        std::cout << "3 - View orders\n";
        std::cout << "4 - Evaluate product\n";
        std::cout << "5 - View product evaluations\n";
        std::cout << "0 - Logout\n";
        std::cout << "Option: ";
        std::cin >> option;

        switch (option) {
            case 1:
                viewProductsGuest();
                break;
            case 2:
                viewCart(cart);
                break;
            case 3:
                showClientOrders();
                break;

            case 4:
                evaluateProduct();
                break;

            case 5:
                viewProductEvaluations();
                break;

            case 0:
                std::cout << "Logging out...\n";
                loggedInClient = nullptr;  // Efetua logout
                break;
            default:
                std::cout << "Invalid option.\n";
        }
    } while (option != 0);
}
void Controller::signUpClient() {
    std::string email, password;

    std::cout << "\n--- Sign Up ---\n";
    std::cout << "Email: ";
    std::cin >> email;

    if (!Utils::isValidEmail(email)) {
        std::cout << "Invalid email format. Use something like: XXXXXX@gmail.com\n";
        return;
    }

    ClientContainer& clients = store.getClients();

    for (int i = 0; i < clients.getSize(); i++) {
        const Client& c = clients.getClient(i);

        if (c.getEmail() == email) {
            std::cout << "An account with that email already exists.\n";
            return;
        }
    }

    std::cout << "Password: ";
    std::cin >> password;

    if (password.length() < 4) {
        std::cout << "Password must have at least 4 characters.\n";
        return;
    }

    Client newClient(email, password);
    clients.addClient(newClient);

    std::ofstream outFile("clients.txt", std::ios::app);

    if (outFile.is_open()) {
        outFile << email << " " << password << "\n";
        outFile.close();
    } else {
        std::cout << "Error saving client to file.\n";
    }

    std::cout << "Account created successfully!\n";
}
void Controller::addToCart(Cart& cart) {
    int productId;
    int quantity;

    std::cout << "Enter the product ID: ";
    std::cin >> productId;

    std::cout << "Enter quantity: ";
    std::cin >> quantity;

    const ProductContainer& products = store.getProducts();

    for (int i = 0; i < products.getSize(); i++) {
        const Product& p = products.getProduct(i);

        if (p.getId() == productId) {
            if (quantity > p.getStock()) {
                std::cout << "Error: Only " << p.getStock() << " units in stock.\n";
                return;
            }

            cart.addProduct(p, quantity);
            std::cout << "Product added to cart.\n";
            return;
        }
    }

    throw ProductNotFoundException();
}
void Controller::viewCart(Cart& cart) {
    int option;
    do {
        cart.showCart();  // Exibir o carrinho
        std::cout << "\n--- Menu Cart ---\n";
        std::cout << "1. Delete product\n";
        std::cout << "2. Complete order\n";
        std::cout << "0. Go back to products menu\n";
        std::cout << "Option: ";
        std::cin >> option;

        switch (option) {
            case 1:
                try {
                    removeFromCart(cart);
                } catch (const ProductNotFoundException& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
                break;
            case 2:
                try {
                    completeOrder(cart);
                } catch (const EmptyCartException& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
                break;
            case 0:
                std::cout << "Returning to products menu...\n";
                listProducts();
                break;
            default:
                std::cout << "Invalid option. Please try again.\n";
        }
    } while (option != 0);  // Continua no menu do carrinho até o cliente escolher "0"
}
void Controller::removeFromCart(Cart& cart) {
    int productId;
    std::cout << "Enter the product ID to remove: ";
    std::cin >> productId;
    cart.removeProductById(productId);
}
void Controller::completeOrder(Cart& cart) {
    if (cart.isEmpty()) {
        throw EmptyCartException();
    }
    if (!isAuthenticated()) {
        std::cout << "You must be logged in to complete the order.\n";
        return;
    }
    // Subtrair stock
    for (const auto& item : cart.getItems()) {
        Product& product = store.findProductById(item.first.getId());
        if (item.second > product.getStock()) {
            std::cout << "Error: Product " << product.getName() << " doesn't have enough stock.\n";
            return;
        }
        product.reduceStock(item.second);
    }
    // Criar e guardar a encomenda no cliente autenticado
    ClientOrder order(cart.getItems(), cart.getTotal());
    loggedInClient->addOrder(order);
    std::cout << "Order completed successfully!\n";
    cart.clear();  // Limpa o carrinho
}
void Controller::showClientOrders() {
    if (loggedInClient == nullptr) {
        std::cout << "Erro: Nenhum cliente está autenticado.\n";
        return;
    }
    const std::vector<ClientOrder>& orders = loggedInClient->getOrders();
    if (orders.empty()) {
        std::cout << "\nYou have no orders yet.\n";
        return;
    }
    std::cout << "\n=== Your Orders ===\n";
    int orderNumber = 1;
    for (const ClientOrder& order : orders) {
        std::cout << "\n--- Order #" << orderNumber++ << " ---\n";
        order.show();
    }
}
// Função para verificar se o cliente está autenticado
bool Controller::isAuthenticated() {
    return loggedInClient != nullptr;
}
//Manager implementation
void Controller::loginManager() {
    std::string email, password;
    std::cout << "\n--- Main Menu ---\n";
    std::cout << "Email: ";
    std::cin >> email;
    std::cout << "Password: ";
    std::cin >> password;

    if (email == store.getManager().getEmail() && password == store.getManager().getPassword()) {
        std::cout << "Manager login successful!\n";
        runManagerMenu();
    } else {
        throw InvalidLoginException();
    }
}
void Controller::runManager() {
    int option;
    do {
        View::showManagerMainMenu();
        option = View::askManagerMainOption();

        switch (option) {
            case 1:
                try {
                    loginManager();
                } catch (const InvalidLoginException& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
                break;
            case 0:
                std::cout << "Returning...\n";
                break;
            default:
                std::cout << "Invalid option.\n";
        }
    } while (option != 0);
}
void Controller::runManagerMenu() {
    int option;
    do {
        std::cout << "\n--- Manager Menu ---\n";
        std::cout << "1. Manage products\n";
        std::cout << "2. Manage suppliers\n";
        std::cout << "3. Manage clients\n";
        std::cout << "0. Logout\n";
        std::cout << "Option: ";
        std::cin >> option;

        switch (option) {
            case 1:
                manageProductsMenu();
                break;
            case 2:
                manageSuppliersMenu();
                break;
            case 3:
                manageClientsMenu();
                break;
            case 0:
                std::cout << "Logging out...\n";
                break;
            default:
                std::cout << "Invalid option. Please try again.\n";
        }
    } while (option != 0);
}
void Controller::manageProductsMenu() {
    int option;
    listProducts();
    do {
        std::cout << "\n--- Manage Products Menu ---\n";
        std::cout << "1. Add product\n";
        std::cout << "2. Edit product\n";
        std::cout << "3. Delete product\n";
        std::cout << "0. Go back\n";
        std::cout << "Option: ";
        std::cin >> option;

        switch (option) {
            case 1:
                addProduct();
                break;
            case 2:
                try {
                    editProduct();
                } catch (const ProductNotFoundException& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
                break;
            case 3:
                try {
                    deleteProduct();
                } catch (const ProductNotFoundException& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
                break;
            case 0:
                std::cout << "Returning to Manager Menu...\n";
                break;
            default:
                std::cout << "Invalid option. Try again.\n";
        }
    } while (option != 0);
}
void Controller::manageSuppliersMenu() {
    int option;
    do {
        std::cout << "\n--- Manage Suppliers Menu ---\n";
        std::cout << "1. Place order to supplier\n";
        std::cout << "2. View supplier orders\n";
        std::cout << "3. View completed supplier orders\n";
        std::cout << "0. Go back\n";
        std::cout << "Option: ";
        std::cin >> option;

        switch (option) {
            case 1:
                placeOrderToSupplier();
                break;
            case 2:
                viewSupplierOrders();
                break;
            case 3:
                viewCompletedSupplierOrders();
                break;
            case 0:
                std::cout << "Returning to Manager Menu...\n";
                break;
            default:
                std::cout << "Invalid option. Try again.\n";
        }
    } while (option != 0);
}
void Controller::manageClientsMenu() {
    int option;
    do {
        std::cout << "\n--- Manage Clients Menu ---\n";
        std::cout << "1. View all client orders\n";
        std::cout << "2. Delete client\n";
        std::cout << "3. Complete client order\n";
        std::cout << "0. Go back\n";
        std::cout << "Option: ";
        std::cin >> option;

        switch (option) {
            case 1:
                viewAllClientsOrders();
                break;
            case 2:
                deleteClientByEmail();
                break;
            case 3:
                try {
                    completeClientOrder();
                } catch (const OrderNotFoundException& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
                break;
            case 0:
                std::cout << "Returning to Manager Menu...\n";
                break;
            default:
                std::cout << "Invalid option. Try again.\n";
        }
    } while (option != 0);
}
void Controller::addProduct() {
    std::cin.ignore();

    std::string name, brand, category, description;
    int stock;
    float priceSupplier, priceClient;

    std::cout << "\n--- Add New Product ---\n";

    std::cout << "Name: ";
    std::getline(std::cin, name);

    std::cout << "Brand: ";
    std::getline(std::cin, brand);

    std::cout << "Stock quantity: ";
    std::cin >> stock;
    std::cin.ignore();

    std::cout << "Category: ";
    std::getline(std::cin, category);

    std::cout << "Description: ";
    std::getline(std::cin, description);

    std::cout << "Supplier price (EUR): ";
    std::cin >> priceSupplier;

    std::cout << "Selling price (EUR): ";
    std::cin >> priceClient;

    ProductContainer& products = store.getProducts();

    int newId = 1;

    for (int i = 0; i < products.getSize(); i++) {
        const Product& product = products.getProduct(i);

        if (product.getId() >= newId) {
            newId = product.getId() + 1;
        }
    }

    Product newProduct(newId, name, brand, stock, category, description, priceSupplier, priceClient);
    products.addProduct(newProduct);

    std::cout << "Product added successfully!\n";

    listProducts();
}
void Controller::editProduct() {
    int id;

    std::cout << "\n--- Edit Product ---\n";
    std::cout << "Enter product ID to edit: ";
    std::cin >> id;

    ProductContainer& products = store.getProducts();

    for (int i = 0; i < products.getSize(); i++) {
        Product& p = products.getProduct(i);

        if (p.getId() == id) {
            std::cin.ignore();

            std::cout << "Current name: " << p.getName() << "\n";
            std::cout << "New name (leave empty to keep): ";

            std::string input;
            std::getline(std::cin, input);

            if (!input.empty()) p.setName(input);

            std::cout << "Current brand: " << p.getBrand() << "\n";
            std::cout << "New brand (leave empty to keep): ";
            std::getline(std::cin, input);

            if (!input.empty()) p.setBrand(input);

            std::cout << "Current stock: " << p.getStock() << "\n";
            std::cout << "New stock (-1 to keep): ";

            int stockInput;
            std::cin >> stockInput;

            if (stockInput >= 0) p.setStock(stockInput);

            std::cin.ignore();

            std::cout << "Current category: " << p.getCategory() << "\n";
            std::cout << "New category (leave empty to keep): ";
            std::getline(std::cin, input);

            if (!input.empty()) p.setCategory(input);

            std::cout << "Current description: " << p.getDescription() << "\n";
            std::cout << "New description (leave empty to keep): ";
            std::getline(std::cin, input);

            if (!input.empty()) p.setDescription(input);

            std::cout << "Current supplier price: " << p.getPriceSupplier() << "\n";
            std::cout << "New supplier price (-1 to keep): ";

            float newSupplierPrice;
            std::cin >> newSupplierPrice;

            if (newSupplierPrice >= 0) p.setPriceSupplier(newSupplierPrice);

            std::cout << "Current selling price: " << p.getPriceClient() << "\n";
            std::cout << "New selling price (-1 to keep): ";

            float newClientPrice;
            std::cin >> newClientPrice;

            if (newClientPrice >= 0) p.setPriceClient(newClientPrice);

            std::cout << "Product updated successfully.\n";

            listProducts();
            return;
        }
    }

    throw ProductNotFoundException();
}
void Controller::deleteProduct() {
    int id;

    std::cout << "\n--- Delete Product ---\n";
    std::cout << "Enter product ID to delete: ";
    std::cin >> id;

    ProductContainer& products = store.getProducts();

    for (int i = 0; i < products.getSize(); i++) {
        const Product& product = products.getProduct(i);

        if (product.getId() == id) {
            std::cout << "Product found: " << product.getName()
                      << " by " << product.getBrand() << "\n";

            std::cout << "Are you sure you want to delete it? (y/n): ";

            char confirm;
            std::cin >> confirm;

            if (confirm == 'y' || confirm == 'Y') {
                products.removeProduct(id);
                std::cout << "Product deleted successfully.\n";
            } else {
                std::cout << "Deletion cancelled.\n";
            }

            listProducts();
            return;
        }
    }

    throw ProductNotFoundException();
}
void Controller::listProducts() {
    const ProductContainer& products = store.getProducts();
    const SupplierOrderContainer& supplierOrders = store.getSupplierOrders();

    std::cout << "\n--- Available Products ---\n";

    if (products.getSize() == 0) {
        std::cout << "No products available.\n";
        return;
    }

    for (int i = 0; i < products.getSize(); i++) {
        const Product& p = products.getProduct(i);
        int pending = 0;

        for (int j = 0; j < supplierOrders.getSize(); j++) {
            const SupplierOrder& order = supplierOrders.getOrder(j);

            if (!order.getStatus() &&
                order.getOrderStatus() != SupplierOrderStatus::Rejected) {
                const ProductContainer& orderProducts = order.getProducts();

                for (int k = 0; k < orderProducts.getSize(); k++) {
                    const Product& op = orderProducts.getProduct(k);

                    if (op.getId() == p.getId()) {
                        pending++;
                    }
                }
            }
        }

        std::cout << "ID: " << p.getId() << "\n";
        std::cout << "Name: " << p.getName() << "\n";
        std::cout << "Brand: " << p.getBrand() << "\n";
        std::cout << "Category: " << p.getCategory() << "\n";
        std::cout << "Description: " << p.getDescription() << "\n";
        std::cout << "Price: " << std::fixed << std::setprecision(2)
                  << p.getPriceClient() << " EUR\n";
        std::cout << "Stock: " << p.getStock();

        if (pending > 0) {
            std::cout << " (pending: " << pending << ")";
        }

        std::cout << "\n--------------------------\n";
    }
}
void Controller::placeOrderToSupplier() {
    SupplierContainer& suppliers = store.getSuppliers();
    ProductContainer& products = store.getProducts();
    SupplierOrderContainer& supplierOrders = store.getSupplierOrders();

    if (suppliers.getSize() == 0 || products.getSize() == 0) {
        std::cout << "No suppliers or products available.\n";
        return;
    }

    std::cout << "\n--- Suppliers and Their Products ---\n";

    for (int i = 0; i < suppliers.getSize(); i++) {
        const Supplier& supplier = suppliers.getSupplier(i);

        std::cout << "\nSupplier ID: " << supplier.getId()
                  << " | Name: " << supplier.getName() << "\n";

        for (int j = 0; j < products.getSize(); j++) {
            const Product& product = products.getProduct(j);

            if (product.getSupplier().getId() == supplier.getId()) {
                std::cout << "   - Product ID: " << product.getId()
                          << " | Name: " << product.getName()
                          << " | Stock: " << product.getStock() << "\n";
            }
        }
    }

    int productId;
    int quantity;

    std::cout << "\nEnter Product ID to order: ";
    std::cin >> productId;

    Product* chosenProduct = nullptr;

    for (int i = 0; i < products.getSize(); i++) {
        Product& product = products.getProduct(i);

        if (product.getId() == productId) {
            chosenProduct = &product;
            break;
        }
    }

    if (chosenProduct == nullptr) {
        std::cout << "Invalid Product ID.\n";
        return;
    }

    std::cout << "Quantity to order: ";
    std::cin >> quantity;

    if (quantity <= 0) {
        std::cout << "Invalid quantity.\n";
        return;
    }

    int orderId = 1;

    for (int i = 0; i < supplierOrders.getSize(); i++) {
        int currentOrderNumber = supplierOrders.getOrder(i).getOrderNumber();

        if (currentOrderNumber >= orderId) {
            orderId = currentOrderNumber + 1;
        }
    }

    SupplierOrder order(
        orderId,
        "2026-01-01",
        chosenProduct->getSupplier()
    );

    for (int i = 0; i < quantity; i++) {
        order.addProduct(*chosenProduct);
    }

    supplierOrders.addOrder(order);

    std::cout << "Order placed to supplier "
              << chosenProduct->getSupplier().getName()
              << "!\n";
}
// Função eliminar um cliente por email
void Controller::deleteClientByEmail() {
    std::string email;

    std::cout << "Enter the email of the client to delete: ";
    std::cin >> email;

    ClientContainer& clients = store.getClients();

    for (int i = 0; i < clients.getSize(); i++) {
        Client& client = clients.getClient(i);

        if (client.getEmail() == email) {
            const auto& orders = client.getOrders();

            for (const auto& order : orders) {
                if (!order.isDelivered()) {
                    std::cout << "Cannot delete client: He/Her have pending orders.\n";
                    return;
                }
            }

            std::cout << "Are you sure you want to delete this client? (y/n): ";

            char confirm;
            std::cin >> confirm;

            if (confirm == 'y' || confirm == 'Y') {
                clients.removeClient(email);
                std::cout << "Client deleted successfully.\n";
            } else {
                std::cout << "Deletion cancelled.\n";
            }

            return;
        }
    }

    std::cout << "Client with email \"" << email << "\" not found.\n";
}
void Controller::viewSupplierOrders() {
    SupplierOrderContainer& orders = store.getSupplierOrders();

    if (orders.getSize() == 0) {
        std::cout << "\nNo supplier orders found.\n";
        return;
    }

    while (true) {
        std::vector<int> pendingIndexes;

        std::cout << "\n--- Open Supplier Orders ---\n";

        int visibleIndex = 1;

        for (int i = 0; i < orders.getSize(); i++) {
            SupplierOrder& order = orders.getOrder(i);

            if (!order.getStatus()) {
                std::cout << visibleIndex << ". Order #"
                          << order.getOrderNumber()
                          << " | Supplier: "
                          << order.getSupplier().getName()
                          << "\n";

                const ProductContainer& products = order.getProducts();

                for (int j = 0; j < products.getSize(); j++) {
                    const Product& product = products.getProduct(j);
                    std::cout << "    - " << product.getName() << "\n";
                }

                pendingIndexes.push_back(i);
                visibleIndex++;
            }
        }

        if (pendingIndexes.empty()) {
            std::cout << "\nThere are no pending supplier orders.\n";
            break;
        }

        std::cout << "\nSelect an order to manage (0 to go back): ";

        int choice;
        std::cin >> choice;

        if (choice == 0) {
            break;
        }

        if (choice < 1 || choice > static_cast<int>(pendingIndexes.size())) {
            std::cout << "Invalid option.\n";
            continue;
        }

        int orderIndex = pendingIndexes[choice - 1];
        SupplierOrder& order = orders.getOrder(orderIndex);

        std::cout << "\nOrder #" << order.getOrderNumber()
                  << " | Supplier: "
                  << order.getSupplier().getName()
                  << "\n";

        const ProductContainer& products = order.getProducts();

        for (int j = 0; j < products.getSize(); j++) {
            const Product& product = products.getProduct(j);
            std::cout << "    - " << product.getName() << "\n";
        }

        std::cout << "Supplier response: " << order.getStatusText() << "\n";
        std::cout << "1. Mark as completed\n";
        std::cout << "2. Cancel order\n";
        std::cout << "0. Go back\n";
        std::cout << "Option: ";

        int action;
        std::cin >> action;

        if (action == 1) {
            if (order.getOrderStatus() == SupplierOrderStatus::Pending) {
                std::cout << "The supplier has not responded yet.\n";
                continue;
            }

            if (order.getOrderStatus() == SupplierOrderStatus::Rejected) {
                std::cout << "A rejected order cannot be completed.\n";
                continue;
            }

            for (int j = 0; j < products.getSize(); j++) {
                const Product& orderedProduct = products.getProduct(j);
                Product& product = store.findProductById(orderedProduct.getId());
                product.setStock(product.getStock() + 1);
            }

            order.markCompleted();

            std::cout << "Order marked as completed. Stock updated.\n";

        } else if (action == 2) {
            orders.removeByIndex(orderIndex);
            std::cout << "Order cancelled.\n";

        } else if (action == 0) {
            continue;

        } else {
            std::cout << "Invalid option.\n";
        }
    }
}
void Controller::viewCompletedSupplierOrders() {
    const SupplierOrderContainer& orders = store.getSupplierOrders();

    bool found = false;

    std::cout << "\n--- Completed Supplier Orders ---\n";

    for (int i = 0; i < orders.getSize(); i++) {
        const SupplierOrder& order = orders.getOrder(i);

        if (order.getStatus()) {
            found = true;

            std::cout << "\nOrder #" << order.getOrderNumber()
                      << " | Supplier: "
                      << order.getSupplier().getName()
                      << " | Response: " << order.getStatusText()
                      << "\n";

            const ProductContainer& products = order.getProducts();

            for (int j = 0; j < products.getSize(); j++) {
                const Product& product = products.getProduct(j);
                std::cout << "  - " << product.getName() << "\n";
            }
        }
    }

    if (!found) {
        std::cout << "\nNo completed supplier orders found.\n";
    }
}
void Controller::viewAllClientsOrders() {
    const ClientContainer& clients = store.getClients();

    if (clients.getSize() == 0) {
        std::cout << "No registered clients.\n";
        return;
    }

    for (int i = 0; i < clients.getSize(); i++) {
        const Client& c = clients.getClient(i);

        std::cout << "\n=============================\n";
        std::cout << "Client: " << c.getEmail() << "\n";

        const auto& orders = c.getOrders();

        if (orders.empty()) {
            std::cout << "No orders.\n";
            continue;
        }

        int count = 1;

        for (const ClientOrder& order : orders) {
            std::cout << "\n--- Order #" << count++ << " ---\n";
            order.show();
        }
    }

    std::cout << "\n=============================\n";
}
void Controller::completeClientOrder() {
    std::string email;

    std::cout << "Enter the client email: ";
    std::cin >> email;

    ClientContainer& clients = store.getClients();

    for (int i = 0; i < clients.getSize(); i++) {
        Client& client = clients.getClient(i);

        if (client.getEmail() == email) {
            auto& orders = client.getOrders();

            if (orders.empty()) {
                std::cout << "This client has no orders.\n";
                return;
            }

            std::cout << "\nOrders for " << email << ":\n";

            for (size_t j = 0; j < orders.size(); j++) {
                std::cout << "\nOrder #" << (j + 1) << ":\n";
                orders[j].show();
            }

            int choice;

            std::cout << "\nEnter the order number to mark as delivered: ";
            std::cin >> choice;

            if (choice < 1 || static_cast<size_t>(choice) > orders.size()) {
                throw OrderNotFoundException();
            }

            if (orders[choice - 1].isDelivered()) {
                std::cout << "This order is already marked as delivered.\n";
            } else {
                orders[choice - 1].setDelivered(true);
                std::cout << "Order marked as delivered.\n";
            }

            return;
        }
    }

    std::cout << "Client not found.\n";
}
void Controller::addProductToClientCart(const std::string& email, const Product& product, int quantity) {
    clientCarts[email].addProduct(product, quantity);
}

// Supplier implementation

bool Controller::isSupplierAuthenticated() {
    return loggedInSupplier != nullptr;
}

void Controller::runSupplier() {
    int option;

    do {
        View::showSupplierMainMenu();
        option = View::askSupplierMainOption();

        switch (option) {
            case 1:
                try {
                    loginSupplier();
                } catch (const InvalidLoginException& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
                break;

            case 0:
                std::cout << "Returning to main menu...\n";
                break;

            default:
                std::cout << "Invalid option.\n";
        }
    } while (option != 0);
}

void Controller::loginSupplier() {
    SupplierContainer& suppliers = store.getSuppliers();

    if (suppliers.getSize() == 0) {
        std::cout << "\nNo suppliers registered.\n";
        return;
    }

    std::string identifier;
    std::string password;

    std::cout << "\n--- Supplier Login ---\n";
    std::cout << "Email or supplier name: ";
    std::getline(std::cin >> std::ws, identifier);
    std::cout << "Password: ";
    std::cin >> password;

    for (int i = 0; i < suppliers.getSize(); i++) {
        const Supplier& supplier = suppliers.getSupplier(i);
        const bool identifierMatches =
                supplier.getEmail() == identifier ||
                supplier.getName() == identifier;

        if (identifierMatches && supplier.getPassword() == password) {
            loggedInSupplier = &supplier;

            std::cout << "Login successful. Welcome, "
                      << supplier.getName() << ".\n";

            runSupplierMenu();
            return;
        }
    }

    throw InvalidLoginException();
}

void Controller::runSupplierMenu() {
    if (!isSupplierAuthenticated()) {
        std::cout << "Access denied. Please login first.\n";
        return;
    }

    int option;

    do {
        std::cout << "\nSupplier: " << loggedInSupplier->getName() << "\n";
        View::showSupplierLoggedMenu();
        option = View::askSupplierLoggedOption();

        switch (option) {
            case 1:
                viewSupplierProducts();
                break;

            case 2:
                viewSupplierProductDetails();
                break;

            case 3:
                updateSupplierProductAvailability();
                break;

            case 4:
                listSupplierRestockRequests();
                break;

            case 5:
                viewSupplierRestockRequestDetails();
                break;

            case 6:
                respondToSupplierRestockRequest();
                break;

            case 0:
                loggedInSupplier = nullptr;
                std::cout << "Logging out...\n";
                break;

            default:
                std::cout << "Invalid option.\n";
        }
    } while (option != 0);
}

void Controller::viewSupplierProducts() {
    if (!isSupplierAuthenticated()) {
        std::cout << "Access denied. Please login first.\n";
        return;
    }

    const ProductContainer& products = store.getProducts();
    bool found = false;

    std::cout << "\n--- Products supplied by "
              << loggedInSupplier->getName() << " ---\n";

    for (int i = 0; i < products.getSize(); i++) {
        const Product& product = products.getProduct(i);

        if (product.getSupplier().getId() != loggedInSupplier->getId()) {
            continue;
        }

        found = true;
        std::cout << "ID: " << product.getId()
                  << " | Name: " << product.getName()
                  << " | Availability: " << product.getStock()
                  << " | Supplier price: " << std::fixed << std::setprecision(2)
                  << product.getPriceSupplier() << " EUR\n";
    }

    if (!found) {
        std::cout << "No supplied products.\n";
    }
}

void Controller::viewSupplierProductDetails() {
    if (!isSupplierAuthenticated()) {
        std::cout << "Access denied. Please login first.\n";
        return;
    }

    int productId;
    std::cout << "Enter product ID: ";
    std::cin >> productId;

    ProductContainer& products = store.getProducts();
    Product* product = findOwnedProduct(
            products,
            productId,
            loggedInSupplier->getId()
    );

    if (product == nullptr) {
        std::cout << "Product not available or access denied.\n";
        return;
    }

    std::cout << "\n--- Supplied Product Details ---\n";
    std::cout << "ID: " << product->getId() << "\n";
    std::cout << "Name: " << product->getName() << "\n";
    std::cout << "Brand: " << product->getBrand() << "\n";
    std::cout << "Category: " << product->getCategory() << "\n";
    std::cout << "Description: " << product->getDescription() << "\n";
    std::cout << "Availability: " << product->getStock() << "\n";
    std::cout << "Supplier price: " << std::fixed << std::setprecision(2)
              << product->getPriceSupplier() << " EUR\n";
    std::cout << "Client price: " << product->getPriceClient() << " EUR\n";
}

void Controller::updateSupplierProductAvailability() {
    if (!isSupplierAuthenticated()) {
        std::cout << "Access denied. Please login first.\n";
        return;
    }

    int productId;
    std::cout << "Enter product ID: ";
    std::cin >> productId;

    ProductContainer& products = store.getProducts();
    Product* product = findOwnedProduct(
            products,
            productId,
            loggedInSupplier->getId()
    );

    if (product == nullptr) {
        std::cout << "Product not available or access denied.\n";
        return;
    }

    std::cout << "Current availability: " << product->getStock() << "\n";

    int quantity;
    std::cout << "New available quantity: ";
    std::cin >> quantity;

    if (quantity < 0) {
        std::cout << "Invalid quantity.\n";
        return;
    }

    product->setStock(quantity);
    std::cout << "Product availability updated successfully.\n";
}

void Controller::listSupplierRestockRequests() {
    if (!isSupplierAuthenticated()) {
        std::cout << "Access denied. Please login first.\n";
        return;
    }

    const SupplierOrderContainer& orders = store.getSupplierOrders();
    bool found = false;

    std::cout << "\n--- Restock Requests for "
              << loggedInSupplier->getName() << " ---\n";

    for (int i = 0; i < orders.getSize(); i++) {
        const SupplierOrder& order = orders.getOrder(i);

        if (order.getSupplier().getId() != loggedInSupplier->getId()) {
            continue;
        }

        found = true;
        std::cout << "Order #" << order.getOrderNumber()
                  << " | Date: " << order.getDate()
                  << " | Status: " << order.getStatusText()
                  << "\n";
    }

    if (!found) {
        std::cout << "No restock orders.\n";
    }
}

void Controller::viewSupplierRestockRequestDetails() {
    if (!isSupplierAuthenticated()) {
        std::cout << "Access denied. Please login first.\n";
        return;
    }

    int orderNumber;
    std::cout << "Enter order number: ";
    std::cin >> orderNumber;

    SupplierOrderContainer& orders = store.getSupplierOrders();
    SupplierOrder* order = findOwnedSupplierOrder(
            orders,
            orderNumber,
            loggedInSupplier->getId()
    );

    if (order == nullptr) {
        std::cout << "Order not available or access denied.\n";
        return;
    }

    showSupplierOrderDetails(*order);
}

void Controller::respondToSupplierRestockRequest() {
    if (!isSupplierAuthenticated()) {
        std::cout << "Access denied. Please login first.\n";
        return;
    }

    int orderNumber;
    std::cout << "Enter order number: ";
    std::cin >> orderNumber;

    SupplierOrderContainer& orders = store.getSupplierOrders();
    SupplierOrder* order = findOwnedSupplierOrder(
            orders,
            orderNumber,
            loggedInSupplier->getId()
    );

    if (order == nullptr || !order->isPending()) {
        std::cout << "Order cannot be answered. It may not exist, may belong "
                     "to another supplier, or may already have a response.\n";
        return;
    }

    showSupplierOrderDetails(*order);

    std::cout << "\n1. Total confirmation\n";
    std::cout << "2. Partial confirmation\n";
    std::cout << "3. Reject\n";
    std::cout << "Response: ";

    int response;
    std::cin >> response;

    switch (response) {
        case 1:
            order->setOrderStatus(SupplierOrderStatus::Confirmed);
            break;

        case 2:
            order->setOrderStatus(SupplierOrderStatus::PartiallyConfirmed);
            break;

        case 3:
            order->setOrderStatus(SupplierOrderStatus::Rejected);
            break;

        default:
            std::cout << "Invalid response.\n";
            return;
    }

    std::cout << "Restock request response registered successfully. "
              << "New status: " << order->getStatusText() << "\n";
}

    std::cout << "Order completed successfully. Stock updated.\n";
}

void Controller::evaluateProduct() {
    if (!isAuthenticated()) {
        std::cout << "You must be logged in to evaluate a product.\n";
        return;
    }

    int productId;
    int stars;
    std::string comment;

    std::cout << "\n--- Evaluate Product ---\n";

    listProducts();

    std::cout << "Enter product ID: ";
    std::cin >> productId;

    bool productExists = false;

    ProductContainer& products = store.getProducts();

    for (int i = 0; i < products.getSize(); i++) {
        if (products.getProduct(i).getId() == productId) {
            productExists = true;
            break;
        }
    }

    if (!productExists) {
        std::cout << "Product not found.\n";
        return;
    }

    std::cout << "Stars (1 to 5): ";
    std::cin >> stars;

    if (stars < 1 || stars > 5) {
        std::cout << "Invalid number of stars. Use a value between 1 and 5.\n";
        return;
    }

    std::cin.ignore();

    std::cout << "Comment: ";
    std::getline(std::cin, comment);

    int newId = store.getEvaluations().getNextId();

    Evaluation evaluation(
            newId,
            productId,
            loggedInClient->getEmail(),
            stars,
            comment
    );

    store.addEvaluation(evaluation);

    std::cout << "Evaluation added successfully!\n";
}

void Controller::viewProductEvaluations() {
    int productId;

    std::cout << "\n--- View Product Evaluations ---\n";

    listProducts();

    std::cout << "Enter product ID: ";
    std::cin >> productId;

    EvaluationContainer& evaluations = store.getEvaluations();

    bool found = false;

    std::cout << "\n--- Evaluations for Product ID " << productId << " ---\n";

    for (int i = 0; i < evaluations.getSize(); i++) {
        const Evaluation& evaluation = evaluations.getEvaluation(i);

        if (evaluation.getProductId() == productId) {
            std::cout << "Evaluation ID: " << evaluation.getId() << "\n";
            std::cout << "Client: " << evaluation.getClientEmail() << "\n";
            std::cout << "Stars: " << evaluation.getStars() << "/5\n";
            std::cout << "Comment: " << evaluation.getComment() << "\n";
            std::cout << "--------------------------\n";

            found = true;
        }
    }

    if (!found) {
        std::cout << "This product has no evaluations yet.\n";
        return;
    }

    double average = evaluations.getAverageStarsByProductId(productId);

    std::cout << "Average stars: " << std::fixed << std::setprecision(1)
              << average << "/5\n";
}

