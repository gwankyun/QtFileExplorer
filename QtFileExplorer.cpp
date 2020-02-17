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

struct QtFileExplorer::Impl
{
    Ui::QtFileExplorerClass ui;
    std::map<QTreeWidgetItem*, QFileInfo> itemMap;
    QString current;
};

QtFileExplorer::QtFileExplorer(QWidget *parent)
    : QMainWindow(parent)
{
    impl = std::make_shared<Impl>();
    auto& ui = impl->ui;
    ui.setupUi(this);

    ui.rightTreeWidget->setColumnWidth(1, 150);

    auto header = ui.rightTreeWidget->header();
    header->setSectionResizeMode(0, QHeaderView::Stretch);
    header->setSectionResizeMode(1, QHeaderView::Fixed);
    header->setStretchLastSection(false);

    enter("D:/");

    connect(ui.rightTreeWidget, &QTreeWidget::itemDoubleClicked,
        this, &QtFileExplorer::on_itemDoubleClicked);
}

void QtFileExplorer::enter(const QString& _path)
{
    qDebug() << _path;
    impl->current = _path;
    auto& ui = impl->ui;
    ui.rightTreeWidget->clear();
    impl->itemMap.clear();

    auto item = new QTreeWidgetItem(ui.rightTreeWidget);
    item->setText(0, "..");

    QString path(_path);
    QDirIterator iter(path, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    while (iter.hasNext())
    {
        auto& str = iter.next();
        QFileInfo fileInfo(iter.fileInfo());
        auto item = new QTreeWidgetItem(ui.rightTreeWidget);
        item->setText(0, iter.fileName());
        if (fileInfo.isFile())
        {
            item->setText(1, QString::number(fileInfo.size()));
        }
        impl->itemMap[item] = fileInfo;
    }
}

void QtFileExplorer::on_itemDoubleClicked(QTreeWidgetItem* item, int column)
{
    qDebug() << item->text(0);

    if (item->text(0) == "..")
    {
        enter(QFileInfo(impl->current).absolutePath());
        return;
    }

    auto& iter = impl->itemMap.find(item);
    if (iter != impl->itemMap.end())
    {
        auto& fileInfo = iter->second;
        if (fileInfo.isDir())
        {
            qDebug() << iter->second.filePath();
            enter(fileInfo.filePath());
        }
    }
}

