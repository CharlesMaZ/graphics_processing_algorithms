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
    void updateImage(int type);

private:
    Ui::MainWindow *ui;
    QImage originalImage;
    QImage modImage;

    QImage applyMask(const QImage &image, const QVector<QVector<float>> &mask);

};
#endif // MAINWINDOW_H
