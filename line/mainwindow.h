#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QLabel>



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
    void updateImage();

private:
    Ui::MainWindow *ui;
    //QLabel *imageLabel;
    QImage originalImage;
    QImage modImage;


    void loadImage();
    QImage rotateImage(const QImage &image, double angle);
    QImage scaleImage(const QImage &image, double scaleFactor);

};
#endif // MAINWINDOW_H
