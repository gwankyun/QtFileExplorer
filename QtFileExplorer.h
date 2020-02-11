#pragma once

#include <QtWidgets/QMainWindow>
#include <memory>

class QtFileExplorer : public QMainWindow
{
    Q_OBJECT

public:
    QtFileExplorer(QWidget *parent = Q_NULLPTR);

private:
    struct Impl;
    std::shared_ptr<Impl> impl;
};
