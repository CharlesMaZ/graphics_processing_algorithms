#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QVector>
#include <QtMath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString imagePath = "D:\studia\przetwarzanie grafiki\lab1\goliath.png";
    originalImage.load(imagePath);
    modImage = originalImage;

    if(!originalImage.isNull()){
        ui->imageLabel->setPixmap(QPixmap::fromImage(originalImage));
    }

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::loadImage);

    connect(ui->maskButton, &QPushButton::clicked, this, [=]() { updateImage(1); });
    connect(ui->maskButton_2, &QPushButton::clicked, this,  [=]() { updateImage(2); });
    connect(ui->maskButton_3, &QPushButton::clicked, this, [=]() { updateImage(3); });
    connect(ui->original, &QPushButton::clicked, this,  [=]() { updateImage(0); });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadImage() {
    QString filePath = QFileDialog::getOpenFileName(this, "Otwórz obraz", "", "Images (*.png *.jpg *.bmp)");
    if (!filePath.isEmpty()) {
        originalImage.load(filePath);
        modImage = originalImage;
        ui->imageLabel->setPixmap(QPixmap::fromImage(originalImage));
    }
}



void MainWindow::updateImage(int type){
    if(originalImage.isNull()){
        return;
    }
    QVector<QVector<float>> maskSharpen={
        {0,-1,0},
        {-1,5,-1},
        {0,-1,0}
    };

    QVector<QVector<float>> maskBlur={
        {1/9.0f,1/9.0f,1/9.0f},
        {1/9.0f,1/9.0f,1/9.0f},
        {1/9.0f,1/9.0f,1/9.0f}
    };

    QVector<QVector<float>> maskEdge={
        {-1,-1,-1},
        {-1,8,-1},
        {-1,-1,-1}
    };
    if(type == 0){
        modImage = originalImage;
    }
    else
    {
        QVector<QVector<float>> selectedMask;

        switch(type){
        case 1: selectedMask = maskSharpen; break;
        case 2: selectedMask = maskBlur;    break;
        case 3: selectedMask = maskEdge;    break;
        default: selectedMask = maskSharpen;
        }

        modImage = applyMask(originalImage, selectedMask);
    }

    ui->imageLabel->setPixmap(QPixmap::fromImage(modImage));

}


QImage MainWindow::applyMask(const QImage &image, const QVector<QVector<float>> &mask) {
    QImage result = image;
    int width = image.width();
    int height = image.height();
    int maskSize = mask.size();
    int offset = maskSize/2;

    for (int y = offset; y < height - offset; ++y) {
        for (int x = offset; x < width - offset; ++x) {
            float r = 0, g = 0, b = 0;

            for (int i = 0; i < maskSize; ++i) {
                for (int j = 0; j < maskSize; ++j) {
                    int ix = x + i - offset;
                    int iy = y + j - offset;

                    QColor color = QColor::fromRgb(image.pixel(ix, iy));
                    float coeff = mask[i][j];

                    r += color.red()   * coeff;
                    g += color.green() * coeff;
                    b += color.blue()  * coeff;
                }
            }

            int nr = qBound(0, static_cast<int>(r), 255);
            int ng = qBound(0, static_cast<int>(g), 255);
            int nb = qBound(0, static_cast<int>(b), 255);

            result.setPixel(x, y, qRgb(nr, ng, nb));
        }
    }

    return result;
}


