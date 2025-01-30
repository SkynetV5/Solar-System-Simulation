#ifndef SPHERE_H
#define SPHERE_H

#include <QMatrix4x4>
#include <QWidget>


class Sphere : public QWidget {
    Q_OBJECT
public:
    explicit Sphere(QWidget* parent = nullptr);
    ~Sphere();

public slots:
    void setRotation(int angle);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    int rotationAngle;
    QImage texture;
};

#endif
