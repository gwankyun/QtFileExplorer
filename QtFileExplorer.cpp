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

    auto header = ui.rightTreeWidget->header();
    header->setSectionResizeMode(Item::NAME, QHeaderView::Stretch);
    header->setSectionResizeMode(Item::SIZE, QHeaderView::Fixed);
    header->setStretchLastSection(false);

    enter("D:/");

    connect(ui.rightTreeWidget, &QTreeWidget::itemDoubleClicked,
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
        auto item = new QTreeWidgetItem(ui.rightTreeWidget);
        item->setText(Item::PATH, fileInfo.absoluteFilePath());
        item->setText(Item::NAME, iter.fileName());
        if (fileInfo.isFile())
        {
            item->setText(Item::SIZE, QString::number(fileInfo.size()));
        }
    }
}

void QtFileExplorer::on_itemDoubleClicked(QTreeWidgetItem* item, int column)
{
    qDebug() << item->text(Item::NAME);

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

