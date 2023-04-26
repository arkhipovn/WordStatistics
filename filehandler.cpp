#include "filehandler.h"

#include <QElapsedTimer>
#include <QEventLoop>
#include <QTimer>

#include <QThread>
#include <QMutexLocker>

FileHandler::FileHandler(QObject *parent) : QObject(parent),
    status_(Status::Stopped)
{
    thread_ = new QThread;
    this->moveToThread(thread_);

    QObject::connect(thread_, &QThread::started, this, &FileHandler::handler);
    QObject::connect(this, &FileHandler::finished, thread_, &QThread::quit);
}

FileHandler::~FileHandler()
{
    // Ожидание завершения будет идти в наследниках..
    delete thread_; // thread_->deleteLater();
}

bool FileHandler::start()
{
    if(thread_->isRunning() ||
            status() != Status::Stopped) {
        return false;
    }
    setStatus(Status::Running);
    thread_->start();
    return true;
}

void FileHandler::pause()
{
    if(thread_->isRunning() && status() == Status::Running) {
        setStatus(Status::Paused);
    }
}

void FileHandler::unpause()
{
    if(thread_->isRunning() && status() == Status::Paused) {
        setStatus(Status::Running);
    }
}

void FileHandler::stop()
{
    setStatus(Status::Stopped);
}

FileHandler::Status FileHandler::status() const
{
    const QMutexLocker locker(&mutex_);
    return status_;
}

void FileHandler::setStatus(const Status &status)
{
    const QMutexLocker locker(&mutex_);
    status_ = status;
}

void FileHandler::delay(int ms)
{
    QEventLoop loop;
    QTimer::singleShot(ms, &loop, SLOT(quit()));
    loop.exec();
}
