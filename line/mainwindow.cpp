#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPixmap>
#include <QPainter>
#include <QImage>
#include <cmath>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // imageLabel = new QLabel(this);
    // imageLabel->setAlignment(Qt::AlignVCenter);
    // setCentralWidget(imageLabel);

    // loadImage();
    originalImage.load("D:\\studia\\przetwarzanie grafiki\\lab5\\nienazwane\\lena.png");
    if(originalImage.isNull()){
        ui->imageLabel->setText("problemz  obrazkiem, elo");
        return;
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
    qDebug() << "Nowy rozmiar obrazu: " << modImage.width() << "x" << modImage.height();


    ui->imageLabel->setPixmap(QPixmap::fromImage(modImage));
    ui->imageLabel->adjustSize();


}

void MainWindow::loadImage(){

    QImage rotated = rotateImage(originalImage, 5.0);
    QImage scaled = scaleImage(rotated, 2);
    //imageLabel->setPixmap((QPixmap::fromImage(scaled)));

}

QImage MainWindow::rotateImage(const QImage &image, double angle){
    QImage rotated(image.size(), QImage::Format_ARGB32);
    rotated.fill(Qt::white);

    double radians = angle * M_PI /180.0;

    double cos = std::cos(radians);
    double sin = std::sin(radians);

    return rotated;
}



QImage MainWindow::scaleImage(const QImage &image, double scale){
    if(scale <= 0.0){
        return image;
    }

    int newWidth = static_cast<int>(image.width() * scale);
    int newHeight = static_cast<int>(image.height() * scale);

    QImage scaled(newWidth, newHeight, QImage::Format_ARGB32);

    for (int i = 0; i < newWidth; ++i) {
        for (int j = 0; j < newHeight; ++j) {
            int xOld = static_cast<int>(round(i/scale));
            int yOld = static_cast<int>(round(j/scale));

            QRgb color = image.pixel(xOld, yOld);
            scaled.setPixel(i, j, color);
        }
    }

    return scaled;
}





















