#include "MyScene.h"
#include <QKeyEvent>
#include <QFocusEvent>
#include <QGraphicsSceneMouseEvent>
#include "MyCar.h"

MyScene::MyScene(const QRectF &sceneRect, QObject *parent):QGraphicsScene(sceneRect, parent), m_cancelTool(true), m_calibrTool(false) {
    m_walls = new QGraphicsItemGroup();
    m_fitness = new QGraphicsItemGroup();
    addItem(m_walls);
    addItem(m_fitness);
    m_wallLine = new QGraphicsLineItem();
    QPen pen(Qt::black);
    pen.setWidth(2);
    m_wallLine->setPen(pen);
    addItem(m_wallLine);
    startTimer(50);

    QObject::connect(&m_evolution, SIGNAL(genChanged(int)), this, SIGNAL(genChanged(int)));
}

void MyScene::addCar(MyCar *car) {
    addItem(car);
    m_evolution.addCar(car);
    QObject::connect(car, SIGNAL(dead()), &m_evolution, SLOT(deadCar()));
}

void MyScene::genStart() {
    m_evolution.clearStart();
}

void MyScene::wallSelected() {
    m_cancelTool=false;
    m_calibrTool=false;
    QPen pen = m_wallLine->pen();
    pen.setColor(Qt::black);
    m_wallLine->setPen(pen);
    m_wallLine->setData(0, "wall");
    m_selectedGroup = m_walls;
    m_selectedPath = &m_paintWalls;
}

void MyScene::cancelSelected() {
    m_cancelTool=true;
    m_calibrTool=false;
    QPen pen = m_wallLine->pen();
    pen.setColor(Qt::red);
    m_wallLine->setPen(pen);
    m_selectedGroup = nullptr;
}

void MyScene::fitnessSelected() {
    m_cancelTool=false;
    m_calibrTool=false;
    QPen pen = m_wallLine->pen();
    pen.setColor(Qt::green);
    m_wallLine->setPen(pen);
    m_wallLine->setData(0, "fit");
    m_selectedGroup = m_fitness;
    m_selectedPath = &m_paintFit;
}

void MyScene::calibrSelected() {
    m_calibrTool=true;
    m_cancelTool=false;
}

void MyScene::arrowSelected() {

}


QGraphicsItemGroup* MyScene::getWalls() const {
    return m_walls;
}

QList<QGraphicsItem*> MyScene::getFittnes() const {
    return m_fittnesRightVerison;
}

QPainterPath* MyScene::getWallsPath() {
    return &m_paintWalls;
}

QPainterPath* MyScene::getFitPath() {
    return &m_paintFit;
}


void MyScene::focusOutEvent(QFocusEvent *focusEvent) {

}

void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    if(m_calibrTool) {
        QPointF pos = mouseEvent->scenePos();
        QLineF line1(pos-QPointF(0,5), pos+QPointF(0,5));
        QLineF line2(pos-QPointF(5,0), pos+QPointF(5,0));
        foreach(QGraphicsItem* item, m_fitness->childItems()) {
            QGraphicsLineItem *line = static_cast<QGraphicsLineItem*>(item);
            if(line->line().intersects(line1)==QLineF::BoundedIntersection || line->line().intersects(line2)==QLineF::BoundedIntersection) {
                if(!m_fittnesRightVerison.contains(item)) {
                    m_fittnesRightVerison.push_back(item);
                    QPen pen=line->pen();
                    pen.setColor(Qt::blue);
                    line->setPen(pen);
                }
            }
        }
        return;
    }
    if(mouseEvent->buttons().testFlag(Qt::LeftButton)) {
        QPointF p = mouseEvent->scenePos();
        m_wallLine->setLine(QLineF(m_wallLine->line().p1(), p));
    }
}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    if(m_calibrTool) {
        m_fittnesRightVerison.clear();
        foreach(QGraphicsItem* item, m_fitness->childItems()) {
            QGraphicsLineItem *line = static_cast<QGraphicsLineItem*>(item);
            QPen pen=line->pen();
            pen.setColor(Qt::green);
            line->setPen(pen);
        }
        return;
    }
    if(mouseEvent->button() == Qt::LeftButton) {
        m_wallLine->setVisible(true);
        QPointF p = mouseEvent->scenePos();
        m_wallLine->setLine(QLineF(p, p));
    }
}

void MyScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    if(m_cancelTool) {
        QLineF line = m_wallLine->line();
        QList<QGraphicsItem*> toRemove;
        foreach(QGraphicsItem *item, m_walls->childItems()) {
            QGraphicsLineItem *line2 = static_cast<QGraphicsLineItem*>(item);
            if(line2->line().intersects(line) == QLineF::BoundedIntersection) {
                toRemove.append(item);
            }
        }
        foreach(QGraphicsItem *item, toRemove) {
            removeItem(item);
            delete item;
        }
        toRemove.clear();
        foreach(QGraphicsItem *item, m_fitness->childItems()) {
            QGraphicsLineItem *line2 = static_cast<QGraphicsLineItem*>(item);
            if(line2->line().intersects(line) == QLineF::BoundedIntersection) {
                toRemove.append(item);
            }
        }
        foreach(QGraphicsItem *item, toRemove) {
            removeItem(item);
            delete item;
        }
        m_wallLine->setVisible(false);
        return;
    }
    if(m_calibrTool) {
        foreach(QGraphicsItem* item, m_fitness->childItems()) {
            QGraphicsLineItem *line = static_cast<QGraphicsLineItem*>(item);
            QPen pen=line->pen();
            pen.setColor(Qt::green);
            line->setPen(pen);
        }
        qDebug() << m_fittnesRightVerison.size();
        return;
    }
    if(mouseEvent->button() == Qt::LeftButton) {
        QGraphicsLineItem *item = new QGraphicsLineItem(m_wallLine->line());
        item->setPen(m_wallLine->pen());
        item->setData(0, m_wallLine->data(0));
        m_selectedPath->moveTo(item->line().p1());
        m_selectedPath->lineTo(item->line().p2());
        m_selectedGroup->addToGroup(item);
        m_wallLine->setVisible(false);
    }
}

bool MyScene::event(QEvent *event) {

    if(event->type() == QEvent::KeyPress) {
        QKeyEvent *key = (QKeyEvent*)(event);
        bool flag=false;
        if(key->key()==Qt::Key_W) {
            emit pressW(true);
            flag=true;
        }
        if(key->key()==Qt::Key_A) {
            emit pressA(true);
            flag=true;
        }
        if(key->key()==Qt::Key_D) {
            emit pressD(true);
            flag=true;
        }
        if(key->key()==Qt::Key_S) {
            emit pressS(true);
            flag=true;
        }
        return flag;
    } else if(event->type() == QEvent::KeyRelease) {
        QKeyEvent *key = (QKeyEvent*)(event);
        bool flag=false;
        if(key->key()==Qt::Key_W) {
            emit pressW(false);
            flag=true;
        }
        if(key->key()==Qt::Key_A) {
            emit pressA(false);
            flag=true;
        }
        if(key->key()==Qt::Key_D) {
            emit pressD(false);
            flag=true;
        }
        if(key->key()==Qt::Key_S) {
            emit pressS(false);
            flag=true;
        }
        return flag;
    }
    else if(event->type() == QEvent::Timer) {
        emit updateTime();
        return true;
    }
    return QGraphicsScene::event(event);
}
