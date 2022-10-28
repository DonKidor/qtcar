#include "evolution.h"
#include <mycar.h>
#include <QRandomGenerator>

Evolution::Evolution(QPointF pos, qreal rot): m_startPos(pos), m_startRot(rot)
{

}

void Evolution::addCar(MyCar *car) {
    m_cars.push_back(car);
    m_bestCar=nullptr;
}

void Evolution::clearStart(QPointF pos, qreal rot) {
    for(int i=0; i<m_cars.size(); ++i) {
        m_cars[i]->setBrain(MyBrain());
    }
    m_startPos=pos;
    m_startRot=rot;
    m_bestCar=nullptr;
    aliveNow=m_cars.size();
    m_generation=0;
    emit genChanged(0);
    for(int i=0; i<m_cars.size(); ++i) {
        m_cars[i]->initCar(pos, rot);
    }
}

void Evolution::deadCar() {
    --aliveNow;
    if(aliveNow<=0) {
        naturalSelection();
        nextGeneration();
        startGeneration();
    }
}

void Evolution::naturalSelection() {
    m_sumFit=0;
    qint32 mxFit=0;
    for(int i=0; i<m_cars.size(); ++i) {
        qint32 fit = m_cars[i]->getFittnes();
        m_sumFit+=fit;
        if(fit>mxFit) {
            mxFit=fit;
            m_bestCar=m_cars[i];
        }
    }

}

void Evolution::nextGeneration() {
    MyBrain bestBrain = m_bestCar->getBrain();
    QVector<MyBrain> nextGen;
    nextGen.reserve(m_cars.size());

    nextGen.push_back(bestBrain);
    for(int i=1; i<6; ++i) {
        nextGen.push_back(MyBrain(bestBrain).mutate());
    }
    int childIndex = 0.8*m_cars.size();
    for(int i=6; i<childIndex; ++i) {
        qint32 x=QRandomGenerator::global()->bounded(0, m_sumFit);
        for(int j=0; j<m_cars.size(); ++j) {
            x-=m_cars[j]->getFittnes();
            if(x<=0) {
                nextGen.push_back(MyBrain(m_cars[j]->getBrain()).mutate());
                break;
            }
        }
    }
    for(int i=childIndex; i<m_cars.size(); ++i) {
        nextGen.push_back(MyBrain());
    }
    for(int i=0; i<m_cars.size(); ++i) m_cars[i]->setBrain(nextGen[i]);
}

void Evolution::startGeneration() {
    aliveNow=m_cars.size();
    ++m_generation;
    emit genChanged(m_generation);
    for(int i=0; i<m_cars.size(); ++i) {
        m_cars[i]->initCar(m_startPos, m_startRot);
    }
}
