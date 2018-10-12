#ifndef MISURA_H
#define MISURA_H

#include <QWidget>

class Misura : public QObject
{
    Q_OBJECT
public:
    explicit Misura();
    ~Misura();



signals:
    void update_progress();
    void start();
    void finished();
    void error(QString err);


public slots:
    void perform_measure();
};

#endif // MISURA_H
