#include "protreewidget.h"
#include<QDir>
#include"protreeitem.h"
#include"const.h"
#include<QHeaderView>
#include<QGuiApplication>
#include<QMenu>
#include<QFileDialog>
#include<removeprodialog.h>
ProTreeWidget::ProTreeWidget(QWidget *parent):QTreeWidget(parent),_active_item(nullptr),_right_btn_item(nullptr) , _dialog_progress(nullptr),_selected_item(nullptr),_thread_create_pro(nullptr),_thread_open_pro(nullptr),_open_progressdlg(nullptr)
{
    this->header()->hide();
    connect(this, &ProTreeWidget::itemPressed , this , &ProTreeWidget::SlotItemPressed);
    _action_import = new QAction(QIcon(":/new/icon/icon/import.png"),tr("导入文件"),this);
    _action_setstart = new QAction(QIcon(":/new/icon/icon/core.png"), tr("设置活动项目") , this);
    _action_closepro = new QAction(QIcon(":/new/icon/icon/close.png"),tr("关闭项目"),this);
    _action_slideshow = new QAction(QIcon(":/new/icon/icon/slideshow.png"), tr("轮播图播放") , this);
    connect(_action_import, &QAction::triggered , this, &ProTreeWidget::SlotImport);
    connect(_action_setstart , &QAction::triggered , this , &ProTreeWidget::SlotSetActive);
    connect(_action_closepro, &QAction::triggered , this , &ProTreeWidget::SlotClosePro);

}

void ProTreeWidget::AddProToTree(const QString &name, const QString &path)
{
    QDir dir(path);
    QString file_path = dir.absoluteFilePath(name);
    if(_set_path.find(file_path) != _set_path.end()){
        return;
    }
    QDir pro_dir(file_path);
    if(!pro_dir.exists()){
        bool enable = pro_dir.mkpath(file_path);
        if(!enable){
            return;
        }
    }
    _set_path.insert(file_path);
    auto *item = new ProTreeItem(this , name, file_path , TreeItemPro);
    item->setData(0, Qt::DisplayRole, name);
    item->setData(0, Qt::DecorationRole, QIcon(":/new/icon/icon/dir.png"));
    item->setData(0, Qt::ToolTipRole, file_path);
    this->addTopLevelItem(item);


}

void ProTreeWidget::SlotItemPressed(QTreeWidgetItem *pressedItem, int column)
{
    if(QGuiApplication::mouseButtons() == Qt::RightButton){
        QMenu menu(this);
        int itemtype = pressedItem->type();
        if(itemtype == TreeItemPro){
            _right_btn_item = pressedItem;
            menu.addAction(_action_import);
            menu.addAction(_action_setstart);
            menu.addAction(_action_closepro);
            menu.addAction(_action_slideshow);
            menu.exec(QCursor::pos());
        }

    }
}

void ProTreeWidget::SlotImport()
{
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::Directory);
    file_dialog.setWindowTitle(tr("选择导入的文件夹"));
    QString path = "";
    if(!_right_btn_item){
        qDebug()<<"_右键空";
        return;
    }
    path = dynamic_cast<ProTreeItem*>(_right_btn_item)->GetPath();
    file_dialog.setDirectory(path);
    file_dialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if(file_dialog.exec()){
        fileNames = file_dialog.selectedFiles();
    }
    if(fileNames.length() <= 0){
        return;
    }
    QString import_path = fileNames.at(0);
    int file_count = 0;
    _dialog_progress = new QProgressDialog(this);
    _thread_create_pro = std::make_shared<ProTreeThread>(std::ref(import_path),std::ref(path), _right_btn_item, file_count , this, _right_btn_item , nullptr);
    connect(_thread_create_pro.get() , &ProTreeThread::SigUpdateProgress , this , &ProTreeWidget::SlotUpdateProgress);
    connect(_thread_create_pro.get() , &ProTreeThread::SigFinishProgress , this , &ProTreeWidget::SlotFinishProgress);
    connect(_dialog_progress, &QProgressDialog::canceled , this , &ProTreeWidget::SlotCancelProgress);
    connect(this, &ProTreeWidget::SigCancelProgress , _thread_create_pro.get() , &ProTreeThread::SlotCancelProgress);
    _thread_create_pro->start();
    _dialog_progress->setWindowTitle("Please wait ...");
    _dialog_progress->setFixedWidth(PROGRESS_WIDTH);
    _dialog_progress->setRange(0 , PROGRESS_WIDTH);
    _dialog_progress->exec();
}

void ProTreeWidget::SlotUpdateProgress(int count)
{
    if(!_dialog_progress){
        return;
    }
    if(count >= PROGRESS_MAX){
        _dialog_progress->setValue(count % PROGRESS_MAX);
    }else{
        _dialog_progress->setValue(count);
    }
}

void ProTreeWidget::SlotFinishProgress()
{
    _dialog_progress->setValue(PROGRESS_MAX);
    _dialog_progress->deleteLater();
}

void ProTreeWidget::SlotCancelProgress()
{
    emit SigCancelProgress();
    delete _dialog_progress;
    _dialog_progress = nullptr;
}

void ProTreeWidget::SlotUpOpenProgress(int count)
{
    if(!_open_progressdlg){
        return;
    }
    if(count >= PROGRESS_MAX){
        _open_progressdlg->setValue(count % PROGRESS_MAX);
    }else{
        _open_progressdlg->setValue(count);
    }
}

void ProTreeWidget::SlotFinishOpenProgress()
{
    if(!_open_progressdlg){
        return;
    }
    _open_progressdlg->setValue(PROGRESS_MAX);
    delete _open_progressdlg;
    _open_progressdlg=nullptr;
}

void ProTreeWidget::SlotCancelOpenProgress()
{
    emit SigCancelOpenProgress();
    delete _open_progressdlg;
    _open_progressdlg = nullptr;
}

void ProTreeWidget::SlotSetActive()
{
    if(!_right_btn_item){
        return;
    }
    QFont nullFont;
    nullFont.setBold(false);
    if(_active_item){
        _active_item->setFont(0 , nullFont);
    }
    _active_item = _right_btn_item;
    nullFont.setBold(true);
    _active_item->setFont(0 , nullFont);
}

void ProTreeWidget::SlotClosePro()
{
    RemoveProDialog remove_pro_dialog;
    auto res = remove_pro_dialog.exec();
    if(res != QDialog::Accepted){
        return;
    }
    bool b_remove = remove_pro_dialog.IsRemoved();
    auto index_right_btn = this->indexOfTopLevelItem(_right_btn_item);
    auto * protreeitem = dynamic_cast<ProTreeItem*>(_right_btn_item);
    // auto * selecteditem = dynamic_cast<ProTreeItem*>(_selected_item);
    auto delete_path = protreeitem->GetPath();
    _set_path.remove(delete_path);
    if(b_remove){
        QDir delete_dir(delete_path);
        delete_dir.removeRecursively();
    }
    if(protreeitem == _active_item){
        _active_item = nullptr;
    }
    delete this->takeTopLevelItem(index_right_btn);
    _right_btn_item = nullptr;

}

void ProTreeWidget::SlotOpenPro(const QString &path)
{
    if(_set_path.find(path) != _set_path.end()){
        return;
    }
    _set_path.insert(path);
    int file_count = 0;
    QDir pro_dir(path);
    QString proname = pro_dir.dirName();
    _open_progressdlg = new QProgressDialog(this);
    _thread_open_pro = std::make_shared<OpenTreeThread>(path, file_count , this , nullptr);

    connect(_thread_open_pro.get() , &OpenTreeThread::SigUpdateProgress , this , &ProTreeWidget::SlotUpOpenProgress);
    connect(_thread_open_pro.get() , &OpenTreeThread::SigFinishProgress , this , &ProTreeWidget::SlotFinishOpenProgress);
    connect(_open_progressdlg, &QProgressDialog::canceled , this , &ProTreeWidget::SlotCancelOpenProgress);
    connect(this, &ProTreeWidget::SigCancelOpenProgress , _thread_open_pro.get() , &OpenTreeThread::SlotCancelProgress);
   _thread_open_pro->start();

    _open_progressdlg->setWindowTitle("Please wait ...");
    _open_progressdlg->setFixedWidth(PROGRESS_WIDTH);
    _open_progressdlg->setRange(0 , PROGRESS_WIDTH);
    _open_progressdlg->exec();

}
