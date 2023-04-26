#ifndef GLOBAL_H
#define GLOBAL_H

#include <QObject>
#include <QUrl>

class Settings : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)

public:
    static Settings* getInstance();

    QString fileName() const;

    void setFileName(const QString &fileName);

    Q_INVOKABLE void setFileUrl(const QUrl &file);
    Q_INVOKABLE bool fileExists() const;

    int numberOfWords() const;
    void setNumberOfWords(int numberOfWords);

    bool updateHistogramOnEveryIteration() const;
    void setUpdateHistogramOnEveryIteration(bool updateHistogramOnEveryIteration);

    int maxlenCharacters() const;
    void setMaxlenCharacters(int maxlenCharacters);

signals:
    void fileNameChanged();

private:
    explicit Settings(QObject *parent = nullptr);

    QString fileName_;      // Путь до файла

    /*
        Кол-во слов которое нужно вывести в статистике
    */
    int numberOfWords_;

    /*
        Если false, то интервал обновления гистограммы будет зависить от текущей итерации
    */
    bool updateHistogramOnEveryIteration_;

    int maxlenCharacters_;  // Максимальное количество символов считанного за раз
};

#endif // GLOBAL_H
