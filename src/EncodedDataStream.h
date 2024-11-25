/**
 * KDiff3 - Text Diff And Merge Tool
 *
 * SPDX-FileCopyrightText: 2023 Michael Reeves <reeves.87@gmail.com>
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */

#ifndef ENCODEDDATASTREAM_H
#define ENCODEDDATASTREAM_H

#include "TypeUtils.h"

#include <QByteArray>
#include <QDataStream>
#include <QIODevice>
#include <QString>
#include <QStringDecoder>

class EncodedDataStream: public QDataStream
{
  private:
    Q_DISABLE_COPY(EncodedDataStream);

    QStringDecoder mDecoder = QStringDecoder("UTF-8", QStringConverter::Flag::ConvertInitialBom);
    QStringEncoder mEncoder = QStringEncoder("UTF-8", QStringEncoder::Flag::ConvertInitialBom);
    QByteArray mEncoding = "UTF-8";
    bool mGenerateBOM = false;
    bool mError = false;

  public:
    using QDataStream::QDataStream;

    void setGenerateByteOrderMark(bool generate) { mGenerateBOM = generate; }

    inline bool hasBOM() const noexcept { return mGenerateBOM; }

    inline void setEncoding(const QByteArray &inEncoding) noexcept
    {
        assert(!inEncoding.isEmpty());

        if(inEncoding == "UTF-8-BOM")
        {
            mGenerateBOM = true;
            mEncoding = "UTF-8";
        }
        else
        {
            mGenerateBOM = inEncoding.startsWith("UTF-16") || inEncoding.startsWith("UTF-32");
            mEncoding = inEncoding;
        }

        mDecoder = QStringDecoder(mEncoding, mGenerateBOM ? QStringConverter::Flag::WriteBom : QStringConverter::Flag::ConvertInitialBom);
        mEncoder = QStringEncoder(mEncoding, mGenerateBOM ? QStringConverter::Flag::WriteBom : QStringConverter::Flag::ConvertInitialBom);

        assert(mDecoder.isValid() && mEncoder.isValid());
        assert(!mGenerateBOM || ((inEncoding.startsWith("UTF-16") || inEncoding.startsWith("UTF-32")) || inEncoding == "UTF-8-BOM"));
    };

    inline qint32 readChar(QChar& c)
    {
        char curData = QChar::Null;
        qint32 len = 0;
        QString s;

        if(!mDecoder.isValid()) return 0;

        do
        {
            len += readRawData(&curData, 1);

            s = mDecoder(QByteArray::fromRawData(&curData, sizeof(curData)));
        } while(!mDecoder.hasError() && s.isEmpty() && !atEnd());

        mError = mDecoder.hasError() || (s.isEmpty() && atEnd());
        if(!mError)
            c = s[0];
        else
            c = QChar::ReplacementCharacter;

        return len;
    }

    quint64 peekChar(QChar &c)
    {
        QStringDecoder decoder = QStringDecoder(mEncoding);
        QIODevice *d = device();
        char buf[4];
        qint64 len = d->peek(buf, sizeof(buf));

        if(len > 0)
        {
            QString s = decoder(QByteArray::fromRawData(buf, sizeof(buf)));
            if(s.isEmpty()) return 0;

            c = s[0];
        }
        else
            c = QChar::Null;
        return len;
    }

    EncodedDataStream &operator<<(const QString &s)
    {
        QByteArray data = mEncoder(s);

        mError = mEncoder.hasError();
        writeRawData(data.constData(), data.length());
        return *this;
    };

    inline bool hasError() noexcept { return mError; }

    //Not implemented but may be inherieted from QDataStream
    EncodedDataStream &operator<<(const QChar &) = delete;
    EncodedDataStream &operator<<(const char *&) = delete;
    EncodedDataStream &operator<<(const char &) = delete;

    EncodedDataStream &operator<<(const QByteArray &bytes)
    {
        writeRawData(bytes.constData(), bytes.length());
        return *this;
    };

    EncodedDataStream &operator>>(QByteArray &) = delete;
    EncodedDataStream &operator>>(QString &) = delete;
    EncodedDataStream &operator>>(QChar &) = delete;
    EncodedDataStream &operator>>(char *&) = delete;
    EncodedDataStream &operator>>(char &) = delete;
};

#endif /* ENCODEDDATASTREAM_H */
