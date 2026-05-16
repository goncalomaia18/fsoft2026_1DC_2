#include "../../headers/model/Cart.h"
#include "../../headers/model/Product.h"
#include "../../headers/exceptions/ProductNotFoundException.h"
#include <iostream>
#include <iomanip>

void Cart::addProduct(const Product& product, int quantity) {
    for (auto& item : items) {
        if (item.first.getName() == product.getName()) {
            item.second += quantity;
            return;
        }
    }
    items.push_back({product, quantity});
}

void Cart::removeProductById(int productId) {
    for (auto it = items.begin(); it != items.end(); ++it) {
        if (it->first.getId() == productId) {
            int currentQty = it->second;
            std::cout << "Product found in cart with " << currentQty << " unit(s).\n";
            std::cout << "How many units do you want to remove? ";
            int toRemove;
            std::cin >> toRemove;

            if (toRemove <= 0) {
                std::cout << "Invalid quantity.\n";
                return;
            }

            if (toRemove >= currentQty) {
                items.erase(it);
                std::cout << "Product completely removed from the cart.\n";
            } else {
                it->second -= toRemove;
                std::cout << toRemove << " unit(s) removed. Remaining quantity: " << it->second << "\n";
            }
            return;
        }
    }
    throw ProductNotFoundException();

}


void Cart::showCart() const {
    if (items.empty()) {
        std::cout << "Cart is empty.\n";
        return;
    }

    std::cout << "\n--- Cart Contents ---\n";
    float total = 0.0f;

    for (const auto& item : items) {
        const Product& p = item.first;
        std::cout << "ID: " << p.getId()
                  << " | Product: " << p.getName()
                  << " | Quantity: " << item.second
                  << " | Price: " << std::fixed << std::setprecision(2)
                  << p.getPriceClient() << " EUR\n";

        total += p.getPriceClient() * item.second;
    }

    std::cout << "------------------------\n";
    std::cout << "Total: " << std::fixed << std::setprecision(2)
              << total << " EUR\n";
}


float Cart::getTotal() const {
    float total = 0.0f;
    for (const auto& item : items) {
        total += item.first.getPriceClient() * item.second;
    }
    return total;
}

bool Cart::isEmpty() const {
    return items.empty();
}

void Cart::clear() {
    items.clear();  // Limpa todos os itens no carrinho
}

const std::vector<std::pair<Product, int>>& Cart::getItems() const {
    return items;
}