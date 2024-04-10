#include "slideshowdlg.h"
#include "ui_slideshowdlg.h"

SlideShowDlg::SlideShowDlg(QWidget *parent, QTreeWidgetItem *first_item, QTreeWidgetItem *last_item)
    : QDialog(parent),_first_item(first_item), _last_item(last_item)
    , ui(new Ui::SlideShowDlg)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    ui->slidePreBtn->SetIcons(":/new/icon/icon/previous.png", ":/new/icon/icon/previous_hover.png" , ":/new/icon/icon/previous_press.png");

    ui->slideNextBtn->SetIcons(":/new/icon/icon/next.png" , ":/new/icon/icon/next_hover.png" , ":/new/icon/icon/next_press.png");

    ui->closeBtn->SetIcons(":/new/icon/icon/closeshow.png" , ":/new/icon/icon/closeshow_hover.png" , ":/new/icon/icon/closeshow_press.png");

    ui->playBtn->SetIcons(":/new/icon/icon/play.png" , ":/new/icon/icon/play_hover.png" , ":/new/icon/icon/play_press.png",":/new/icon/icon/pause.png" , ":/new/icon/icon/pause_hover.png" , ":/new/icon/icon/pause_press.png");

    ui->picAnimation->SetPixmap(_first_item);
    ui->picAnimation->Start();
}



SlideShowDlg::~SlideShowDlg()
{
    delete ui;
}
