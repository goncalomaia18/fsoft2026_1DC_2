//
// Created by cmgm on 16/05/2026.
//

#ifndef FSOFT2026_1DC__EMPTYCARTEXCEPTION_H
#define FSOFT2026_1DC__EMPTYCARTEXCEPTION_H
#include <exception>

class EmptyCartException : public std::exception {
public:
    const char* what() const noexcept override;
};

#endif //FSOFT2026_1DC__EMPTYCARTEXCEPTION_H
