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

Likelihood::Likelihood()
{

}

void Likelihood::bruteForce1D(int bins, Parameter* p, Parameters* params)
{
    m_likelihood.Initialize(bins);
/*    p->setMax(1.4);
    p->setMin(0.01);*/
    m_stepSize = (p->max() - p->min())/(float)(bins+0);
    m_currentVal = p->min();
    for (int i=0;i<bins;i++) {
        m_likelihood.Val[i] = 0;
        m_likelihood.Index[i] = m_currentVal + m_stepSize*i;
    }

    m_ready = true;
    m_bins = bins;
    m_currentBin = 0;
    m_parameter = p;
    m_parameters = params;
}

bool Likelihood::tick()
{
    if (!m_ready)
        return false;

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
    return true;
}
