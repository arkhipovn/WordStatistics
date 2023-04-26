#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QObject>
#include <QMutex>

class QThread;

// Общий интерфейс

class FileHandler : public QObject
{
    Q_OBJECT

public:
    explicit FileHandler(QObject *parent = nullptr);
    virtual ~FileHandler();

    enum class Status {
        Running,
        Paused,
        Stopped
    };

    Status status() const;

public slots:
    bool start();       // Запустить. Если поток уже запущен, то ничего сделано не будет
    void pause();       // Поставить на паузу
    void unpause();     // Снять с паузы
    void stop();        // Остановить считывания файла

signals:
    void finished();                        // Завершение обработки данных и сигнал для завершения потока
    void progress(uint numberOfLines);      // Текущий прогресс обработки данных. numberOfLines - кол-во обработанных строк

protected slots:
    virtual void handler() = 0;

protected:
    QThread *thread_;
    mutable QMutex mutex_;

    Status status_;

    void setStatus(const Status &status);
    void delay(int ms);
};


#endif // FILEHANDLER_H
