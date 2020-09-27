#include "markdownhtmlconverter.h"
#include <QDebug>
#include <QRegularExpression>

QString MarkdownHtmlConverter::convert(QString text)
{
    QList<QPair<QString, PatternHandler>> patterns = MarkdownHtmlConverter::patterns();

    for (auto pattern: patterns) {
        QRegularExpression regExp(pattern.first, QRegularExpression::MultilineOption);
        QRegularExpressionMatch match;

        while ((match = regExp.match(text)).hasMatch()) {
            QString replacement = pattern.second(match);
            text = text.replace(match.capturedStart(0), match.capturedLength(0), replacement);
        }
    }

//    return "<html><head><style>body {font-size: 12px;}</style></head><body>" + text + "</body></html>";
    return text;
}

QList<MarkdownRule> MarkdownHtmlConverter::patterns()
{
    QList<MarkdownRule> patterns;

    // ##### Heading
    patterns.append(MarkdownRule("^##### (.*)$", &MarkdownHtmlConverter::handleH5));

    // #### Heading
    patterns.append(MarkdownRule("^#### (.*)$", &MarkdownHtmlConverter::handleH4));

    // ### Heading
    patterns.append(MarkdownRule("^### (.*)$", &MarkdownHtmlConverter::handleH3));

    // ## Heading
    patterns.append(MarkdownRule("^## (.*)$", &MarkdownHtmlConverter::handleH2));

    // # Heading
    patterns.append(MarkdownRule("^# (.*)$", &MarkdownHtmlConverter::handleH1));

    // 1. First item
    // 1. Second item
    patterns.append(MarkdownRule("^(?:[0-9]+\\. .*\n?)+$", &MarkdownHtmlConverter::handleOrderedList));

    // - First item
    // - Second item
    patterns.append(MarkdownRule("^(?:\\- .*\n?)+$", &MarkdownHtmlConverter::handleUnorderedList));

    // ***, ---, ______ (Horizontal Rule)
    patterns.append(MarkdownRule("^([\\*]{3,}|[-]{3,}|[_]{3,})\\s*$", &MarkdownHtmlConverter::handleHorizontalRule));

    // **bold**
    patterns.append(MarkdownRule("\\*\\*([^\\n]+)\\*\\*", &MarkdownHtmlConverter::handleBold));

    // *italic*
    patterns.append(MarkdownRule("\\*([^\\n]+)\\*", &MarkdownHtmlConverter::handleItalic));

    // ~~strikethrough~~
    patterns.append(MarkdownRule("~~(.+)~~", &MarkdownHtmlConverter::handleStrikethrough));

    // ```lang
    // Fenced code block
    // ```
    patterns.append(MarkdownRule("^```(.*)\n((?:.*\n)*)``` *$",
                                 &MarkdownHtmlConverter::handleFencedCodeBlock));

    // `inline code`
    patterns.append(MarkdownRule("`(.*?)`", &MarkdownHtmlConverter::handleInlineCode));

    // [title](https://www.example.com)
    patterns.append(MarkdownRule("\\[([^\\]]*?)\\]\\((.*?)\\)", &MarkdownHtmlConverter::handleLink));

    // > Blockquote
    patterns.append(MarkdownRule("^> (.*)$", &MarkdownHtmlConverter::handleBlockquote));

    // new line
    patterns.append(MarkdownRule("\\\\\\n", &MarkdownHtmlConverter::handleNewLine));
    patterns.append(MarkdownRule("\\n\\n", &MarkdownHtmlConverter::handleNewLine));

    return patterns;
}

QString MarkdownHtmlConverter::handleOrderedList(QRegularExpressionMatch match)
{
    QString result = "<ol>";
    for (auto line: match.captured(0).split('\n')) {
        result += "<li>" + line.mid(3, line.length() - 3) + "</li>";
    }

    return result + "</ol>";
}

QString MarkdownHtmlConverter::handleUnorderedList(QRegularExpressionMatch match)
{
    QString result = "<ul>";
    for (auto line: match.captured(0).split('\n')) {
        result += "<li>" + line.mid(2, line.length() - 2) + "</li>";
    }

    return result + "</ul>";
}

QString MarkdownHtmlConverter::handleH1(QRegularExpressionMatch match)
{
    return "<h1>" + match.captured(1) + "</h1>";
}

QString MarkdownHtmlConverter::handleH2(QRegularExpressionMatch match)
{
    return "<h2>" + match.captured(1) + "</h2>";
}

QString MarkdownHtmlConverter::handleH3(QRegularExpressionMatch match)
{
    return "<h3>" + match.captured(1) + "</h3>";
}

QString MarkdownHtmlConverter::handleH4(QRegularExpressionMatch match)
{
    return "<h4>" + match.captured(1) + "</h4>";
}

QString MarkdownHtmlConverter::handleH5(QRegularExpressionMatch match)
{
    return "<h5>" + match.captured(1) + "</h5>";
}

QString MarkdownHtmlConverter::handleItalic(QRegularExpressionMatch match)
{
    return "<i>" + match.captured(1) + "</i>";
}

QString MarkdownHtmlConverter::handleBold(QRegularExpressionMatch match)
{
    return "<b>" + match.captured(1) + "</b>";
}

QString MarkdownHtmlConverter::handleNewLine(QRegularExpressionMatch match)
{
    return "<br>";
}

QString MarkdownHtmlConverter::handleInlineCode(QRegularExpressionMatch match)
{
    QString style;
    style += "padding: 2px 40px;";
    style += "background: rgb(45, 45, 45);";
    style += "border: 1px solid; border-radius: 4px;";

    return "<font face=\"Consolas\" style=\"" + style + "\">" + match.captured(1) + "</font>";
}

QString MarkdownHtmlConverter::handleLink(QRegularExpressionMatch match)
{
    QString style;
    style += "color: rgb(55, 148, 255);";

    return "<a style=\"" + style + "\" href=\"" + match.captured(2) + "\">" + match.captured(1) + "</a>";
}

QString MarkdownHtmlConverter::handleBlockquote(QRegularExpressionMatch match)
{
    return "<blockquote>" + match.captured(1) + "</blockquote>";
}

QString MarkdownHtmlConverter::handleHorizontalRule(QRegularExpressionMatch match)
{
    QString style;
    style += "background-color: lightgray;";
    style += "width: 100%;";
    style += "font-size: 1px;";

    return "<div style=\"" + style + "\"><div>";
}

QString MarkdownHtmlConverter::handleStrikethrough(QRegularExpressionMatch match)
{
    return "<s>" + match.captured(1) + "</s>";
}

QString MarkdownHtmlConverter::handleFencedCodeBlock(QRegularExpressionMatch match)
{
    QString style = "font-family: Consolas; background: rgb(45, 45, 45)";
    return "<pre style=\"" + style + "\">" + match.captured(2) + "</pre>";
}
