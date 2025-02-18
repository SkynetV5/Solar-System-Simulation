#ifndef SPHERE_H
#define SPHERE_H

#include <QMatrix4x4>
#include <QWidget>
#include <QTimer>


class Sphere : public QWidget {
    Q_OBJECT
public:
    explicit Sphere(const QString& texturePath, int width, int height, QWidget* parent = nullptr);
    ~Sphere();

public slots:
    void setRotation(float angle);
    void setViewAngle(float angle);
    float rotation() const { return rotationAngle; }

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    float rotationAngle;
    float viewAngle;
    QImage texture;
};

#endif
