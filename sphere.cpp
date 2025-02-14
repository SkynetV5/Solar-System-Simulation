#include "sphere.h"
#include <cmath>
#include <QPainter>
#include <QVector3D>
#include <QPainterPath>

Sphere::Sphere(const QString& texturePath, int width, int height, QWidget* parent)
    : QWidget(parent), rotationAngle(0), texture(texturePath) {
    setMinimumSize(width, height);
    setMouseTracking(true);

}

void Sphere::setRotation(int angle) {
    rotationAngle = angle;
    update();
}

void Sphere::setViewAngle(float angle) {
    viewAngle = angle;
    update();
}

void Sphere::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    int centerX = width() / 2;
    int centerY = height() / 2;
    int radius = qMin(width(), height()) / 4;

    if (texture.isNull()) {
        painter.setPen(Qt::red);
        painter.drawText(centerX - 50, centerY, "Brak tekstury!");
        return;
    }

    QMatrix4x4 projection;
    projection.perspective(60.0f, 1.0f, 0.1f, 1000.0f);

    QMatrix4x4 view;
    view.translate(0, 0, -400);
    view.rotate(viewAngle, 1, 0, 0);

    QMatrix4x4 model;
    model.rotate(rotationAngle, 0, 1, 0);

    const int numLatitudes = 80;
    const int numLongitudes = 160;

    // Specjalne traktowanie dla obszarów biegunowych
    const float polarThreshold = 0.95f;  // Próg dla obszarów biegunowych

    for (int i = 0; i < numLatitudes; ++i) {
        float phi1 = M_PI * float(i) / float(numLatitudes);
        float phi2 = M_PI * float(i + 1) / float(numLatitudes);

        // Obliczanie współczynników dla biegunów
        float cosPhiTop = cos(phi1);
        float cosPhiBottom = cos(phi2);
        bool isNearNorthPole = cosPhiTop > polarThreshold;
        bool isNearSouthPole = cosPhiBottom < -polarThreshold;

        for (int j = 0; j < numLongitudes; ++j) {
            float theta1 = 2.0f * M_PI * float(j) / float(numLongitudes);
            float theta2 = 2.0f * M_PI * float(j + 1) / float(numLongitudes);

            QVector3D p1(
                radius * sin(phi1) * cos(theta1),
                radius * cos(phi1),
                radius * sin(phi1) * sin(theta1)
                );
            QVector3D p2(
                radius * sin(phi1) * cos(theta2),
                radius * cos(phi1),
                radius * sin(phi1) * sin(theta2)
                );
            QVector3D p3(
                radius * sin(phi2) * cos(theta2),
                radius * cos(phi2),
                radius * sin(phi2) * sin(theta2)
                );
            QVector3D p4(
                radius * sin(phi2) * cos(theta1),
                radius * cos(phi2),
                radius * sin(phi2) * sin(theta1)
                );

            QVector3D normal = QVector3D::crossProduct(p2 - p1, p3 - p1).normalized();
            QVector3D transformedNormal = (model * QVector4D(normal, 0.0f)).toVector3D().normalized();
            float visibility = -QVector3D::dotProduct(transformedNormal, QVector3D(0, 0, 1));

            if (visibility > 0.0f) {
                // Modyfikacja współrzędnych tekstury dla obszarów biegunowych
                float u1 = float(j) / float(numLongitudes);
                float u2 = float(j + 1) / float(numLongitudes);
                float v1 = float(i) / float(numLatitudes);
                float v2 = float(i + 1) / float(numLatitudes);

                // Korekta współrzędnych tekstury dla biegunów
                if (isNearNorthPole) {
                    v1 = 0.0f;  // Górny biegun
                } else if (isNearSouthPole) {
                    v2 = 1.0f;  // Dolny biegun
                }

                // Dodanie małego marginesu do tekstury
                float marginU = 1.0f / texture.width();
                float marginV = 1.0f / texture.height();

                // Rozszerzone mapowanie tekstury dla biegunów
                int texX = qMax(0, int((u1 - marginU) * texture.width()));
                int texY = qMax(0, int((v1 - marginV) * texture.height()));
                int texW = qMin(texture.width() - texX, int((u2 - u1 + 2 * marginU) * texture.width()));
                int texH = qMin(texture.height() - texY, int((v2 - v1 + 2 * marginV) * texture.height()));

                QImage fragment = texture.copy(texX, texY, texW, texH);

                QVector4D tp1 = projection * view * model * QVector4D(p1, 1.0f);
                QVector4D tp2 = projection * view * model * QVector4D(p2, 1.0f);
                QVector4D tp3 = projection * view * model * QVector4D(p3, 1.0f);
                QVector4D tp4 = projection * view * model * QVector4D(p4, 1.0f);

                if (tp1.w() != 0.0f) tp1 /= tp1.w();
                if (tp2.w() != 0.0f) tp2 /= tp2.w();
                if (tp3.w() != 0.0f) tp3 /= tp3.w();
                if (tp4.w() != 0.0f) tp4 /= tp4.w();

                float scale = radius * 4.5f;
                QPointF sp1(centerX + tp1.x() * scale, centerY - tp1.y() * scale);
                QPointF sp2(centerX + tp2.x() * scale, centerY - tp2.y() * scale);
                QPointF sp3(centerX + tp3.x() * scale, centerY - tp3.y() * scale);
                QPointF sp4(centerX + tp4.x() * scale, centerY - tp4.y() * scale);

                QTransform transform;
                QPolygonF sourcePolygon;
                sourcePolygon << QPointF(0, 0)
                              << QPointF(fragment.width(), 0)
                              << QPointF(fragment.width(), fragment.height())
                              << QPointF(0, fragment.height());

                QPolygonF targetPolygon;
                targetPolygon << sp1 << sp2 << sp3 << sp4;

                if (QTransform::quadToQuad(sourcePolygon, targetPolygon, transform)) {
                    painter.setTransform(transform);
                    painter.drawImage(QPoint(0, 0), fragment);
                    painter.resetTransform();
                }
            }
        }
    }
}

Sphere::~Sphere() {}
