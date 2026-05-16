//
// Created by cmgm on 16/05/2026.
//

#ifndef FSOFT2026_1DC__ORDERNOTFOUNDEXCEPTION_H
#define FSOFT2026_1DC__ORDERNOTFOUNDEXCEPTION_H
#include <exception>

class OrderNotFoundException : public std::exception {
public:
    const char* what() const noexcept override;
};
#endif //FSOFT2026_1DC__ORDERNOTFOUNDEXCEPTION_H
