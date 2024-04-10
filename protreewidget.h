#ifndef PROTREEWIDGET_H
#define PROTREEWIDGET_H

#include <QTreeWidget>
#include<QAction>
#include"protreethread.h"
#include<QProgressDialog>
#include"opentreethread.h"
class SlideShowDlg;
class ProTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    ProTreeWidget(QWidget *parent = nullptr);
    void AddProToTree(const QString& name, const QString & path);
private:
    QSet<QString > _set_path;
    QTreeWidgetItem * _active_item;
    QTreeWidgetItem* _right_btn_item;
    QTreeWidgetItem* _selected_item;
    QAction * _action_import;
    QAction * _action_setstart;
    QAction * _action_closepro;
    QAction * _action_slideshow;
    QProgressDialog * _dialog_progress;
    QProgressDialog * _open_progressdlg;
    std::shared_ptr<ProTreeThread> _thread_create_pro;
    std::shared_ptr<OpenTreeThread> _thread_open_pro;
    std::shared_ptr<SlideShowDlg> _slide_show_dlg;
private slots:
    void SlotItemPressed(QTreeWidgetItem *pressedItem, int column);
    void SlotImport();
    void SlotUpdateProgress(int count);
    void SlotFinishProgress();
    void SlotCancelProgress();
    void SlotDoubleClickItem(QTreeWidgetItem *doubleItem, int col);

    void SlotUpOpenProgress(int count);
    void SlotFinishOpenProgress();
    void SlotCancelOpenProgress();
    void SlotSetActive();
    void SlotClosePro();

    void SlotSlideShow();
public slots:
    void SlotOpenPro(const QString& path);
    void SlotNextShow();
    void SlotPreShow();
signals:
    void SigCancelProgress();
    void SigCancelOpenProgress();
    void SigUpdateSelected(const QString& );
    void SigUpdatePic(const QString&);
    void SigClearSelected();
};

#endif // PROTREEWIDGET_H
