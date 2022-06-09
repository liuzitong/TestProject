#include "analysisResult.h"
#include <QDebug>

AnalysisResult::AnalysisResult(float md, float psd, float VFI, int GHT,int dotSeen,int dotWeakSeen,int dotUnseen, QObject *parent):
    QObject(parent),m_md(md),m_psd(psd),m_VFI(VFI),m_GHT(GHT),m_dotSeen(dotSeen),m_dotWeakSeen(dotWeakSeen),m_dotUnseen(dotUnseen)
{
    if(m_psd>4.3) m_p_psd=0.5;
    else if(m_psd>3.7) m_p_psd=1;
    else if(m_psd>3.2) m_p_psd=2;
    else if(m_psd>2.5) m_p_psd=5;
    else if(m_psd>2.0) m_p_psd=10;

    if(m_md<-5.5)  m_p_md=0.5;
    else if(m_md<-3.5) m_p_md=1;
    else if(m_md<-2.6) m_p_md=2;
    else if(m_md<-2.0) m_p_md=5;
    else if(m_md<-1.5) m_p_md=10;

    qDebug()<<m_p_md;
    qDebug()<<m_dotSeen;
    qDebug()<<m_dotUnseen;
    qDebug()<<m_dotWeakSeen;
}


