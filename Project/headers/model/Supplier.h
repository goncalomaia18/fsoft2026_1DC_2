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

public:
    Supplier() = default;
    Supplier(int id, const std::string& name, const std::string& contact, const std::string& email);

    int getId() const;
    std::string getName() const;
    std::string getContact() const { return contact; }
    std::string getEmail() const { return email; }

    void setEmail(const std::string& newEmail);
};

#endif //FSOFT2026_1DC__SUPPLIER_H
