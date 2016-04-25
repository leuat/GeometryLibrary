#ifndef PARTICLE_H
#define PARTICLE_H
#include <QVector3D>
#include <QMap>
#include <QVector>

class ParticleType {
public:
    float size;
    QVector3D color;
    QString name;

    ParticleType() {

    }

    ParticleType(QString _name, float _size, QVector3D _color) {
        size = _size;
        color = _color;
        name = _name;
    }
};


class ParticleTypes {
    QMap<QString, ParticleType> map;
public:
    static ParticleTypes& getInstance() {
        static ParticleTypes instance;
        return instance;
    }
    ParticleTypes(ParticleTypes const&) = delete;
    void operator=(ParticleTypes const&)  = delete;

    ParticleTypes() {
        QVector3D mc = QVector3D(0.1,1.0,0.7);
        QVector3D O = QVector3D(1,0.3,0.2);

        map.insert("Ar", ParticleType("Ar",1, mc));
        map.insert("Si", ParticleType("Si", 1, mc));
        map.insert("O", ParticleType("O", 0.5, O));
        map.insert("1", ParticleType("Si", 1, mc));
        map.insert("2", ParticleType("O", 0.5, O));
    }

    QMap<QString, ParticleType> getMap() const;
};



class Particle
{
private:
    QVector3D pos;
    ParticleType type;

public:

    Particle();
    QVector3D getPos() const;
    void setPos(const QVector3D &value);
    ParticleType getType() const;
    void setType(const ParticleType &value);
};

class Particles {
private:
    QVector<Particle *> particles;
    QVector3D boundsMin, boundsMax;
    float boundsSize;

    float m_intrinsicPorosity;
    float m_calculatedPorosity;
public:
    QVector<Particle *>& getParticles();
    QVector<QVector3D> getQVector3DList();

    void save(const char *filename);
    void open(const char *filename);
    void copyFrom(Particles& plist);

    ~Particles() {
        //clear();
    }

    void clear() {
        for (Particle* p : particles)
            delete p;
        particles.clear();
    }

    void appendToQVector3DList(QVector<QVector3D>& list);


    int size() {
        return particles.size();
    }

    Particle* operator[](const int i) {
        return particles[i];
    }

    void clearPointersOnly() {
        particles.clear();
    }

    void calculatePorosity();

    void calculateBoundingBox( ) {
        float LARGE = 1E20;

        boundsMin = QVector3D(LARGE, LARGE, LARGE);
        boundsMax = QVector3D(-LARGE, -LARGE, -LARGE);
        for (Particle* p: particles) {
            for (int i=0;i<3;i++) {
                boundsMin[i] = std::min(boundsMin[i],p->getPos()[i]);
                boundsMax[i] = std::max(boundsMax[i],p->getPos()[i]);
            }
        }
        boundsSize = (boundsMax - boundsMin).length();
    }
    QVector3D scalePos(Particle* p, float scale) {
        return (p->getPos() - (boundsMax+boundsMin)*0.5)/boundsSize*scale;
    }

    QVector3D getBoundsMin() const;
    QVector3D getBoundsMax() const;



    float getBoundsSize() const;
    float getIntrinsicPorosity() const;
    float getCalculatedPorosity() const;
    void setCalculatedPorosity(float calculatedPorosity);
    void setIntrinsicPorosity(float intrinsicPorosity);
};

class NBHList {
private:
    QVector<Particles> list;

    Particle* findNBH(Particle* org, Particles& lst, double ha);

public:
    Particles& operator[](const int i) {
        return list[i];
    }

    int size() {
        return list.size();
    }

    void Create(Particles& plist);


};


#endif // PARTICLE_H
