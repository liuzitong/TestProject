#ifndef TRANSLATE_SVC_H
#define TRANSLATE_SVC_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTranslator>


class TranslateController : public QObject
{
    Q_OBJECT
    Q_ENUMS(Language)
public:
    enum class Language
    {
        English = 1,
        Chinese
    };

public:
    static TranslateController* instance(QQmlEngine *engine)
    {
        static TranslateController controller(engine);
        return &controller;
    }

    void retranslateUi() {
//        m_engine->retranslate();
    }

    Q_INVOKABLE void loadLanguage(Language lang) {
        switch (lang) {
        case Language::Chinese:
            if (m_translator->load("./language/Translator_qml_zh_CN.qm")) {
                emit message(tr("--- Language changed to Chinese"));
            }
            break;
        case Language::English:
            m_translator->load("");
            emit message(tr("--- Language changed to English"));
            break;
        }

        retranslateUi();
    }

signals:
    void message(const QString &msg);

private:
    TranslateController(QQmlEngine *engine) {
        m_engine = engine;
        m_translator = new QTranslator(this);
        QCoreApplication::installTranslator(m_translator);
    }

    QQmlEngine *m_engine = nullptr;
    QTranslator *m_translator = nullptr;
};

#endif // TRANSLATE_SVC_H
