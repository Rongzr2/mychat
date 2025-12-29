#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include "ui_registerdialog.h"
#include "httpmgr.h"

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();
    void showtip(QString str, bool b_ok);

public slots:
    void on_get_btn_clicked();

    void on_cancel_btn_clicked();

    void slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err_code);

private:
    Ui::RegisterDialog *ui;
    QMap<ReqId, std::function<void(const QJsonObject&)>> _handlers;
    void initHttpHandlers();
};

#endif // REGISTERDIALOG_H
