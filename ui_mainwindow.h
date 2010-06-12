/********************************************************************************
** Form generated from reading ui file 'mainwindow.ui'
**
** Created: Sat Jun 12 14:18:22 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QFormLayout>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBar>
#include <QtGui/QToolButton>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_3;
    QPlainTextEdit *plainTextEdit;
    QProgressBar *progressBar;
    QPushButton *pushButton_4;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QSpacerItem *horizontalSpacer;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_2;
    QFormLayout *formLayout_2;
    QToolButton *toolButton;
    QLineEdit *lineEdit;
    QToolButton *toolButton_2;
    QLineEdit *lineEdit_2;
    QLabel *label_5;
    QLabel *label_6;
    QFrame *line_3;
    QVBoxLayout *verticalLayout_2;
    QComboBox *comboBox;
    QFormLayout *formLayout_6;
    QLabel *label;
    QSpinBox *spinBox;
    QFrame *line_2;
    QGridLayout *gridLayout;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_4;
    QGridLayout *gridLayout_6;
    QCheckBox *checkBox_6;
    QCheckBox *checkBox_7;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_3;
    QWidget *Edit_tab;
    QGridLayout *gridLayout_4;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QLabel *label_edit_id;
    QLabel *label_edit_region;
    QLabel *label_edit_name;
    QPushButton *edit_img_pushButton;
    QTreeWidget *treeWidget;
    QSpacerItem *horizontalSpacer_2;
    QWidget *settings_tab;
    QWidget *layoutWidget;
    QFormLayout *formLayout;
    QLabel *label_2;
    QComboBox *verbose_combobox;
    QLabel *label_3;
    QComboBox *logging_combobox;
    QLabel *def_ios_lbl;
    QSpinBox *default_ios_spinbox;
    QPushButton *pushButton_settings_searchPath;
    QLineEdit *lineEdit_default_path;
    QLabel *label_7;
    QComboBox *startupTab_combobox;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout;
    QCheckBox *checkBox;
    QCheckBox *overwrite_checkbox;
    QCheckBox *checkBox_hiddenFiles;
    QPushButton *save_pushButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 756);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout_3 = new QGridLayout(centralWidget);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setMargin(11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        plainTextEdit = new QPlainTextEdit(centralWidget);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(plainTextEdit->sizePolicy().hasHeightForWidth());
        plainTextEdit->setSizePolicy(sizePolicy);
        plainTextEdit->setMinimumSize(QSize(0, 150));
        plainTextEdit->setSizeIncrement(QSize(1, 1));
        QFont font;
        font.setFamily(QString::fromUtf8("Courier New"));
        font.setPointSize(8);
        font.setBold(true);
        font.setWeight(75);
        plainTextEdit->setFont(font);
        plainTextEdit->viewport()->setProperty("cursor", QVariant(QCursor(Qt::ArrowCursor)));
        plainTextEdit->setFrameShadow(QFrame::Sunken);
        plainTextEdit->setLineWidth(1);
        plainTextEdit->setReadOnly(true);
        plainTextEdit->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        gridLayout_3->addWidget(plainTextEdit, 6, 0, 1, 5);

        progressBar = new QProgressBar(centralWidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(progressBar->sizePolicy().hasHeightForWidth());
        progressBar->setSizePolicy(sizePolicy1);
        progressBar->setMinimumSize(QSize(200, 0));
        progressBar->setValue(0);

        gridLayout_3->addWidget(progressBar, 7, 0, 1, 1);

        pushButton_4 = new QPushButton(centralWidget);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setMaximumSize(QSize(100, 16777215));

        gridLayout_3->addWidget(pushButton_4, 7, 2, 1, 1);

        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        sizePolicy1.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy1);

        gridLayout_3->addWidget(pushButton_2, 7, 3, 1, 1);

        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        sizePolicy1.setHeightForWidth(pushButton_3->sizePolicy().hasHeightForWidth());
        pushButton_3->setSizePolicy(sizePolicy1);

        gridLayout_3->addWidget(pushButton_3, 7, 4, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 7, 1, 1, 1);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setMinimumSize(QSize(735, 464));
        tabWidget->setBaseSize(QSize(0, 0));
        tabWidget->setMouseTracking(false);
        tabWidget->setMovable(true);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_2 = new QGridLayout(tab);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setMargin(11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        formLayout_2 = new QFormLayout();
        formLayout_2->setSpacing(6);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        formLayout_2->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        toolButton = new QToolButton(tab);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, toolButton);

        lineEdit = new QLineEdit(tab);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setMinimumSize(QSize(250, 0));
        lineEdit->setDragEnabled(true);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, lineEdit);

        toolButton_2 = new QToolButton(tab);
        toolButton_2->setObjectName(QString::fromUtf8("toolButton_2"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, toolButton_2);

        lineEdit_2 = new QLineEdit(tab);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setMinimumSize(QSize(250, 0));
        lineEdit_2->setDragEnabled(true);

        formLayout_2->setWidget(3, QFormLayout::FieldRole, lineEdit_2);

        label_5 = new QLabel(tab);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_5);

        label_6 = new QLabel(tab);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_6);


        gridLayout_2->addLayout(formLayout_2, 0, 0, 1, 1);

        line_3 = new QFrame(tab);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::VLine);
        line_3->setFrameShadow(QFrame::Sunken);

        gridLayout_2->addWidget(line_3, 0, 2, 2, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        comboBox = new QComboBox(tab);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        verticalLayout_2->addWidget(comboBox);

        formLayout_6 = new QFormLayout();
        formLayout_6->setSpacing(6);
        formLayout_6->setObjectName(QString::fromUtf8("formLayout_6"));
        label = new QLabel(tab);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout_6->setWidget(0, QFormLayout::LabelRole, label);

        spinBox = new QSpinBox(tab);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setEnabled(false);
        spinBox->setMinimum(3);
        spinBox->setMaximum(254);

        formLayout_6->setWidget(0, QFormLayout::FieldRole, spinBox);


        verticalLayout_2->addLayout(formLayout_6);


        gridLayout_2->addLayout(verticalLayout_2, 0, 3, 2, 1);

        line_2 = new QFrame(tab);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout_2->addWidget(line_2, 2, 0, 1, 4);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        checkBox_2 = new QCheckBox(tab);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));
        checkBox_2->setEnabled(false);
        checkBox_2->setCheckable(true);

        gridLayout->addWidget(checkBox_2, 0, 1, 1, 1);

        checkBox_3 = new QCheckBox(tab);
        checkBox_3->setObjectName(QString::fromUtf8("checkBox_3"));
        checkBox_3->setEnabled(false);
        checkBox_3->setCheckable(true);
        checkBox_3->setTristate(false);

        gridLayout->addWidget(checkBox_3, 1, 0, 1, 1);

        checkBox_4 = new QCheckBox(tab);
        checkBox_4->setObjectName(QString::fromUtf8("checkBox_4"));
        checkBox_4->setEnabled(false);
        checkBox_4->setCheckable(true);

        gridLayout->addWidget(checkBox_4, 1, 1, 1, 1);


        gridLayout_2->addLayout(gridLayout, 3, 2, 1, 2);

        gridLayout_6 = new QGridLayout();
        gridLayout_6->setSpacing(6);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        checkBox_6 = new QCheckBox(tab);
        checkBox_6->setObjectName(QString::fromUtf8("checkBox_6"));

        gridLayout_6->addWidget(checkBox_6, 0, 1, 1, 1);

        checkBox_7 = new QCheckBox(tab);
        checkBox_7->setObjectName(QString::fromUtf8("checkBox_7"));

        gridLayout_6->addWidget(checkBox_7, 1, 1, 1, 1);

        lineEdit_4 = new QLineEdit(tab);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));
        lineEdit_4->setEnabled(false);
        lineEdit_4->setMinimumSize(QSize(220, 0));

        gridLayout_6->addWidget(lineEdit_4, 0, 0, 1, 1);

        lineEdit_3 = new QLineEdit(tab);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        lineEdit_3->setEnabled(false);
        lineEdit_3->setMinimumSize(QSize(220, 0));
        lineEdit_3->setMaxLength(6);
        lineEdit_3->setDragEnabled(true);

        gridLayout_6->addWidget(lineEdit_3, 1, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout_6, 3, 0, 1, 1);

        tabWidget->addTab(tab, QString());
        Edit_tab = new QWidget();
        Edit_tab->setObjectName(QString::fromUtf8("Edit_tab"));
        gridLayout_4 = new QGridLayout(Edit_tab);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setMargin(11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_edit_id = new QLabel(Edit_tab);
        label_edit_id->setObjectName(QString::fromUtf8("label_edit_id"));
        label_edit_id->setMinimumSize(QSize(200, 0));

        horizontalLayout->addWidget(label_edit_id);

        label_edit_region = new QLabel(Edit_tab);
        label_edit_region->setObjectName(QString::fromUtf8("label_edit_region"));
        label_edit_region->setMinimumSize(QSize(100, 0));

        horizontalLayout->addWidget(label_edit_region);


        verticalLayout_3->addLayout(horizontalLayout);

        label_edit_name = new QLabel(Edit_tab);
        label_edit_name->setObjectName(QString::fromUtf8("label_edit_name"));
        label_edit_name->setMinimumSize(QSize(475, 0));

        verticalLayout_3->addWidget(label_edit_name);


        gridLayout_4->addLayout(verticalLayout_3, 0, 0, 1, 1);

        edit_img_pushButton = new QPushButton(Edit_tab);
        edit_img_pushButton->setObjectName(QString::fromUtf8("edit_img_pushButton"));
        edit_img_pushButton->setMinimumSize(QSize(110, 0));

        gridLayout_4->addWidget(edit_img_pushButton, 0, 2, 1, 1);

        treeWidget = new QTreeWidget(Edit_tab);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        QFont font1;
        font1.setPointSize(8);
        treeWidget->setFont(font1);
        treeWidget->setAutoFillBackground(false);
        treeWidget->setFrameShape(QFrame::StyledPanel);
        treeWidget->setFrameShadow(QFrame::Sunken);
        treeWidget->setAlternatingRowColors(true);
        treeWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
        treeWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
        treeWidget->setIndentation(20);
        treeWidget->setSortingEnabled(true);
        treeWidget->setAnimated(true);

        gridLayout_4->addWidget(treeWidget, 1, 0, 1, 3);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_2, 0, 1, 1, 1);

        tabWidget->addTab(Edit_tab, QString());
        settings_tab = new QWidget();
        settings_tab->setObjectName(QString::fromUtf8("settings_tab"));
        layoutWidget = new QWidget(settings_tab);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 100, 391, 161));
        formLayout = new QFormLayout(layoutWidget);
        formLayout->setSpacing(6);
        formLayout->setMargin(11);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_2);

        verbose_combobox = new QComboBox(layoutWidget);
        verbose_combobox->setObjectName(QString::fromUtf8("verbose_combobox"));
        verbose_combobox->setFrame(true);

        formLayout->setWidget(0, QFormLayout::FieldRole, verbose_combobox);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_3);

        logging_combobox = new QComboBox(layoutWidget);
        logging_combobox->setObjectName(QString::fromUtf8("logging_combobox"));

        formLayout->setWidget(1, QFormLayout::FieldRole, logging_combobox);

        def_ios_lbl = new QLabel(layoutWidget);
        def_ios_lbl->setObjectName(QString::fromUtf8("def_ios_lbl"));

        formLayout->setWidget(2, QFormLayout::LabelRole, def_ios_lbl);

        default_ios_spinbox = new QSpinBox(layoutWidget);
        default_ios_spinbox->setObjectName(QString::fromUtf8("default_ios_spinbox"));
        default_ios_spinbox->setMaximum(255);
        default_ios_spinbox->setValue(36);

        formLayout->setWidget(2, QFormLayout::FieldRole, default_ios_spinbox);

        pushButton_settings_searchPath = new QPushButton(layoutWidget);
        pushButton_settings_searchPath->setObjectName(QString::fromUtf8("pushButton_settings_searchPath"));
        pushButton_settings_searchPath->setMinimumSize(QSize(75, 0));

        formLayout->setWidget(3, QFormLayout::LabelRole, pushButton_settings_searchPath);

        lineEdit_default_path = new QLineEdit(layoutWidget);
        lineEdit_default_path->setObjectName(QString::fromUtf8("lineEdit_default_path"));
        lineEdit_default_path->setMinimumSize(QSize(224, 0));
#ifndef QT_NO_TOOLTIP
        lineEdit_default_path->setToolTip(QString::fromUtf8(""));
#endif // QT_NO_TOOLTIP
        lineEdit_default_path->setInputMask(QString::fromUtf8(""));
	lineEdit_default_path->setText(QString::fromUtf8("/media"));

        formLayout->setWidget(3, QFormLayout::FieldRole, lineEdit_default_path);

        label_7 = new QLabel(layoutWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label_7);

        startupTab_combobox = new QComboBox(layoutWidget);
        startupTab_combobox->setObjectName(QString::fromUtf8("startupTab_combobox"));
        startupTab_combobox->setMinimumSize(QSize(107, 0));

        formLayout->setWidget(4, QFormLayout::FieldRole, startupTab_combobox);

        layoutWidget1 = new QWidget(settings_tab);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 10, 391, 80));
        verticalLayout = new QVBoxLayout(layoutWidget1);
        verticalLayout->setSpacing(6);
        verticalLayout->setMargin(11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        checkBox = new QCheckBox(layoutWidget1);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        verticalLayout->addWidget(checkBox);

        overwrite_checkbox = new QCheckBox(layoutWidget1);
        overwrite_checkbox->setObjectName(QString::fromUtf8("overwrite_checkbox"));
        overwrite_checkbox->setChecked(true);

        verticalLayout->addWidget(overwrite_checkbox);

        checkBox_hiddenFiles = new QCheckBox(layoutWidget1);
        checkBox_hiddenFiles->setObjectName(QString::fromUtf8("checkBox_hiddenFiles"));
        checkBox_hiddenFiles->setChecked(true);

        verticalLayout->addWidget(checkBox_hiddenFiles);

        save_pushButton = new QPushButton(settings_tab);
        save_pushButton->setObjectName(QString::fromUtf8("save_pushButton"));
        save_pushButton->setGeometry(QRect(10, 400, 115, 28));
        save_pushButton->setMinimumSize(QSize(115, 0));
        tabWidget->addTab(settings_tab, QString());

        gridLayout_3->addWidget(tabWidget, 0, 0, 1, 5);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 800, 25));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(2);
        comboBox->setCurrentIndex(2);
        verbose_combobox->setCurrentIndex(5);
        logging_combobox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        plainTextEdit->setToolTip(QApplication::translate("MainWindow", "Useful information", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        pushButton_4->setToolTip(QApplication::translate("MainWindow", "Tell wit to create a image using the selected parameters", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pushButton_4->setText(QApplication::translate("MainWindow", "Do It", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pushButton_2->setToolTip(QApplication::translate("MainWindow", "Clear the text", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pushButton_2->setText(QApplication::translate("MainWindow", "Clear", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pushButton_3->setToolTip(QApplication::translate("MainWindow", "Copy the text in the output box", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pushButton_3->setText(QApplication::translate("MainWindow", "Copy", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton->setToolTip(QApplication::translate("MainWindow", "Enter the path to the files to be used to make the disc image", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton->setText(QApplication::translate("MainWindow", "Browse...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lineEdit->setToolTip(QApplication::translate("MainWindow", "Enter the path to the files to be used to make the disc image", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lineEdit->setText(QApplication::translate("MainWindow", "<input>", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_2->setToolTip(QApplication::translate("MainWindow", "Enter the output file path", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_2->setText(QApplication::translate("MainWindow", "Browse...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lineEdit_2->setToolTip(QApplication::translate("MainWindow", "Enter the output file path", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lineEdit_2->setText(QApplication::translate("MainWindow", "<output>", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "Input:", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "Output:", 0, QApplication::UnicodeUTF8));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Region <auto>", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "JAPAN", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "USA", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "EUROPE", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "KOREA", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        comboBox->setToolTip(QApplication::translate("MainWindow", "Set the region used for the output image.  This is independent of the TID.  If auto is selected, wit will try to guess the region based on the TID.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        label->setToolTip(QApplication::translate("MainWindow", "What IOS should be used in the TMD", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label->setText(QApplication::translate("MainWindow", "Game IOS", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        spinBox->setToolTip(QApplication::translate("MainWindow", "What IOS should be used in the TMD", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        checkBox_2->setToolTip(QApplication::translate("MainWindow", "Apply title & TID changes to the disc header", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        checkBox_2->setText(QApplication::translate("MainWindow", "Disc Hdr", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        checkBox_3->setToolTip(QApplication::translate("MainWindow", "Apply the title & TID changes to the partition header", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        checkBox_3->setText(QApplication::translate("MainWindow", "Partition Hdr", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        checkBox_4->setToolTip(QApplication::translate("MainWindow", "Apply the new TID to the TMD & ticket", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        checkBox_4->setText(QApplication::translate("MainWindow", "Tmd / Ticket", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        checkBox_6->setToolTip(QApplication::translate("MainWindow", "Do you want to change the title of the game?", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        checkBox_6->setText(QApplication::translate("MainWindow", "Update Title", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        checkBox_7->setToolTip(QApplication::translate("MainWindow", "Do you want to change the TID of the game?", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        checkBox_7->setText(QApplication::translate("MainWindow", "Update ID", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lineEdit_4->setToolTip(QApplication::translate("MainWindow", "Enter a new game name", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lineEdit_4->setText(QApplication::translate("MainWindow", "<Game Name>", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lineEdit_3->setToolTip(QApplication::translate("MainWindow", "Enter a new TID ( 6 characters )", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lineEdit_3->setText(QApplication::translate("MainWindow", "<ID>", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Compose Image", 0, QApplication::UnicodeUTF8));
        label_edit_id->setText(QString());
        label_edit_region->setText(QString());
        label_edit_name->setText(QString());
        edit_img_pushButton->setText(QApplication::translate("MainWindow", "Load Image", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(2, QApplication::translate("MainWindow", "Size", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(1, QApplication::translate("MainWindow", "Offset", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("MainWindow", "Name", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(Edit_tab), QApplication::translate("MainWindow", "Edit Image", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Text output:", 0, QApplication::UnicodeUTF8));
        verbose_combobox->clear();
        verbose_combobox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Normal", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Quiet", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Verbose", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Verbose+", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Verbose++", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Verbose+++", 0, QApplication::UnicodeUTF8)
        );
        label_3->setText(QApplication::translate("MainWindow", "Logging :", 0, QApplication::UnicodeUTF8));
        logging_combobox->clear();
        logging_combobox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Off", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Disc", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Disc + Part", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        def_ios_lbl->setToolTip(QApplication::translate("MainWindow", "Default IOS to use for booting: If this is less than 3, the IOS in the TMD will be used", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        def_ios_lbl->setText(QApplication::translate("MainWindow", "Default IOS:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        default_ios_spinbox->setToolTip(QApplication::translate("MainWindow", "Default IOS to use for booting: If this is less than 3, the IOS in the TMD will be used", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pushButton_settings_searchPath->setText(QApplication::translate("MainWindow", "Search Path...", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "Start at:", 0, QApplication::UnicodeUTF8));
        startupTab_combobox->clear();
        startupTab_combobox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Compose", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Edit", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Settings", 0, QApplication::UnicodeUTF8)
        );
        checkBox->setText(QApplication::translate("MainWindow", "Test Mode", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        overwrite_checkbox->setToolTip(QApplication::translate("MainWindow", "Tell wit to overwrite existing files", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        overwrite_checkbox->setText(QApplication::translate("MainWindow", "Overwrite Existing", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        checkBox_hiddenFiles->setToolTip(QApplication::translate("MainWindow", "Skip \".svn\" files in tree view to speed up parsing", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        checkBox_hiddenFiles->setText(QApplication::translate("MainWindow", "Ignore svn Files in Filetree", 0, QApplication::UnicodeUTF8));
        save_pushButton->setText(QApplication::translate("MainWindow", "Save Settings", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(settings_tab), QApplication::translate("MainWindow", "Settings", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(MainWindow);
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
