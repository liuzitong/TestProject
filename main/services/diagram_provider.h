#ifndef DIGRAM_PROVIDER_H
#define DIGRAM_PROVIDER_H

#include <QObject>
#include <QVariant>
#include <QVariantList>
#include <QSize>
#include <QPointF>

class DiagramProvider : public QObject
{
    Q_OBJECT
public:
    explicit DiagramProvider(QObject *parent = nullptr);
    Q_INVOKABLE void drawDiagram(QString name,int os_od,int range,QVariantList dotList,QVariantList values);


signals:

public slots:
private:
//    QPointF convertDegLocToPixLoc(QPoint DegLoc);
//    QSize imageSize=QSize(250,250);
};

#endif // DIGRAM_PROVIDER_H
