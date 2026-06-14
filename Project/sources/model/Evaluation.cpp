#include "../../headers/model/Evaluation.h"

Evaluation::Evaluation() {
    id = 0;
    productId = 0;
    clientEmail = "";
    stars = 0;
    comment = "";
}

Evaluation::Evaluation(int id, int productId, const std::string& clientEmail, int stars, const std::string& comment) {
    this->id = id;
    this->productId = productId;
    this->clientEmail = clientEmail;
    this->stars = stars;
    this->comment = comment;
}

int Evaluation::getId() const {
    return id;
}

int Evaluation::getProductId() const {
    return productId;
}

std::string Evaluation::getClientEmail() const {
    return clientEmail;
}

int Evaluation::getStars() const {
    return stars;
}

std::string Evaluation::getComment() const {
    return comment;
}