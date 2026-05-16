//
// Created by cmgm on 16/05/2026.
//

#ifndef FSOFT2026_1DC__CLIENT_H
#define FSOFT2026_1DC__CLIENT_H
#include <string>
#include "ClientOrder.h"

class Client {
private:
    std::string email;
    std::string password;
    std::vector<ClientOrder> orders;
    std::string name;



public:
    Client(); // vazio
    Client(const std::string& email, const std::string& password);

    std::string getName() const;
    std::string getEmail() const;
    std::string getPassword() const;
    void addOrder(const ClientOrder& order);
    std::vector<ClientOrder>& getOrders();
    const std::vector<ClientOrder>& getOrders() const;


};
#endif //FSOFT2026_1DC__CLIENT_H
