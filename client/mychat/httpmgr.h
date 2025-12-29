#ifndef HTTPMGR_H
#define HTTPMGR_H
#include "singleton.h"
#include <QString>
#include <QUrl>
#include <QObject>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>

// CRTP
class HttpMgr:public QObject, public Singleton<HttpMgr>,
          public std::enable_shared_from_this<HttpMgr>
{
    Q_OBJECT

public:
    ~HttpMgr();
    void PostHttpReq(QUrl url, QJsonObject json, ReqId id, Modules mod);

private:
    friend class Singleton<HttpMgr>;
    HttpMgr();
    QNetworkAccessManager _manager;

signals:
    void sig_http_finish(ReqId id, QString res, ErrorCodes err_code, Modules mod);
    void sig_reg_mod_finish(ReqId id, QString res, ErrorCodes err_code);

public slots:
    void slot_http_finish(ReqId id, QString res, ErrorCodes err_code, Modules mod);

#endif // HTTPMGR_H
};
