#include "filereader.h"

#include <QThread>

#include <QFile>
#include <QTextStream>

#include <QElapsedTimer>
#include <QDebug>

#include "fileparser.h"
#include "settings.h"

FileReader::FileReader(QObject *parent) : FileHandler(parent), parser_(nullptr)
{
}

FileReader::FileReader(FileParser *parser, QObject *parent)  : FileHandler(parent), parser_(parser)
{
}

FileReader::~FileReader()
{
    stop();
    if(thread_->isRunning()) {
        QElapsedTimer time; time.start();
        while(!thread_->isFinished() && time.elapsed() < 10000) delay(100);
    }
}

void FileReader::handler()
{
    if(Settings::getInstance()->fileExists()) {
        QFile file(Settings::getInstance()->fileName());

        if(file.open(QIODevice::ReadOnly)) {
            QTextStream stream(&file);
            QString line;

            uint numberOfIteration = 0;

            while(status() != Status::Stopped) {
                if(status() == Status::Paused) {
                    // Если пауза, то подождем немного
                    delay(100);
                }
                else {
                    if(stream.atEnd()) {
                        stop();
                    }
                    else {
                        line = stream.read(Settings::getInstance()->maxlenCharacters());
                        if(!line.isEmpty()) {
                            // emit readyRead(line);
                            if(parser_ != nullptr) parser_->parse(line);

                            if(++numberOfIteration % 100000 == 0)
                                emit progress(numberOfIteration);
                        }
                    }

                    //   if(stream.readLineInto(&line)) {
                    //       if(!line.isEmpty()) {
                    //           // emit readyRead(line);
                    //           if(parser_ != nullptr) parser_->parse(line);

                    //           if(++numberOfIteration % 100000 == 0)
                    //               emit progress(numberOfIteration);
                    //       }
                    //   }
                    //   else {
                    //       stop();
                    //   }
                }
            }

            emit progress(numberOfIteration);
            file.close();
        }
    }

    emit finished();
}
