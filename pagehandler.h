#ifndef PAGEHANDLER_H
#define PAGEHANDLER_H

#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QString>
#include <QDebug>
#include <QEventLoop>
#include <QFile>
#include <QNetworkRequest>

class PageHandler: public QObject {
    Q_OBJECT
    public:
        explicit PageHandler(QObject *parent = 0);
    ~PageHandler();
    public slots:
        void parse(QString url);
    private slots:
        void onPage_loaded(QNetworkReply*);
    signals:
        void finished();
    private:
        QNetworkAccessManager *m_manager;
        QFile *m_fUSD;

        void fFindUSD(const QByteArray url);
        bool fGet(QString url);

  };
#endif // PAGEHANDLER_H
