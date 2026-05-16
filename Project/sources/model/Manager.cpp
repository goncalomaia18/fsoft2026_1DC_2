#include "../../headers/model/Manager.h"

Manager::Manager() {}

Manager::Manager(const std::string& email, const std::string& password)
        : email(email), password(password) {}

std::string Manager::getEmail() const {
    return email;
}

std::string Manager::getPassword() const {
    return password;
}