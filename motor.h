#ifndef MOTOR_H
#define MOTOR_H

#include <QObject>

class Motor : public QObject
{
    Q_OBJECT
public:
    explicit Motor(QObject *parent = nullptr);

private:
    int InitGPIB();
    int SendStringGPIB(char *x);
    int userDescriptor;

signals:
    void GPIBconnected();
    void GPIB_err_connection();

public slots:
    void perform_motion();
};

#endif // MOTOR_H
