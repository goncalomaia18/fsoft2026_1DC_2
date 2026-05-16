//
// Created by cmgm on 16/05/2026.
//

#include <iostream>
#include "../../headers/views/View.h"

void View::showMainMenu() {
    std::cout << "\n=== PescaTudo ===\n";
    std::cout << "1. Client Area\n";
    std::cout << "2. Manager Area\n";
    std::cout << "3. Supplier Area\n";
    std::cout << "0. Exit\n";
}

int View::askMainOption() {
    int option;
    std::cout << "Select an option: ";
    std::cin >> option;
    return option;
}

void View::showClientMenu() {
    std::cout << "\n--- Main Menu ---\n";
    std::cout << "1. View Products\n";
    std::cout << "2. Login\n";
    std::cout << "3. Sign Up\n";
    std::cout << "0. Exit\n";
}

int View::askClientOption() {
    int option;
    std::cout << "Select an option: ";
    std::cin >> option;
    return option;
}

void View::showManagerMainMenu() {
    std::cout << "\n--- Main Menu ---\n";
    std::cout << "1. Login\n";
    std::cout << "0. Exit\n";
}

int View::askManagerMainOption() {
    int option;
    std::cout << "Select an option: ";
    std::cin >> option;
    return option;
}