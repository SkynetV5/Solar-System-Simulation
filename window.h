#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include "sphere.h"

class Window : public QWidget {
    Q_OBJECT

public:
    explicit Window(QWidget *parent = nullptr);
    ~Window();


private:
    Sphere* earthWidget;

};

#endif // WINDOW_H
