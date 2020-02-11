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

    ui.rightTreeWidget->setColumnWidth(1, 200);

    auto header = ui.rightTreeWidget->header();
    header->setSectionResizeMode(0, QHeaderView::Stretch);
    header->setSectionResizeMode(1, QHeaderView::Fixed);
    header->setStretchLastSection(false);

    QString path("D:/");
    QDirIterator iter(path, QDir::Files | QDir::Dirs);
    while (iter.hasNext())
    {
        iter.next();
        QFileInfo fileInfo(iter.fileInfo());
        auto item = new QTreeWidgetItem(ui.rightTreeWidget);
        item->setText(0, iter.fileName());
        if (fileInfo.isFile())
        {
            item->setText(1, QString::number(fileInfo.size()));
        }
    }
}
