#ifndef LIKELIHOOD_H
#define LIKELIHOOD_H
#include "lgraph.h"
#include "../models/parameters.h"

class Likelihood
{
protected:
    LGraph m_data;
    LGraph m_model;

    LGraph m_likelihood;
    Parameters* m_parameters;
    Parameter* m_parameter;
    double m_currentVal;
    double m_stepSize;
    int m_bins, m_currentBin;
    bool m_ready = false;

public:
    Likelihood();

    virtual void CalculateModel(Parameters* p) = 0;

    void BruteForce1D(int bins, Parameter* p, Parameters* params);
    bool Tick();

    LGraph data() const;
    void setData(const LGraph &data);
    LGraph model() const;
    void setModel(const LGraph &model);
    LGraph likelihood() const;
    void setLikelihood(const LGraph &likelihood);
};

#endif // LIKELIHOOD_H
