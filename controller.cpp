#include "controller.h"

#include "filereader.h"
#include "fileparser.h"
#include "settings.h"

Controller::Controller(QObject *parent) : QObject(parent)
{
    wordStatistics_ = new WordStatistics(this);
    parser_ = new FileParser;
    reader_ = new FileReader(parser_);

    //    QObject::connect(reader_, &FileReader::readyRead, parser_, &FileParser::parse);
    QObject::connect(reader_, &FileReader::progress, wordStatistics_, &WordStatistics::setNumberOfLines);
    QObject::connect(reader_, &FileReader::finished, parser_, &FileParser::setFileReadingFinished);

    QObject::connect(parser_, &FileParser::parsed, wordStatistics_, &WordStatistics::set);
    QObject::connect(parser_, &FileParser::progress, wordStatistics_, &WordStatistics::setNumberOfParsedLines);

    QObject::connect(parser_, &FileParser::finished, this, &Controller::parseFinished);
}

Controller::~Controller()
{
    stop();
    delete reader_;
    delete parser_;
}

bool Controller::start()
{
    wordStatistics_->clear();
    if(parser_->start()) {
        return reader_->start();
    }
    return false;
}

void Controller::stop()
{
    parser_->stop();
    reader_->stop();
}

void Controller::pause()
{
    parser_->pause();
    reader_->pause();
}

void Controller::unpause()
{
    parser_->unpause();
    reader_->unpause();
}

void Controller::clear()
{
    wordStatistics_->clear();
}

WordStatistics *Controller::wordStatistics() const
{
    return wordStatistics_;
}

