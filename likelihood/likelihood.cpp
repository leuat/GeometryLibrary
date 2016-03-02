#include "likelihood.h"
#include <qdebug.h>
#include "../models/model.h"

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

void Likelihood::tickLikelihood()
{
    Parameter *parameter = m_model->parameters()->getParameter(m_currentParameter);
    parameter->setValue(m_currentVal);
    calculateModel(m_model);
    m_likelihood.val[m_currentBin] = LGraph::ChiSQ(m_data, m_modelData);
    m_currentVal += m_stepSize;
    m_currentBin++;
    if (m_currentBin == m_bins) {
        //m_likelihood.LikelihoodFromChisq();
        //m_likelihood.fitSpline(m_likelihood,100);
        m_minVal = m_likelihood.getMin();
        parameter->setValue(m_minVal.x());
        calculateModel(m_model);
        m_done = true;
        m_ready = false;
    }
}

void Likelihood::tickModelStatistics()
{
    if (m_done)
        return;
    if (!m_ready)
        return;
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
        m_ready = false;
        m_done = true;
    }
}

Likelihood::Likelihood()
{
    // This is probably wrong? Since in the constructor, this if test doesn't make any sense since it is defined in the h-file.
//    if (m_currentBin++ == m_bins) {
//        m_ready = false;
//        m_done = true;
//    }
}

void Likelihood::bruteForce1D(int bins, QString parameterKey, Model *model)
{
    if(!model) {
        qDebug() << "Likelihood::bruteForce1D: Error, model is nullptr";
        return;
    }
    m_likelihood.initialize(bins);
/*    p->setMax(1.4);
    p->setMin(0.01);*/
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
    m_ready = true;
}

void Likelihood::modelAnalysis(int count, Model *model)
{
    m_bins = count;
    m_currentBin = 0;
    m_ready = true;
    m_analysisType = AnalysisType::ModelStatistics;
    m_model = model;
    m_likelihood.initialize(80);
}

bool Likelihood::tick()
{
    if (!m_ready) return false;

    if (m_analysisType == AnalysisType::LikelihoodStatistics) {
        tickLikelihood();
    }

    if (m_analysisType == AnalysisType::ModelStatistics) {
        tickModelStatistics();
    }


    return true;
}
