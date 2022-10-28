#ifndef MYCAR_H
#define MYCAR_H

#include <QGraphicsObject>
#include <mybrain.h>

class MyCar : public QGraphicsObject {
    Q_OBJECT
public:
    MyCar(QGraphicsItem* =nullptr);
    QRectF boundingRect() const override;
    void initCar(QPointF, qreal);
    void setBrain(const MyBrain&);
    MyBrain getBrain();
    qint32 getFittnes() const;
protected:
    virtual void paint(QPainter*, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    virtual bool event(QEvent *ev) override;

private:
    const qreal MAX_SPEED=8;
    const qreal MAX_LEN=150;
    const int DEADTIMERMAX=60;

    bool m_signals[4];
    bool m_dead;

    MyBrain m_brain;
    int m_deadTimer;
    int m_fitIndex;

    qint32 m_fittnes;
    qreal m_fitKoef;

    QGraphicsItemGroup *m_lines;
    qreal m_velocity;
    QVector<qreal> m_lenght;
    QList<QGraphicsItem*> m_fittneses;

    void carDead();
    void getLengths();
    void checkHit();
signals:
    void clicked();
    void dead();

public slots:
    void updateTime();
    void forward(bool);
    void brake(bool);
    void turnLeft(bool);
    void turnRight(bool);

};

#endif // MYCAR_H
