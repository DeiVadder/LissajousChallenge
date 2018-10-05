#ifndef LJOBJECT_H
#define LJOBJECT_H

#include <QObject>
#include <QPainterPath>
#include <QPointF>
#include <QVector>

class LjObject : public QObject
{
    Q_OBJECT

public:
    explicit LjObject(qreal offsetX, qreal offsetY, qreal multiplier = 1, QObject *parent = nullptr);

    QPointF getCurrent();
    QPainterPath getPath();

public slots:
    void addPoint(const QPointF &point);
    void addPoint(const float &x, const float y);
    void addPointByAngle();

    void updatePath();
    void setNextX(const qreal &x);
    void setNextY(const qreal &y);

    void reset();
signals:
    void newXPos(const qreal &);
    void newYPos(const qreal &);

    void fullCircle();

private:
    float distance(const QPointF& pt1, const QPointF& pt2);
    QPointF getLineStart(const QPointF& pt1, const QPointF& pt2);
    QPointF getLineEnd(const QPointF& pt1, const QPointF& pt2);

private:
    int m_CurrentIntex = -1;

    qreal m_Multiplier;
    qreal m_OffsetX, m_OffsetY;
    qreal m_NextX = 0, m_NextY = 0;

    float m_Angle = -90;

    QVector<QPointF> m_Points;

    QRectF m_Rect;

};

#endif // LJOBJECT_H
