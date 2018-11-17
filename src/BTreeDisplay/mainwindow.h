#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "BTree.h"
#include "BTree.cpp"
#include <QMainWindow>
#include <QPainter>
#include <Qrect>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    BTree<int> *tree;
    list<BTree<int>*>tree_list;
    QRectF demo_rec;

private:
    int change=0;
    Ui::MainWindow *ui;
    void updateRec();
    void setItems();
    QRect scaledTo(QRect rec1,QPointF p1,QPointF p2);
    QPointF S;

protected:
    void paintEvent(QPaintEvent*);
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_button_pre_clicked();
    void on_button_next_clicked();
    void on_button_exit_clicked();
    void on_groupBox_2_clicked();
    void on_pushButton_5_clicked();
};

#endif // MAINWINDOW_H
