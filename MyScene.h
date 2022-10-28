#ifndef MYSCENE_H
#define MYSCENE_H
#include <QGraphicsScene>
#include <evolution.h>
#include <QPainterPath>

class MyCar;

class MyScene: public QGraphicsScene {
    Q_OBJECT

public:
    MyScene(const QRectF &sceneRect, QObject *parent=nullptr);
    void addWall(QLineF);
    QGraphicsItemGroup* getWalls() const;
    QList<QGraphicsItem*> getFittnes() const;
    QPainterPath* getWallsPath();
    QPainterPath* getFitPath();
    void addCar(MyCar*);

    const int carCount=150;
private:
    QGraphicsItemGroup* m_walls;
    QGraphicsItemGroup* m_fitness;
    QPainterPath m_paintWalls;
    QPainterPath m_paintFit;
    QList<QGraphicsItem*> m_fittnesRightVerison;

    QGraphicsLineItem* m_wallLine;
    QVector<MyCar*> m_cars;
    bool m_cancelTool;
    bool m_calibrTool;

    Evolution m_evolution;

    QPainterPath* m_selectedPath;
    QGraphicsItemGroup* m_selectedGroup;
protected:
    virtual bool event(QEvent *event) override;

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    virtual void focusOutEvent(QFocusEvent *focusEvent) override;

public slots:
    void wallSelected();
    void cancelSelected();
    void fitnessSelected();
    void arrowSelected();
    void calibrSelected();

    void genStart();

signals:
    void pressW(bool);
    void pressA(bool);
    void pressD(bool);
    void pressS(bool);
    void updateTime();

    void genChanged(int);

};

#endif // MYSCENE_H
