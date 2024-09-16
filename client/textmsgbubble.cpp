#include <QTextBlock>
#include <QFontMetricsF>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include "textmsgbubble.h"

TextMsgBubble::TextMsgBubble(ChattingRole role, const QString &text, QWidget *parent)
    :m_font("Microsoft YaHei")
    , m_text(new QTextEdit)
    , MsgBubbleBase(role, parent)
{
    /*perpare for style sheet*/
    m_text->setObjectName("text_edit");

    /*read only text*/
    m_text->setReadOnly(true);

    /*setup font*/
    m_font.setPointSize(10);
    m_text->setFont(m_font);

    /*disable scroll policy*/
    m_text->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_text->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_text->installEventFilter(this);

    /*add TextEdit widget to the widget inside bubble*/
    this->setupBubbleWidget(m_text);

    /*setup text for bubble*/
    this->setupTextMsg(text);

    /*init style sheet*/
    addStyleSheet();
}

TextMsgBubble::~TextMsgBubble(){}

bool TextMsgBubble::eventFilter(QObject *object, QEvent *event)
{
    if(object == m_text && event->type() == QEvent::Paint){
        //adjustBubbleSize();
    }
    return MsgBubbleBase::eventFilter(object, event);
}

void TextMsgBubble::adjustBubbleSize()
{
    qreal new_height {0};

    /*the margin of text to textedit border*/
    qreal text_to_border = m_text->document()->documentMargin();

    /*the margin of each line bloc(it should be )k*/
    int contents_margin = this->layout()->contentsMargins().top();

    QTextDocument* doc = m_text->document();

    /*refresh layout*/
    doc->adjustSize();

    /*traversal each line block of the text and get its height value*/
    for (QTextBlock it = doc->begin(); it != doc->end(); it = it.next())
    {
        QTextLayout *layout = it.layout();
        QRectF rect = layout->boundingRect();

        /*using a parameter to adjust height for user experience*/
        new_height += (rect.height() - COMPENSATION_VALUE);
    }

    //auto new_height = doc->documentLayout()->documentSize().height();
    qDebug() << "height = " << new_height;

    /* set the height of the bubble
     * 1.text height
     * 2.the margin of text
     * 3.the distance from QTextEdit to the Bubble frame
     */
    m_text->setMaximumHeight(new_height + 2 * (text_to_border + contents_margin));
}

void TextMsgBubble::addStyleSheet()
{
    m_text->setStyleSheet("#text_edit{color:black;background:transparent;border:none}");
}

void TextMsgBubble::setupTextMsg(const QString &str)
{
    /*set plain text*/
    m_text->setPlainText(str);

    /*calculate height*/
    qreal new_height {0};

    QTextDocument* doc = m_text->document();

    /*font settings, we have to know the size of font*/
    QFontMetricsF font_settings(m_text->font());

    /*the margin of text to textedit border*/
    auto text_to_border = m_text->document()->documentMargin();

    /*the margin of text which inside textedit*/
    auto inner_left =  this->layout()->contentsMargins().left();
    auto inner_right = this->layout()->contentsMargins().right();

    /*the margin of each line bloc(it should be )k*/
    int contents_margin = this->layout()->contentsMargins().top();

    /*record max length*/
    std::size_t max_width = 0;

   for(QTextBlock it = m_text->document()->begin(); it != m_text->document()->end(); it = it.next()){
        /*calculate the length of each line with the width of this font*/

       int txtW = int(font_settings.horizontalAdvance(it.text()));
       max_width = max_width < txtW ? txtW : max_width;  // Update with the longest width
   }

   /*set max width*/
   //setMaximumWidth(max_width + inner_left + inner_right + 2 * text_to_border);
   setMaximumWidth(max_width +1.5 * (inner_left + inner_right) + 2 * text_to_border);

    /*refresh layout*/
   doc->adjustSize();

   /*traversal each line block of the text and get its height value*/
   for (QTextBlock it = doc->begin(); it != doc->end(); it = it.next())
   {
       QTextLayout *layout = it.layout();
       QRectF rect = layout->boundingRect();

        /*using a parameter to adjust height for user experience*/
       new_height += (rect.height() - COMPENSATION_VALUE);
   }

   //auto new_height = doc->documentLayout()->documentSize().height();
   qDebug() << "height = " << new_height;

   m_text->setMaximumHeight(new_height + 2 * (text_to_border + contents_margin));
}
