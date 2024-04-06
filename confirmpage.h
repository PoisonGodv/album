#ifndef CONFIRMPAGE_H
#define CONFIRMPAGE_H

#include <QWizardPage>

namespace Ui {
class ConFirmPage;
}

class ConFirmPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ConFirmPage(QWidget *parent = nullptr);
    ~ConFirmPage();

private:
    Ui::ConFirmPage *ui;
};

#endif // CONFIRMPAGE_H
