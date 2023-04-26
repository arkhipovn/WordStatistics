#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QUrl>

#include "wordstatistics.h"

class FileReader;
class FileParser;

class Controller : public QObject
{
    Q_OBJECT

    Q_PROPERTY(WordStatistics *wordStatistics READ wordStatistics CONSTANT)

public:
    explicit Controller(QObject *parent = nullptr);
    ~Controller();

    WordStatistics *wordStatistics() const;

signals:
    void parseFinished();

public slots:
    bool start();
    void stop();
    void pause();
    void unpause();
    void clear();

private:
    FileReader *reader_;
    FileParser *parser_;
    WordStatistics *wordStatistics_;
};


#endif // CONTROLLER_H
