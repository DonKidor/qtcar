#include "MyCar.h"
#include <MyScene.h>
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>

MyCar::MyCar(QGraphicsItem *parent) : QGraphicsObject(parent), m_velocity(0), m_signals{false,false,false,false}
{
    m_lines = new QGraphicsItemGroup(this);
    QGraphicsLineItem *line;
    int index=0;
    for(int i=-90;i<=90; i+=45) {
        QLineF mline(0,0,0,MAX_LEN);
        QTransform tran;
        mline = mline * tran.rotate(i);
        line = new QGraphicsLineItem(mline);
        m_lines->addToGroup(line);
        line->setVisible(false);
        ++index;
    }
    m_lenght.resize(index);

}
void MyCar::setBrain(const MyBrain &another) {
    m_brain=another;
}

qint32 MyCar::getFittnes() const {
    return m_fittnes;
}

MyBrain MyCar::getBrain() {
    return m_brain;
}

void MyCar::initCar(QPointF t_pos, qreal t_rot) {
    MyScene *t_scene = static_cast<MyScene*>(scene());
    m_fittneses = t_scene->getFittnes();
    m_fitIndex=0;

    m_fittnes = 0;
    m_fitKoef = 5;
    m_deadTimer=DEADTIMERMAX;
    m_dead = false;
    QObject::connect(t_scene, SIGNAL(updateTime()), this, SLOT(updateTime()));
    setTransform(QTransform());
    setPos(t_pos);
    setRotation(t_rot);
}

bool MyCar::event(QEvent *ev) {
    if(ev->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouse = static_cast<QMouseEvent*>(ev);
        if(mouse->button()==Qt::LeftButton) {
            emit clicked();
            return true;
        }
    }
    return QGraphicsObject::event(ev);
}

void MyCar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->save();
    painter->setBrush(QBrush(Qt::black));
    painter->drawRect(QRectF(-10,-20,20,40));
    painter->restore();
}

QRectF MyCar::boundingRect() const {
    return QRectF(-10,-20,20,40);
}

void MyCar::forward(bool enabled) {
    m_signals[0]=enabled;
}

void MyCar::turnLeft(bool enabled) {
    m_signals[1]=enabled;
}

void MyCar::turnRight(bool enabled) {
    m_signals[2]=enabled;
}

void MyCar::brake(bool enabled) {
    m_signals[3]=enabled;
}

void MyCar::getLengths() {
    MyScene *mscene = (MyScene*) scene();

    QList<QGraphicsItem*> items=m_lines->childItems();
    auto it = items.begin();
    int index=0;
    for(;it!=items.end();++it) {
        QGraphicsLineItem *el=(QGraphicsLineItem*) *it;
        bool flag=false;
        QPointF interPoint;
        foreach(QGraphicsItem *el2, mscene->getWalls()->childItems()) {
            QLineF line2 = ((QGraphicsLineItem*)el2)->line();
            QLineF line1 = el->line() * sceneTransform();
            QPointF tempPoint;
            if(line1.intersects(line2, &tempPoint) == QLineF::BoundedIntersection) {
                tempPoint = tempPoint * sceneTransform().inverted();
                if(flag) {
                    if(tempPoint.manhattanLength() >= interPoint.manhattanLength()) continue;
                }
                interPoint = tempPoint;
                flag=true;
            }
        }
        if(flag) {
            m_lenght[index]=interPoint.x()*interPoint.x()+interPoint.y()*interPoint.y();
            //el->setPen(QPen(Qt::red));
        } else {
            m_lenght[index]=MAX_LEN*MAX_LEN;
            //el->setPen(QPen(Qt::black));
        }

    }
}


void MyCar::checkHit() {
    MyScene *t_scene = static_cast<MyScene*>(scene());


    if(this->collidesWithPath(this->mapFromScene(*t_scene->getWallsPath()))) {
        carDead();
    }

    if(m_fittneses[m_fitIndex]->collidesWithItem(this)) {
        ++m_fitIndex;
        m_fittnes+=500+10*m_fitKoef;
        m_fitKoef = 5;
        m_deadTimer=DEADTIMERMAX;
    }

    if(m_fitIndex==m_fittneses.size()) m_fitIndex=0;

    /*QList<QGraphicsItem*> t_list= this->collidingItems();
    foreach(QGraphicsItem* item, t_list) {
        if(item->data(0)=="wall") {
            carDead();
            break;
        } else if(item->data(0)=="fit" && !m_fittneses.contains(item)) {
            m_fittneses.insert(item);
            m_fittnes+=100+100*m_fitKoef;
            m_fitKoef = 5;
            m_deadTimer=DEADTIMERMAX;
        }
    }*/

    //if(m_fittneses.size()==0) return;
    /*
    foreach(QGraphicsItem* item, t_list) {
        if(this->collidesWithItem(item) && !m_fittneses.contains(item)) {
            m_fittneses.insert(item);
            m_fittnes+=100*m_fitKoef;
            m_fitKoef = 5;
            m_deadTimer=DEADTIMERMAX;
            //((QGraphicsLineItem*) item)->setPen(QPen(Qt::red));
        }
    }

    t_list = t_scene->getWalls()->childItems();
    foreach(QGraphicsItem* item, t_list) {
        if(this->collidesWithItem(item)) {
            carDead();
            break;
        }
    }*/
}

void MyCar::carDead() {
    m_dead = true;
    MyScene *t_scene = static_cast<MyScene*>(scene());
    QObject::disconnect(t_scene, SIGNAL(updateTime()), this, SLOT(updateTime()));
    emit dead();
}

void MyCar::updateTime() {
    --m_deadTimer;
    if(m_deadTimer<=0) {
        carDead();
        return;
    }

    checkHit();
    getLengths();

    QVector<qreal> data(7);
    for(int i=0; i<5; ++i) data[i]=((MAX_LEN*MAX_LEN)-m_lenght[i])/(MAX_LEN*MAX_LEN);
    data[5]=m_velocity/MAX_SPEED;
    data[6]=1;
    QVector<bool> tempVector= m_brain.think(data);
    for(int i=0; i<4; ++i) m_signals[i]=tempVector[i];


    if(m_velocity!=0) {
        qreal dv=(m_velocity>0?1:-1)*0.2;
        if(m_velocity*(m_velocity-dv)<0) m_velocity=0;
        else m_velocity-=dv;
    }

    if(m_signals[0]) {
        m_velocity+=1;
    }
    if(m_signals[3]) {
        m_velocity-=1;
    }
    QTransform tran;
    if(m_signals[1]) {
        tran.rotate(-8);
    }
    if(m_signals[2]) {
        tran.rotate(8);
    }

    setTransform(transform()*tran);
    if(abs(m_velocity)>MAX_SPEED) {
        m_velocity/=abs(m_velocity);
        m_velocity*=MAX_SPEED;
    }
    QPointF p=QPointF(0, m_velocity)*transform();



    m_fitKoef*=0.99;

    moveBy(p.x(), p.y());
}

