#ifndef PDF_CREATOR_H
#define PDF_CREATOR_H

#include <QObject>
#include <QWidget>


class PDFCreator
{
    enum class PDFType{

    };

public:
    PDFCreator();
    bool createPDF();

};

#endif // PDF_CREATOR_H
