
#ifndef FSOFT2026_1DC__EVALUATION_H
#define FSOFT2026_1DC__EVALUATION_H

#include <string>

class Evaluation {
private:
    int id;
    int productId;
    std::string clientEmail;
    int stars;
    std::string comment;

public:
    Evaluation();
    Evaluation(int id, int productId, const std::string& clientEmail, int stars, const std::string& comment);

    int getId() const;
    int getProductId() const;
    std::string getClientEmail() const;
    int getStars() const;
    std::string getComment() const;
};

#endif //FSOFT2026_1DC__EVALUATION_H
