/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     listenerri <listenerri@gmail.com>
 *
 * Maintainer: listenerri <listenerri@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "networksecretdialog.h"

#include <QApplication>
#include <QJsonArray>
#include <QGridLayout>
#include <QDebug>
#include <QFile>
#include <QPushButton>
#include <QKeyEvent>
#include <QLabel>

DWIDGET_USE_NAMESPACE

const QMap<QString, QString> ConnTypeIconKeyMap {
    {"802-3-ethernet", "network-wired"},
    {"802-11-wireless", "notification-network-wireless-full"},
    {"vpn", "network-vpn"},
    {"pppoe", "network-wired"},
};

NetworkSecretDialog::NetworkSecretDialog(QJsonDocument jsonDoc, QWidget *parent)
    : DDialog(parent)
{
    m_secretKeyStrMap = {
        {"psk", tr("Password")},
        {"wep-key0", tr("Key")},
        {"wep-key1", tr("Key")},
        {"wep-key2", tr("Key")},
        {"wep-key3", tr("key")},
        {"leap-password", "unknown"},
        {"password", tr("Password")},
        {"password-raw", "unknown"},
        {"ca-cert-password", "unknown"},
        {"client-cert-password", "unknown"},
        {"phase2-ca-cert-password", "unknown"},
        {"phase2-client-cert-password", "unknown"},
        {"private-key-password", tr("Private Pwd")},
        {"phase2-private-key-password", "unknown"},
        {"pin", "unknown"},
        {"proxy-password", tr("Proxy Password")},
        {"IPSec secret", tr("Password")},
        {"Xauth password", tr("Group Password")},
    };

    m_allInputValid = false;

    parseJsonData(jsonDoc);
    initUI();
}

void NetworkSecretDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        // override the return code when ESC key pressed
        qApp->exit(1);
    }

    DDialog::keyPressEvent(event);
}

void NetworkSecretDialog::parseJsonData(const QJsonDocument &jsonDoc)
{
    m_jsonObj = jsonDoc.object();
    m_connName = m_jsonObj.value("connId").toString();
    m_connType = m_jsonObj.value("connType").toString();
    m_connSettingName = m_jsonObj.value("settingName").toString();
    for (auto secret : m_jsonObj.value("secrets").toArray()) {
        m_secretKeyList.append(secret.toString());
    }
}

void NetworkSecretDialog::initUI()
{
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Dialog);

    setTitle(tr("Password required to connect <font color=\"#faca57\">%1</font>").arg(m_connName));
    addSpacing(10);

    const auto ratio = qApp->devicePixelRatio();
    QPixmap iconPix = QIcon::fromTheme(
                ConnTypeIconKeyMap.value(m_connType, "network-wired")).pixmap(QSize(64, 64) / ratio);
    iconPix.setDevicePixelRatio(ratio);
    setIconPixmap(iconPix);

    addButtons(QStringList() << tr("Cancel") << tr("Connect"));
    setDefaultButton(1);
    getButton(1)->setEnabled(false);

    QWidget *widget = new QWidget;
    QGridLayout *gridLayout = new QGridLayout;
    widget->setLayout(gridLayout);

    addContent(widget);

    for (int row = 0; row < m_secretKeyList.size(); ++row) {
        const QString &secret = m_secretKeyList.at(row);
        DPasswordEdit *lineEdit = new DPasswordEdit();
        lineEdit->setClearButtonEnabled(true);
        gridLayout->addWidget(new QLabel(m_secretKeyStrMap.value(secret) + ":"), row, 0);
        gridLayout->addWidget(lineEdit, row, 1);
        m_lineEditList.append(lineEdit);

        if (row == 0) {
            lineEdit->setFocus();
        }

        connect(lineEdit, &DLineEdit::textChanged, this, &NetworkSecretDialog::checkInputValid);
    }

    connect(this, &NetworkSecretDialog::buttonClicked, this, &NetworkSecretDialog::onButtonsClicked);
}

void NetworkSecretDialog::onButtonsClicked(int index, const QString &text)
{
    Q_UNUSED(text);

    if (index == 0) {
        qApp->exit(1);
    } else {
        submit();
    }
}

void NetworkSecretDialog::submit()
{
    if (m_allInputValid) {
        QJsonObject resultJsonObj;
        QJsonArray secretsJsonArray;
        for (auto lineEdit : m_lineEditList) {
            secretsJsonArray.append(lineEdit->text());
        }
        resultJsonObj.insert("secrets", secretsJsonArray);

        QFile file;
        if (!file.open(stdout, QFile::WriteOnly)) {
            qDebug() << "open STDOUT failed";
            qApp->exit(-4);
        }
        file.write(QJsonDocument(resultJsonObj).toJson());
        file.flush();
        file.close();
    } else {
        qDebug() << "some input is invalid!";
    }
}

void NetworkSecretDialog::checkInputValid()
{
    bool allValid = true;

    for (int i = 0; i < m_secretKeyList.size(); ++i) {
        DPasswordEdit * const lineEdit = m_lineEditList.at(i);
        if (!passwordIsValid(lineEdit->text(), m_secretKeyList.at(i))) {
            allValid = false;
            lineEdit->setAlert(true);
        } else {
            lineEdit->setAlert(false);
        }
    }

    m_allInputValid = allValid;

    getButton(1)->setEnabled(m_allInputValid);
}

bool NetworkSecretDialog::passwordIsValid(const QString &text, const QString &secretKey)
{
    bool valid = true;

    if (secretKey.startsWith("wep")) {
        valid = wepKeyIsValid(text, WepKeyType::Hex);
    } else if (secretKey == "psk") {
        valid = wpaPskIsValid(text);
    } else{
        valid = !text.isEmpty();
    }

    return valid;
}

bool NetworkSecretDialog::wepKeyIsValid(const QString &key, WepKeyType type)
{
    if (key.isEmpty()) {
        return false;
    }

    const int keylen = key.length();

    if (type != WepKeyType::NotSpecified) {
        if (type == WepKeyType::Hex) {
            if (keylen == 10 || keylen == 26) {
                /* Hex key */
                for (int i = 0; i < keylen; ++i) {
                    if (!(key.at(i).isDigit() || (key.at(i) >= 'A' && key.at(i) <= 'F') || (key.at(i) >= 'a' && key.at(i) <= 'f'))) {
                        return false;
                    }
                }
                return true;
            } else if (keylen == 5 || keylen == 13) {
                /* ASCII KEY */
                for (int i = 0; i < keylen; ++i) {
                    if (!key.at(i).isPrint()) {
                        return false;
                    }
                }
                return true;
            }

            return false;
        } else if (type == WepKeyType::Passphrase) {
            if (!keylen || keylen > 64) {
                return false;
            }

            return true;
        }
    }

    return false;
}

bool NetworkSecretDialog::wpaPskIsValid(const QString &psk)
{
    if (psk.isEmpty()) {
        return false;
    }

    const int psklen = psk.length();

    if (psklen < 8 || psklen > 64) {
        return false;
    }

    if (psklen == 64) {
        /* Hex PSK */
        for (int i = 0; i < psklen; ++i) {
            if (!psk.at(i).isLetterOrNumber()) {
                return false;
            }
        }
    }

    return true;
}
