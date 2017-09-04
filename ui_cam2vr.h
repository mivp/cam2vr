/********************************************************************************
** Form generated from reading UI file 'cam2vr.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAM2VR_H
#define UI_CAM2VR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Cam2VRDialog
{
public:
    QVBoxLayout *verticalLayout;

    void setupUi(QDialog *Cam2VRDialog)
    {
        if (Cam2VRDialog->objectName().isEmpty())
            Cam2VRDialog->setObjectName(QStringLiteral("Cam2VRDialog"));
        Cam2VRDialog->resize(720, 486);
        verticalLayout = new QVBoxLayout(Cam2VRDialog);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);

        retranslateUi(Cam2VRDialog);

        QMetaObject::connectSlotsByName(Cam2VRDialog);
    } // setupUi

    void retranslateUi(QDialog *Cam2VRDialog)
    {
        Cam2VRDialog->setWindowTitle(QApplication::translate("Cam2VRDialog", "OpenGL LoopThrough Preview", 0));
    } // retranslateUi

};

namespace Ui {
    class Cam2VRDialog: public Ui_Cam2VRDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAM2VR_H
