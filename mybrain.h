#ifndef MYBRAIN_H
#define MYBRAIN_H

#include <QGenericMatrix>

class MyBrain
{
public:
    MyBrain();
    MyBrain(const MyBrain&);
    MyBrain(const MyBrain&, const MyBrain&);
    QVector<bool> think(const QVector<qreal>&);

    MyBrain& mutate();
    MyBrain& operator=(const MyBrain&);
private:

    const qreal MUTATION_RATE=0.01;
    const QVector<int> m_brainSize {7, 7, 5, 7, 5};
    QVector<QVector<qreal> > m_brainNeuron;

    void initNeurons();
    QVector<QVector<QVector<qreal> > > m_connections;

};

#endif // MYBRAIN_H
