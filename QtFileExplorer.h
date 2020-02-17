#pragma once

#include <QtWidgets/QMainWindow>
#include <memory>

class QtFileExplorer : public QMainWindow
{
    Q_OBJECT

public:
    QtFileExplorer(QWidget *parent = Q_NULLPTR);

private slots:
    void on_itemDoubleClicked(QTreeWidgetItem* item, int column);

private:
    struct Impl;
    std::shared_ptr<Impl> impl;

    void enter(const QString& path);
};
