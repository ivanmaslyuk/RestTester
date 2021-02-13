#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include <QAction>
#include "Models/parammodel.h"
#include "Models/requestmodel.h"
#include "Delegates/requestlistdelegate.h"
#include "TreeModel/treemodel.h"
#include <QTreeWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->titleBar->hide();

    // Example request
    auto request = RequestModel();
    ui->endpoint->setRequestData(request);

    ui->treeView->setHeaderHidden(true);

    TreeModel *treeModel = new TreeModel(ui->treeView);
    ui->treeView->setModel(treeModel);

    ui->treeView->setExpandsOnDoubleClick(false);

    ui->treeView->setStyleSheet("QTreeView::branch {  border-image: url(none.png); }");

    auto requestListDelegate = new RequestListDelegate(ui->treeView);
    ui->treeView->setItemDelegate(requestListDelegate);
    connect(ui->treeView, &QTreeView::pressed, [=](QModelIndex index){
        TreeItem *treeItem = static_cast<TreeItem *>(index.internalPointer());
        RequestModel request = treeItem->data().value<RequestModel>();

        if (!request.method.isEmpty())
            ui->endpoint->setRequestData(request);

        if (ui->treeView->isExpanded(index))
            ui->treeView->collapse(index);
        else
            ui->treeView->expand(index);
    });


}

MainWindow::~MainWindow()
{
    delete ui;
}
