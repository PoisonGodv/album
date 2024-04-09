#include "mainwindow.h"
#include "qfiledialog.h"
#include "ui_mainwindow.h"
#include<QAction>
#include<QMenu>
#include"wizard.h"
#include"protree.h"
#include"protreewidget.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QMenu *menu_file = menuBar() -> addMenu(tr("文件(&F)"));
    //创建项目
    QAction *act_create_pro = new QAction(QIcon(":/new/icon/icon/createpro.png"),tr("创建项目") , this);
    act_create_pro->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
    menu_file->addAction(act_create_pro);
    //打开项目
    QAction *act_open_pro = new QAction(QIcon(":/new/icon/icon/openpro.png"),tr("打开项目"),this);
    act_open_pro->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    menu_file->addAction(act_open_pro);

    //创建设置菜单
    QMenu *menu_set = menuBar()->addMenu(tr("设置(&S)"));

    //设置背景音乐
    QAction* act_music = new QAction(QIcon(":/new/icon/icon/music.png"), tr("背景音乐") , this);
    act_music->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_M));
    menu_set->addAction(act_music);

    //链接创建项目槽函数
    connect(act_create_pro , &QAction::triggered , this , &MainWindow::SlotCreatPro);
    connect(act_open_pro , &QAction::triggered , this , & MainWindow::SlotOpenPro);


    _protree = new ProTree();

    ui->proLayout->addWidget(_protree);
    auto *tree_widget = dynamic_cast<ProTree*>(_protree)->GetTreeWidget();
    auto *pro_tree_widget = dynamic_cast<ProTreeWidget*>(tree_widget);
    connect(this , &MainWindow::SigOpenPro , pro_tree_widget, &ProTreeWidget::SlotOpenPro);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SlotCreatPro(bool)
{
    Wizard wizard(this);
    qDebug()<<"slot create";
    wizard.setWindowTitle("创建项目");
    auto *page = wizard.page(0);
    page->setTitle(tr("设置项目配置"));
    //链接槽函数
    connect(&wizard , &Wizard::SigProSettings, dynamic_cast<ProTree*>(_protree), &ProTree::AddProToTree);

    wizard.show();
    wizard.exec();
    //断开信号
    disconnect();
}

void MainWindow::SlotOpenPro(bool)
{
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::Directory);
    file_dialog.setWindowTitle(tr("选择导入的文件夹"));
    file_dialog.setDirectory(QDir::currentPath());
    file_dialog.setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    if(file_dialog.exec()){
        fileNames = file_dialog.selectedFiles();
    }
    if(fileNames.length() <= 0){
        return;
    }
    QString import_path = fileNames.at(0);
    emit SigOpenPro(import_path);

}
