//
// Created by cmgm on 16/05/2026.
//

#ifndef FSOFT2026_1DC__MANAGER_H
#define FSOFT2026_1DC__MANAGER_H
#include <string>

class Manager {
private:
    std::string email;
    std::string password;

public:
    Manager();
    Manager(const std::string& email, const std::string& password);

    std::string getEmail() const;
    std::string getPassword() const;
};
#endif //FSOFT2026_1DC__MANAGER_H
