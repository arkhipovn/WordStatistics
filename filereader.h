#ifndef FILEREADER_H
#define FILEREADER_H

#include "filehandler.h"

class FileParser;

class FileReader : public FileHandler
{
    Q_OBJECT

public:
    explicit FileReader(QObject *parent = nullptr);
    explicit FileReader(FileParser *parser, QObject *parent = nullptr);
    ~FileReader();

    QString fileName() const;
    void setFileName(const QString &fileName);

signals:
    /*
        Сначала делал передачу данных между потоками по сигналу readyRead
        В этом случае агрегации FileParser в текущем классе не нужна (и ее не было)

        Столкнулся с такой особенностью:
        Поток, в котором идет разбор строк постоянно занят, а тк по сигналу (от FileReader) слот будет выполняется в потоке, где идет разбор файла,
        слот вызывался не сразу. А через какое-то время, когда поток освободится (разберет все строки что уже есть).
        Из-за этого в этот момент было небольшое время, когда ничего не обрабатывалось и на гистограмме казалось, что все подвисло на короткий промежуток времени
        (на самом деле просто данных не было и поэтому не было обновления гистограммы)
        Этого можно было избежать иногда вызывая delay() во время обработки данных, чтобы строки успевали добавится в очередь
    */
    //    void readyRead(const QString &line);

protected slots:
    void handler() override;

private:
    FileParser *parser_;
};

#endif // FILEREADER_H
