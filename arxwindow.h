#ifndef ARXWINDOW_H
#define ARXWINDOW_H

#include <QDialog>
#include "app.h"
namespace Ui {
class ARXwindow;
}

class ARXwindow : public QDialog
{
    Q_OBJECT

public:
    explicit ARXwindow(QWidget *parent = nullptr,App* appPtr = nullptr);
    ~ARXwindow();

private slots:
    void on_btnUstaw_clicked();

    void on_btnRestart_clicked();

private:
    Ui::ARXwindow *ui;
    App* app;
    void setData();
};

#endif // ARXWINDOW_H
