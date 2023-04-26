#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <QQueue>
#include <QHash>

#include "filehandler.h"
#include "word.h"

class FileParser : public FileHandler
{
    Q_OBJECT

public:
    explicit FileParser(QObject *parent = nullptr);
    ~FileParser();

public slots:
    void parse(const QString &line);        // Слот для добавления в очередь
    void setFileReadingFinished();          // Говорим, что считывание файла завершено

signals:
    void parsed(const WordList &list);

protected slots:
    void handler() override;

private:
    QQueue <QString> queue_;                // Очередь строк для обработки
    QHash <QString, int> wordCounter_;      // Счетчик кол-ва слов
    bool fileReadingFinished_;              // Завершено ли считывание файла

    mutable QMutex queueMutex_;             // Мьютекс для работы с очередью

    QString dequeue();
    bool queueIsEmpty() const;
    void clear();

    bool checkStatUpdate(uint numberOfIteration) const;     // Для проверки нужно ли выполнять сортировку данных
    WordList statSort() const;                              // Сортировка данных и выбор N слов
};


#endif // FILEPARSER_H

