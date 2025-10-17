#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QImage>
#include <QGraphicsScene>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loadImage();
    void updateImage();
    //void updateLAB();

private:
    struct XYZ{
        float X;
        float Y;
        float Z;
    };
    struct LAB{
        float L;
        float A;
        float B;
    };
    struct RGB{
        float R;
        float G;
        float B;
    };



    Ui::MainWindow *ui;
    QImage originalImage;
    QImage modImage;

    XYZ rgb2xyz(QColor rgb, XYZ &xyz);
    QColor xyz2rgb(XYZ xyz, QColor &rgb);
    LAB xyz2lab(XYZ xyz, LAB &lab);
    XYZ lab2xyz(LAB lab, XYZ &xyz);

};
#endif // MAINWINDOW_H
