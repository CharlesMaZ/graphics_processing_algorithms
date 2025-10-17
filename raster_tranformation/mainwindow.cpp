#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPixmap>
#include <QPainter>
#include <QImage>
#include <cmath>
#include <QFileDialog>


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

    //na interfejsie podanie dwóch liczb rzeczywistych, oby nie 0
    //za kazdym razem uzytkownika trzeba nowy wyraz
    //petla na nowym skalowaynm obrazku, przeliczamy na stary i przepisujemy rgb na nowy(skalowany) - dla metody najbliższego sąsiada
    //wpadamy w ułamkowy piksel - round

    //rotacja
    //o kąt alfa
    //najpierw zrobic obrazek wilkosci originalu
    //obracanie tylko środka (może obrazek wyjśc poza qlabel - nie ma gdzie wpsiać pikseli, wiec nie wpisujemy)
    //jesli obrasek wychodzi poza oryginał, (bo bedziemy miec pustkę po odwróceniu) to wypełniamy np. białym

    //
    modImage = originalImage;
    ui->imageLabel->setPixmap(QPixmap::fromImage(modImage));

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::loadImage);
    connect(ui->rotationSlider, &QSlider::valueChanged, this, &MainWindow::updateImage);
    connect(ui->scaleSlider, &QSlider::valueChanged, this, &MainWindow::updateImage);
    connect(ui->radioScale, &QRadioButton::toggled, this, &MainWindow::updateImage);
    connect(ui->rotateScale, &QRadioButton::toggled, this, &MainWindow::updateImage);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateImage(){
    double angle = ui->rotationSlider->value();
    double scale = ui->scaleSlider->value()/10.0;


    qDebug() << "Wartość suwaka (surowa):" << ui->scaleSlider->value();
    qDebug() << "Przeliczone skalowanie:" << scale;

    if(ui->rotateScale->isChecked()){
        modImage = rotateImage(originalImage, angle);
    }
    else if(ui->radioScale->isChecked()){
        modImage = scaleImage(originalImage, scale);
    }
    qDebug() << "Nowy rozmir obrazu: " << modImage.width() << "x" << modImage.height();


    ui->imageLabel->setPixmap(QPixmap::fromImage(modImage));
    ui->imageLabel->adjustSize();


}


void MainWindow::loadImage() {
    QString filePath = QFileDialog::getOpenFileName(this, "Otwórz obraz", "", "Images (*.png *.jpg *.bmp)");
    if (!filePath.isEmpty()) {
        originalImage.load(filePath);
        modImage = originalImage;
        ui->imageLabel->setPixmap(QPixmap::fromImage(originalImage));
    }
}


QImage MainWindow::rotateImage(const QImage &image, double angle){
    int width = image.width();
    int height = image.height();
    QImage rotated(width, height, QImage::Format_ARGB32);
    rotated.fill(Qt::white);

    double radians = angle * M_PI /180.0;

    double cosAngle = std::cos(radians);
    double sinAngle = std::sin(radians);

    int xCenter = width / 2;
    int yCenter = height / 2;

    for(int xNew = 0; xNew < width; ++xNew){
        for(int yNew = 0; yNew < height; ++yNew){
            int x = xNew - xCenter;
            int y = yNew - yCenter;
            int xOld = static_cast<int>( round( x * cosAngle + y * sinAngle ) ) + xCenter;
            int yOld = static_cast<int>( round(-x * sinAngle + y * cosAngle ) ) + yCenter;

            if(xOld >= 0 && xOld < width && yOld >= 0 && yOld < height){
                rotated.setPixel(xNew, yNew, image.pixel(xOld, yOld));
            }
        }
    }
    return rotated;
}



QImage MainWindow::scaleImage(const QImage &image, double scale){
    if(scale <= 0.0){
        return image;
    }

    int newWidth = static_cast<int>(image.width() *scale);
    int newHeight = static_cast<int>(image.height() *scale);

    QImage scaled(newWidth, newHeight, QImage::Format_ARGB32);

    for (int i = 0; i < newWidth; ++i) {
        for (int j = 0; j < newHeight; ++j) {
            int xOld = static_cast<int>(round(i/scale));
            int yOld = static_cast<int>(round(j/scale));

            xOld = std::min(std::max(xOld, 0), image.width() - 1);
            yOld = std::min(std::max(yOld, 0), image.height() - 1);


            QRgb color = image.pixel(xOld, yOld);
            scaled.setPixel(i, j, color);
        }
    }

    return scaled;
}





















