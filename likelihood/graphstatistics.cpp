#include "graphstatistics.h"
#include <QDebug>

LGraph GraphStatistics::sigma() const
{
    return m_sigma;
}

LGraph GraphStatistics::average() const
{
    return m_average;
}

LGraph GraphStatistics::average_plus_sigma() const
{
    return m_average_plus_sigma;
}

GraphStatistics::GraphStatistics()
{

}

void GraphStatistics::add(LGraph &graph)
{
    if (m_data.length()==0)
        m_length = graph.Bins;

    if (graph.Bins == m_length)
        m_data.append(graph);
    else
        qDebug() << "GraphStatistics::add error: graphs are not of same length";
   }

void GraphStatistics::calculate()
{
    m_average.Initialize(m_length);
    m_sigma.Initialize(m_length);
    m_average_plus_sigma.Initialize(m_length);

    for (int d=0;d<m_data.length();d++)
        for (int i=0;i<m_length;i++)
            m_average[i]+=m_data[d][i];

    for (int i=0;i<m_length;i++)
        m_average[i] =m_average[i]/(float)m_data.length();

    // Calculate exp (e - d)^2
    for (int d=0;d<m_data.length();d++)
        for (int i=0;i<m_length;i++)
            m_sigma[i]+=pow(m_data[d][i]- m_average[i],2);

    for (int i=0;i<m_length;i++) {
  //      qDebug() << m_sigma[i];
        m_sigma[i] = sqrt(m_sigma[i]/(float)m_data.length());
        m_average.Index[i] = m_data[0].Index[i];
        m_sigma.Index[i] = m_data[0].Index[i];
        m_average_plus_sigma.Index[i] = m_data[0].Index[i];
        m_average_plus_sigma[i] = m_average[i] + m_sigma[i];
    }




}
