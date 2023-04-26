#ifndef WORDSTATISTICS_H
#define WORDSTATISTICS_H

#include <QAbstractListModel>
#include "word.h"

class WordStatistics : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(uint numberOfLines READ numberOfLines NOTIFY numberOfLinesChanged)
    Q_PROPERTY(uint numberOfParsedLines READ numberOfParsedLines NOTIFY numberOfParsedLinesChanged)

public:
    explicit WordStatistics(QObject *parent = nullptr);

    enum Role {
        TextRole = Qt::UserRole + 1,
        CountRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    WordList wordList() const;

    void clear();
    void clearWordList();

    uint numberOfLines() const;
    uint numberOfParsedLines() const;

public slots:
    void set(const WordList &list);

    void setNumberOfLines(uint numberOfLines);
    void setNumberOfParsedLines(uint numberOfParsedLines);

signals:
    void wordListChanged();
    void numberOfLinesChanged();
    void numberOfParsedLinesChanged();

private:
    WordList wordList_;

    uint numberOfLines_;
    uint numberOfParsedLines_;
};


#endif // WORDSTATISTICS_H
