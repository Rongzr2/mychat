#ifndef GLOBAL_H
#define GLOBAL_H
#include <QWidget>
#include <functional>
#include "QStyle"
#include <memory>
#include <mutex>
#include <iostream>
#include <QByteArray>
#include <QDir>
#include <QSettings>

extern std::function<void(QWidget*)> repolish;
extern QString gate_url_prefix;

enum ReqId{
    ID_GET_VERIFY_CODE = 1001,
    ID_REQ_USER = 1002,
};

enum Modules{
    REGISTERMOD = 0,
};

enum ErrorCodes{
    SUCCESS = 0,
    ERR_JSON = 1,  // Json解析失败
    ERR_NETWORK = 2,
};

#endif // GLOBAL_H
