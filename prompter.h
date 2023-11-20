#ifndef PROMPTER_H
#define PROMPTER_H

#include <QMainWindow>
#include <QPainter>
#include <QImage>
#include <QDir>
#include <QObject> // for connecting signals and slots
#include <QTimer>  // for scroll animation
#include <QKeyEvent> // for catching the esc key when in full screen mode

#include <QBrush>
#include <QFont>
#include <QPen>
#include <QWidget>

#include <QScreen> // for getting resolution
#include <QMessageBox>


QT_BEGIN_NAMESPACE
namespace Ui { class Prompter; }
QT_END_NAMESPACE

class Prompter : public QMainWindow
{
    Q_OBJECT

public:
    Prompter(QWidget *parent = nullptr);
    ~Prompter();
    void paintEvent(QPaintEvent *event);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void on_pushButtonScroll_clicked();
    void on_pushButtonReset_clicked();
    void on_pushButtonExit_clicked();
    void on_dialScrollSpeed_valueChanged(int value);
    void on_radioButtonLeft_clicked();
    void on_radioButtonCentered_clicked();
    void on_radioButtonRight_clicked();
    void on_checkBoxWordWrap_stateChanged();
    void on_checkBoxMirror_stateChanged();
    void on_pushButtonFullScreen_clicked();
    void on_spinBoxFontSizePrompter_valueChanged();

    // let the prompter text scroll
    void scrollPrompter();

    // update openGL widget with mirrored prompter text
    void updatePrompterImage();

    // updated seletcted font
    void onFontComboBoxTextEditChanged(const QFont& font);

    void updateTextEditFont();
    void updatePrompterFont();


private:
    Ui::Prompter *ui;

    int width;
    int height;

    int prompterWidgetGeometryX;
    int prompterWidgetGeometryY;
    int prompterWidgetGeometryWidth;
    int prompterWidgetGeometryHeight;

    // new for OpenGL drawing
    QFont textEditFont;
    QFont prompterFont = QFont("Arial");
    int fontSizeTextEdit = 12;
    int fontSizePrompter = 50;
    QPen textPen;


    QImage imagePrompterText;
    int scrollValueY;
    int textDirection;
    QColor textColor;
    QColor backgroundColor;

    // timer for scroll animation
    QTimer *timer;
};
#endif // PROMPTER_H
