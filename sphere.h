#ifndef SPHERE_H
#define SPHERE_H

#include <QMatrix4x4>
#include <QWidget>


class Sphere : public QWidget {
    Q_OBJECT
public:
    explicit Sphere(const QString& texturePath, int width, int height, QWidget* parent = nullptr);
    ~Sphere();

public slots:
    void setRotation(int angle);
    void setViewAngle(float angle);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    int rotationAngle;
    float viewAngle;
    QImage texture;
};

#endif
