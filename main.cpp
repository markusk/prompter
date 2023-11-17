#include "prompter.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Prompter w;

    // add application icon
    w.setWindowIcon(QIcon(":/icons/prompter.icns")); // path from resource file


    w.show();
    return a.exec();
}
