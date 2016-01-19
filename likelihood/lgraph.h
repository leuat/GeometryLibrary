#pragma once
#include "likelihood/spline.h"

using namespace std;

#include <QVector>
#include <QPointF>
#include <vector>
#include <complex>
#ifdef USE_FFTW
#include <fftw3.h>
#endif
#include <misc/util.h>
#include <vector>
#include <string>


#define NONE -1E20

class LGraph {
 public:
  vector<double> Val;
  vector<double> Index;
  vector<double> IndexScaled;

  LGraph* chisq;

  LGraph(QVector<QPointF>& points);
  QVector<QPointF> toQVector();
  
  double mean, std;

  string name;

  vector<complex<double> > ValFFT;
  int Bins;

  LGraph() {
    chisq = 0;
  }

  void Copy(const LGraph& copy) {
   Initialize(copy.Bins);
   for (int i=0;i<Bins;i++) {
     Val[i] = copy.Val[i];
     Index[i] = copy.Index[i];
     IndexScaled[i] = copy.IndexScaled[i];
   }
 }

 ~LGraph() {
   Delete();
  }

  void Delete() {
  }


  double& operator[](int i) {
    return Val[i];
  }

  void Initialize(int bins) {
    Delete();
    Bins = bins;
    Val.resize(bins);
    Index.resize(bins);
    IndexScaled.resize(bins);
    ValFFT.resize(bins);
    


    for (int i=0;i<bins;i++) {
      Val[i] = 0.0;
      Index[i] = i;
      IndexScaled[i] = i;
      ValFFT[i] = 0.0;
    }

  }



  void WashHaakon() {
    for (int i=0;i<Bins/2;i++) {
      double a = Index[i];
      double b = Val[i];
      int j = Bins - i - 1;
      Index[i] = Index[j];
      Val[i] = Val[j];
      
      Index[j] = a;
      Val[j] = b;
    }
    scaleX(1.0);
    vector<double>::iterator itI = Index.begin();
    for (vector<double>::iterator it = Val.begin(); it != Val.end(); it) {
      
      vector<double>::iterator a = it;
      vector<double>::iterator b = itI;
      if (it != Val.end()) {
	if (abs(*a)> 1e+12) {
	  Val.erase (it);
	  Index.erase (itI);

	}
	else
	  {

	    it++;
	    itI++;
	  }

      }
    }
    Bins = Val.size();
  }

  void dampLargeModes(double v);


  void fitSpline(LGraph& spline);

  void Mul(LGraph& o) {
    for (int i=0;i<min(Bins, o.Bins);i++) {
      Val[i] *= o.Val[i];
    }
  }

  void Add(double v) {
    for (int i=0;i<Bins;i++) {
      Val[i] += v;
    }
  }


  void Add(LGraph& o, double v) {
    for (int i=0;i<min(Bins, o.Bins);i++) {
      Val[i] += v*o.Val[i];
    }
  }

  float getAverageValueAt(int idx, int spread);

  void MulFFT(LGraph& o) {
    for (int i=0;i<Bins;i++) {
      ValFFT[i] *= o.ValFFT[i];
    }
  }


  void Gaussian(double B);
  void Smooth(double B);


  void LikelihoodFromChisq() {
    // find min
    double m = 1E20;
    for (int i=0;i<Bins;i++)
      m = min(Val[i], m);

    for (int i=0;i<Bins;i++) {
      Val[i] = exp(-(Val[i]-m));
    }
  }


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
