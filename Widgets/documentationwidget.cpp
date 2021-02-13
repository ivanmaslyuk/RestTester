#include "documentationwidget.h"
#include "ui_documentationwidget.h"
#include <QDebug>
#include <QRegExp>
#include <QRegularExpression>
#include "Utils/markdownhtmlconverter.h"

DocumentationWidget::DocumentationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DocumentationWidget)
{
    ui->setupUi(this);
    ui->rawText->hide();

    connect(ui->editButton, &QPushButton::clicked, this, &DocumentationWidget::editButtonPressed);
    ui->preview->setFocusPolicy(Qt::NoFocus);
}

DocumentationWidget::~DocumentationWidget()
{
    delete ui;
}

void DocumentationWidget::setContent(QString docs)
{
    this->content = docs;
    updateContent();
}

void DocumentationWidget::hide()
{
    QWidget::hide();
    if (isInEditingMode()) {
        showPreview();
        updateContent();
    }
}

void DocumentationWidget::updateContent()
{
    ui->rawText->setText(this->content);

//    QTextDocument *document = new QTextDocument(this);
//    document->setMarkdown(this->content);
//    ui->preview->setDocument(document);
    ui->preview->setHtml(MarkdownHtmlConverter::convert(this->content));
}

void DocumentationWidget::showEditor()
{
    ui->rawText->show();
    ui->preview->hide();
    ui->rawText->setFocus();
    ui->editButton->setText("Done");
}

void DocumentationWidget::showPreview()
{
    ui->preview->show();
    ui->rawText->hide();
    ui->editButton->setText("Edit");
}

bool DocumentationWidget::isInEditingMode()
{
    return ui->preview->isHidden();
}

void DocumentationWidget::editButtonPressed()
{
    if (isInEditingMode()) {
        this->content = ui->rawText->toPlainText();
        updateContent();
        showPreview();
        emit contentEdited(this->content);
    } else {
        showEditor();
    }
}
