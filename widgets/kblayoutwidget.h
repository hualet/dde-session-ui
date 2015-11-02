#ifndef KBLAYOUTWIDGET
#define KBLAYOUTWIDGET

#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QHBoxLayout>

#include "constants.h"

class LayoutButton: public QPushButton
{
    Q_OBJECT
public:
    LayoutButton(QString text, QWidget* parent = 0);
    ~LayoutButton();

    void updateStyle();
    void setButtonChecked(bool checked);
    QString m_text;

signals:
    void mouseEnter(QString label);
    void mouseLeave(QString label);
    void onlyOneChecked(QString text);
public slots:
    void OnlyMeChecked(bool checked);
protected:
    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);

private:

    QLabel* m_iconLabel;
    QLabel* m_textLabel;
};

class KbLayoutWidget: public QListWidget
{
    Q_OBJECT
public:
    KbLayoutWidget(QStringList buttons, QWidget* parent = 0);
    ~KbLayoutWidget();

    void initData(QStringList buttons);
    void initUI();
    void initConnect();
    void updateUI();
public slots:
    void setButtonsChecked(QString text);
private:
    QHBoxLayout* m_Layout;
    QStringList m_buttons;
    QList<LayoutButton* > m_layoutButtons;
};
#endif // KEYBOARDLAYOUTFRAME
