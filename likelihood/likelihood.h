#ifndef LIKELIHOOD_H
#define LIKELIHOOD_H
#include "lgraph.h"
#include "../models/parameters.h"
#include "graphstatistics.h"

#include <QVector>

class Likelihood
{
protected:
    LGraph m_data;
    LGraph m_model;
    GraphStatistics m_statistics;

    LGraph m_likelihood;
    Parameters* m_parameters;
    Parameter* m_parameter;
    QPointF m_minVal;
    double m_currentVal;
    double m_stepSize;

    int m_bins, m_currentBin;

    bool m_ready = false;
    bool m_done = false;

    void tickLikelihood();
    void tickModelStatistics();

public:
    Likelihood();

    virtual void calculateModel(Parameters* p) = 0;
    enum AnalysisType { LikelihoodStatistics, ModelStatistics };
    AnalysisType m_analysisType = AnalysisType::LikelihoodStatistics;

    void bruteForce1D(int bins, Parameter* parameter, Parameters* parameterrs);
    void modelAnalysis(int count, Parameters* parameters);

    bool tick();


    LGraph data() const;
    void setData(const LGraph &data);
    LGraph model() const;
    void setModel(const LGraph &model);
    LGraph likelihood() const;
    void setLikelihood(const LGraph &likelihood);
    QPointF getMinVal() const;
    bool getDone() const;
    void setDone(bool done);
};

#endif // LIKELIHOOD_H
