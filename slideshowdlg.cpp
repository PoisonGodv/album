#include "slideshowdlg.h"
#include "ui_slideshowdlg.h"
#include"prelistwid.h"
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

    connect(ui->closeBtn, &QPushButton::clicked , this , &SlideShowDlg::close);
    connect(ui->slideNextBtn , &QPushButton::clicked , this , & SlideShowDlg::SlotSlideNext);
    connect(ui->slidePreBtn ,&QPushButton::clicked , this , &SlideShowDlg::SlotSlidePre);

    auto * prelistWid = dynamic_cast<PreListWid* > (ui->preListWidget);

    connect(ui->picAnimation, &PicAnimationWid::SigUpPreList, prelistWid , &PreListWid::SlotUpPreList);

    connect(ui->picAnimation, &PicAnimationWid::SigSelecteItem, prelistWid , &PreListWid::SlotUpSelect);

    connect(prelistWid , &PreListWid::SigUpSelectShow, ui->picAnimation, &PicAnimationWid::SlotUpSelectShow);

    connect(ui->playBtn , &PicStateBtn::clicked , ui->picAnimation , &PicAnimationWid::SlotStartOrStop);

    connect(ui->picAnimation, &PicAnimationWid::SigStart, ui->playBtn , &PicStateBtn::SlotStart );
    connect(ui->picAnimation, &PicAnimationWid::SigStop, ui->playBtn , &PicStateBtn::SlotStop );

    ui->picAnimation->SetPixmap(_first_item);


    ui->picAnimation->Start();
}



SlideShowDlg::~SlideShowDlg()
{
    delete ui;
}

void SlideShowDlg::SlotSlideNext()
{
    ui->picAnimation->SlideNext();
}

void SlideShowDlg::SlotSlidePre()
{
    ui->picAnimation->SlidePre();
}
