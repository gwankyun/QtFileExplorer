#include "stdafx.h"
#include "QtFileExplorer.h"
#include "ui_QtFileExplorer.h"
#include <utility>
#include <qdiriterator.h>
#include <qdir.h>
#include <qdebug.h>
#include <qvector.h>
#include <qfileinfo.h>
#include <qstring.h>
#include <qtreewidget.h>
#include <map>

struct Item
{
    enum
    {
        PATH = 0,
        NAME = 1,
        SIZE = 2
    };
};

struct QtFileExplorer::Impl
{
    Ui::QtFileExplorerClass ui;
};

QtFileExplorer::QtFileExplorer(QWidget *parent)
    : QMainWindow(parent)
{
    impl = std::make_shared<Impl>();
    auto& ui = impl->ui;
    ui.setupUi(this);

    ui.rightTreeWidget->setColumnWidth(Item::SIZE, 150);

    ui.rightTreeWidget->setColumnHidden(Item::PATH, true);

    ui.leftTreeWidget->setColumnHidden(1, true);

    auto header = ui.rightTreeWidget->header();
    header->setSectionResizeMode(Item::NAME, QHeaderView::Stretch);
    header->setSectionResizeMode(Item::SIZE, QHeaderView::Fixed);
    header->setStretchLastSection(false);

    enter("D:/");

    connect(ui.rightTreeWidget, &QTreeWidget::itemDoubleClicked,
        this, &QtFileExplorer::on_itemDoubleClicked);

    connect(ui.leftTreeWidget, &QTreeWidget::itemDoubleClicked,
        this, &QtFileExplorer::on_itemDoubleClicked);
}

void QtFileExplorer::enter(const QString& _path)
{
    qDebug() << _path;
    auto& ui = impl->ui;
    ui.rightTreeWidget->clear();

    auto item = new QTreeWidgetItem(ui.rightTreeWidget);
    item->setText(Item::NAME, "..");
    item->setText(Item::PATH, _path);

    QString path(_path);
    QDirIterator iter(path, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    while (iter.hasNext())
    {
        auto& str = iter.next();
        QFileInfo fileInfo(iter.fileInfo());
        auto righTitem = new QTreeWidgetItem(ui.rightTreeWidget);

        righTitem->setText(Item::PATH, fileInfo.absoluteFilePath());
        righTitem->setText(Item::NAME, iter.fileName());
        if (fileInfo.isFile())
        {
            righTitem->setText(Item::SIZE, QString::number(fileInfo.size()));
        }
        else
        {
            auto leftItem = new QTreeWidgetItem(ui.leftTreeWidget);
            leftItem->setText(1, fileInfo.absoluteFilePath());
            leftItem->setText(0, iter.fileName());
        }
    }
}

void QtFileExplorer::on_itemDoubleClicked(QTreeWidgetItem* item, int column)
{
    auto objectName = sender()->objectName();
    qDebug() << item->text(Item::NAME);
    qDebug() << objectName;

    if (objectName == "rightTreeWidget")
    {
        QFileInfo fileInfo(item->text(Item::PATH));

        if (item->text(Item::NAME) == "..")
        {
            enter(fileInfo.absolutePath());
            return;
        }

        if (fileInfo.isDir())
        {
            enter(fileInfo.absoluteFilePath());
        }
    }
    else if (objectName == "leftTreeWidget")
    {
        QDirIterator iter(item->text(1), QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
        while (iter.hasNext())
        {
            auto& str = iter.next();
            QFileInfo fileInfo(iter.fileInfo());

            if (fileInfo.isDir())
            {
                auto leftItem = new QTreeWidgetItem(item);
                leftItem->setText(1, fileInfo.absoluteFilePath());
                leftItem->setText(0, iter.fileName());
            }
        }
    }
}

