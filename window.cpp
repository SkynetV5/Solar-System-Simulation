#include "window.h"
#include "sphere.h"
#include <QVBoxLayout>
#include <QSlider>
#include <QHBoxLayout>
#include <QPainter>
#include <QLabel>
#include <QDebug>



Window::Window(QWidget* parent) : QWidget(parent) {
    setWindowTitle("Układ Słoneczny");

    // Tworzymy główny layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);


    // Kontener na układ planet
    QWidget* solarSystemContainer = new QWidget(this);
    solarSystemContainer->setFixedSize(1500, 700); // Ustawiamy stały rozmiar kontenera
    mainLayout->addWidget(solarSystemContainer);

    // Pozycja środka (Słońca)
    const int centerX = solarSystemContainer->width() / 2 + 10;
    const int centerY = solarSystemContainer->height() / 2 + 10;

    // Tworzenie Słońca
    sunWidget = new Sphere("C:/Users/SKYNET/Desktop/SKYNET/Solar-System-Simulation/Assets/sun.jpg", 180, 180, solarSystemContainer);
    sunWidget->move(centerX -90, centerY -65); // Centrujemy Słońce

    // Definicja planet
    struct PlanetData {
        QString texturePath;
        int size;
        int spacing;
    };

    std::vector<PlanetData> planets = {
        {"C:/Users/SKYNET/Desktop/SKYNET/Solar-System-Simulation/Assets/mercury.jpg", 40, 40},
        {"C:/Users/SKYNET/Desktop/SKYNET/Solar-System-Simulation/Assets/wenus.jpg", 60, 55},
        {"C:/Users/SKYNET/Desktop/SKYNET/Solar-System-Simulation/Assets/earth01.jpg", 75, 50},
        {"C:/Users/SKYNET/Desktop/SKYNET/Solar-System-Simulation/Assets/mars.jpg", 65, 70},
        {"C:/Users/SKYNET/Desktop/SKYNET/Solar-System-Simulation/Assets/jupiter.jpg", 100, 80},
        {"C:/Users/SKYNET/Desktop/SKYNET/Solar-System-Simulation/Assets/saturn.jpg", 95, 70},
        {"C:/Users/SKYNET/Desktop/SKYNET/Solar-System-Simulation/Assets/uran.jpg", 80, 70},
        {"C:/Users/SKYNET/Desktop/SKYNET/Solar-System-Simulation/Assets/neptune.jpg", 80, 70}
    };

    // Pozycja startowa dla pierwszej planety (na prawo od Słońca)
    int currentX = centerX + 70; // Odległość pierwszej planety od Słońca

    orbitalSpeeds = {
        2.0,    // Mercury - najszybszy
        1.5,    // Venus
        1.0,    // Earth
        0.8,    // Mars
        0.4,    // Jupiter
        0.3,    // Saturn
        0.2,    // Uranus
        0.1     // Neptune - najwolniejszy
    };

    rotationSpeeds = {
        0.3,    // Mercury
        -0.2,   // Venus (ujemna wartość dla ruchu wstecznego)
        15.0,   // Earth
        14.6,   // Mars
        36.4,   // Jupiter
        33.6,   // Saturn
        -20.9,  // Uranus (ujemna wartość dla ruchu wstecznego)
        22.4    // Neptune
    };;

    orbitalDistances = {
        140,  // Mercury
        180,  // Venus
        230,  // Earth
        280,  // Mars
        350,  // Jupiter
        430,  // Saturn
        500,  // Uranus
        570   // Neptune
    };

    orbitalAngles.resize(planets.size(), 0.0);



    // Tworzenie i pozycjonowanie planet
    for (const auto& planet : planets) {
        Sphere* planetWidget = new Sphere(planet.texturePath, planet.size, planet.size, solarSystemContainer);

        float viewAngle = -10.0f * M_PI / 180.0f;  // 30 stopni w radianach
        int baseY = centerY - (planet.size / 2);
        int depthOffset = static_cast<int>(currentX * sin(viewAngle) * 0.2f);
        int planetY = baseY - depthOffset;

        // Ustawianie pozycji planety
        planetWidget->move(currentX, planetY);

        // Przesuwamy pozycję X dla następnej planety
        currentX += planet.spacing;

        planetWidgets.push_back(planetWidget);
    }

    // Create animation timer
    animationTimer = new QTimer(this);
    animationTimer->setInterval(16); // ~60 FPS
    connect(animationTimer, &QTimer::timeout, this, &Window::updatePlanetPositions);
    animationTimer->start();


    // QHBoxLayout* controlLayout = new QHBoxLayout();

    // // Rotation control
    // QVBoxLayout* rotationControl = new QVBoxLayout();
    // QLabel* rotationLabel = new QLabel("Rotation:", this);
    // QSlider* rotationSlider = new QSlider(Qt::Horizontal, this);
    // rotationSlider->setRange(0, 360);
    // rotationSlider->setValue(0);
    // rotationControl->addWidget(rotationLabel);
    // rotationControl->addWidget(rotationSlider);

    // // View angle control
    // QVBoxLayout* viewControl = new QVBoxLayout();
    // QLabel* viewLabel = new QLabel("View Angle:", this);
    // QSlider *viewAngleSlider = new QSlider(Qt::Horizontal, this);
    // viewAngleSlider->setRange(-75, 75);  // Limit to 75 degrees to prevent extreme angles
    // viewAngleSlider->setValue(0);  // Start at 30 degrees
    // viewControl->addWidget(viewLabel);
    // viewControl->addWidget(viewAngleSlider);

    // controlLayout->addLayout(rotationControl);
    // controlLayout->addLayout(viewControl);
    // mainLayout->addLayout(controlLayout);

    // // Podłączanie slidera do wszystkich planet
    // for (Sphere* planet : planetWidgets) {
    //     connect(rotationSlider, &QSlider::valueChanged, planet, &Sphere::setRotation);
    //     connect(viewAngleSlider, &QSlider::valueChanged, planet, &Sphere::setViewAngle);
    // }
    // connect(rotationSlider, &QSlider::valueChanged, sunWidget, &Sphere::setRotation);
    //  connect(viewAngleSlider, &QSlider::valueChanged, sunWidget, &Sphere::setViewAngle);

    resize(1500, 800);
}


void Window::updatePlanetPositions() {

    const int centerX = width() / 2;
    const int centerY = height() / 2;

    sunWidget->move(centerX -90, centerY -90);

    sunWidget->setRotation(sunWidget->rotation() + sunRotationSpeed);

    for (size_t i = 0; i < planetWidgets.size(); ++i) {

        orbitalAngles[i] +=  2.0f * orbitalSpeeds[i] * (M_PI / 180.0) ;
        if (orbitalAngles[i] >= 2.0 * M_PI) {
            orbitalAngles[i] -= 2.0 * M_PI;
        }

        planetWidgets[i]->setRotation(planetWidgets[i]->rotation() + rotationSpeeds[i]);

        float viewAngle = 5.0f * M_PI / 180.0f;
        int orbitX = centerX + orbitalDistances[i] * cos(orbitalAngles[i]);
        int orbitY = centerY + (orbitalDistances[i] * sin(orbitalAngles[i]) * cos(viewAngle));

        planetWidgets[i]->move(
            orbitX - planetWidgets[i]->width() / 2,
            orbitY - planetWidgets[i]->height() / 2
            );

    }

    update();
}

void Window::drawOrbits(QPainter& painter) {
    const int centerX = width() / 2 +10;
    const int centerY = height() / 2 +50;

    painter.setPen(QPen(QColor(100, 100, 100, 100), 1, Qt::DashLine));

    float viewAngle = 5.0f * M_PI / 180.0f;

    for (int distance : orbitalDistances) {
        // Draw elliptical orbit
        QRectF orbitRect(
            centerX - distance,
            centerY - distance * cos(viewAngle),
            distance * 2,
            distance * 2 * cos(viewAngle)
            );
        painter.drawEllipse(orbitRect);
    }
}

void Window::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    QPixmap background("C:/Users/SKYNET/Desktop/SKYNET/Solar-System-Simulation/Assets/stars.jpg");
    painter.drawPixmap(0, 0, width(), height(), background);

     drawOrbits(painter);
    QWidget::paintEvent(event);
}

Window::~Window() {
     delete animationTimer;
}
