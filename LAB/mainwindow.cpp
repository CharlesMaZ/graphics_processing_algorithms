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

    ui->LSlider->setMinimum(-50);
    ui->LSlider->setMaximum(50);
    ui->LSlider->setValue(0);

    ui->ASlider->setMinimum(-50);
    ui->ASlider->setMaximum(50);
    ui->ASlider->setValue(0);

    ui->BSlider->setMinimum(-50);
    ui->BSlider->setMaximum(50);
    ui->BSlider->setValue(0);

    connect(ui->LSlider, &QSlider::valueChanged, this, &MainWindow::updateImage);
    connect(ui->ASlider, &QSlider::valueChanged, this, &MainWindow::updateImage);
    connect(ui->BSlider, &QSlider::valueChanged, this, &MainWindow::updateImage);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::loadImage);

}

MainWindow::~MainWindow()
{
    delete ui;
}

// void MainWindows::loadImage(){
//     QString filePath = QFileDialog::getOpenFileName(this, )
// }
void MainWindow::loadImage() {
    QString filePath = QFileDialog::getOpenFileName(this, "Otwórz obraz", "", "Images (*.png *.jpg *.bmp)");
    if (!filePath.isEmpty()) {
        originalImage.load(filePath);
        modImage = originalImage;
        ui->imageLabel->setPixmap(QPixmap::fromImage(originalImage));
    }
}


void MainWindow::updateImage(){
    if(originalImage.isNull()){
        return;
    }

    int deltaL = ui->LSlider->value();
    int deltaA = ui->ASlider->value();
    int deltaB = ui->BSlider->value();

    modImage = originalImage.copy();

    for (int y = 0; y < modImage.height(); ++y) {
        for (int x = 0; x < modImage.width(); ++x) {
            QColor color = QColor::fromRgb(modImage.pixel(x,y));

            XYZ xyz;
            LAB lab;
            rgb2xyz(color, xyz);
            xyz2lab(xyz, lab);

            //qbound - zwraca min jeśli val < mi i odwroenie

            lab.L = qBound(0.0f, lab.L + deltaL, 100.0f);
            lab.A = qBound(-128.0f, lab.A + deltaA, 127.0f);
            lab.B = qBound(-128.0f, lab.B + deltaB, 127.0f);

            lab2xyz(lab, xyz);
            QColor newColor = xyz2rgb(xyz, color);
            xyz2rgb(xyz, newColor);

            modImage.setPixel(x, y, qRgb(newColor.red(), newColor.green(), newColor.blue()));


            // int r = qBound(0, newColor.red(), 255);
            // int g = qBound(0, newColor.green(), 255);
            // int b = qBound(0, newColor.blue(), 255);

            // modImage.setPixel(x,y, qRgb(r,g,b));
        }
    }


    ui->imageLabel->setPixmap(QPixmap::fromImage(modImage));

    //3 suwaki LAB które działają rze


}
// XYZ rgb2xyz(QColor rgb, XYZ &xyz);
// QColor xyz2rgb(XYZ xyz, QColor &rgb);
// LAB xyz2lab(XYZ xyz, LAB &lab);
// XYZ lab2xyz(LAB lab, XYZ &xyz);

MainWindow::XYZ MainWindow::rgb2xyz(QColor rgb, XYZ &xyz){

    double r = pow(rgb.red() / 255.0, 2.2);
    double g = pow(rgb.green() / 255.0, 2.2);
    double b = pow(rgb.blue() / 255.0, 2.2);

    xyz.X = r * 0.4124564 + g * 0.3575761 + b * 0.1804375;
    xyz.Y = r * 0.2126729 + g * 0.7151522 + b * 0.0721750;
    xyz.Z = r * 0.0193339 + g * 0.1191920 + b * 0.9503041;

    return xyz;

}

QColor MainWindow::xyz2rgb(XYZ xyz, QColor &rgb){
    double r = xyz.X * 3.2404542 + xyz.Y * -1.5371385 + xyz.Z * -0.4985314;
    double g = xyz.X * -0.9692660 + xyz.Y * 1.8760108 + xyz.Z * 0.0415560;
    double b = xyz.X * 0.0556434 + xyz.Y * -0.2040259 + xyz.Z * 1.0572252;

    //clamp 0-1
    r = qBound(0.0, r, 1.0);
    g = qBound(0.0, g, 1.0);
    b = qBound(0.0, b, 1.0);

    rgb.setRed((int)(r * 255));
    rgb.setGreen((int)(g * 255));
    rgb.setBlue((int)(b * 255));

    return rgb;
}

static float f(float t){
    const float delta = 6.0f / 29.0f;
    if(t > delta * delta * delta){
        return cbrtf(t);
    }
    else{
        return t/(3*delta*delta) +4.0f / 29.0f;
    }
}

MainWindow::LAB MainWindow::xyz2lab(XYZ xyz, LAB &lab){
    //D65
    //gamma = 1
    float x0 = 0.95047f;
    float y0 = 1.00000f;
    float z0 = 1.08883f;

    //do uniknięcia nielinowości przy małych wartościach funkca pomocnicza
    float fx = f(xyz.X / x0);
    float fy = f(xyz.Y / y0);
    float fz = f(xyz.Z / z0);

    lab.L = 116* fy - 16.0f;
    lab.A = 500*(fx - fy);
    lab.B = 200*(fy - fz);

    // lab.L = 116*cbrt(xyz.Y/y0) - 16.0f;
    // lab.A = 500*(cbrt(xyz.X/x0) - cbrt(xyz.Y/y0));
    // lab.B = 200*(cbrt(xyz.Y/y0) - cbrt(xyz.Z/z0));

    return lab;
}

static float flab2xyz(float t){
    const float delta = 6.0f / 29.0f;
    if (t > delta){
        return t * t * t;
    }
    else{
        return 3.0f * delta * delta * (t - 4.0f / 29.0f);
    }
}

MainWindow::XYZ MainWindow::lab2xyz(LAB lab, XYZ &xyz) {
    float x0 = 0.95047f;
    float y0 = 1.00000f;
    float z0 = 1.08883f;

    float fy = (lab.L + 16.0f) / 116.0f;
    float fx = fy + lab.A / 500.0f;
    float fz = fy - lab.B / 200.0f;

    xyz.X = flab2xyz(fx) * x0;
    xyz.Y = flab2xyz(fy) * y0;
    xyz.Z = flab2xyz(fz) * z0;

    return xyz;
}




