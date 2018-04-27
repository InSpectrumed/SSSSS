#include "pagehandler.h"


PageHandler::PageHandler(QObject *parent)
    : QObject(parent), m_manager(new QNetworkAccessManager(this))
{

   m_fUSD = new QFile("1.txt");

   if(!m_fUSD->open(QIODevice::WriteOnly))
   {
       qDebug() << "No OPEN.";

       return;
   }


   QDateTime startDate(QDate(2016, 1, 1), QTime(0, 0, 0));
   //QDateTime ReturnDate = startDate.addDays(1);
   QDateTime endDate(QDate(2018, 1, 1), QTime(0, 0, 0));
   //qDebug() << ReturnDate.toString ("yyyy-MM-dd") << " Days from startDate to endDate: " << startDate.daysTo(endDate);

   int a_iSize = startDate.daysTo(endDate);

   QString a_sWrit;
   for(int i = 0; i < a_iSize; i++)
   {

       a_sWrit = startDate.toString ("yyyy-MM-dd") + " ";
       m_fUSD->write( a_sWrit.toLocal8Bit() );
       fGet("https://index.minfin.com.ua/exchange/archive/privat/" + startDate.toString ("yyyy-MM-dd"));
       qDebug() << startDate.toString ("yyyy-MM-dd");

       startDate = startDate.addDays(1);
   }

    m_fUSD->close();


   connect(m_manager, SIGNAL(finished(QNetworkReply*)), SLOT(onPage_loaded(QNetworkReply*)));

   //qDebug() << "PageHandler";
}

PageHandler::~PageHandler()
{
    delete m_fUSD;

    qDebug() << "Close.";
}

void PageHandler::parse(QString url)
{
    QNetworkRequest request(url);
    m_manager->get(request);
}

void PageHandler::onPage_loaded(QNetworkReply *reply)
{
    QFile file("1.html");
    if( file.open(QIODevice::WriteOnly) )
    {
      QByteArray content= reply->readAll();

      file.write(content); // пишем в файл
    }

    reply->deleteLater();
}

bool PageHandler::fGet(QString url)
{
    // создаем объект для запроса
        QNetworkRequest request(url);//QUrl("https://index.minfin.com.ua/exchange/archive/privat/2017-01-03"));

        // Выполняем запрос, получаем указатель на объект
        // ответственный за ответ
        QNetworkReply* reply=  m_manager->get(request);

        // Подписываемся на сигнал о готовности загрузки
        // реализуем ожидание конца загрузки
          QEventLoop loop;
          connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
          loop.exec();

          // выводим содержимое
          if (reply->error() == QNetworkReply::NoError)
          {
              QByteArray content= reply->readAll();

              fFindUSD(content);
/*
            QFile file("1.html");
            if( file.open(QIODevice::WriteOnly) )
            {
              QByteArray content= reply->readAll();

               if( content.indexOf("Курс конвертации по платежным картам КБ") >= 0)
               {
                   qDebug() << content.size();
               }

              file.write(content); // пишем в файл
            }
            */
          }
          else
          { // вывод ошибки
             qDebug() << reply->errorString();
             return false;
          }

          reply->deleteLater();

          return true;
}

void PageHandler::fFindUSD(const QByteArray url)
{
    QString a_sWrit = "No DATA\r\n";
    if( url.indexOf("Ближайшие данные есть на") >= 0)
    {
        m_fUSD->write( a_sWrit.toLocal8Bit());
        return;
    }


    QString a_sFind[4];
    a_sFind[0] = "<caption>Курс конвертации по платежным картам КБ&nbsp;Приватбанк на&nbsp;";
    a_sFind[1] = "USD</td><td class='r0 smart-name'>доллар США</td><td class='r0' align='right'>";
    a_sFind[2] = "</td>";
    a_sFind[3] = "</small></nobr></span></td><td class='r0' align='right'>";

    int a_iStart = url.indexOf(a_sFind[0]);
    a_iStart += a_sFind[0].size();
    a_iStart = url.indexOf(a_sFind[1], a_iStart);
    a_iStart += a_sFind[1].size();
    a_iStart += 9;
    int a_iFin = url.indexOf(a_sFind[2], a_iStart);

    a_sWrit = url.mid(a_iStart, a_iFin - a_iStart) + " ";
    m_fUSD->write( a_sWrit.toLocal8Bit());

    //qDebug() << url.mid(a_iStart, a_iFin - a_iStart);

    a_iStart = url.indexOf(a_sFind[3], a_iFin);
    a_iStart += a_sFind[3].size();
    a_iFin = url.indexOf(a_sFind[2], a_iStart);

    a_sWrit = url.mid(a_iStart, a_iFin - a_iStart) + "\r\n";
    m_fUSD->write( a_sWrit.toLocal8Bit());
    //qDebug() << url.mid(a_iStart, a_iFin - a_iStart);

}


