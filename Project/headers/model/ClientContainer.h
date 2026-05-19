#ifndef FSOFT2026_1DC__CLIENTCONTAINER_H
#define FSOFT2026_1DC__CLIENTCONTAINER_H

#include "Client.h"
#include <string>
#include <stdexcept>

class ClientContainer {
private:
    Client* clientsArray;
    int capacity;
    int count;

    void expand();

public:
    ClientContainer();
    ~ClientContainer();

    void addClient(const Client& client);
    Client& findByEmail(const std::string& email);
    void removeClient(const std::string& email);

    int getSize() const;
    Client& getClient(int index);
    const Client& getClient(int index) const;
};

#endif //FSOFT2026_1DC__CLIENTCONTAINER_H
