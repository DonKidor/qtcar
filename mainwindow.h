#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MyScene;
class MyCar;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    MyScene *scene;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
