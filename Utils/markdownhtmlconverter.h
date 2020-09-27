#ifndef MARKDOWNHTMLCONVERTER_H
#define MARKDOWNHTMLCONVERTER_H

#include <QString>
#include <QList>


typedef QString (*PatternHandler)(QRegularExpressionMatch match);
typedef QPair<QString, PatternHandler> MarkdownRule;


class MarkdownHtmlConverter
{
public:
    static QString convert(QString text);

private:
    static QList<MarkdownRule> patterns();
    static QString handleOrderedList(QRegularExpressionMatch match);
    static QString handleUnorderedList(QRegularExpressionMatch match);
    static QString handleH1(QRegularExpressionMatch match);
    static QString handleH2(QRegularExpressionMatch match);
    static QString handleH3(QRegularExpressionMatch match);
    static QString handleH4(QRegularExpressionMatch match);
    static QString handleH5(QRegularExpressionMatch match);
    static QString handleItalic(QRegularExpressionMatch match);
    static QString handleBold(QRegularExpressionMatch match);
    static QString handleNewLine(QRegularExpressionMatch match);
    static QString handleInlineCode(QRegularExpressionMatch match);
    static QString handleLink(QRegularExpressionMatch match);
    static QString handleBlockquote(QRegularExpressionMatch match);
    static QString handleHorizontalRule(QRegularExpressionMatch match);
    static QString handleStrikethrough(QRegularExpressionMatch match);
    static QString handleFencedCodeBlock(QRegularExpressionMatch match);
};

#endif // MARKDOWNHTMLCONVERTER_H
