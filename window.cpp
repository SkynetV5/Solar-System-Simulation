#include "window.h"
#include "sphere.h"
#include <QVBoxLayout>
#include <QSlider>


Window::Window(QWidget* parent)  : QWidget(parent) {
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    Sphere* earthWidget = new Sphere(this);
    layout->addWidget(earthWidget);

    QSlider* rotationSlider = new QSlider(Qt::Horizontal, this);
    rotationSlider->setRange(0, 360);
    rotationSlider->setValue(0);
    layout->addWidget(rotationSlider);

    connect(rotationSlider, &QSlider::valueChanged,
            earthWidget, &Sphere::setRotation);

    resize(600, 500);
}

Window::~Window() {

}
