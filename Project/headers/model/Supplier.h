//
// Created by cmgm on 16/05/2026.
//

#ifndef FSOFT2026_1DC__SUPPLIER_H
#define FSOFT2026_1DC__SUPPLIER_H

#include <string>

class Supplier {
private:
    int id;
    std::string name;
    std::string contact;
    std::string email;
    std::string password;

public:
    Supplier();

    Supplier(int id, const std::string& name, const std::string& contact,
             const std::string& email);

    Supplier(int id, const std::string& name, const std::string& contact,
             const std::string& email, const std::string& password);

    int getId() const;
    std::string getName() const;
    std::string getContact() const;
    std::string getEmail() const;
    std::string getPassword() const;

    void setEmail(const std::string& newEmail);
    void setPassword(const std::string& newPassword);
};

#endif //FSOFT2026_1DC__SUPPLIER_H
