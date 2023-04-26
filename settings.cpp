#include "settings.h"

#include <QFile>
#include <QDebug>

Settings *Settings::getInstance()
{
    static Settings settings;
    return &settings;
}

Settings::Settings(QObject *parent) : QObject(parent),
    numberOfWords_(15),
    updateHistogramOnEveryIteration_(false),
    maxlenCharacters_(100)
{
}

int Settings::maxlenCharacters() const
{
    return maxlenCharacters_;
}

void Settings::setMaxlenCharacters(int maxlenCharacters)
{
    maxlenCharacters_ = maxlenCharacters;
}

bool Settings::updateHistogramOnEveryIteration() const
{
    return updateHistogramOnEveryIteration_;
}

void Settings::setUpdateHistogramOnEveryIteration(bool updateHistogramOnEveryIteration)
{
    updateHistogramOnEveryIteration_ = updateHistogramOnEveryIteration;
}

int Settings::numberOfWords() const
{
    return numberOfWords_;
}

void Settings::setNumberOfWords(int numberOfWords)
{
    numberOfWords_ = numberOfWords;
}

QString Settings::fileName() const
{
    return fileName_;
}

void Settings::setFileUrl(const QUrl &file)
{
    setFileName(file.toLocalFile());
}

void Settings::setFileName(const QString &fileName)
{
    if(fileName_ != fileName) {
        fileName_ = fileName;
        emit fileNameChanged();
    }
}

bool Settings::fileExists() const
{
    return !fileName_.isEmpty() &&
            QFile::exists(fileName_);
}
