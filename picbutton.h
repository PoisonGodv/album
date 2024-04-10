#ifndef PICBUTTON_H
#define PICBUTTON_H

#include <QPushButton>
#include<QPixmap>
#include<QEvent>

class PicButton : public QPushButton
{
public:
    PicButton(QWidget *parent = nullptr);
    void SetIcons(const QString& normal , const QString& hover, const QString& pressed);
private:
    void setNormalIcon();
    void setHoverIcon();
    void setPressIcon();
    QString _normal;
    QString _hover;
    QString _pressed;
protected:
    bool event(QEvent *e) override;


};

#endif // PICBUTTON_H
