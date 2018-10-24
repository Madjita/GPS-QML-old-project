#include "bdata.h"


/*!
     \brief Конструктор класса (инициализация)
     \param queryModel - зануляем переменную для запросов к БД (инициализация)
*/
BData::BData()
{
    queryModel = nullptr; // зануляем переменную для запросов к БД (инициализация)


    connect(this,&BData::sendZapros,this,&BData::writeBD,Qt::DirectConnection);

}

void BData::getZapros(QString data)
{
    emit sendZapros(data);
}

//************************************************
//Функция подключения к БД
QString BData::connectBD()
{
    //Проверка на существование драйвера QSQLITE для подключения к БД
    if (!QSqlDatabase::drivers().contains("QSQLITE"))
    {
        //Вывести ошибку подключения
        qDebug() << "This demo needs the QODBC driver";
    }

    // QMessageBox::critical(BData::pa, "Unable to load database", "This demo needs the QODBC driver");

    db = new QSqlDatabase();

    //Инициализация БД как (QSQLITE)
   *db = QSqlDatabase::addDatabase("QSQLITE");
    // Настройка пути до БД в корень папки программы
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    // Настройка пути до БД в корень папки программы до самой  БД
    QString pathToDB = QDir::currentPath()+QString("/Gps.db");
    // Подключение БД
    db->setDatabaseName(pathToDB);

    // Информация о файле
    //  QFileInfo checkFile(pathToDB);

    //Проверка открытия БД
    if(!db->open())
    {
        qDebug()<<"ERROR: "<<QSqlError( db->lastError()).text();

        QMessageBox::warning(nullptr,"Error","Couldn't open setting database");

        return "false";
    }
    else
    {
        //Если БД открыта, то инициализация переменных запросов для БД
        query = new QSqlQuery(*db);
        queryModel = new QSqlQueryModel();
        queryModel_1 = new QSqlQueryModel();
        queryModel_2 = new QSqlQueryModel();
        queryModel_3 = new QSqlQueryModel();
        queryModel_4 = new QSqlQueryModel();
        queryModel_5 = new QSqlQueryModel();
        queryModel_6 = new QSqlQueryModel();
        queryModel_7 = new QSqlQueryModel();
        queryModel_8 = new QSqlQueryModel();

        qDebug()<<"База данных загружена [OK]. Рабочая папка: " << db->databaseName();


    }

    return "true";
}
//************************************************


//************************************************
//Отключение БД
void BData::DiscConnect()
{
    qDebug() << db->defaultConnection;

    if(db->isOpen())
    {
        db->close();
    }

    QSqlDatabase::removeDatabase(db->defaultConnection);

    delete  query;
    delete queryModel ;
    delete queryModel_1;
    delete queryModel_2;
    delete queryModel_3;
    delete queryModel_4;
    delete queryModel_5;
    delete queryModel_6;
    delete queryModel_7;
    delete queryModel_8;

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

    return nullptr;
}
//************************************************

//************************************************
// Запрос к БД
// Принимает строку SQL запроса
QSqlQuery BData::zapros(QString data)
{
    DiscConnect();
    connectBD();


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
    DiscConnect();
    connectBD();


    queryModel->setQuery(data);

    return queryModel;
}
//************************************************

void BData::writeBD(QString data)
{
    DiscConnect();
    connectBD();

    queryModel->setQuery(data);
}


//************************************************
//Запрос на запись данных в БД по каждой плате
QSqlQueryModel* BData::zaprosQueryModel2(QString data, int plata)
{

    DiscConnect();
    connectBD();


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

    return queryModel_1;
}

void BData::updateIncrementTable(QString nameTable)
{
    queryModel->setQuery("UPDATE sqlite_sequence set seq=0 WHERE Name='"+nameTable+"'");
}

QStringList BData::allTable()
{
    DiscConnect();
    connectBD();


    QStringList listTable;


    queryModel->setQuery("select * from sqlite_master where type ='table'");

    for(int i=0; queryModel->rowCount();i++)
    {
        listTable << queryModel->data(queryModel->index(i,1), Qt::EditRole).toString();
    }

    return listTable;

}
//************************************************


//************************************************
// Запрос на поиск ID приборов (имя )
QString BData::Id_Find(QString data)
{
    DiscConnect();
    connectBD();


    //Загрузка Этапов

    if(queryModel != nullptr)
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

QStringList BData::report(QString data, int column)
{

    DiscConnect();
    connectBD();


    QStringList list;

    queryModel->setQuery(data);

    for(int i=0;i< queryModel->rowCount();i++)
    {
        list.append(queryModel->data(queryModel->index(i,column), Qt::EditRole).toString());
    }

    return list;
}
//************************************************
