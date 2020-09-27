#ifndef DOCUMENTATIONWIDGET_H
#define DOCUMENTATIONWIDGET_H

#include <QWidget>

namespace Ui {
class DocumentationWidget;
}

class DocumentationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DocumentationWidget(QWidget *parent = nullptr);
    ~DocumentationWidget();

    void setContent(QString docs);
    void hide();

private:
    Ui::DocumentationWidget *ui;
    QString content;

    void updateContent();
    void showEditor();
    void showPreview();
    bool isInEditingMode();

private slots:
    void editButtonPressed();

signals:
    void contentEdited(QString content);
};



#endif // DOCUMENTATIONWIDGET_H
