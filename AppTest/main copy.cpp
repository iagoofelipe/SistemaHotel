#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QDebug>
#include <QObject>

class API
{

public:

    void on_finished(QNetworkReply *reply) {
        qDebug() << "http code" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        // convertendo em JSON
        auto content = reply->readAll();
        QJsonDocument jdoc = QJsonDocument::fromJson(content);

        if(!jdoc.isNull()) {

            if(jdoc.isArray()) {
                const auto array = jdoc.array();

                qDebug() << "array" << array;

                for(const QJsonValue& obj : array)
                    qDebug() << "obj" << obj.toObject();
            }

            else if(jdoc.isObject())
                qDebug() << "object" << jdoc.object();

        } else {
            qDebug() << "json error" << content;
        }

        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "Request successful";

        } else {
            qDebug() << "Error:" << reply->errorString();
        }

        reply->deleteLater();
        QCoreApplication::quit();
    }
};

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    API api;

    QNetworkAccessManager manager;
    QUrl url("http://127.0.0.1:8000/api/room");
    QNetworkRequest request(url);

    QNetworkReply *reply = manager.get(request);

    QObject::connect(reply, &QNetworkReply::finished, [&]() {
        api.on_finished(reply);
    });

    /*
    QObject::connect(reply, &QNetworkReply::readyRead, [reply]() {

        // convertendo em JSON
        QJsonDocument jdoc = QJsonDocument::fromJson(reply->readAll());
        if(!jdoc.isNull()) {

            if(jdoc.isArray())
                qDebug() << "array" << jdoc.array();

            else if(jdoc.isObject())
                qDebug() << "object" << jdoc.object();

            else
                qDebug() << "undefined";

        } else {
            qDebug() << "json error";
        }
    });

    QObject::connect(reply, &QNetworkReply::finished, [reply]() {
        qDebug() << "http code" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "Request successful";
        } else {
            qDebug() << "Error:" << reply->errorString();
        }
        reply->deleteLater();
        QCoreApplication::quit();
    });


    QObject::connect(&manager, &QNetworkAccessManager::finished, [](QNetworkReply *rep) {
        if (rep->error()) {
            qDebug() << "Error: " << rep->errorString();
        }
    });
    */

    return a.exec();
}
