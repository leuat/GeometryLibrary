#include "likelihood.h"
#include <qdebug.h>

LGraph Likelihood::data() const
{
    return m_data;
}

void Likelihood::setData(const LGraph &data)
{
    m_data = data;
}

LGraph Likelihood::model() const
{
    return m_model;
}

void Likelihood::setModel(const LGraph &model)
{
    m_model = model;
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

void Likelihood::tickLikelihood()
{
    m_parameter->setValue(m_currentVal);
    calculateModel(m_parameters);
    m_likelihood.Val[m_currentBin] = LGraph::ChiSQ(m_data, m_model);
    m_currentVal += m_stepSize;
    m_currentBin++;
    if (m_currentBin == m_bins) {
        //m_likelihood.LikelihoodFromChisq();
        //m_likelihood.fitSpline(m_likelihood,100);
        m_minVal = m_likelihood.getMin();
        m_parameter->setValue(m_minVal.x());
        calculateModel(m_parameters);
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
    if (m_parameters == nullptr) {
        qDebug() << "m_parameters is null, should not happen";
        return;
    }

    m_parameters->getParameter("seed")->setValue(rand()%10000);
    calculateModel(m_parameters);
    m_statistics.add(m_model);
    m_statistics.calculate();
    m_model.Copy(m_statistics.average());
    m_data.Copy(m_statistics.average_plus_sigma());

    if (m_currentBin++ == m_bins) {
        m_ready = false;
        m_done = true;
    }

}

Likelihood::Likelihood()
{

    if (m_currentBin++ == m_bins) {
        m_ready = false;
        m_done = true;
    }


}

void Likelihood::bruteForce1D(int bins, Parameter* parameter, Parameters* parameters)
{
    m_likelihood.Initialize(bins);
/*    p->setMax(1.4);
    p->setMin(0.01);*/
    m_stepSize = (parameter->max() - parameter->min())/(float)(bins+0);
    m_currentVal = parameter->min();
    for (int i=0;i<bins;i++) {
        m_likelihood.Val[i] = 0;
        m_likelihood.Index[i] = m_currentVal + m_stepSize*i;
    }

    m_ready = true;
    m_bins = bins;
    m_currentBin = 0;
    m_parameter = parameter;
    m_parameters = parameters;
    m_analysisType = AnalysisType::LikelihoodStatistics;
}

void Likelihood::modelAnalysis(int count, Parameters *parameters)
{
    m_bins = count;
    m_currentBin = 0;
    m_ready = true;
    m_analysisType = AnalysisType::ModelStatistics;
    m_parameters = parameters;
    m_likelihood.Initialize(80);

}



bool Likelihood::tick()
{
    if (!m_ready)
        return false;

    if (m_analysisType == AnalysisType::LikelihoodStatistics)
        tickLikelihood();
    if (m_analysisType == AnalysisType::ModelStatistics)
        tickModelStatistics();


    return true;
}



