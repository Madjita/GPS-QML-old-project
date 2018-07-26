#include "clockcircle.h"

#include <QFileDialog>
#include <QXmlStreamReader>

ClockCircle::ClockCircle(QQuickItem *parent) :
    QQuickPaintedItem(parent),
    m_backgroundColor(Qt::white),
    m_borderActiveColor(Qt::blue),
    m_borderNonActiveColor(Qt::gray),
    m_angle(0),
    m_angleConst(0.06),
    m_circleTime(QTime(0,0,0,0))
{
    internalTimer = new QTimer(this);   // Инициализируем таймер
    /* А также подключаем сигнал от таймера к лямбда функции
     * Структура лямбда-функции [объект](аргументы){тело}
     * */
    connect(internalTimer, &QTimer::timeout, [=](){
        setAngle(angle() - m_angleConst); //0.06);//0.0016666666666667 -для 3 часов   0.003);                    // поворот определяется в градусах.
        setCircleTime(circleTime().addMSecs(50));   // Добавляем к текущему времени 50 милисекунд
        update();                                   // Перерисовываем объект
    });

    //Чтоб посчитать время и установить констунту нужно
    //Если 50 милисекунд это шаг тогда в 1 секунде 20 шагов
    // в 1 минуте 20*60 и т.д
    //Разделить 360 на получившееся число и установить константу в Angle();
}

void ClockCircle::paint(QPainter *painter)
{
    // Отрисовка объекта
    QBrush  brush(m_backgroundColor);               // выбираем цвет фона, ...
    QBrush  brushActive(m_borderActiveColor);       // активный цвет ободка, ...
    QBrush  brushNonActive(m_borderNonActiveColor); // не активный цвет ободка

    painter->setPen(Qt::NoPen);                             // Убираем абрис
    painter->setRenderHints(QPainter::Antialiasing, true);  // Включаем сглаживание

    painter->setBrush(brushNonActive);                          // Отрисовываем самый нижний фон в виде круга
    painter->drawEllipse(boundingRect().adjusted(1,1,-1,-1));   // с подгонкой под текущие размеры, которые
    // будут определяться в QML-слое.
    // Это будет не активный фон ободка

    // Прогресс бар будет формироваться с помощью отрисовки Pie графика
    painter->setBrush(brushActive);                         // Отрисовываем активный фон ободка в зависимости от угла поворота
    painter->drawPie(boundingRect().adjusted(1,1,-1,-1),    // с подгонкой под размеры в QML слое
                     90*16,         // Стартовая точка
                     m_angle*16);   // угол поворота, до которого нужно отрисовать объект

    painter->setBrush(brush);       // основной фон таймера, перекрытием которого поверх остальных
    painter->drawEllipse(boundingRect().adjusted(10,10,-10,-10));   // будет сформирован ободок (он же прогресс бар)
}

void ClockCircle::clear()
{
    setCircleTime(QTime(0,0,0,0));  // Очищаем время
    setAngle(0);                    // Выставляем поворот на ноль
    update();                       // Обновляем объект
    emit cleared();                 // ИСпускаем сигнал очистки
}

void ClockCircle::start()
{
    internalTimer->start(50);       // Запускаем таймер с шагом 50 мс


    QFile file("D:\\NewProject\\CustomQuickItem\\status.html");

    file.open(QIODevice::ReadWrite);


    QString str = file.readAll();
     int j=0;
    QString find;
    QString find2;

    while ((j = str.indexOf("<span id=\"program_step_remain_value\">", j)) != -1) {
        qDebug() << "Found <span id=\"monitor-temp-pv-value\" tag at index position" << j;
        ++j;

        for(int i=36; i < 60;i++)
        {
            find += str[j+i];
        }

    }


//    j=0;


//    while ((j = str.indexOf("<span id=\"date-value\">", j)) != -1) {
//        qDebug() << "Found <span id=\"monitor-temp-pv-value\" tag at index position" << j;
//        ++j;

//        for(int i=21; i < 50;i++)
//        {
//            find += str[j+i];
//        }

//    }

//     j=0;

//    while ((j = str.indexOf("<span id=\"operation-status-value\">", j)) != -1) {
//        qDebug() << "Found <span id=\"monitor-temp-pv-value\" tag at index position" << j;
//        ++j;

//        for(int i=33; i < 100;i++)
//        {
//            find += str[j+i];
//        }

//    }


//    j=0;

//    while ((j = str.indexOf("<span id=\"monitor-temp-sv-value\">", j)) != -1) {
//        qDebug() << "Found <span id=\"monitor-temp-pv-value\" tag at index position" << j;
//        ++j;

//        for(int i=32; i < 50;i++)
//        {
//            find2 += str[j+i];
//        }

//    }



    qDebug () << find.split(QLatin1Char('<')).first();
    qDebug () << find2;

    setName(find);

}

void ClockCircle::stop()
{
    internalTimer->stop();          // Останавливаем таймер
}

QString ClockCircle::name() const
{
    return m_name;
}

QColor ClockCircle::backgroundColor() const
{
    return m_backgroundColor;
}

QColor ClockCircle::borderActiveColor() const
{
    return m_borderActiveColor;
}

QColor ClockCircle::borderNonActiveColor() const
{
    return m_borderNonActiveColor;
}

qreal ClockCircle::angle() const
{
    return m_angle;
}

qreal ClockCircle::constAngle() const
{
    return m_angleConst;
}

QTime ClockCircle::circleTime() const
{
    return m_circleTime;
}

void ClockCircle::setName(const QString name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(name);
}

void ClockCircle::setBackgroundColor(const QColor backgroundColor)
{
    if (m_backgroundColor == backgroundColor)
        return;

    m_backgroundColor = backgroundColor;
    emit backgroundColorChanged(backgroundColor);
}

void ClockCircle::setBorderActiveColor(const QColor borderActiveColor)
{
    if (m_borderActiveColor == borderActiveColor)
        return;

    m_borderActiveColor = borderActiveColor;
    emit borderActiveColorChanged(borderActiveColor);
}

void ClockCircle::setBorderNonActiveColor(const QColor borderNonActiveColor)
{
    if (m_borderNonActiveColor == borderNonActiveColor)
        return;

    m_borderNonActiveColor = borderNonActiveColor;
    emit borderNonActiveColorChanged(borderNonActiveColor);
}

void ClockCircle::setAngle(const qreal angle)
{
    if (m_angle == angle)
        return;

    m_angle = angle;

    /* Данное добавление сделано для того,
     * чтобы обнулить поворот при достижении таймером
     * 60 секунд
     * */
    if(m_angle <= -360) m_angle = 0;
    emit angleChanged(m_angle);
}

void ClockCircle::setConstAngle(const qreal angel)
{
    if (m_angleConst == angel)
        return;

    //Чтоб посчитать время и установить констунту нужно
    //Если 50 милисекунд это шаг тогда в 1 секунде 20 шагов
    // в 1 минуте 20*60 и т.д
    //Разделить 360 на получившееся число и установить константу в Angle();


    m_angleConst = angel;

    emit constAngleChanged(m_angleConst);
}

void ClockCircle::setCircleTime(const QTime circleTime)
{
    if (m_circleTime == circleTime)
        return;

    m_circleTime = circleTime;
    emit circleTimeChanged(circleTime);
}
