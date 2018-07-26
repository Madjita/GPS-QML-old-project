#ifndef BDATA_H
#define BDATA_H

#include <QString>
#include <QObject>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDir>
#include <QCoreApplication>
#include <QSqlResult>
#include <QTableWidget>
#include <QSqlQueryModel>
#include <QTextEdit>
#include <QThread>

// Класс Базы данных
class BData : public QObject
{
    // макрос объекта для реализациии сигнал слотовых соединений.
    Q_OBJECT

public:
    // Инициализация консткрутора
    BData();


    //База данных
    QSqlDatabase db;

    //Запрос к базе:
    QSqlQuery* query;
    QSqlQueryModel* queryModel;

    QSqlQueryModel* queryModel_1;
    QSqlQueryModel* queryModel_2;
    QSqlQueryModel* queryModel_3;
    QSqlQueryModel* queryModel_4;
    QSqlQueryModel* queryModel_5;
    QSqlQueryModel* queryModel_6;
    QSqlQueryModel* queryModel_7;
    QSqlQueryModel* queryModel_8;


//Слоты
public slots:

    //Подключение БД
    QString connect();
    //Отключение БД
    QString DiscConnect();
    //Создание Таблицы в БД
    QString create(QString,QVector<QString>);
    //Запрос в БД
    QSqlQuery zapros(QString);
    // Модельный запрос в БД
    QSqlQueryModel* zaprosQueryModel(QString);
    // Модельный запрос в БД (перегрузка) для записи по каждой плате
    QSqlQueryModel* zaprosQueryModel(QString,int);

    // Запрос на поиск ID устрйоств по имени
    QString Id_Find(QString);

};

#endif // BDATA_H
