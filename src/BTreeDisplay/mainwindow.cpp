#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QTime>
#include <Qt>
#include <QtGlobal>
#include <QLineF>
#include <QRectF>
#include <QString>
#include <QMessageBox>
#include <list>
#include <QTime>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    S=QPointF(width(),height());
    tree=NULL;  //B树初始化
   // resize(1250, 800);  //窗口初始化
    setWindowTitle(tr("B-Tree"));   //窗口标题初始化
    ui->spinBox->setValue(3);   //数值初始化
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));//随机数种子
    ui->radioButton->setChecked(1);
    setItems();
}

void MainWindow::setItems()
{
    demo_rec=QRectF(QPointF(width()*0.05,height()*0.05),QPointF(width()*0.7,height()*0.9));
    //图标初始化
    qreal a=min(width()*0.03,height()*0.05);
    QSize item_size=QSize(a,a);
    QSize item_size2=QSize(a*0.8,a*0.8);
    QIcon icon,icon2,icon3,icon4;
    icon.addFile(":/imgs/imgs/play.png");
    ui->pushButton->setIcon(icon);
    ui->pushButton->setFlat(true);
    ui->pushButton->setIconSize(item_size);
    icon2.addFile(":/imgs/imgs/pre.png");
    ui->button_pre->setIcon(icon2);
    ui->button_pre->setFlat(true);
    ui->button_pre->setIconSize(item_size);
    icon3.addFile(":/imgs/imgs/next.png");
    ui->button_next->setIcon(icon3);
    ui->button_next->setFlat(true);
    ui->button_next->setIconSize(item_size);
    icon4.addFile(":/imgs/imgs/exit.png");
    ui->button_exit->setIcon(icon4);
    ui->button_exit->setFlat(true);
    ui->button_exit->setIconSize(item_size2);
    //按钮位置变更
    if(!change)return;
    QPointF R=QPointF(width(),height());
    ui->button_next->setGeometry(this->scaledTo(ui->button_next->geometry(),R,S));
    ui->button_pre->setGeometry(this->scaledTo(ui->button_pre->geometry(),R,S));
    ui->pushButton->setGeometry(this->scaledTo(ui->pushButton->geometry(),R,S));
    ui->pushButton_2->setGeometry(this->scaledTo(ui->pushButton_2->geometry(),R,S));
    ui->pushButton_5->setGeometry(this->scaledTo(ui->pushButton_5->geometry(),R,S));
    ui->radioButton->setGeometry(this->scaledTo(ui->radioButton->geometry(),R,S));
    ui->radioButton_2->setGeometry(this->scaledTo(ui->radioButton_2->geometry(),R,S));
    ui->groupBox->setGeometry(this->scaledTo(ui->groupBox->geometry(),R,S));
    ui->groupBox_2->setGeometry(this->scaledTo(ui->groupBox_2->geometry(),R,S));
    ui->groupBox_3->setGeometry(this->scaledTo(ui->groupBox_3->geometry(),R,S));
    ui->spinBox->setGeometry(this->scaledTo(ui->spinBox->geometry(),R,S));
    ui->spinBox_2->setGeometry(this->scaledTo(ui->spinBox_2->geometry(),R,S));
    ui->label->setGeometry(this->scaledTo(ui->label->geometry(),R,S));
    ui->line->setGeometry(this->scaledTo(ui->line->geometry(),R,S));
    ui->button_exit->setGeometry(this->scaledTo(ui->button_exit->geometry(),R,S));
    S=R;
}


QRect MainWindow::scaledTo(QRect rec1, QPointF p2, QPointF p1)
{
    int w=(double)rec1.width()/((qreal)p1.x()/p2.x());
    int h=(double)rec1.height()/((qreal)p1.y()/p2.y());
    int x=(double)rec1.left()/((qreal)p1.x()/p2.x());
    int y=(double)rec1.top()/((qreal)p1.y()/p2.y());
    return QRect(x,y,w,h);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::paintEvent(QPaintEvent *)
{
    if(tree)tree->Painting(demo_rec.width(),demo_rec.height());
    QPainter painter(this);
    this->setItems();
   // painter.setBrush(QBrush(Qt::yellow));
    //painter.drawRect(demo_rec);
    if(!tree)return;
    qreal sx=demo_rec.left()+demo_rec.width()*0.02;
    qreal sy=demo_rec.top()+demo_rec.height()*0.02;
    painter.setPen(QPen(Qt::red,2));
    for(auto node:tree->v){
        QRectF rec=node->border.translated(sx,sy);
        painter.drawRect(rec);
        if(node!=tree->getRoot()){
            QLineF line=node->line.translated(sx,sy);
            painter.drawLine(line);
        }
    }
    int fontsize=20-2*(tree->height());
    QFont font("Frutiger LT 45 Light",fontsize,QFont::Bold);
    //设置字体的类型，大小，加粗，斜体
    //font.setLetterSpacing(QFont::AbsoluteSpacing);
    //设置间距
    painter.setFont(font);
    //添加字体
    painter.setPen(QPen(Qt::black, 2));

    for(auto node:tree->v){
        qreal i=0;
        QRectF rec=node->border.translated(sx,sy);
        qreal dx=rec.width()/node->size();

        for(auto num:node->keys){
            qreal x=rec.left()+i*dx;
            QRectF text_rec=QRectF(x,rec.top(),dx,rec.height());
            QString text=QString::number(num,10);
            painter.drawText(text_rec,Qt::AlignCenter,text);
            i+=1;
        }
    }
    this->setItems();
}

void MainWindow::on_pushButton_clicked()
{
    change=1;
    int k=ui->spinBox->value();
    if(k<=2||k>6){
        QMessageBox::warning(this,tr("警告"),tr("阶数过大或过小"));
        return;
    }
    if(tree==NULL)
        tree=new BTree<int>(k);
    else if(tree->getK()!=k){
        for(auto it:tree_list)
            delete it;
        tree_list.clear();
        tree=new BTree<int>(k);
    }

    if(tree_list.size()!=0){
        typename list<BTree<int>*>::iterator it=tree_list.begin();
        while(*it!=tree)it++;
        for(it++;it!=tree_list.end();){
            delete (*it);
            it=tree_list.erase(it);
        }
        BTree<int>*new_tree=new BTree<int>(*tree);
        tree=new_tree;
        tree_list.push_back(tree);
    }
    else{
        tree_list.push_back(tree);
    }
    int op=ui->radioButton->isChecked()?1:2;
    if(op==0)return;//无操作
    int val=ui->spinBox_2->value();
    if(op==1){//插入
        if(!tree->insert(val)){
            QMessageBox::information(this,"提示","键值已存在");
            return;
        }
    }
    else if(op==2){
        if(!tree->erase(val)){
            QMessageBox::information(this,"提示","键值不存在");
            return;
        }
    }
    tree->printTree();
    this->update();
}


void MainWindow::on_pushButton_2_clicked()
{
    this->ui->spinBox_2->setValue((int)(qrand()%100));
}

void MainWindow::on_pushButton_5_clicked()
{
    if(tree==NULL||tree->size()==0)return;
    int val=~(1<<31);
    do{
        tree->findNextNode(tree->getRoot(),qrand()%100,val);
    }while(val==~(1<<31));
    this->ui->spinBox_2->setValue(val);
}


void MainWindow::on_button_pre_clicked()
{
    if(!tree)return;
    typename list<BTree<int>*>::reverse_iterator it=tree_list.rbegin();
    for(;it!=tree_list.rend();++it){
        if(*it==tree)break;
    }
    ++it;
    if(it!=tree_list.rend())tree=*it;
    ui->spinBox_2->setValue((*tree->op.rbegin()).second);
    this->update();
}

void MainWindow::on_button_next_clicked()
{
    if(!tree)return;
    typename list<BTree<int>*>::iterator it=tree_list.begin();
    for(;it!=tree_list.end();++it){
        if(*it==tree)break;
    }
    ++it;
    if(it!=tree_list.end())tree=*it;
    ui->spinBox_2->setValue((*tree->op.rbegin()).second);
    this->update();
}

void MainWindow::on_button_exit_clicked()
{
    this->close();
}
void MainWindow::on_groupBox_2_clicked()
{
    return;
}
