#include "../headers/model/Client.h"

Client::Client(const std::string& email, const std::string& password)
        : email(email), password(password) {}

std::string Client::getEmail() const {
    return email;
}

std::string Client::getPassword() const {
    return password;
}

std::string Client::getName() const {
    return name;
}

void Client::addOrder(const ClientOrder& order) {
    orders.push_back(order);
}
const std::vector<ClientOrder>& Client::getOrders() const {
    return orders;
}
std::vector<ClientOrder>& Client::getOrders() {
    return orders;
}