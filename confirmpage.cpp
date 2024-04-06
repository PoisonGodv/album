#include "confirmpage.h"
#include "ui_confirmpage.h"

ConFirmPage::ConFirmPage(QWidget *parent)
    : QWizardPage(parent)
    , ui(new Ui::ConFirmPage)
{
    ui->setupUi(this);
}

ConFirmPage::~ConFirmPage()
{
    delete ui;
}
