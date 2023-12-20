#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QVBoxLayout>
#include <QWidget>

#include "hosttreemodel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //ui->setupUi(this);
    initUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUI()
{
    this->resize(800, 600);
    setWindowTitle(QString("host manager"));

    //center widget
    auto centralwidget = new QWidget(this);
    centralwidget->setObjectName("centralwidget");
    auto verticalLayout = new QVBoxLayout(centralwidget);
    verticalLayout->setObjectName("verticalLayout");
    auto treeView = new QTreeView(centralwidget);
    treeView->setObjectName("treeView");
    verticalLayout->addWidget(treeView);
    this->setCentralWidget(centralwidget);


    // menu bar and stuff
    auto menubar = new QMenuBar(this);
    menubar->setObjectName("menubar");
    menubar->setGeometry(QRect(0, 0, 800, 23));
    this->setMenuBar(menubar);

    auto statusbar = new QStatusBar(this);
    statusbar->setObjectName("statusbar");
    this->setStatusBar(statusbar);

    auto treeModel=new hostTreeModel(this);
    treeView->setModel(treeModel);

}

