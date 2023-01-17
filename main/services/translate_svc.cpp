#include "translate_svc.h"


Perimeter::TranslateController *Perimeter::TranslateController::instance()
{
    static TranslateController controller/*(engine)*/;
    return &controller;
}

void Perimeter::TranslateController::loadLanguage(QLocale::Language lang) {
    switch (int(lang)) {
    case QLocale::Chinese:
        if (m_translator->load("./perimeter_zh_CN.qm")) {
//            emit message(tr("--- Language changed to Chinese"));
        }
        break;
    default:
        m_translator->load("");
//        emit message(tr("--- Language changed to English"));
        break;
    }

//    emit triggerChanged();
    //        retranslateUi();
}

Perimeter::TranslateController::TranslateController() {
    //        m_engine = engine;
    m_translator = new QTranslator(this);
    QCoreApplication::installTranslator(m_translator);
}
