#include "likelihood/lgraph.h"
#include <fstream>
#include <string>
#include <iostream>



#ifdef USE_FFT

void LGraph::FFT() {
  double *in;
    fftw_complex* out;
    fftw_plan p;

    in = (double*) fftw_malloc(sizeof(double) * Bins);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * Bins);
    
   
    for (int i=0;i<Bins;i++) {
      in[i] = Val[i];
      out[i][0] = 0.0;
      out[i][1] = 0.0;
    }

    p = fftw_plan_dft_r2c_1d(Bins, in, out,FFTW_ESTIMATE);
    fftw_execute(p);
    fftw_destroy_plan(p);
   
    for (int i=0;i<Bins;i++) 
      ValFFT[i] = complex<double>(out[i][0],out[i][1]);

   
    fftw_free(in); 
    fftw_free(out);
  }

void LGraph::FFTInv() {
    double *out;
    fftw_complex* in;
    fftw_plan p;
    
    out = (double*) fftw_malloc(sizeof(double) * Bins);
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * Bins);
    
    
    for (int i=0;i<Bins;i++) {
      in[i][0] = ValFFT[i].real();
      in[i][1] = ValFFT[i].imag();
      out[i] = 0.0;
    }

    p = fftw_plan_dft_c2r_1d(Bins, in, out,FFTW_ESTIMATE);

    fftw_execute(p); /* repeat as needed */
    fftw_destroy_plan(p);

    for (int i=0;i<Bins;i++) {
      Val[i] = out[i] / (double)Bins;
    }
    
   
    fftw_free(in); 
    fftw_free(out);
  }

#endif

void LGraph::SaveBin(string Filename) {
    ofstream f(Filename.c_str(), ios::out | ios::binary);
    f.write((char *)&Bins, sizeof(unsigned int));
    //    f.write((char *)Val, sizeof(double)*Bins);
    f.close();

  }

void LGraph::LoadBin(string Filename) {
    ifstream f(Filename.c_str(), ios::in | ios::binary);
    f.read((char *)&Bins, sizeof(unsigned int));
    Initialize(Bins);
    //f.read((char *)Val, sizeof(double)*Bins);
    f.close();
  }


void LGraph::cutModes(int from, int to) {
  for (int i=from; i<to; i++) {
    ValFFT[i] = complex<double>(0,0);
    //ValFFT[Bins-1-i] = complex<double>(0,0);

  }
}

LGraph::LGraph(QVector<QPointF> &points)
{
    Initialize(points.size());
    for (int i=0;i<points.size();i++) {
        QPointF& p = points[i];
        Val[i] = p.y();
        Index[i] = p.x();
        IndexScaled[i] = p.y();
    }
}

QVector<QPointF> LGraph::toQVector()
{
    QVector<QPointF> points;

}

void LGraph::dampLargeModes(double v) {
  for (int i=0; i<Bins; i++) {
    double l = (Bins-i);
    ValFFT[i] *= exp(-l*l/(v*v));
      
  }
}


float LGraph::getAverageValueAt(int idx, int N) {

  float val = 0;
  float cnt = 0;
  for (int i=0;i<N;i++) {
    int k = idx + (i-N/2);
    if ((k>=0) && (k<Bins)) {

	val+= Val[k];
	cnt++;
      }
  }
    return val / cnt;
}


void LGraph::fitSpline(LGraph& fit) {


  vector<double> x,y;

  int N = 4;

  LGraph sm;
  sm.Copy(*this);
  sm.Smooth(50);

  for (int i=0;i<=N;i++) {
    int b = i*Bins/(N);
    if (b==Bins) b--;
    x.push_back(i/(double)N);
    y.push_back(sm.getAverageValueAt(b, 250));
    //cout << Index[b] << " " <<Val[b] << endl;
  }

  Spline<double, double> sp(x, y);

  fit.Copy(*this);
  for (int i=0;i<Bins;i++)  {
    fit.Val[i] = sp[(double)i/(double)Bins]; 
  }
  
}

void LGraph::Mean() {
  mean = 0;
  for (int i=0;i<Bins;i++) {
    mean+=Val[i];
  }
  mean/=(double)Bins;

}

void LGraph::Std() {
  std = 0;
  Mean();

  for (int i=0;i<Bins;i++) {
    double v= Val[i] - mean;
    std += v*v;
  }
  std/=(double)Bins;
  std = sqrt(std);

}

void LGraph::Scale(double v) {
  for (int i=0;i<Bins;i++) {
    Val[i] *= v;
  }

}

void LGraph::InvertStats() {
  for (int i=0;i<Bins;i++) {
    Val[i] = 1.0-Val[i];
  }

}


void LGraph::Normalize() {
  double m = -1E20;
  for (int i=0;i<Bins;i++)
    m = max(Val[i], m);


  for (int i=0;i<Bins;i++)
    Val[i]/=m;
  
}


void LGraph::scaleX(double v) {
  for (int i=0;i<Bins;i++) {
    IndexScaled[i] = Index[i]*v;
  }

}


void LGraph::Gaussian(double B) {
  int x[2] = {0,1};

  double sum = 0;
  double scale = (Index[Bins-1] - Index[0])/(double)Bins;
  //cout << scale << endl;
  for (int i=0;i<Bins; i++) {
    double dist = 1E10;
    double val = 0;
    for (int l=0;l<1;l++) {
      double d = scale*(0 - i);
      //d/=Bins;
      dist = min(dist,d);
      val += exp(-dist*dist/(2.0*B*B));
      
    }
    
    sum+=val;
    Val[i] = val;
  }
  for (int i=0;i<Bins;i++)
    Val[i]/=sum;

}


void LGraph::Smooth(double B) {
#ifdef USE_FFT
  LGraph gauss;
  gauss.Copy(*this);
  gauss.Gaussian(B);
  gauss.SaveText("gauss.txt");
  gauss.FFT();
  FFT();
  MulFFT(gauss);
  FFTInv();
#endif
}



void LGraph::loadWeirdFormat(string flux, string lambda) {
  ifstream f(flux.c_str(), ios::in);
  if (!f.good())
    throw string("File " + flux + " not found");
  char inpf[2000];
  char inpl[2000];
  Bins = 0;
   while(!f.eof()) {
      f.getline(inpf,2000);
      Bins+=6;
     }
      Bins-=6;
   
   cout << "Total bins " << Bins << endl;
   //  exit(1);
  f.close();
  Initialize(Bins);
  f.open(flux.c_str(), ios::in);
  ifstream l(lambda.c_str(), ios::in);
  int i= 0;
  
   while(!f.eof() && i<Bins) {
      
     f.getline(inpf,2000);
     l.getline(inpl,2000);
     vector<string> tokf, tokl;
     Util::Tokenize(inpf, tokf," ");
     Util::Tokenize(inpl, tokl," ");
     if (tokf.size()==6) {
       for (int j=0;j<6;j++) {
     Index[i] = pow(10.0,(atof(Util::trim(tokl[j]).c_str())));
	 IndexScaled[i] = Index[i];
     Val[i] = atof(Util::trim(tokf[j]).c_str());
	 
	 //cout << i <<" of bins " << Bins << "with values " << Index[i] << " flux " << Val[i] << endl;
	 i++;
       }
     }
   }
   
   f.close();
   l.close();
}



void LGraph::dampNoise(double A, double B) {
  for (int i=0;i<Bins;i++) {

    double scale = exp(-A*(double)i);
    Val[i] *= scale;
  }
  
}

void LGraph::LoadText(string Filename) {

  name = Filename;
  ifstream f(Filename.c_str(), ios::in);
  if (!f.good())
    throw string("File " + Filename + " not found");
  char inp[2000];
  Bins = 0;
  std::vector<float> index;
  std::vector<float> val;
/*   while(!f.eof()) {
      f.getline(inp,2000);

      Bins++;
     }
   Bins--;
  f.close();
  Initialize(Bins);*/
//  f.open(Filename.c_str(), ios::in);
   while(!f.eof()) {
      f.getline(inp,2000);
      if (inp[0]=='#')
          continue;
//      cout << inp << endl;

      vector<string> tok;
      Util::Tokenize(inp, tok," ");
      if (tok.size()==2) {
        float value = atof(Util::trim(tok[1]).c_str());
        if (abs(value)<10E10)
         {
        index.push_back(atof(Util::trim(tok[0]).c_str()));
        val.push_back(value);
        //cout << value << endl;
        }
      }
     }

  f.close();
   Bins = index.size();
   Initialize(Bins);
   for (int i=0;i<Bins;i++) {
        Index[i] = index[i];
        IndexScaled[i] = index[i];
        Val[i] = val[i];
   }
}



void LGraph::SaveText(string Filename) {
  ofstream f(Filename.c_str(), ios::out);
  for (int i=0;i<Bins;i++) {
    f << IndexScaled[i] << " " << Val[i] << endl;
  }
  f.close();
}

void LGraph::SaveTextFFT(string Filename) {
  ofstream f(Filename.c_str(), ios::out);
  for (int i=0;i<Bins;i++) {
    f << Index[i] << " " << norm(ValFFT[i]) << endl;
  }
  f.close();
}


void LGraph::RenderGauss(double position, double sigma, double amp, int poww) {
  for (int i=0;i<Bins;i++) {
    double p = position - Index[i];
    p = pow(p/sigma, poww);
    double val = amp*exp(-p);
    Val[i] = max(Val[i], val);
  }
}



float LGraph::getValAtScaledIndex(double& index) {
  for (int i=0;i<Bins-1;i++) {
    //   cout << index << " vs " << IndexScaled[i] << endl;
    if (index>=IndexScaled[i] && index<IndexScaled[i+1])
      return Val[i];
  }
  return NONE;
}

double LGraph::ChiSQ(LGraph& temp, LGraph& two) {
  double chisq = 0;

  for (int i=0;i<temp.Bins;i++) {
    
    double val = two.getValAtScaledIndex(temp.Index[i]);

    if (val!=NONE) {
      chisq += abs(pow((temp.Val[i] - val), 2.0));
    }
  }
  return chisq;
}




int LGraph::getIndexAtScaledIndex(double& index) {
  for (int i=0;i<Bins-1;i++) {
    if (index>=IndexScaled[i] && index<IndexScaled[i+1])
      return i;
  }
  return -1;
}



void LGraph::MulWithIndex(LGraph& o) {
    for (int i=0;i<Bins;i++) {
      int j = o.getIndexAtScaledIndex(IndexScaled[i]);
      if (j!=-1 && j>0 && j<o.Bins) {
        Val[i] *= o.Val[j];
      }

    }
  }


void LGraph::calculateStatistics() {
  double top = -1E12;
  int idx;

  cout << "eh " << Bins << endl;
  for (int i=0;i<Bins;i++) {
    if (Val[i]>top) {
      top = Val[i];
      idx = i;
    }
  }
  cout << "føkk dis" << endl;
  int winner = idx;
  mean = Index[idx];
  
  double val = Val[idx];
  bool ok = true;
  
  while (ok && Val[idx]>0.5 && ok) {
    idx--;
    if (idx<0) { 
      break;
      ok = false;
    }
    
  }
  //  cout << idx << endl;
  if (!ok) {
    idx = winner;
    while (Val[idx]>0.5) {
      idx++;
      if (idx>=Bins) { 
	cout << "ERROR Likelihood width larger than chisq bins - bad sample" << endl;
      }
      
    }

  }
  
  double pos = Index[idx];
  std = abs(mean - pos);
}

