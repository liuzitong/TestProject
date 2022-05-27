#ifndef ANALYSISRESULT_H
#define ANALYSISRESULT_H

#include <QObject>

class AnalysisResult : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float md READ getmd)
    Q_PROPERTY(float psd READ getpsd)
    Q_PROPERTY(float VFI READ getVFI)
    Q_PROPERTY(int GHT READ getGHT)
    Q_PROPERTY(float p_md READ getp_md)
    Q_PROPERTY(float p_psd READ getp_psd)
public:
    Q_INVOKABLE explicit AnalysisResult(float md,float psd,float VFI,int GHT,QObject *parent = nullptr);
    Q_INVOKABLE virtual ~AnalysisResult()=default;
    float getmd(){return m_md;}
    float getpsd(){return m_psd;}
    float getVFI(){return m_VFI;}
    float getGHT(){return m_GHT;}
    float getp_md(){return m_p_md;}
    float getp_psd(){return m_p_psd;}

private:
    float m_md,m_psd,m_p_md,m_p_psd,m_VFI;
    int m_GHT;


signals:

public slots:
};

#endif // ANALYSISRESULT_H
