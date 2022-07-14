#ifndef ANALYSISRESULT_H
#define ANALYSISRESULT_H

#include <QObject>

class StaticAnalysisResult : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float md READ getmd)
    Q_PROPERTY(float psd READ getpsd)
    Q_PROPERTY(float VFI READ getVFI)
    Q_PROPERTY(int GHT READ getGHT)
    Q_PROPERTY(float p_md READ getp_md)
    Q_PROPERTY(float p_psd READ getp_psd)
    Q_PROPERTY(int dotSeen READ getDotSeen)
    Q_PROPERTY(int dotWeakSeen READ getDotWeakSeen)
    Q_PROPERTY(int dotUnseen READ getDotUnseen)
public:
    Q_INVOKABLE explicit StaticAnalysisResult(float md,float p_md, float psd,float p_psd, float VFI, int GHT,int dotSeen,int dotWeakSeen,int dotUnseen, QObject *parent=nullptr):
        QObject(parent),m_md(md),m_p_md(p_md),m_psd(psd),m_p_psd(p_psd),m_VFI(VFI),m_GHT(GHT),m_dotSeen(dotSeen),m_dotWeakSeen(dotWeakSeen),m_dotUnseen(dotUnseen){};
    Q_INVOKABLE virtual ~StaticAnalysisResult()=default;
    float getmd(){return m_md;}
    float getpsd(){return m_psd;}
    float getVFI(){return m_VFI;}
    float getGHT(){return m_GHT;}
    float getp_md(){return m_p_md;}
    float getp_psd(){return m_p_psd;}
    int getDotSeen(){return m_dotSeen;}
    int getDotWeakSeen(){return m_dotWeakSeen;}
    int getDotUnseen(){return m_dotUnseen;}

private:
    float m_md,m_p_md,m_psd,m_p_psd,m_VFI;
    int m_GHT,m_dotSeen,m_dotWeakSeen,m_dotUnseen;


signals:

public slots:
};

#endif // ANALYSISRESULT_H
