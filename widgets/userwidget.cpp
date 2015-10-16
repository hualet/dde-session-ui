#include "userwidget.h"
#include <QApplication>
#include <QtWidgets>
#include <QtGui>
#include <QtCore>

UserWidget::UserWidget(QWidget *parent)
    : QFrame(parent),
    m_showAnimation(new QPropertyAnimation),
    m_hideAnimation(new QPropertyAnimation)
{

    initUI();
    initConnect();

}
UserWidget::~UserWidget()
{
}
void UserWidget::initConnect() {
    connect(this, SIGNAL(selectedUser(QString)),
            SLOT(loginInUser(QString)));

    QList<UserButton*> m_children =
            this->findChildren<UserButton *>();
    for(int i = 0; i < m_children.length(); i++) {
        connect(m_children[i], SIGNAL(imageClicked(QString)), this, SLOT(loginInUser(QString)));
    }

    connect(m_showAnimation, &QPropertyAnimation::finished, [=]{
        emit sizeUpdate();
    });
}
void UserWidget::initUI() {
    this->setFixedSize(USER_ICON_WIDTH, USER_ICON_HEIGHT);
    m_Layout = new QHBoxLayout;
    m_Layout->setMargin(0);
    m_Layout->addSpacing(0);
    m_Layout->addStretch();
    m_showAnimation->setPropertyName("geometry");
    m_showAnimation->setTargetObject(this);
    m_hideAnimation->setPropertyName("geometry");
    m_hideAnimation->setTargetObject(this);

    addUser(QString(":img/user/kakaxi.png"), "administrator");
    addUser(QString(":img/user/mingren.png"), "demo");
    addUser(QString(":img/user/mingren.png"), "test");
    addUser(QString(":img/user/mingren.png"), "tom");
    addUser(QString(":img/user/mingren.png"), "linda");

    setLayout(m_Layout);

    this->move((qApp->desktop()->screenGeometry().width() - this->width())/2,
                              (qApp->desktop()->screenGeometry().height() - this->height())/2-85);

    setCurrentUser();
}
void UserWidget::setCurrentUser() {
    QList<UserButton*> m_children =
            this->findChildren<UserButton *>();

    for(int i = 0; i < m_children.length(); i++) {

        if (m_children[i]->objectName() != m_currentUser) {
            m_children[i]->hide();
        } else {
            m_children[i]->setImageSize(UserButton::AvatarLargerSize);
        }
    }
}

void UserWidget::setUserAvatarSamll() {
    QList<UserButton*> m_children =
            this->findChildren<UserButton *>();

    for(int i = 0; i < m_children.length(); i++) {
        qDebug() << "m_children" << i << m_children[i]->objectName();
        m_children[i]->setImageSize(UserButton::AvatarSmallSize);
        m_children[i]->show();
    }
}

void UserWidget::addUser(QString url, QString name) {
    UserButton* add_user = new UserButton(url, name);
    add_user->setObjectName(name);
    add_user->setImageSize(add_user->AvatarSmallSize);
    add_user->update();

    userList << name;
    QVBoxLayout* m_userItemLayout = new QVBoxLayout;
    m_userItemLayout->setMargin(0);
    m_userItemLayout->setSpacing(0);
    m_userItemLayout->addWidget(add_user);
    m_Layout->addLayout(m_userItemLayout);
    m_Layout->addStretch();
}

void UserWidget::loginInUser(QString nam) {
    m_currentUser = nam;
    packUpWidget();
}

void UserWidget::testing(QString em) {
    qDebug() << "testing" << em;
}

void UserWidget::setGeometry(QRect rect) {
    this->move(rect.x(), rect.y());
    this->setFixedSize(rect.width(), rect.height());
}
//////////////////////////////////////--Multi-User expand animation
void UserWidget::expandWidget() {
    qDebug() << USER_ICON_WIDTH << USER_ICON_HEIGHT << userList.length();

    m_showAnimation->setStartValue(QRect((qApp->desktop()->screenGeometry().width() -
                                          this->width())/2,
                                   (qApp->desktop()->screenGeometry().height() -
                                    this->height())/2-85,
                                         USER_ICON_WIDTH, USER_ICON_HEIGHT));
    m_showAnimation->setEndValue(QRect((qApp->desktop()->screenGeometry().width() -
                                        USER_ICON_WIDTH*userList.length())/2,
                                       (qApp->desktop()->screenGeometry().height() -
                                        this->height())/2-85,
                                             USER_ICON_WIDTH*userList.length(),
                                       USER_ICON_HEIGHT));
    setUserAvatarSamll();
    m_showAnimation->start();
}
//////////////////////////////////////--Multi-User unexpand animation
void UserWidget::packUpWidget() {
    m_hideAnimation->setStartValue(QRect((qApp->desktop()->screenGeometry().width() -
                                          USER_ICON_WIDTH*userList.length())/2,
                                       (qApp->desktop()->screenGeometry().height() -
                                        this->height())/2-85,
                                             USER_ICON_WIDTH*userList.length(),
                                         USER_ICON_HEIGHT));
    m_hideAnimation->setEndValue(QRect((qApp->desktop()->screenGeometry().width() -
                                        USER_ICON_WIDTH)/2,
                                   (qApp->desktop()->screenGeometry().height() -
                                    this->height())/2-85,
                                         USER_ICON_WIDTH, USER_ICON_HEIGHT));

    QList<UserButton*> m_children =
            this->findChildren<UserButton *>();
    for(int i = 0; i < m_children.length(); i++) {
        if (m_children[i]->objectName()!= m_currentUser) {          
            QTimer::singleShot(userList.length()*70-(i+1)*70, [=]{ m_children[i]->hide();});
        } else {
            m_children[i]->setImageSize(UserButton::AvatarLargerSize);
        }
    }

    m_hideAnimation->start();
}