#include <iostream>
#include <algorithm>
#include <iomanip>
#include <fstream>
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
#include <thread>
#include <chrono>


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
    const auto& products = store.getProducts();
    int option;
    // Determinar qual carrinho usar
    Cart* cartPtr = nullptr;
    if (isAuthenticated()) {
        cartPtr = &clientCarts[loggedInClient->getEmail()];
    } else {
        static Cart guestCart;  // carrinho temporário para clientes não logados
        cartPtr = &guestCart;
    }
    Cart& cart = *cartPtr;  // referência ao carrinho correto
    std::cout << "\n--- Available Products ---\n";
    if (products.empty()) {
        std::cout << "No products available.\n";
        return;
    }
    for (const Product& p : products) {
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
                std::cout << "Returning to client menu  ...\n";
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

    for (Client& c : store.getClients()) {
        if (c.getEmail() == email && c.getPassword() == password) {
            std::cout << "Login successful! Welcome back, " << email << ".\n";
            loggedInClient = &c;  // <- guardar o cliente autenticado
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
    // Verificar formato do email
    if (!Utils::isValidEmail(email)) {
        std::cout << "Invalid email format. Use something like: XXXXXX@gmail.com\n";
        return;
    }
    // Verificar se já existe
    for (const Client& c : store.getClients()) {
        if (c.getEmail() == email) {
            std::cout << "An account with that email already exists.\n";
            return;
        }
    }
    std::cout << "Password: ";
    std::cin >> password;
    // Verificar tamanho mínimo da password
    if (password.length() < 4) {
        std::cout << "Password must have at least 4 characters.\n";
        return;
    }
    Client newClient(email, password);
    store.getClients().push_back(newClient);
    // Guardar cliente no ficheiro
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
    // Encontrar o produto pelo ID
    for (const Product& p : store.getProducts()) {
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
    std::cin.ignore(); // Limpa o buffer de entrada
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
    int newId = 1;
    const auto& products = store.getProducts();
    if (!products.empty()) {
        newId = products.back().getId() + 1;
    }
    Product newProduct(newId, name, brand, stock, category, description, priceSupplier, priceClient);
    store.getProducts().push_back(newProduct);
    std::cout << "Product added successfully!\n";
    listProducts();
}
void Controller::editProduct() {
    int id;
    std::cout << "\n--- Edit Product ---\n";
    std::cout << "Enter product ID to edit: ";
    std::cin >> id;
    // Procurar produto por ID
    std::vector<Product>& products = store.getProducts(); // obter referÃªncia
    for (Product& p : products) {
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
    listProducts();
}
void Controller::deleteProduct() {
    int id;
    std::cout << "\n--- Delete Product ---\n";
    std::cout << "Enter product ID to delete: ";
    std::cin >> id;
    std::vector<Product>& products = store.getProducts();
    auto it = std::find_if(products.begin(), products.end(), [id](const Product& p) {
        return p.getId() == id;
    });
    if (it != products.end()) {
        std::cout << "Product found: " << it->getName() << " by " << it->getBrand() << "\n";
        std::cout << "Are you sure you want to delete it? (y/n): ";
        char confirm;
        std::cin >> confirm;

        if (confirm == 'y' || confirm == 'Y') {
            products.erase(it);
            std::cout << "Product deleted successfully.\n";
        } else {
            std::cout << "Deletion cancelled.\n";
        }
    } else {
        throw ProductNotFoundException();
    }
    listProducts();
}
void Controller::listProducts() {
    const auto& products = store.getProducts();
    const auto& supplierOrders = store.getSupplierOrders();
    std::cout << "\n--- Available Products ---\n";
    if (products.empty()) {
        std::cout << "No products available.\n";
        return;
    }
    for (const Product& p : products) {
        int pending = 0;
        for (const SupplierOrder& order : supplierOrders) {
            if (!order.getStatus()) {
                for (const Product& op : order.getProducts()) {
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
    const auto& suppliers = store.getSuppliers();
    const auto& products = store.getProducts();
    if (suppliers.empty() || products.empty()) {
        std::cout << "No suppliers or products available.\n";
        return;
    }
    std::cout << "\n--- Suppliers and Their Products ---\n";
    for (const Supplier& s : suppliers) {
        std::cout << "\nSupplier ID: " << s.getId()
                  << " | Name: " << s.getName() << "\n";
        for (const Product& p : products) {
            if (p.getSupplier().getId() == s.getId()) {
                std::cout << "   - Product ID: " << p.getId()
                          << " | Name: " << p.getName()
                          << " | Stock: " << p.getStock() << "\n";
            }
        }
    }
    int productId, quantity;
    std::cout << "\nEnter Product ID to order: ";
    std::cin >> productId;
    Product* chosenProduct = nullptr;
    for (Product& p : store.getProducts()) {
        if (p.getId() == productId) {
            chosenProduct = &p;
            break;
        }
    }
    if (!chosenProduct) {
        std::cout << "Invalid Product ID.\n";
        return;
    }
    std::cout << "Quantity to order: ";
    std::cin >> quantity;
    int orderId = store.getSupplierOrders().size() + 1;
    SupplierOrder order(orderId, "2024-01-01", chosenProduct->getSupplier());
    for (int i = 0; i < quantity; ++i)
        order.addProduct(*chosenProduct);
    store.getSupplierOrders().push_back(order);
    std::cout << "Order placed to supplier " << chosenProduct->getSupplier().getName() << "!\n";
}
// Função eliminar um cliente por email
void Controller::deleteClientByEmail() {
    std::string email;
    std::cout << "Enter the email of the client to delete: ";
    std::cin >> email;
    std::vector<Client>& clients = store.getClients();

    for (auto it = clients.begin(); it != clients.end(); ++it) {
        if (it->getEmail() == email) {
            const auto& orders = it->getOrders();
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
                clients.erase(it);
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
    auto& orders = store.getSupplierOrders();
    if (orders.empty()) {
        std::cout << "\nNo supplier orders found.\n";
        return;
    }
    while (true) {
        std::vector<int> pendingIndexes;
        std::cout << "\n--- Pending Supplier Orders ---\n";
        int idx = 1;
        for (size_t i = 0; i < orders.size(); ++i) {
            if (!orders[i].getStatus()) {
                std::cout << idx << ". Order #" << orders[i].getOrderNumber()
                          << " | Supplier: " << orders[i].getSupplier().getName() << "\n";
                for (const Product& p : orders[i].getProducts()) {
                    std::cout << "    - " << p.getName() << "\n";
                }
                pendingIndexes.push_back(i);
                ++idx;
            }
        }
        if (pendingIndexes.empty()) {
            std::cout << "\nThere are no pending supplier orders.\n";
            break;
        }
        std::cout << "\nSelect an order to manage (0 to go back): ";
        int choice;
        std::cin >> choice;
        if (choice == 0) break;
        if (choice < 1 || choice > (int)pendingIndexes.size()) {
            std::cout << "Invalid option.\n";
            continue;
        }
        int orderIdx = pendingIndexes[choice - 1];
        SupplierOrder& order = orders[orderIdx];
        std::cout << "\nOrder #" << order.getOrderNumber() << " | Supplier: " << order.getSupplier().getName() << "\n";
        for (const Product& p : order.getProducts()) {
            std::cout << "    - " << p.getName() << "\n";
        }
        std::cout << "1. Mark as completed\n";
        std::cout << "2. Cancel order\n";
        std::cout << "0. Go back\n";
        std::cout << "Option: ";
        int action;
        std::cin >> action;
        if (action == 1) {
            order.markCompleted();
            std::cout << "Order marked as completed!\n";
        } else if (action == 2) {
            orders.erase(orders.begin() + orderIdx);
            std::cout << "Order cancelled!\n";
        } else if (action == 0) {
            continue;
        } else {
            std::cout << "Invalid option.\n";
        }
    }
}
void Controller::viewCompletedSupplierOrders() {
    const auto& orders = store.getSupplierOrders();
    bool found = false;
    for (const SupplierOrder& order : orders) {
        if (order.getStatus()) {
            found = true;
            std::cout << "Order #" << order.getOrderNumber()
                      << " | Supplier: " << order.getSupplier().getName() << "\n";
            for (const Product& p : order.getProducts()) {
                std::cout << "  - " << p.getName() << "\n";
            }
        }
    }
    if (!found)
        std::cout << "\nNo completed supplier orders found.\n";
}
void Controller::viewAllClientsOrders() {
    const auto& clients = store.getClients();
    if (clients.empty()) {
        std::cout << "No registered clients.\n";
        return;
    }
    for (const Client& c : clients) {
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
            order.show();  // já imprime produtos, total e status
        }
    }
    std::cout << "\n=============================\n";
}
void Controller::completeClientOrder() {
    std::string email;
    std::cout << "Enter the client email: ";
    std::cin >> email;
    std::vector<Client>& clients = store.getClients();  // permite modificar
    for (Client& client : clients) {
        if (client.getEmail() == email) {
            auto& orders = client.getOrders();
            if (orders.empty()) {
                std::cout << "This client has no orders.\n";
                return;
            }
            std::cout << "\nOrders for " << email << ":\n";
            for (size_t i = 0; i < orders.size(); ++i) {
                std::cout << "\nOrder #" << (i + 1) << ":\n";
                orders[i].show();
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
        std::cout << "\n--- Supplier Menu ---\n";
        std::cout << "1. Login\n";
        std::cout << "0. Back\n";
        std::cout << "Option: ";
        std::cin >> option;

        switch (option) {
            case 1:
                loginSupplier();
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
    std::string email;

    const std::vector<Supplier>& suppliers = store.getSuppliers();

    if (suppliers.empty()) {
        std::cout << "\nNo suppliers registered.\n";
        return;
    }

    std::cout << "\n--- Supplier Login ---\n";
    std::cout << "Email: ";
    std::cin >> email;

    for (const Supplier& supplier : suppliers) {
        if (supplier.getEmail() == email) {
            loggedInSupplier = &supplier;

            std::cout << "Login successful. Welcome, "
                      << supplier.getName() << ".\n";

            runSupplierMenu();
            return;
        }
    }

    std::cout << "Supplier not found.\n";
}

void Controller::runSupplierMenu() {
    if (!isSupplierAuthenticated()) {
        std::cout << "Access denied. Please login first.\n";
        return;
    }

    int option;

    do {
        std::cout << "\n--- Supplier Area ---\n";
        std::cout << "Supplier: " << loggedInSupplier->getName() << "\n";
        std::cout << "1. View pending orders\n";
        std::cout << "2. Complete order\n";
        std::cout << "3. View completed orders\n";
        std::cout << "0. Logout\n";
        std::cout << "Option: ";
        std::cin >> option;

        switch (option) {
            case 1:
                viewSupplierOwnPendingOrders();
                break;

            case 2:
                completeSupplierOwnOrder();
                break;

            case 3:
                viewSupplierOwnCompletedOrders();
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

void Controller::viewSupplierOwnPendingOrders() {
    if (!isSupplierAuthenticated()) {
        std::cout << "Access denied. Please login first.\n";
        return;
    }

    const auto& orders = store.getSupplierOrders();
    bool found = false;

    std::cout << "\n--- Pending Orders for "
              << loggedInSupplier->getName() << " ---\n";

    for (const SupplierOrder& order : orders) {
        if (order.getSupplier().getId() == loggedInSupplier->getId()
            && !order.getStatus()) {

            found = true;

            std::cout << "\nOrder #" << order.getOrderNumber() << "\n";

            for (const Product& product : order.getProducts()) {
                std::cout << "  - " << product.getName()
                          << " | Brand: " << product.getBrand()
                          << "\n";
            }
        }
    }

    if (!found) {
        std::cout << "No pending orders found.\n";
    }
}

void Controller::viewSupplierOwnCompletedOrders() {
    if (!isSupplierAuthenticated()) {
        std::cout << "Access denied. Please login first.\n";
        return;
    }

    const auto& orders = store.getSupplierOrders();
    bool found = false;

    std::cout << "\n--- Completed Orders for "
              << loggedInSupplier->getName() << " ---\n";

    for (const SupplierOrder& order : orders) {
        if (order.getSupplier().getId() == loggedInSupplier->getId()
            && order.getStatus()) {

            found = true;

            std::cout << "\nOrder #" << order.getOrderNumber() << "\n";

            for (const Product& product : order.getProducts()) {
                std::cout << "  - " << product.getName()
                          << " | Brand: " << product.getBrand()
                          << "\n";
            }
        }
    }

    if (!found) {
        std::cout << "No completed orders found.\n";
    }
}

void Controller::completeSupplierOwnOrder() {
    if (!isSupplierAuthenticated()) {
        std::cout << "Access denied. Please login first.\n";
        return;
    }

    auto& orders = store.getSupplierOrders();
    std::vector<int> pendingIndexes;

    std::cout << "\n--- Complete Supplier Order ---\n";

    int visibleIndex = 1;

    for (size_t i = 0; i < orders.size(); ++i) {
        if (orders[i].getSupplier().getId() == loggedInSupplier->getId()
            && !orders[i].getStatus()) {

            std::cout << visibleIndex << ". Order #"
                      << orders[i].getOrderNumber() << "\n";

            for (const Product& product : orders[i].getProducts()) {
                std::cout << "   - " << product.getName() << "\n";
            }

            pendingIndexes.push_back(static_cast<int>(i));
            visibleIndex++;
        }
    }

    if (pendingIndexes.empty()) {
        std::cout << "No pending orders to complete.\n";
        return;
    }

    int choice;
    std::cout << "\nSelect order to complete: ";
    std::cin >> choice;

    if (choice < 1 || choice > static_cast<int>(pendingIndexes.size())) {
        std::cout << "Invalid order option.\n";
        return;
    }

    SupplierOrder& order = orders[pendingIndexes[choice - 1]];

    for (const Product& orderedProduct : order.getProducts()) {
        Product& product = store.findProductById(orderedProduct.getId());
        product.setStock(product.getStock() + 1);
    }

    order.markCompleted();

    std::cout << "Order completed successfully. Stock updated.\n";
}