#ifndef WORD_H
#define WORD_H

#include <QString>
#include <QList>
#include <QDebug>

struct Word
{
    QString text;
    int count;

    friend bool operator <(const Word &l, const Word &r);
    friend bool operator >(const Word &l, const Word &r);
    friend QDebug operator <<(QDebug stream, const Word &word) { return stream << word.text << ":" << word.count; }

    Word& operator =(const Word &r);
};

using WordList = QVector <Word>;


#endif // WORD_H
