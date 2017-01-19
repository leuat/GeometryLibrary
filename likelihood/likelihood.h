#ifndef LIKELIHOOD_H
#define LIKELIHOOD_H
#include "lgraph.h"
#include "../parameters.h"
#include "graphstatistics.h"

#include <QVector>

class Likelihood
{
protected:
    struct MCData {
        QString parametersFilename;
        QString bestFarametersFilename;
        int currentStep = 0;
        int rejectedSteps = 0;
        int totalSteps = 100;
        float chiSquared = 0;
    };

    LGraph m_data;
    LGraph m_modelData;
    GraphStatistics m_statistics;
    LGraph m_likelihood;
    QString m_currentParameter;
    QString m_likelihoodFileName = "mc.txt";

    class Model *m_model = nullptr;
    MCData *m_mcData = nullptr;
    QPointF m_minVal;
    double m_currentVal=0;
    double m_stepSize=1;

    int m_bins=0, m_currentBin=0;
    bool m_done = false;

    void tickLikelihoodMonteCarlo();
    void tickLikelihoodBruteforce1D();
    void tickLikelihoodFullMonteCarlo();
    void tickModelStatistics();

    bool debugGraphs = false;

public:
    Likelihood();
    void setDebug(bool b) {
        debugGraphs = b;
    }

    virtual void calculateModel(class Model* modelData) = 0;
    enum AnalysisType { LikelihoodStatistics, ModelStatistics, None };
    enum AnalysisAlgorithm { Bruteforce1D, MonteCarlo, FullMonteCarlo };
    AnalysisType m_analysisType = AnalysisType::None;
    AnalysisAlgorithm m_analysisAlgorithm = AnalysisAlgorithm::Bruteforce1D;
    void bruteForce1D(int bins, QString parameterKey, Model *modelData);
    void monteCarlo(Model *model, int steps, AnalysisAlgorithm analysisAlgorithm);
    void modelAnalysis(int count, Model *modelData);
    bool tick();

    LGraph data() const;
    void setData(const LGraph &data);
    LGraph modelData() const;
    void setModelData(const LGraph &modelData);
    LGraph& likelihood();
    void setLikelihood(const LGraph &likelihood);
    QPointF getMinVal() const;
    bool getDone() const;
    void setDone(bool done);
    GraphStatistics getStatistics() const;
    QString getLikelihoodFileName() const;
    void setLikelihoodFileName(const QString &likelihoodFileName);
};

#endif // LIKELIHOOD_H
