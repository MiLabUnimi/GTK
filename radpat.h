#ifndef RADPAT_H
#define RADPAT_H

#include <QMainWindow>

namespace Ui {
class RadPat;
}

class RadPat : public QMainWindow
{
    Q_OBJECT

public:
    explicit RadPat(QWidget *parent = 0);
    ~RadPat();

private:
    Ui::RadPat *ui;
};

#endif // RADPAT_H
