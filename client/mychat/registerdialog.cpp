#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "global.h"
#include <QRegularExpression>

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    ui->pass_edit->setEchoMode(QLineEdit::Password);
    ui->confirm_edit->setEchoMode(QLineEdit::Password);

    ui->err_tip->setProperty("state","normal");
    repolish(ui->Widget);

    // 连接注册界面和http请求
    connect(HttpMgr::GetInstance().get(), &HttpMgr::sig_reg_mod_finish, this, &RegisterDialog::slot_reg_mod_finish);

    this->initHttpHandlers();
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::showtip(QString str, bool b_ok)
{
    if(!b_ok){
        ui->err_tip->setProperty("state","err");
    }else{
        ui->err_tip->setProperty("state","normal");
    }
    ui->err_tip->setText(str);
    repolish(ui->err_tip);
}

void RegisterDialog::on_get_btn_clicked()
{
    // 验证邮箱地址是否正确
    auto email=ui->email_edit->text();

    // 邮箱地址的正则表达式
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match=regex.match(email).hasMatch(); //执行正则表达式匹配

    if(match){
        //发送验证码
        QJsonObject json_obj;
        json_obj["email"] = email;
        HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/get_verifycode"),
                                           json_obj, ReqId::ID_GET_VERIFY_CODE, Modules::REGISTERMOD);
    }else{
        showtip(tr("邮箱地址不正确"), false);
    }
}

// 实现取消槽函数
void RegisterDialog::on_cancel_btn_clicked() {
    close(); // 关闭窗口
}

void RegisterDialog::slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err_code)
{
    if(err_code!=ErrorCodes::SUCCESS){
        showtip(tr("网络请求错误"), false);
        return;
    }

    // 解析Json字符串, res需转化为QByteArray
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    // json解析错误
    if(jsonDoc.isNull()){
        showtip(tr("Json解析错误"), false);
        return;
    }
    if(!jsonDoc.isObject()){
        showtip(tr("Json解析错误"), false);
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();

    //调用对应的逻辑,根据id回调。
    _handlers[id](jsonDoc.object());

    return;
}

void RegisterDialog::initHttpHandlers()
{
    // 注册获取验证码回包逻辑
    _handlers.insert(ReqId::ID_GET_VERIFY_CODE, [this](QJsonObject jsonObj){
        int error = jsonObj["error"].toInt();
        if(error!=ErrorCodes::SUCCESS){
            showtip(tr("参数错误"), false);
            return;
        }

        auto email = jsonObj["email"].toString();
        showtip(tr("验证码已发送到邮箱"), true);
        qDebug() << "email is " << email;
    });
}
