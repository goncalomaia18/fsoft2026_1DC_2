#include "../../headers/model/EvaluationContainer.h"

EvaluationContainer::EvaluationContainer() {
    capacity = 5;
    count = 0;
    evaluationsArray = new Evaluation[capacity];
}

EvaluationContainer::~EvaluationContainer() {
    delete[] evaluationsArray;
}

void EvaluationContainer::expand() {
    capacity *= 2;
    Evaluation* newArray = new Evaluation[capacity];

    for (int i = 0; i < count; i++) {
        newArray[i] = evaluationsArray[i];
    }

    delete[] evaluationsArray;
    evaluationsArray = newArray;
}

void EvaluationContainer::addEvaluation(const Evaluation& evaluation) {
    if (count == capacity) {
        expand();
    }

    evaluationsArray[count] = evaluation;
    count++;
}

int EvaluationContainer::getSize() const {
    return count;
}

Evaluation& EvaluationContainer::getEvaluation(int index) {
    if (index < 0 || index >= count) {
        throw std::out_of_range("Index invalido.");
    }

    return evaluationsArray[index];
}

const Evaluation& EvaluationContainer::getEvaluation(int index) const {
    if (index < 0 || index >= count) {
        throw std::out_of_range("Index invalido.");
    }

    return evaluationsArray[index];
}

int EvaluationContainer::getNextId() const {
    return count + 1;
}

double EvaluationContainer::getAverageStarsByProductId(int productId) const {
    int totalStars = 0;
    int numberOfEvaluations = 0;

    for (int i = 0; i < count; i++) {
        if (evaluationsArray[i].getProductId() == productId) {
            totalStars += evaluationsArray[i].getStars();
            numberOfEvaluations++;
        }
    }

    if (numberOfEvaluations == 0) {
        return 0.0;
    }

    return static_cast<double>(totalStars) / numberOfEvaluations;
}