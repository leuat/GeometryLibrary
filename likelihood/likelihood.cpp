#include "likelihood.h"


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

Likelihood::Likelihood()
{

}

void Likelihood::BruteForce1D(int bins, Parameter* p, Parameters* params)
{
    m_likelihood.Initialize(bins);
    float step = (p->max() - p->min())/(float)bins;
    float val = p->min();
    for (int i=0;i<bins;i++) {
        p->setValue(val);
        CalculateModel(params);
        m_likelihood.Val[i] = m_data.ChiSQ(m_data, m_model);
        m_likelihood.Index[i] = val;
        m_likelihood.IndexScaled[i] = val;
        val+=step;
    }
}
