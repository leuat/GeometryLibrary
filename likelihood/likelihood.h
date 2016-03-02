#ifndef LIKELIHOOD_H
#define LIKELIHOOD_H
#include "lgraph.h"
#include "../parameters.h"
#include "graphstatistics.h"

#include <QVector>

class Likelihood
{
protected:
    LGraph m_data;
    LGraph m_modelData;
    GraphStatistics m_statistics;
    LGraph m_likelihood;
    QString m_currentParameter;
    class Model *m_model = nullptr;

    QPointF m_minVal;
    double m_currentVal=0;
    double m_stepSize=1;

    int m_bins=0, m_currentBin=0;
    bool m_ready = false;
    bool m_done = false;

    void tickLikelihood();
    void tickModelStatistics();

public:
    Likelihood();

    virtual void calculateModel(class Model* modelData) = 0;
    enum AnalysisType { LikelihoodStatistics, ModelStatistics };
    AnalysisType m_analysisType = AnalysisType::LikelihoodStatistics;
    void bruteForce1D(int bins, QString parameterKey, Model *modelData);
    void modelAnalysis(int count, Model *modelData);
    bool tick();

    LGraph data() const;
    void setData(const LGraph &data);
    LGraph modelData() const;
    void setModelData(const LGraph &modelData);
    LGraph likelihood() const;
    void setLikelihood(const LGraph &likelihood);
    QPointF getMinVal() const;
    bool getDone() const;
    void setDone(bool done);
    GraphStatistics getStatistics() const;
};

#endif // LIKELIHOOD_H
