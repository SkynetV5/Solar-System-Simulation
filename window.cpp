#include "window.h"
#include "sphere.h"
#include <QVBoxLayout>
#include <QSlider>
#include <QHBoxLayout>
#include <QPainter>

Window::Window(QWidget* parent) : QWidget(parent) {
    setWindowTitle("Układ Słoneczny");
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Tworzymy poziomy układ planet
    QHBoxLayout* planetsLayout = new QHBoxLayout();

    // Tworzenie Słońca
    Sphere* sunWidget = new Sphere("C:/Users/SKYNET/Desktop/SKYNET/Solar-System-Simulation/Assets/sun.jpg", 250, 250, this);
    planetsLayout->addWidget(sunWidget);

    // Tworzenie planet
    struct PlanetData {
        QString texturePath;
        int size;
    };

    std::vector<PlanetData> planets = {
        {"C:/Users/SKYNET/Desktop/SKYNET/Solar-System-Simulation/Assets/mercury.jpg", 40},
        {"C:/Users/SKYNET/Desktop/SKYNET/Solar-System-Simulation/Assets/wenus.jpg", 60},
        {"C:/Users/SKYNET/Desktop/SKYNET/Solar-System-Simulation/Assets/earth01.jpg", 75},
        {"C:/Users/SKYNET/Desktop/SKYNET/Solar-System-Simulation/Assets/mars.jpg", 65},
        {"C:/Users/SKYNET/Desktop/SKYNET/Solar-System-Simulation/Assets/jupiter.jpg", 150},
        {"C:/Users/SKYNET/Desktop/SKYNET/Solar-System-Simulation/Assets/saturn.jpg", 140},
        {"C:/Users/SKYNET/Desktop/SKYNET/Solar-System-Simulation/Assets/uran.jpg", 100},
        {"C:/Users/SKYNET/Desktop/SKYNET/Solar-System-Simulation/Assets/neptune.jpg", 90}
    };

    std::vector<Sphere*> planetWidgets;
    for (const auto& planet : planets) {
        Sphere* planetWidget = new Sphere(planet.texturePath, planet.size, planet.size, this);
        planetsLayout->addWidget(planetWidget);
        planetWidgets.push_back(planetWidget);
    }

    mainLayout->addLayout(planetsLayout);

    // Slider do obracania planet
    QSlider* rotationSlider = new QSlider(Qt::Horizontal, this);
    rotationSlider->setRange(0, 360);
    rotationSlider->setValue(0);
    mainLayout->addWidget(rotationSlider);

    for (Sphere* planet : planetWidgets) {
        connect(rotationSlider, &QSlider::valueChanged, planet, &Sphere::setRotation);
    }

    resize(1200, 600);
}

void Window::paintEvent(QPaintEvent* event) {
    // Tworzymy obiekt QPixmap, aby załadować teksturę
    QPixmap background("C:/Users/SKYNET/Desktop/SKYNET/Solar-System-Simulation/Assets/stars.jpg");

    // Tworzymy obiekt QPainter do rysowania
    QPainter painter(this);
    painter.drawPixmap(0, 0, width(), height(), background);  // Rysowanie tła

    QWidget::paintEvent(event);  // Wywołanie domyślnego paintEvent
}

Window::~Window() {
}

