#ifndef LISSAJOUS_H
#define LISSAJOUS_H

#include <QWidget>

class LjObject;
class QTimer;
class Lissajous : public QWidget
{
    Q_OBJECT

public:
    Lissajous(QWidget *parent = 0);
    ~Lissajous() =default;

protected:
    void paintEvent(QPaintEvent *event)override;

    QTimer *m_UpdateTimer;
    QVector<QVector<LjObject*> > m_Objects;
};

#endif // LISSAJOUS_H
