# WordStatistics

`Controller` - класс, через который идет взаимодействия интерфейса с логикой.

`WordStatistics` – модель списка слов и их количества.

`BarChart` – класс наследник от `QQuickPaintedItem` для отображения гистограммы. Тк в задании было сказано, что нежелательно использовать `ChartView` для отображения диаграммы решил нарисовать таким образом. Не будет считаться минусом такой подход? Если нужно, то могу нарисовать с помощью прямоугольников на `QML` или используя `Canvas`.

`Settings` – настройки программы. Включает:
- полный путь до выбранного файла;
- `numberOfWords` максимальное количество слов для отображения в статистике;
- `maxlenCharacters` максимальное количество символов, которое считывается за 1 раз из файла. Это нужно для перестраховки. Вдруг весь файл будет в одну строку;
- `updateHistogramOnEveryIteration`. Флаг нужен для того, чтобы данные обновлялись не после каждой итерации, а через какой-то промежуток. Необходимостью сделать это послужил способо подсчёта слов, который я избрал. Дело в том, что я сохраняю данные в хеш таблице. А позже нужно их отсортировать и выбрать N штук для отрисовки. Если делать данную операцию на каждом шаге подсчет слов будет осуществляется не быстро (зависаний не будет и картинка будет плавнее изменятся). Поэтому решил с увеличением итераций отображать изменения реже.

`FileHandler` – общий интерфейс для классов считывания и обработки данных.

`FileParser` – класс, который осуществляет обработку строк и отправляет данные в `WordStatistics`.

`FileReader` – класс, который считывает файл и отправляет данные в объект класса `FileParser`.

Сначала делал передачу данных между потоками по сигналу `FileReader::readyRead`. Позже столкнулся с некоторой особенностью. И в итоге сделал агрегацию (указатель на объект класса `FileParser` теперь есть в `FileReader`). И чтобы передать данные просто вызывают метод `FileParser::parse`. Так как после такого изменения данные записываются в очередь и считываются из нее в разных потоках добавил `QMutex`.
Особенность, из-за которой переделал:
Поток, в котором идет разбор строк постоянно занят. А тк по сигналу `FileReader::readyRead` слот будет выполняется в потоке, где идет разбор файла, слот вызывался не сразу, а через какое-то время, когда поток освободится (разберет все строки что уже есть). Из-за этого в этот момент было небольшое время, когда ничего не обрабатывалось и на гистограмме казалось, что все подвисло на короткий промежуток времени (на самом деле просто данных не было и поэтому не было обновления гистограммы). Выглядело это не очень красиво. Этого можно было избежать если периодически вызывать `delay()` во время выбора строк из очереди.
