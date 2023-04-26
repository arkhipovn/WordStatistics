#include "fileparser.h"

#include <QThread>
#include <QMutexLocker>
#include <QElapsedTimer>

#include "settings.h"


FileParser::FileParser(QObject *parent) : FileHandler(parent),
    fileReadingFinished_(false)
{

}

FileParser::~FileParser()
{
    stop();
    if(thread_->isRunning()) {
        QElapsedTimer time; time.start();
        while(!thread_->isFinished() && time.elapsed() < 10000) delay(100);
    }
}

void FileParser::parse(const QString &line)
{
    if(status() != Status::Stopped) {
        const QMutexLocker locker(&queueMutex_);
        queue_.append(line);
    }
}

void FileParser::setFileReadingFinished()
{
    fileReadingFinished_ = true;
}

void FileParser::clear()
{
    fileReadingFinished_ = false;
    wordCounter_.clear();
    const QMutexLocker locker(&queueMutex_);
    queue_.clear();
}

bool FileParser::checkStatUpdate(uint numberOfIteration) const
{
    if(numberOfIteration < 1000) {
        return true;
    }
    else if(numberOfIteration < 30000) {
        if(numberOfIteration % 1000 == 0)
            return true;
    }
    else if(numberOfIteration < 70000) {
        if(numberOfIteration % 5000 == 0)
            return true;
    }
    else {
        if(numberOfIteration % 10000 == 0)
            return true;
    }
    return false;
}

WordList FileParser::statSort() const
{
    WordList list;
    // Перепишем в вектор из хеш
    for(auto it = wordCounter_.constBegin(); it != wordCounter_.constEnd(); ++it) {
        list.append({ it.key(), it.value() });
    }

    // Отсортируем
    std::sort(list.begin(), list.end(), std::greater<Word>());

    // Возьмем первые 15
    if(list.size() > Settings::getInstance()->numberOfWords())
        list.resize(Settings::getInstance()->numberOfWords());
    return list;
}

void FileParser::handler()
{
    clear();    // Проверим нужно ли очистить от предыдущего раза..

    uint numberOfIteration = 0;
    bool update = Settings::getInstance()->updateHistogramOnEveryIteration();

    QString text;

    while(status() != Status::Stopped) {
        if(status() == Status::Paused) {
            // Если пауза, то подождем немного
            delay(100);
        }
        else if(queueIsEmpty()) {
            // Если нет данных, а считывание файла еще в процессе, то подождем немного
            if(fileReadingFinished_) {
                if(!text.isEmpty()) {
                    // Если текст не закончился а файл закончился
                    ++wordCounter_[text];   // увеличим счетчик слов
                }

                auto wordList = statSort();
                emit parsed(wordList);
                emit progress(numberOfIteration);
                stop();
            }
            else {
                delay(10);
            }
        }
        else {
            text += dequeue();

            int position = 0;
            int size = 0;

            // Пройдемся по строке и найдем первый символ
            for(int i = 0; i < text.size(); ++i) {
                if(text.at(i).isLetter()) {
                    if(text.at(i).isUpper()) {  // Если с большой буквы написано, то изменим ее на маленькую
                        text[i] = text.at(i).toLower();
                    }

                    if(size == 0) {
                        position = i;
                    }
                    ++size;
                }
                else if(size > 0) {
                    // Нашли разделитель
                    ++wordCounter_[text.mid(position, size)];   // увеличим счетчик слов
                    size = 0;
                }
            }

            if(size > 0) {
                // Что-то осталось, а разделитель не нашли значит посмотрим дальше что придет
                text = text.mid(position, size);
            }
            else {
                // Считали все что нужно
                text.clear();   // Забудем про это строку
            }

            ++numberOfIteration;

            if(!Settings::getInstance()->updateHistogramOnEveryIteration()) {    // Проверим нужно ли сейчас обновлять статистику
                update = checkStatUpdate(numberOfIteration);
            }

            if(update) {
                auto wordList = statSort();
                emit parsed(wordList);
                emit progress(numberOfIteration);
            }
        }
    }

    clear();
    emit finished();
}

QString FileParser::dequeue()
{
    const QMutexLocker locker(&queueMutex_);
    return queue_.dequeue();
}

bool FileParser::queueIsEmpty() const
{
    const QMutexLocker locker(&queueMutex_);
    return queue_.isEmpty();
}


