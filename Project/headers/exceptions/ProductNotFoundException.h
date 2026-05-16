//
// Created by cmgm on 16/05/2026.
//

#ifndef FSOFT2026_1DC__PRODUCTNOTFOUNDEXCEPTION_H
#define FSOFT2026_1DC__PRODUCTNOTFOUNDEXCEPTION_H
#include <exception>

class ProductNotFoundException : public std::exception {
public:
    const char* what() const noexcept override;
};

#endif //FSOFT2026_1DC__PRODUCTNOTFOUNDEXCEPTION_H
