#include "../../headers/model/ClientContainer.h"

ClientContainer::ClientContainer() {
    capacity = 5;
    count = 0;
    clientsArray = new Client[capacity];
}

ClientContainer::~ClientContainer() {
    delete[] clientsArray;
}

void ClientContainer::expand() {
    capacity *= 2;
    Client* newArray = new Client[capacity];

    for (int i = 0; i < count; i++) {
        newArray[i] = clientsArray[i];
    }

    delete[] clientsArray;
    clientsArray = newArray;
}

void ClientContainer::addClient(const Client& client) {
    if (count == capacity) {
        expand();
    }
    clientsArray[count] = client;
    count++;
}

Client& ClientContainer::findByEmail(const std::string& email) {
    for (int i = 0; i < count; i++) {
        if (clientsArray[i].getEmail() == email) {
            return clientsArray[i];
        }
    }
    throw std::runtime_error("Client not found.");
}

void ClientContainer::removeClient(const std::string& email) {
    int indexToRemove = -1;
    for (int i = 0; i < count; i++) {
        if (clientsArray[i].getEmail() == email) {
            indexToRemove = i;
            break;
        }
    }

    if (indexToRemove == -1) {
        throw std::runtime_error("Client not found to delete.");
    }

    for (int i = indexToRemove; i < count - 1; i++) {
        clientsArray[i] = clientsArray[i + 1];
    }

    count--;
}

int ClientContainer::getSize() const {
    return count;
}

Client& ClientContainer::getClient(int index) {
    if (index < 0 || index >= count) {
        throw std::out_of_range("Index invalido.");
    }
    return clientsArray[index];
}

const Client& ClientContainer::getClient(int index) const {
    if (index < 0 || index >= count) {
        throw std::out_of_range("Index invalido.");
    }
    return clientsArray[index];
}
