#ifndef PROTO_H
#define PROTO_H

#include <QObject>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "util.h"
#include "libqt/net/proto/message.h"

enum encoding : std::uint8_t
{
    encode_none = 0x0,
    encode_fbs,  // flatbuffers
    encode_json, // json
    encode_pb,   // protobuf
};

struct JsonMsg : Message
{
public:
    JsonMsg()
        : id{0}
        , len{0}
        , seq{0}
        , time{0}
        , crc16{0}
        , encode_type{encode_json}
    {}
    JsonMsg(const quint16 _id, const quint16 _len, const quint16 _seq, const quint16 _tm, const quint16 _crc)
        : id{_id}
        , len{_len}
        , seq{_seq}
        , time{_tm}
        , crc16{_crc}
        , encode_type{encode_json}
    {}
    ~JsonMsg()
    {}

    virtual qint64 size() override
    {
        return payload.size() + 15;
    }

    virtual qint64 encode(QByteArray& buf) override
    {
        QJsonObject js;
        js["id"] = id;
        js["len"] = 0;
        js["seq"] = seq;
        js["time"] = time;
        js["crc16"] = crc16;
        js["encode_type"] = encode_type;
        js["payload"] = payload;
        js["len"] = js.size();

        QJsonDocument doc{js};
        buf = doc.toJson();
        qDebug() << "encode to " << buf;
        return buf.size();
    }

    virtual qint64 decode(const QByteArray& buf) override
    {
        QJsonObject js = QJsonDocument::fromJson(buf).object();

        len = js["len"].toInt();
        if (buf.size() < len)
            return 0;

        id = js["id"].toInt();
        seq = js["seq"].toInt();
        time = js["time"].toInt();
        crc16 = js["crc16"].toInt();
        encode_type = static_cast<encoding>(js["encode_type"].toInt());
        payload = js["payload"].toObject();
        qDebug() << "decode from " << buf;
        return len;
    }

    void reset()
    {
        id = 0;
        len = 0;
        seq = 0;
        time = 0;
        crc16 = 0;
        encode_type = encode_json;
        for (auto key : payload.keys())
            payload.remove(key);
    }

    quint16 id;
    quint16 len;
    qint32 seq;
    quint16 time;
    quint16 crc16;
    encoding encode_type;
    QJsonObject payload;
};

#endif
