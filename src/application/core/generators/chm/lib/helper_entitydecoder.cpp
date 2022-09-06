/*
    Kchmviewer - a CHM and EPUB file viewer with broad language support
    SPDX-FileCopyrightText: 2004-2014 George Yunaev gyunaev@ulduzsoft.com

    SPDX-License-Identifier: GPL-3.0-or-later
*/

#include <QTextCodec>

#include "helper_entitydecoder.h"

HelperEntityDecoder::HelperEntityDecoder(QTextCodec *encoder)
{
    changeEncoding(encoder);
}

static inline QString encodeWithCodec(QTextCodec *encoder, const QByteArray &str)
{
    return (encoder ? encoder->toUnicode(str.constData()) : QString::fromUtf8(str));
}

void HelperEntityDecoder::changeEncoding(QTextCodec *encoder)
{
    // Set up m_entityDecodeMap characters according to current textCodec
    m_entityDecodeMap.clear();

    m_entityDecodeMap[QStringLiteral("AElig")] = encodeWithCodec(encoder, "\306");  // capital AE diphthong (ligature)
    m_entityDecodeMap[QStringLiteral("Aacute")] = encodeWithCodec(encoder, "\301"); // capital A, acute accent
    m_entityDecodeMap[QStringLiteral("Acirc")] = encodeWithCodec(encoder, "\302");  // capital A, circumflex accent
    m_entityDecodeMap[QStringLiteral("Agrave")] = encodeWithCodec(encoder, "\300"); // capital A, grave accent
    m_entityDecodeMap[QStringLiteral("Aring")] = encodeWithCodec(encoder, "\305");  // capital A, ring
    m_entityDecodeMap[QStringLiteral("Atilde")] = encodeWithCodec(encoder, "\303"); // capital A, tilde
    m_entityDecodeMap[QStringLiteral("Auml")] = encodeWithCodec(encoder, "\304");   // capital A, dieresis or umlaut mark
    m_entityDecodeMap[QStringLiteral("Ccedil")] = encodeWithCodec(encoder, "\307"); // capital C, cedilla
    m_entityDecodeMap[QStringLiteral("Dstrok")] = encodeWithCodec(encoder, "\320"); // whatever
    m_entityDecodeMap[QStringLiteral("ETH")] = encodeWithCodec(encoder, "\320");    // capital Eth, Icelandic
    m_entityDecodeMap[QStringLiteral("Eacute")] = encodeWithCodec(encoder, "\311"); // capital E, acute accent
    m_entityDecodeMap[QStringLiteral("Ecirc")] = encodeWithCodec(encoder, "\312");  // capital E, circumflex accent
    m_entityDecodeMap[QStringLiteral("Egrave")] = encodeWithCodec(encoder, "\310"); // capital E, grave accent
    m_entityDecodeMap[QStringLiteral("Euml")] = encodeWithCodec(encoder, "\313");   // capital E, dieresis or umlaut mark
    m_entityDecodeMap[QStringLiteral("Iacute")] = encodeWithCodec(encoder, "\315"); // capital I, acute accent
    m_entityDecodeMap[QStringLiteral("Icirc")] = encodeWithCodec(encoder, "\316");  // capital I, circumflex accent
    m_entityDecodeMap[QStringLiteral("Igrave")] = encodeWithCodec(encoder, "\314"); // capital I, grave accent
    m_entityDecodeMap[QStringLiteral("Iuml")] = encodeWithCodec(encoder, "\317");   // capital I, dieresis or umlaut mark
    m_entityDecodeMap[QStringLiteral("Ntilde")] = encodeWithCodec(encoder, "\321"); // capital N, tilde
    m_entityDecodeMap[QStringLiteral("Oacute")] = encodeWithCodec(encoder, "\323"); // capital O, acute accent
    m_entityDecodeMap[QStringLiteral("Ocirc")] = encodeWithCodec(encoder, "\324");  // capital O, circumflex accent
    m_entityDecodeMap[QStringLiteral("Ograve")] = encodeWithCodec(encoder, "\322"); // capital O, grave accent
    m_entityDecodeMap[QStringLiteral("Oslash")] = encodeWithCodec(encoder, "\330"); // capital O, slash
    m_entityDecodeMap[QStringLiteral("Otilde")] = encodeWithCodec(encoder, "\325"); // capital O, tilde
    m_entityDecodeMap[QStringLiteral("Ouml")] = encodeWithCodec(encoder, "\326");   // capital O, dieresis or umlaut mark
    m_entityDecodeMap[QStringLiteral("THORN")] = encodeWithCodec(encoder, "\336");  // capital THORN, Icelandic
    m_entityDecodeMap[QStringLiteral("Uacute")] = encodeWithCodec(encoder, "\332"); // capital U, acute accent
    m_entityDecodeMap[QStringLiteral("Ucirc")] = encodeWithCodec(encoder, "\333");  // capital U, circumflex accent
    m_entityDecodeMap[QStringLiteral("Ugrave")] = encodeWithCodec(encoder, "\331"); // capital U, grave accent
    m_entityDecodeMap[QStringLiteral("Uuml")] = encodeWithCodec(encoder, "\334");   // capital U, dieresis or umlaut mark
    m_entityDecodeMap[QStringLiteral("Yacute")] = encodeWithCodec(encoder, "\335"); // capital Y, acute accent
    m_entityDecodeMap[QStringLiteral("OElig")] = encodeWithCodec(encoder, "\338");  // capital Y, acute accent
    m_entityDecodeMap[QStringLiteral("oelig")] = encodeWithCodec(encoder, "\339");  // capital Y, acute accent

    m_entityDecodeMap[QStringLiteral("aacute")] = encodeWithCodec(encoder, "\341"); // small a, acute accent
    m_entityDecodeMap[QStringLiteral("acirc")] = encodeWithCodec(encoder, "\342");  // small a, circumflex accent
    m_entityDecodeMap[QStringLiteral("aelig")] = encodeWithCodec(encoder, "\346");  // small ae diphthong (ligature)
    m_entityDecodeMap[QStringLiteral("agrave")] = encodeWithCodec(encoder, "\340"); // small a, grave accent
    m_entityDecodeMap[QStringLiteral("aring")] = encodeWithCodec(encoder, "\345");  // small a, ring
    m_entityDecodeMap[QStringLiteral("atilde")] = encodeWithCodec(encoder, "\343"); // small a, tilde
    m_entityDecodeMap[QStringLiteral("auml")] = encodeWithCodec(encoder, "\344");   // small a, dieresis or umlaut mark
    m_entityDecodeMap[QStringLiteral("ccedil")] = encodeWithCodec(encoder, "\347"); // small c, cedilla
    m_entityDecodeMap[QStringLiteral("eacute")] = encodeWithCodec(encoder, "\351"); // small e, acute accent
    m_entityDecodeMap[QStringLiteral("ecirc")] = encodeWithCodec(encoder, "\352");  // small e, circumflex accent
    m_entityDecodeMap[QStringLiteral("Scaron")] = encodeWithCodec(encoder, "\352"); // small e, circumflex accent
    m_entityDecodeMap[QStringLiteral("egrave")] = encodeWithCodec(encoder, "\350"); // small e, grave accent
    m_entityDecodeMap[QStringLiteral("eth")] = encodeWithCodec(encoder, "\360");    // small eth, Icelandic
    m_entityDecodeMap[QStringLiteral("euml")] = encodeWithCodec(encoder, "\353");   // small e, dieresis or umlaut mark
    m_entityDecodeMap[QStringLiteral("iacute")] = encodeWithCodec(encoder, "\355"); // small i, acute accent
    m_entityDecodeMap[QStringLiteral("icirc")] = encodeWithCodec(encoder, "\356");  // small i, circumflex accent
    m_entityDecodeMap[QStringLiteral("igrave")] = encodeWithCodec(encoder, "\354"); // small i, grave accent
    m_entityDecodeMap[QStringLiteral("iuml")] = encodeWithCodec(encoder, "\357");   // small i, dieresis or umlaut mark
    m_entityDecodeMap[QStringLiteral("ntilde")] = encodeWithCodec(encoder, "\361"); // small n, tilde
    m_entityDecodeMap[QStringLiteral("oacute")] = encodeWithCodec(encoder, "\363"); // small o, acute accent
    m_entityDecodeMap[QStringLiteral("ocirc")] = encodeWithCodec(encoder, "\364");  // small o, circumflex accent
    m_entityDecodeMap[QStringLiteral("ograve")] = encodeWithCodec(encoder, "\362"); // small o, grave accent
    m_entityDecodeMap[QStringLiteral("oslash")] = encodeWithCodec(encoder, "\370"); // small o, slash
    m_entityDecodeMap[QStringLiteral("otilde")] = encodeWithCodec(encoder, "\365"); // small o, tilde
    m_entityDecodeMap[QStringLiteral("ouml")] = encodeWithCodec(encoder, "\366");   // small o, dieresis or umlaut mark
    m_entityDecodeMap[QStringLiteral("szlig")] = encodeWithCodec(encoder, "\337");  // small sharp s, German (sz ligature)
    m_entityDecodeMap[QStringLiteral("thorn")] = encodeWithCodec(encoder, "\376");  // small thorn, Icelandic
    m_entityDecodeMap[QStringLiteral("uacute")] = encodeWithCodec(encoder, "\372"); // small u, acute accent
    m_entityDecodeMap[QStringLiteral("ucirc")] = encodeWithCodec(encoder, "\373");  // small u, circumflex accent
    m_entityDecodeMap[QStringLiteral("ugrave")] = encodeWithCodec(encoder, "\371"); // small u, grave accent
    m_entityDecodeMap[QStringLiteral("uuml")] = encodeWithCodec(encoder, "\374");   // small u, dieresis or umlaut mark
    m_entityDecodeMap[QStringLiteral("yacute")] = encodeWithCodec(encoder, "\375"); // small y, acute accent
    m_entityDecodeMap[QStringLiteral("yuml")] = encodeWithCodec(encoder, "\377");   // small y, dieresis or umlaut mark

    m_entityDecodeMap[QStringLiteral("iexcl")] = encodeWithCodec(encoder, "\241");
    m_entityDecodeMap[QStringLiteral("cent")] = encodeWithCodec(encoder, "\242");
    m_entityDecodeMap[QStringLiteral("pound")] = encodeWithCodec(encoder, "\243");
    m_entityDecodeMap[QStringLiteral("curren")] = encodeWithCodec(encoder, "\244");
    m_entityDecodeMap[QStringLiteral("yen")] = encodeWithCodec(encoder, "\245");
    m_entityDecodeMap[QStringLiteral("brvbar")] = encodeWithCodec(encoder, "\246");
    m_entityDecodeMap[QStringLiteral("sect")] = encodeWithCodec(encoder, "\247");
    m_entityDecodeMap[QStringLiteral("uml")] = encodeWithCodec(encoder, "\250");
    m_entityDecodeMap[QStringLiteral("ordf")] = encodeWithCodec(encoder, "\252");
    m_entityDecodeMap[QStringLiteral("laquo")] = encodeWithCodec(encoder, "\253");
    m_entityDecodeMap[QStringLiteral("not")] = encodeWithCodec(encoder, "\254");
    m_entityDecodeMap[QStringLiteral("shy")] = encodeWithCodec(encoder, "\255");
    m_entityDecodeMap[QStringLiteral("macr")] = encodeWithCodec(encoder, "\257");
    m_entityDecodeMap[QStringLiteral("deg")] = encodeWithCodec(encoder, "\260");
    m_entityDecodeMap[QStringLiteral("plusmn")] = encodeWithCodec(encoder, "\261");
    m_entityDecodeMap[QStringLiteral("sup1")] = encodeWithCodec(encoder, "\271");
    m_entityDecodeMap[QStringLiteral("sup2")] = encodeWithCodec(encoder, "\262");
    m_entityDecodeMap[QStringLiteral("sup3")] = encodeWithCodec(encoder, "\263");
    m_entityDecodeMap[QStringLiteral("acute")] = encodeWithCodec(encoder, "\264");
    m_entityDecodeMap[QStringLiteral("micro")] = encodeWithCodec(encoder, "\265");
    m_entityDecodeMap[QStringLiteral("para")] = encodeWithCodec(encoder, "\266");
    m_entityDecodeMap[QStringLiteral("middot")] = encodeWithCodec(encoder, "\267");
    m_entityDecodeMap[QStringLiteral("cedil")] = encodeWithCodec(encoder, "\270");
    m_entityDecodeMap[QStringLiteral("ordm")] = encodeWithCodec(encoder, "\272");
    m_entityDecodeMap[QStringLiteral("raquo")] = encodeWithCodec(encoder, "\273");
    m_entityDecodeMap[QStringLiteral("frac14")] = encodeWithCodec(encoder, "\274");
    m_entityDecodeMap[QStringLiteral("frac12")] = encodeWithCodec(encoder, "\275");
    m_entityDecodeMap[QStringLiteral("frac34")] = encodeWithCodec(encoder, "\276");
    m_entityDecodeMap[QStringLiteral("iquest")] = encodeWithCodec(encoder, "\277");
    m_entityDecodeMap[QStringLiteral("times")] = encodeWithCodec(encoder, "\327");
    m_entityDecodeMap[QStringLiteral("divide")] = encodeWithCodec(encoder, "\367");

    m_entityDecodeMap[QStringLiteral("copy")] = encodeWithCodec(encoder, "\251"); // copyright sign
    m_entityDecodeMap[QStringLiteral("reg")] = encodeWithCodec(encoder, "\256");  // registered sign
    m_entityDecodeMap[QStringLiteral("nbsp")] = encodeWithCodec(encoder, "\240"); // non breaking space

    m_entityDecodeMap[QStringLiteral("fnof")] = QChar((unsigned short)402);

    m_entityDecodeMap[QStringLiteral("Delta")] = QChar((unsigned short)916);
    m_entityDecodeMap[QStringLiteral("Pi")] = QChar((unsigned short)928);
    m_entityDecodeMap[QStringLiteral("Sigma")] = QChar((unsigned short)931);

    m_entityDecodeMap[QStringLiteral("beta")] = QChar((unsigned short)946);
    m_entityDecodeMap[QStringLiteral("gamma")] = QChar((unsigned short)947);
    m_entityDecodeMap[QStringLiteral("delta")] = QChar((unsigned short)948);
    m_entityDecodeMap[QStringLiteral("eta")] = QChar((unsigned short)951);
    m_entityDecodeMap[QStringLiteral("theta")] = QChar((unsigned short)952);
    m_entityDecodeMap[QStringLiteral("lambda")] = QChar((unsigned short)955);
    m_entityDecodeMap[QStringLiteral("mu")] = QChar((unsigned short)956);
    m_entityDecodeMap[QStringLiteral("nu")] = QChar((unsigned short)957);
    m_entityDecodeMap[QStringLiteral("pi")] = QChar((unsigned short)960);
    m_entityDecodeMap[QStringLiteral("rho")] = QChar((unsigned short)961);

    m_entityDecodeMap[QStringLiteral("lsquo")] = QChar((unsigned short)8216);
    m_entityDecodeMap[QStringLiteral("rsquo")] = QChar((unsigned short)8217);
    m_entityDecodeMap[QStringLiteral("rdquo")] = QChar((unsigned short)8221);
    m_entityDecodeMap[QStringLiteral("bdquo")] = QChar((unsigned short)8222);
    m_entityDecodeMap[QStringLiteral("trade")] = QChar((unsigned short)8482);
    m_entityDecodeMap[QStringLiteral("ldquo")] = QChar((unsigned short)8220);
    m_entityDecodeMap[QStringLiteral("ndash")] = QChar((unsigned short)8211);
    m_entityDecodeMap[QStringLiteral("mdash")] = QChar((unsigned short)8212);
    m_entityDecodeMap[QStringLiteral("bull")] = QChar((unsigned short)8226);
    m_entityDecodeMap[QStringLiteral("hellip")] = QChar((unsigned short)8230);
    m_entityDecodeMap[QStringLiteral("emsp")] = QChar((unsigned short)8195);
    m_entityDecodeMap[QStringLiteral("rarr")] = QChar((unsigned short)8594);
    m_entityDecodeMap[QStringLiteral("rArr")] = QChar((unsigned short)8658);
    m_entityDecodeMap[QStringLiteral("crarr")] = QChar((unsigned short)8629);
    m_entityDecodeMap[QStringLiteral("le")] = QChar((unsigned short)8804);
    m_entityDecodeMap[QStringLiteral("ge")] = QChar((unsigned short)8805);
    m_entityDecodeMap[QStringLiteral("lte")] = QChar((unsigned short)8804); // wrong, but used somewhere
    m_entityDecodeMap[QStringLiteral("gte")] = QChar((unsigned short)8805); // wrong, but used somewhere
    m_entityDecodeMap[QStringLiteral("dagger")] = QChar((unsigned short)8224);
    m_entityDecodeMap[QStringLiteral("Dagger")] = QChar((unsigned short)8225);
    m_entityDecodeMap[QStringLiteral("euro")] = QChar((unsigned short)8364);
    m_entityDecodeMap[QStringLiteral("asymp")] = QChar((unsigned short)8776);
    m_entityDecodeMap[QStringLiteral("isin")] = QChar((unsigned short)8712);
    m_entityDecodeMap[QStringLiteral("notin")] = QChar((unsigned short)8713);
    m_entityDecodeMap[QStringLiteral("prod")] = QChar((unsigned short)8719);
    m_entityDecodeMap[QStringLiteral("ne")] = QChar((unsigned short)8800);

    m_entityDecodeMap[QStringLiteral("amp")] = QStringLiteral("&");   // ampersand
    m_entityDecodeMap[QStringLiteral("gt")] = QStringLiteral(">");    // greater than
    m_entityDecodeMap[QStringLiteral("lt")] = QStringLiteral("<");    // less than
    m_entityDecodeMap[QStringLiteral("quot")] = QStringLiteral("\""); // double quote
    m_entityDecodeMap[QStringLiteral("apos")] = QStringLiteral("'");  // single quote
    m_entityDecodeMap[QStringLiteral("frasl")] = QStringLiteral("/");
    m_entityDecodeMap[QStringLiteral("minus")] = QStringLiteral("-");
    m_entityDecodeMap[QStringLiteral("oplus")] = QStringLiteral("+");
    m_entityDecodeMap[QStringLiteral("Prime")] = QStringLiteral("\"");
}

QString HelperEntityDecoder::decode(const QString &entity) const
{
    // If entity is an ASCII code like &#12349; - just decode it
    if (entity.isEmpty()) {
        return QLatin1String("");
    } else if (entity[0] == QLatin1Char('#')) {
        bool valid;
        unsigned int ascode = entity.midRef(1).toUInt(&valid);

        if (!valid) {
            qWarning("HelperEntityDecoder::decode: could not decode HTML entity '%s'", qPrintable(entity));
            return QString();
        }

        return (QString)(QChar(ascode));
    } else {
        QMap<QString, QString>::const_iterator it = m_entityDecodeMap.find(entity);

        if (it == m_entityDecodeMap.end()) {
            qWarning("HelperEntityDecoder::decode: could not decode HTML entity '%s'", qPrintable(entity));
            return QLatin1String("");
        }

        return *it;
    }
}
