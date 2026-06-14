#include "../../headers/model/Supplier.h"

Supplier::Supplier()
        : id(0), name(""), contact(""), email(""), password("") {}

Supplier::Supplier(int id, const std::string& name, const std::string& contact,
                   const std::string& email)
        : Supplier(id, name, contact, email, "") {}

Supplier::Supplier(int id, const std::string& name, const std::string& contact,
                   const std::string& email, const std::string& password)
        : id(id), name(name), contact(contact), email(email), password(password) {}

int Supplier::getId() const {
    return id;
}

std::string Supplier::getName() const {
    return name;
}

std::string Supplier::getContact() const {
    return contact;
}

std::string Supplier::getEmail() const {
    return email;
}

std::string Supplier::getPassword() const {
    return password;
}

void Supplier::setEmail(const std::string& newEmail) {
    email = newEmail;
}

void Supplier::setPassword(const std::string& newPassword) {
    password = newPassword;
}
