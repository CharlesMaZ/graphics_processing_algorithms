#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QVector>
#include <QtMath>
#include <QPainter>

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

    connect(ui->brigtnessSlider, &QSlider::valueChanged, this, &MainWindow::updateImage);
    connect(ui->contrastSlider, &QSlider::valueChanged, this, &MainWindow::updateImage);
    connect(ui->gammaSlider, &QSlider::valueChanged, this, &MainWindow::updateImage);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::loadImage);

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

    QImage hist = histogram(modImage);
    ui->histogramLabel->setPixmap(QPixmap::fromImage(hist));

    QImage histR = histogramR(modImage);
    ui->histogramR->setPixmap(QPixmap::fromImage(histR));

    QImage histG = histogramG(modImage);
    ui->histogramG->setPixmap(QPixmap::fromImage(histG));

    QImage histB = histogramB(modImage);
    ui->histogramB->setPixmap(QPixmap::fromImage(histB));

}
void MainWindow::updateImage(){
    if(originalImage.isNull()){
        return;
    }

    int brightness = ui->brigtnessSlider->value();
    int contrast = ui->contrastSlider->value();
    double gamma = ui->gammaSlider->value() / 100.0f; //byc moze na float;

    modImage = originalImage.copy();

    modImage = adjustBrightness(modImage, brightness);
    modImage = adjustContrast(modImage, contrast);
    modImage = adjustGamma(modImage, gamma);

    ui->imageLabel->setPixmap(QPixmap::fromImage(modImage));

    QImage hist = histogram(modImage);
    ui->histogramLabel->setPixmap(QPixmap::fromImage(hist));


    QImage histR = histogramR(modImage);
    ui->histogramR->setPixmap(QPixmap::fromImage(histR));

    QImage histG = histogramG(modImage);
    ui->histogramG->setPixmap(QPixmap::fromImage(histG));

    QImage histB = histogramB(modImage);
    ui->histogramB->setPixmap(QPixmap::fromImage(histB));


}


QImage MainWindow::adjustBrightness(const QImage &image, int brightness){
    QImage modImg = image.copy();

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QColor color = image.pixelColor(x, y);

            int r = std::clamp(color.red() + brightness, 0, 255);
            int g = std::clamp(color.green() + brightness, 0, 255);
            int b = std::clamp(color.blue() + brightness, 0, 255);

            modImg.setPixelColor(x, y, QColor(r,g,b));
        }
    }

    return modImg;
}



QImage MainWindow::adjustContrast(const QImage &image, int contrast){
    QImage modImg = image;

    //wspolczynnik kontrastu niby przez kogos z apple
    double factor = (259.0 * (contrast + 255)) / (255*(259-contrast));
    //roznica kolorow wzgledem srodka

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QColor color = image.pixelColor(x, y);

            int r = std::clamp(static_cast<int>(factor * (color.red() - 128) + 128), 0, 255);
            int g = std::clamp(static_cast<int>(factor * (color.green() - 128) + 128), 0, 255);
            int b = std::clamp(static_cast<int>(factor * (color.blue() - 128) + 128), 0, 255);

            modImg.setPixelColor(x, y, QColor(r,g,b));
        }
    }

    return modImg;
}




QImage MainWindow::adjustGamma(const QImage &image, double gamma){
    QImage modImg = image;

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QColor color = image.pixelColor(x, y);

            int r = std::clamp(static_cast<int>(std::pow(color.red()/255.0, gamma) * 255), 0, 255);
            int g = std::clamp(static_cast<int>(std::pow(color.green()/255.0, gamma) * 255), 0, 255);
            int b = std::clamp(static_cast<int>(std::pow(color.blue()/255.0, gamma) * 255), 0, 255);

            modImg.setPixelColor(x, y, QColor(r,g,b));
        }
    }

    return modImg;
}

QImage MainWindow::histogram(const QImage& image){
    std::array<int, 256> columns = {0};

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QColor color = image.pixelColor(x,y);
            int gray = qGray(color.rgb());
            columns[gray]++;
        }
    }

    int maxCol = *std::max_element(columns.begin(), columns.end());

    QImage histImage(256, 256, QImage::Format_RGB32);
    histImage.fill(Qt::white);
    QPainter painter(&histImage);
    painter.setPen(Qt::black);

    for (int i = 0; i < 256; ++i) {
        int height = static_cast<int>((columns[i]/ static_cast<double>(maxCol)) * 256);
        painter.drawLine(i, 256, i, 256 - height);
    }

    return histImage;

}



QImage MainWindow::histogramR(const QImage& image){
    std::array<int, 256> columns = {0};

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QColor color = image.pixelColor(x,y);
            int red = color.red();
            columns[red]++;
        }
    }

    int maxCol = *std::max_element(columns.begin(), columns.end());

    QImage histImage(256, 256, QImage::Format_RGB32);
    histImage.fill(Qt::white);
    QPainter painter(&histImage);
    painter.setPen(Qt::black);

    for (int i = 0; i < 256; ++i) {
        int height = static_cast<int>((columns[i]/ static_cast<double>(maxCol)) * 256);
        painter.drawLine(i, 256, i, 256 - height);
    }

    return histImage;

}



QImage MainWindow::histogramG(const QImage& image){
    std::array<int, 256> columns = {0};

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QColor color = image.pixelColor(x,y);
            int green = color.green();
            columns[green]++;
        }
    }

    int maxCol = *std::max_element(columns.begin(), columns.end());

    QImage histImage(256, 256, QImage::Format_RGB32);
    histImage.fill(Qt::white);
    QPainter painter(&histImage);
    painter.setPen(Qt::black);

    for (int i = 0; i < 256; ++i) {
        int height = static_cast<int>((columns[i]/ static_cast<double>(maxCol)) * 256);
        painter.drawLine(i, 256, i, 256 - height);
    }

    return histImage;

}


QImage MainWindow::histogramB(const QImage& image){
    std::array<int, 256> columns = {0};

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QColor color = image.pixelColor(x,y);
            int blue = color.blue();
            columns[blue]++;
        }
    }

    int maxCol = *std::max_element(columns.begin(), columns.end());

    QImage histImage(256, 256, QImage::Format_RGB32);
    histImage.fill(Qt::white);
    QPainter painter(&histImage);
    painter.setPen(Qt::black);

    for (int i = 0; i < 256; ++i) {
        int height = static_cast<int>((columns[i]/ static_cast<double>(maxCol)) * 256);
        painter.drawLine(i, 256, i, 256 - height);
    }

    return histImage;

}









