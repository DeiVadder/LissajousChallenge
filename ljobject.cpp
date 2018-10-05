#include "ljobject.h"

#include <QtMath>
#include <QDebug>

const double angleSpeed(0.5);

LjObject::LjObject(qreal offsetX, qreal offsetY, qreal multiplier, QObject *parent) :
    QObject(parent), m_Multiplier(multiplier), m_OffsetX(offsetX * 60), m_OffsetY(offsetY * 60)
{
    m_Rect = {0,0,60,60};
}

QPointF LjObject::getCurrent()
{
    if(m_CurrentIntex >= 0 && m_CurrentIntex < m_Points.size())
        return  m_Points.at(m_CurrentIntex);
    else
        return  QPointF();
}

QPainterPath LjObject::getPath()
{
    QPointF pt1;
    QPointF pt2;
    QPainterPath path;

    if(m_Points.size() < 3){
        //not enough point to curve -> use direct lines
        for(int i(0); i < m_Points.size() -1; i++) {
            path.moveTo(m_Points.at(i));
            path.lineTo(m_Points.at(i+1));
        }
        return  path;
    }

    for(int i(0); i < m_Points.size() -1; i++){
        pt1 = getLineStart(m_Points.at(i), m_Points.at(i + 1));
        if(i == 0)
            path.moveTo(pt1);
        else
            path.quadTo(m_Points.at(i), pt1);

        pt2 = getLineEnd(m_Points.at(i), m_Points.at(i + 1));
        path.lineTo(pt2);
    }

    return  path;
}

void LjObject::addPoint(const QPointF &point)
{
    m_CurrentIntex ++;
    if(m_CurrentIntex >= 0 && m_CurrentIntex < m_Points.size()){
        m_Points[m_CurrentIntex] = point;
    } else
        m_Points.append(point);

    emit newXPos(m_Points.at(m_CurrentIntex).x());
    emit newYPos(m_Points.at(m_CurrentIntex).y());

    m_Points[m_CurrentIntex] +=(QPointF {m_OffsetX,m_OffsetY});
}

void LjObject::addPoint(const float &x, const float y)
{
    addPoint(QPointF(x,y));
}

void LjObject::addPointByAngle()
{
    float r = m_Rect.width()/2.0;
    QPointF f{(r * qCos(m_Angle * M_PI / 180.0))+r, (r*qSin(m_Angle * M_PI / 180.0))+r};

    addPoint(f);

    m_Angle += angleSpeed * m_Multiplier;

    if(m_OffsetX == 0 && m_OffsetY == 0 && m_Angle > 270){
//        m_Angle = -90;
        emit fullCircle();
    }
}

void LjObject::updatePath()
{
    addPoint(m_NextX, m_NextY);
    m_NextX = 0;
    m_NextY = 0;
}

void LjObject::setNextX(const qreal &x)
{
    m_NextX = x;
}

void LjObject::setNextY(const qreal &y)
{
    m_NextY = y;
}

void LjObject::reset()
{
    m_Angle = -90;
    m_CurrentIntex = 0;
}

float LjObject::distance(const QPointF &pt1, const QPointF &pt2)
{
    float hd = (pt1.x() - pt2.x()) * (pt1.x() - pt2.x());
    float vd = (pt1.y() - pt2.y()) * (pt1.y() - pt2.y());
    return std::sqrt(hd + vd);
}

QPointF LjObject::getLineStart(const QPointF &pt1, const QPointF &pt2)
{
    QPointF pt;
    float rat = 1.0 / distance(pt1, pt2);
    if (rat > 0.5) {
        rat = 0.5;
    }
    pt.setX((1.0 - rat) * pt1.x() + rat * pt2.x());
    pt.setY((1.0 - rat) * pt1.y() + rat * pt2.y());
    return pt;
}

QPointF LjObject::getLineEnd(const QPointF &pt1, const QPointF &pt2)
{
    QPointF pt;
    float rat = 1.0 / distance(pt1, pt2);
    if (rat > 0.5) {
        rat = 0.5;
    }
    pt.setX(rat * pt1.x() + (1.0 - rat)*pt2.x());
    pt.setY(rat * pt1.y() + (1.0 - rat)*pt2.y());
    return pt;
}
