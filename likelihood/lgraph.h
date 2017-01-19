#pragma once
#include "spline.h"

using namespace std;

#include <QVector>
#include <QPointF>
#include <vector>
#include <complex>
#ifdef USE_FFTW
#include "../fftw3.h"
#endif
#include "../misc/util.h"
#include <vector>
#include <string>


#define NONE -1E20

class LGraph {
public:
    vector<double> val;
    vector<double> index;
    vector<double> IndexScaled;

    LGraph* chisq;

    LGraph(QVector<QPointF>& points);
    void fromQVector(QVector<QPointF>& points);
    QVector<QPointF> toQVector();

    double mean, std;

    string name;

    vector<complex<double> > ValFFT;
    int Bins;

    LGraph() {
        chisq = 0;
    }

    void normalizeArea();

    void Copy(const LGraph& copy) {
        initialize(copy.Bins);
        for (int i=0;i<Bins;i++) {
            val[i] = copy.val[i];
            index[i] = copy.index[i];
            IndexScaled[i] = copy.IndexScaled[i];
        }
    }

    ~LGraph() {
        Delete();
    }

    void Delete() {
    }


    double& operator[](int i) {
        return val[i];
    }

    void initialize(int bins) {
        Delete();
        Bins = bins;
        val.resize(bins);
        index.resize(bins);
        IndexScaled.resize(bins);
        ValFFT.resize(bins);

        for (int i=0;i<bins;i++) {
            val[i] = 0.0;
            index[i] = i;
            IndexScaled[i] = i;
            ValFFT[i] = 0.0;
        }

    }



    void WashHaakon() {
        for (int i=0;i<Bins/2;i++) {
            double a = index[i];
            double b = val[i];
            int j = Bins - i - 1;
            index[i] = index[j];
            val[i] = val[j];

            index[j] = a;
            val[j] = b;
        }
        scaleX(1.0);
        vector<double>::iterator itI = index.begin();
        for (vector<double>::iterator it = val.begin(); it != val.end(); it) {
            vector<double>::iterator a = it;
            if (it != val.end()) {
                if (abs(*a)> 1e+12) {
                    val.erase (it);
                    index.erase (itI);
                } else {
                    it++;
                    itI++;
                }
            }
        }
        Bins = val.size();
    }

    void dampLargeModes(double v);


    void fitSpline(LGraph& spline, int N);

    void Mul(LGraph& o) {
        for (int i=0;i<min(Bins, o.Bins);i++) {
            val[i] *= o.val[i];
        }
    }

    void Add(double v) {
        for (int i=0;i<Bins;i++) {
            val[i] += v;
        }
    }


    void Add(LGraph& o, double v) {
        for (int i=0;i<min(Bins, o.Bins);i++) {
            val[i] += v*o.val[i];
        }
    }

    float getAverageValueAt(int idx, int spread);

    void MulFFT(LGraph& o) {
        for (int i=0;i<Bins;i++) {
            ValFFT[i] *= o.ValFFT[i];
        }
    }

    QPointF getMin();


    void Gaussian(double B);
    void Smooth(double B);


    void LikelihoodFromChisq();


    void calculateStatistics();

    void cutModes(int from, int to);

    static double ChiSQ(LGraph& temp, LGraph& two);

    void loadWeirdFormat(string flux, string lambda);

    void Normalize();


    void scaleX(double);

#ifdef USE_FFT
    void FFT();
    void FFTInv();
#endif
    void SaveBin(string Filename);
    void LoadBin(string Filename);

    void LoadText(string Filename);
    void SaveText(string Filename);

    void Mean();
    void Std();

    void Scale(double v);
    void InvertStats();

    void dampNoise(double A, double B);

    void SaveTextFFT(string Filename);

    void RenderGauss(double position, double sigma, double amp, int poww);



    float getValAtScaledIndex(double& index);

    int getIndexAtScaledIndex(double& index);

    void MulWithIndex(LGraph& o);

};
