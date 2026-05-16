#include "../../headers/model/Supplier.h"

Supplier::Supplier(int id, const std::string& name, const std::string& contact, const std::string& email)
        : id(id), name(name), contact(contact), email(email) {}

int Supplier::getId() const { return id; }
std::string Supplier::getName() const { return name; }
void Supplier::setEmail(const std::string& newEmail) { email = newEmail; }