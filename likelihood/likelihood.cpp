#include "likelihood.h"
#include <qdebug.h>
#include "../models/model.h"
#include <QStandardPaths>
#include "../misc/random.h"



LGraph Likelihood::data() const
{
    return m_data;
}

void Likelihood::setData(const LGraph &data)
{
    m_data = data;
}

LGraph Likelihood::modelData() const
{
    return m_modelData;
}

void Likelihood::setModelData(const LGraph &modelData)
{
    m_modelData = modelData;
}

LGraph Likelihood::likelihood() const
{
    return m_likelihood;
}

void Likelihood::setLikelihood(const LGraph &likelihood)
{
    m_likelihood = likelihood;
}

QPointF Likelihood::getMinVal() const
{
    return m_minVal;
}

bool Likelihood::getDone() const
{
    return m_done;
}

void Likelihood::setDone(bool done)
{
    m_done = done;
}

GraphStatistics Likelihood::getStatistics() const
{
    return m_statistics;
}

QString Likelihood::getLikelihoodFileName() const
{
    return m_likelihoodFileName;
}

void Likelihood::setLikelihoodFileName(const QString &likelihoodFileName)
{
    m_likelihoodFileName = likelihoodFileName;
}

void Likelihood::tickLikelihoodMonteCarlo()
{
    m_model->parameters()->save(m_mcData->parametersFilename);
    m_model->randomWalk();
    calculateModel(m_model);
    float chiSquared = LGraph::ChiSQ(m_data, m_modelData);
    if(chiSquared < m_mcData->chiSquared) {
        m_mcData->chiSquared = chiSquared;
        m_model->parameters()->save(m_mcData->bestFarametersFilename);
        qDebug() << "   Accepted move, current chi squared: " << chiSquared << endl;
    } else {
        m_model->parameters()->load(m_mcData->parametersFilename);
    }

    if(++m_mcData->currentStep == m_mcData->totalSteps) {
        calculateModel(m_model);
        chiSquared = m_mcData->chiSquared;
        m_analysisType = AnalysisType::None;
        m_done = true;
        m_minVal = QPointF(0, m_mcData->chiSquared);
        qDebug() << "   MC finished with chi squared: " << chiSquared << endl;
    }
}

// This method explicitly calculates and rejects certain points
void Likelihood::tickLikelihoodFullMonteCarlo()
{
    m_model->parameters()->save(m_mcData->parametersFilename);
    Parameters* originalParameters = m_model->parameters()->copy();
    m_model->randomWalk();
    calculateModel(m_model);
    float chiSquared = LGraph::ChiSQ(m_data, m_modelData);

    float proposal_likelihood = exp(-chiSquared);
    float current_likelihood = m_model->parameters()->getLikelihood();

    // Always accept first step!
    float p_accept = 1;
    qDebug() << "Proposal / old: " << proposal_likelihood << ", "<<current_likelihood << endl;
    bool accept = true;
    if (current_likelihood != 0) {
        // MC step: only accept when random uniform is < prop/current
        p_accept = proposal_likelihood / current_likelihood;
        accept = Random::nextDouble(0,1) < p_accept;
    }

    if (accept) {
        qDebug() << "ACCEPT with " << p_accept <<endl;
        ++m_mcData->currentStep;
        m_model->parameters()->setLikelihood(proposal_likelihood);
        double acceptRatio = (m_mcData->currentStep) / (double)(m_mcData->currentStep + m_mcData->rejectedSteps);
        m_model->parameters()->FlushMonteCarloStep(m_likelihoodFileName, m_mcData->currentStep, proposal_likelihood,
                                                   "acceptance ratio: " + QString::number(acceptRatio), true);
    }
    else {
        // If not accepted, revert to original parameters
        qDebug() << "REJECT with " << p_accept <<endl;
        m_model->parameters()->copyFrom(originalParameters);
        ++m_mcData->rejectedSteps;
    }


    if(m_mcData->currentStep == m_mcData->totalSteps) {
        calculateModel(m_model);
        chiSquared = m_mcData->chiSquared;
        m_analysisType = AnalysisType::None;
        m_done = true;
        m_minVal = QPointF(0, m_mcData->chiSquared);
        qDebug() << "   Full MC finished with chi squared: " << chiSquared << endl;
    }
}


void Likelihood::tickLikelihoodBruteforce1D()
{
    Parameter *parameter = m_model->parameters()->getParameter(m_currentParameter);
    parameter->setValue(m_currentVal);
    calculateModel(m_model);
    m_likelihood.val[m_currentBin] = LGraph::ChiSQ(m_data, m_modelData);
    m_currentVal += m_stepSize;
    m_currentBin++;
    if (m_currentBin == m_bins) {
        m_minVal = m_likelihood.getMin();
        parameter->setValue(m_minVal.x());
        calculateModel(m_model);
        m_analysisType = AnalysisType::None;
        m_done = true;
    }
}

void Likelihood::tickModelStatistics()
{
    if (m_model == nullptr) {
        qDebug() << "m_model is null, should not happen";
        return;
    }
    m_model->parameters()->getParameter("seed")->setValue(rand()%10000);
    calculateModel(m_model);
    m_statistics.add(m_modelData);
    m_statistics.calculate();
    m_modelData.Copy(m_statistics.average());
    m_data.Copy(m_statistics.average_plus_sigma());

    if (m_currentBin++ == m_bins) {
        m_analysisType = AnalysisType::None;
        m_done = true;
    }
}

Likelihood::Likelihood()
{

}

void Likelihood::monteCarlo(Model *model, int steps, AnalysisAlgorithm analysisAlgorithm)
{
    if(!model) {
        qDebug() << "Likelihood::bruteForce1D: Error, model is nullptr";
        return;
    }
    m_mcData = new MCData();
    m_mcData->totalSteps = steps;
    m_mcData->parametersFilename = QString("/tmp/mcparams.txt");
    m_mcData->bestFarametersFilename = QString("/tmp/mcparamsbest.txt");
    m_model = model;

    calculateModel(m_model);
    m_mcData->chiSquared = LGraph::ChiSQ(m_data, m_modelData);
    qDebug() << "   Starting monte carlo with initial chi squared: " << m_mcData->chiSquared;

    m_analysisType = AnalysisType::LikelihoodStatistics;
    m_analysisAlgorithm = analysisAlgorithm;

    m_mcData->currentStep = 0;
    m_mcData->totalSteps = steps;
    m_mcData->rejectedSteps = 0;

}

void Likelihood::bruteForce1D(int bins, QString parameterKey, Model *model)
{
    if(!model) {
        qDebug() << "Likelihood::bruteForce1D: Error, model is nullptr";
        return;
    }
    m_likelihood.initialize(bins);
    Parameter *parameter = model->parameters()->getParameter(parameterKey);
    m_stepSize = (parameter->max() - parameter->min())/(float)(bins+0);
    m_currentVal = parameter->min();
    for (int i=0; i<bins; i++) {
        m_likelihood.val[i] = 0;
        m_likelihood.index[i] = m_currentVal + m_stepSize*i;
    }

    m_bins = bins;
    m_currentBin = 0;
    m_model = model;
    m_currentParameter = parameterKey;
    m_analysisType = AnalysisType::LikelihoodStatistics;
    m_analysisAlgorithm = AnalysisAlgorithm::Bruteforce1D;
}

void Likelihood::modelAnalysis(int count, Model *model)
{
    m_bins = count;
    m_currentBin = 0;
    m_analysisType = AnalysisType::ModelStatistics;
    m_model = model;
    m_likelihood.initialize(80);
}

bool Likelihood::tick()
{
    if(m_analysisType == AnalysisType::None) return false;

    if (m_analysisType == AnalysisType::LikelihoodStatistics) {
        if(m_analysisAlgorithm == AnalysisAlgorithm::Bruteforce1D) {
            tickLikelihoodBruteforce1D();
        } else if(m_analysisAlgorithm == AnalysisAlgorithm::MonteCarlo) {
            tickLikelihoodMonteCarlo();
        } else if(m_analysisAlgorithm == AnalysisAlgorithm::FullMonteCarlo) {
            tickLikelihoodFullMonteCarlo();
        }
    }

    if (m_analysisType == AnalysisType::ModelStatistics) {
        tickModelStatistics();
    }




    return true;
}
