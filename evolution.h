#ifndef EVOLUTION_H
#define EVOLUTION_H

#include <QObject>
#include <QPointF>

class MyCar;

class Evolution: public QObject
{
    Q_OBJECT
public:
    Evolution(QPointF=QPointF(0,0), qreal=0);
    void addCar(MyCar*);
    void clearStart(QPointF=QPointF(0,0), qreal=0);
private:
    bool allDead;
    int aliveNow;
    QVector<MyCar*> m_cars;
    MyCar *m_bestCar;
    int m_generation;

    QPointF m_startPos;
    qreal m_startRot;

    qint64 m_sumFit;
    void naturalSelection();
    void nextGeneration();
    void startGeneration();

signals:
    void genChanged(int);

public slots:
    void deadCar();

};

#endif // EVOLUTION_H
