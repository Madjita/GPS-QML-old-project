#include "bdata.h"


//************************************************
//Конструктор
BData::BData()
{
    queryModel = NULL; // зануляем переменную для запросов к БД (инициализация)

}
//************************************************

//************************************************
//Функция подключения к БД
QString BData::connect()
{
    //Проверка на существование драйвера QSQLITE для подключения к БД
    if (!QSqlDatabase::drivers().contains("QSQLITE"))
    {
        //Вывести ошибку подключения
        qDebug() << "This demo needs the QODBC driver";

    }

    // QMessageBox::critical(BData::pa, "Unable to load database", "This demo needs the QODBC driver");

    //Инициализация БД как (QSQLITE)
    db = QSqlDatabase::addDatabase("QSQLITE");
    // Настройка пути до БД в корень папки программы
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    // Настройка пути до БД в корень папки программы до самой  БД
    QString pathToDB = QDir::currentPath()+QString("/Gps.db");
    // Подключение БД
    db.setDatabaseName(pathToDB);

    // Информация о файле
  //  QFileInfo checkFile(pathToDB);

    //Проверка открытия БД
    if(! db.open())
    {
        qDebug()<<"ERROR: "<<QSqlError( db.lastError()).text();

        return "false";
    }
    else
    {
        //Если БД открыта, то инициализация переменных запросов для БД
        query = new QSqlQuery(db);
        queryModel = new QSqlQueryModel();
        queryModel_1 = new QSqlQueryModel();
        queryModel_2 = new QSqlQueryModel();
        queryModel_3 = new QSqlQueryModel();
        queryModel_4 = new QSqlQueryModel();
        queryModel_5 = new QSqlQueryModel();
        queryModel_6 = new QSqlQueryModel();
        queryModel_7 = new QSqlQueryModel();
        queryModel_8 = new QSqlQueryModel();

        qDebug()<<"База данных загружена [OK]. Рабочая папка: " << db.databaseName();

    }

//    QTime midnight(0,0,0);
//    qsrand(midnight.secsTo(QTime::currentTime()));

//    timer = new QTimer();
//    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(work()));


    return "true";
}
//************************************************


//************************************************
//Отключение БД
QString BData::DiscConnect()
{
    db.close();
}
//************************************************

//************************************************
//Создать таблицу в БД
// Передается имя таблицы и QVector<QString> - массив данных
QString BData::create(QString name, QVector<QString> list)
{
    QString data = "CREATE TABLE "+name+" (";

    for(int i=0;i<list.count();i++)
    {
        data +=list[i];
    }

    data +=")";

    query->exec(data);



    QString error = query->lastError().databaseText();

    if(error != "")
    {
        return error;
    }

    return 0;
}
//************************************************

//************************************************
// Запрос к БД
// Принимает строку SQL запроса
QSqlQuery BData::zapros(QString data)
{
    query->exec(data);

    QString error = query->lastError().databaseText();

    if(error != "")
    {
        return *query;
    }


    QSqlQuery q = *query;

    q.first();

    return q;
}
//************************************************

//************************************************
//Запрос к БД (использую в программе )
QSqlQueryModel* BData::zaprosQueryModel(QString data)
{
    queryModel->setQuery(data);

    return queryModel;
}
//************************************************


//************************************************
//Запрос на запись данных в БД по каждой плате
QSqlQueryModel *BData::zaprosQueryModel(QString data, int plata)
{


    switch (plata)
    {
    case 1:
    {

        queryModel_1->setQuery(data);
        return queryModel_1;
    }
    case 2:
    {

        queryModel_2->setQuery(data);
        return queryModel_2;
    }
    case 3:
    {

        queryModel_3->setQuery(data);
        return queryModel_3;
    }
    case 4:
    {

        queryModel_4->setQuery(data);
        return queryModel_4;
    }
    case 5:
    {

        queryModel_5->setQuery(data);
        return queryModel_5;
    }
    case 6:
    {

        queryModel_6->setQuery(data);
        return queryModel_6;
    }
    case 7:
    {

        queryModel_7->setQuery(data);
        return queryModel_7;
    }
    case 8:
    {

        queryModel_8->setQuery(data);
        return queryModel_8;
    }

    }
}
//************************************************


//************************************************
// Запрос на поиск ID приборов (имя )
QString BData::Id_Find(QString data)
{


    //Загрузка Этапов

    if(queryModel != NULL)
        delete queryModel;

    queryModel = new QSqlQueryModel();

    queryModel->setQuery(data);


    QString Id;

    for(int i=0;i< queryModel->rowCount();i++)
    {
        Id = queryModel->data(queryModel->index(i,0), Qt::EditRole).toString();
    }


    return Id;
}
//************************************************
