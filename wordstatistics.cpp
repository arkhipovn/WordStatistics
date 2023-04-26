#include "wordstatistics.h"

WordStatistics::WordStatistics(QObject *parent) : QAbstractListModel(parent),
    numberOfLines_(0),
    numberOfParsedLines_(0)
{

}

void WordStatistics::set(const WordList &list)
{
    if(wordList_.size() == list.size()) {
        wordList_ = list;
        emit dataChanged(index(0, 0), index(list.size() - 1, 0), { TextRole, CountRole });
    }
    else {
        clearWordList();
        beginInsertRows(QModelIndex(), 0, list.size() - 1);
        wordList_ = list;
        endInsertRows();
    }

    emit wordListChanged();
}

int WordStatistics::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return wordList_.size();
}

QVariant WordStatistics::data(const QModelIndex &index, int role) const
{
    QVariant v;
    if(!index.isValid())
        return v;

    switch (role) {
        case TextRole:  return wordList_.at(index.row()).text;
        case CountRole: return wordList_.at(index.row()).count;
        default:        return v;
    }
}

QHash<int, QByteArray> WordStatistics::roleNames() const
{
    static const QHash <int, QByteArray> roles = QHash <int, QByteArray> {
        { TextRole, "wordText" },
        { CountRole, "wordCount" }
    };
    return roles;
}

uint WordStatistics::numberOfParsedLines() const
{
    return numberOfParsedLines_;
}

void WordStatistics::setNumberOfParsedLines(uint numberOfParsedLines)
{
    if(numberOfParsedLines_ != numberOfParsedLines) {
        numberOfParsedLines_ = numberOfParsedLines;
        emit numberOfParsedLinesChanged();
    }
}

uint WordStatistics::numberOfLines() const
{
    return numberOfLines_;
}

void WordStatistics::setNumberOfLines(uint numberOfLines)
{
    if(numberOfLines_ != numberOfLines) {
        numberOfLines_ = numberOfLines;
        emit numberOfLinesChanged();
    }
}

WordList WordStatistics::wordList() const
{
    return wordList_;
}

void WordStatistics::clear()
{
    clearWordList();
    setNumberOfLines(0);
    setNumberOfParsedLines(0);
}

void WordStatistics::clearWordList()
{
    if(!wordList_.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, wordList_.size() - 1);
        wordList_.clear();
        endRemoveRows();
        emit wordListChanged();
    }
}
