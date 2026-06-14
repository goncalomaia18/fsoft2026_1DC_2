
#ifndef FSOFT2026_1DC__EVALUATIONCONTAINER_H
#define FSOFT2026_1DC__EVALUATIONCONTAINER_H

#include "Evaluation.h"
#include <stdexcept>

class EvaluationContainer {
private:
    Evaluation* evaluationsArray;
    int capacity;
    int count;

    void expand();

public:
    EvaluationContainer();
    ~EvaluationContainer();

    void addEvaluation(const Evaluation& evaluation);

    int getSize() const;
    Evaluation& getEvaluation(int index);
    const Evaluation& getEvaluation(int index) const;

    int getNextId() const;
    double getAverageStarsByProductId(int productId) const;
};


#endif //FSOFT2026_1DC__EVALUATIONCONTAINER_H
