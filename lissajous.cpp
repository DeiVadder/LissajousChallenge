#include "lissajous.h"

#include <QTimer>
#include "QPainter"

#include "ljobject.h"
#include <QDebug>

const int cols(20);
const int rows(12);
const double multi (1);

Lissajous::Lissajous(QWidget *parent)
    : QWidget(parent)
{
    resize(60 *cols,60*rows);
    m_UpdateTimer = new QTimer(this);
    m_UpdateTimer->setInterval(0);

    connect(m_UpdateTimer, &QTimer::timeout, this, QOverload<>::of(&Lissajous::update));
    m_UpdateTimer->start();

    LjObject *controll = nullptr;

    for(int i(0); i < cols; i++){
        QVector<LjObject *> v;
        for(int j(0); j < rows; j++){
            float multis = (i == 0 && j == 0 ) ? 1 : i == 0 ? (j + 1) * multi : j == 0 ? (i + 1) * multi : 0;/* i != 0 ? i + multi : j != 0 ? j +multi : 1;*/
            LjObject *obj = new LjObject(i, j, multis, this);
            if(i == 0 || j == 0)
                connect(m_UpdateTimer, &QTimer::timeout, obj, &LjObject::addPointByAngle);
            else
                connect(m_UpdateTimer, &QTimer::timeout, obj, &LjObject::updatePath);

            if(i == 0 && j == 0)
                controll = obj;
            if(controll)
                connect(controll, &LjObject::fullCircle, obj, &LjObject::reset);
            v.append(obj);
        }
        m_Objects.append(v);
    }

    for(int i(1); i < m_Objects.at(0).size(); i++){
        const LjObject *sender = m_Objects.at(0).at(i);
        for(int j(1); j < m_Objects.size() && i < m_Objects.at(j).size() ; j++){
            const LjObject *obj = m_Objects.at(j).at(i);
            connect(sender, &LjObject::newYPos, obj, &LjObject::setNextY);
        }
    }

    for(int i(1); i < m_Objects.size(); i++){
        const LjObject *sender = m_Objects.at(i).at(0);
        for(int j(1); j < m_Objects.at(i).size(); j++){
            const LjObject *obj = m_Objects.at(i).at(j);
            connect(sender, &LjObject::newXPos, obj, &LjObject::setNextX);
        }
    }
}

void Lissajous::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter p(this);
    p.setBrush(QColor("white"));
    p.drawRect(rect());

    for(auto vec : m_Objects){
        for(auto obj : vec){
            p.setBrush(QColor("transparent"));
            p.drawPath(obj->getPath());
            p.setBrush(QColor("red"));
            p.drawEllipse(obj->getCurrent(),3,3);
        }
    }
}
