#include "sphere.h"
#include <cmath>
#include <QPainter>
#include <QVector3D>

Sphere::Sphere(QWidget* parent)
    : QWidget(parent), rotationAngle(0), texture("C:/Users/SKYNET/Desktop/SKYNET/Solar-System-Simulation/Assets/earth.jpg") {
    setMinimumSize(400, 400);
    setMouseTracking(true);
    texture = texture.mirrored();
}

void Sphere::setRotation(int angle) {
    rotationAngle = angle;
    update();
}

void Sphere::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int centerX = width() / 2;
    int centerY = height() / 2;
    int radius = qMin(width(), height()) / 3;

    int numLatitudes = 300;  // Liczba równoleżników
    int numLongitudes = 300; // Liczba południków

    painter.setPen(Qt::blue);

    if (texture.isNull()) {
        painter.setPen(Qt::red);
        painter.drawText(centerX - 50, centerY, "Brak tekstury!");
        return;
    }

    for (int i = 0; i < numLatitudes; ++i) {
        float lat1 = M_PI * float(i) / float(numLatitudes) - M_PI / 2;
        float lat2 = M_PI * float(i + 1) / float(numLatitudes) - M_PI / 2;

        for (int j = 0; j < numLongitudes; ++j) {
            float lon1 = 2 * M_PI * float(j) / float(numLongitudes);
            float lon2 = 2 * M_PI * float(j + 1) / float(numLongitudes);

            // Punkty sferyczne (4 na każdy fragment siatki)
            QVector3D p1(radius * cos(lat1) * cos(lon1), radius * sin(lat1), radius * cos(lat1) * sin(lon1));
            QVector3D p2(radius * cos(lat1) * cos(lon2), radius * sin(lat1),  radius * cos(lat1) * sin(lon2));
            QVector3D p3(radius * cos(lat2) * cos(lon2), radius * sin(lat2),radius * cos(lat2) * sin(lon2));
            QVector3D p4(radius * cos(lat2) * cos(lon1), radius * sin(lat2),radius * cos(lat2) * sin(lon1));


            // Mapowanie tekstury
            QPointF t1(float(j) / numLongitudes * texture.width(), float(i) / numLatitudes * texture.height());
            QPointF t2(float(j + 1) / numLongitudes * texture.width(), float(i) / numLatitudes * texture.height());
            QPointF t3(float(j + 1) / numLongitudes * texture.width(), float(i + 1) / numLatitudes * texture.height());
            QPointF t4(float(j) / numLongitudes * texture.width(), float(i + 1) / numLatitudes * texture.height());



            // Transformacja i rysowanie
            QMatrix4x4 matrix;
            matrix.rotate(rotationAngle, 0, 1, 0);

            QVector4D vec1 = matrix * QVector4D(p1, 1.0);
            QVector4D vec2 = matrix * QVector4D(p2, 1.0);
            QVector4D vec3 = matrix * QVector4D(p3, 1.0);
            QVector4D vec4 = matrix * QVector4D(p4, 1.0);


            float scaleFactor = 1; // Dostosuj tę wartość, aby uzyskać odpowiednie dopasowanie
            vec1.setX(vec1.x() * scaleFactor);
            vec1.setY(vec1.y() * scaleFactor);

            vec2.setX(vec2.x() * scaleFactor);
            vec2.setY(vec2.y() * scaleFactor);

            vec3.setX(vec3.x() * scaleFactor);
            vec3.setY(vec3.y() * scaleFactor);

            vec4.setX(vec4.x() * scaleFactor);
            vec4.setY(vec4.y() * scaleFactor);

            QVector<QPointF> polygon = {
                QPointF(centerX + vec1.x(), centerY - vec1.y()),
                QPointF(centerX + vec2.x(), centerY - vec2.y()),
                QPointF(centerX + vec3.x(), centerY - vec3.y()),
                QPointF(centerX + vec4.x(), centerY - vec4.y())
            };

            // Rysowanie fragmentu z teksturą
            painter.drawImage(QRectF(polygon[0], polygon[2]), texture, QRectF(t1, t3));
        }
    }
}

Sphere::~Sphere() {}
