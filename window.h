#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include "sphere.h"
#include <QSlider>
#include <QTimer>
#include <cmath>

class Window : public QWidget {
    Q_OBJECT

public:
    explicit Window(QWidget *parent = nullptr);
    ~Window();

protected:
    void paintEvent(QPaintEvent* event) override;


private:
    Sphere* earthWidget;
    float animationAngle;
    QSlider *viewAngleSlider;
    QTimer* animationTimer;
    std::vector<Sphere*> planetWidgets;
    std::vector<double> orbitalPeriods;
    std::vector<double> orbitalSpeeds;
    std::vector<double> rotationSpeeds;
    std::vector<double> orbitalAngles;
    std::vector<int> orbitalDistances;
    QWidget* solarSystemContainer;
    Sphere* sunWidget;
    double sunRotationSpeed = 13.3;
    void updatePlanetPositions();
    void drawOrbits(QPainter& painter);



};

#endif // WINDOW_H
