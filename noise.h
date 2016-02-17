#pragma once
#include <QObject>
#include <cmath>
#include <fstream>
#include <string>
#include <iostream>
#include <QVector>
#include <QVector3D>

using namespace std;

/*
 * Parent class for noise modules. Currently only implemented as perlin & simplex noise.
 *
 * must implement raw_2d & raw_3d
 *
 *
*/
class Noise : public QObject {
    Q_OBJECT
    Q_ENUMS(NoiseType)
public:
    enum NoiseType { Simplex, Perlin};

    int octaves, seed;
    double persistence;
    double frequency;


    Noise(int o,double f,double p,int s) {
        octaves = o;
        persistence = p;
        frequency = f;
        seed = s;
        weights = 0;
    }
    Noise() {
    }

    //  virtual double Get(const double x, const double y) = 0;
    // virtual double Get(const double x, const double y, const double z) = 0;

    double Get( const double x, const double y, const double z);
    double Get( const double x, const double y, const double z, const double per, const double f);
    double Get( const double x, const double y);


    double* weights;

    double* generateSpectralWeights (double lacunarity,     int octaves, double h, double frequency);


    double getRidgedMf(QVector3D p, float frequency, int octaves, float lacunarity, float warp, float offset, float gain);


    virtual double raw_2d(const double& x, const double& y) = 0;
    virtual double raw_3d(const double& x, const double& y, const double& z) = 0;

    double average;
    double standard_deviation;

    /*
     * Calculates E(x) and sigma(x) for the current noise module
     *
     *
    */
    void calculate_statistics(double N, string filename) {
        average = 0;
        double s = 0.01123;
        standard_deviation = 0;
        for (int i=0;i<N;i++) {

            double val = Get(i*s+0.1, i*s+0.211, i*s+0.123);
            average += val;
        }
        average /= N;
        for (int i=0;i<N;i++) {
            double val = Get(i*s+0.1, i*s+0.211, i*s+0.123);
            standard_deviation += (average-val)*(average-val);
        }
        standard_deviation = sqrt(standard_deviation/N);

        if (filename != "") {
            ofstream f( filename.c_str(), ios::out);
            for (int i=0;i<50000;i++) {
                double s = 0.159516413;
                double val = Get((double)i*s, 0.121+i*s*0.341,0.4312+i*s*0.1231 + 0.64123);
                f << i << " " << val << endl;
            }
            f.close();


        }

    }

};
