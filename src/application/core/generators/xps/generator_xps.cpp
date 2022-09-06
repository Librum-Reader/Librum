/*
    SPDX-FileCopyrightText: 2006, 2009 Brad Hards <bradh@frogmouth.net>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "generator_xps.h"

#include <KAboutData>
#include <KLocalizedString>
#include <QBuffer>
#include <QDateTime>
#include <QFile>
#include <QImageReader>
#include <QList>
#include <QMutex>
#include <QPainter>
#include <QPrinter>
#include <QUrl>

#include <core/area.h>
#include <core/document.h>
#include <core/fileprinter.h>
#include <core/page.h>

OKULAR_EXPORT_PLUGIN(XpsGenerator, "libokularGenerator_xps.json")

Q_DECLARE_METATYPE(QGradient *)
Q_DECLARE_METATYPE(XpsPathFigure *)
Q_DECLARE_METATYPE(XpsPathGeometry *)

// From Qt4
static int hex2int(char hex)
{
    QChar hexchar = QLatin1Char(hex);
    int v;
    if (hexchar.isDigit()) {
        v = hexchar.digitValue();
    } else if (hexchar >= QLatin1Char('A') && hexchar <= QLatin1Char('F')) {
        v = hexchar.cell() - 'A' + 10;
    } else if (hexchar >= QLatin1Char('a') && hexchar <= QLatin1Char('f')) {
        v = hexchar.cell() - 'a' + 10;
    } else {
        v = -1;
    }
    return v;
}

// Modified from Qt4
static QColor hexToRgba(const QByteArray &name)
{
    const int len = name.length();
    if (len == 0 || name[0] != '#') {
        return QColor();
    }
    int r, g, b;
    int a = 255;
    if (len == 7) {
        r = (hex2int(name[1]) << 4) + hex2int(name[2]);
        g = (hex2int(name[3]) << 4) + hex2int(name[4]);
        b = (hex2int(name[5]) << 4) + hex2int(name[6]);
    } else if (len == 9) {
        a = (hex2int(name[1]) << 4) + hex2int(name[2]);
        r = (hex2int(name[3]) << 4) + hex2int(name[4]);
        g = (hex2int(name[5]) << 4) + hex2int(name[6]);
        b = (hex2int(name[7]) << 4) + hex2int(name[8]);
    } else {
        r = g = b = -1;
    }
    if ((uint)r > 255 || (uint)g > 255 || (uint)b > 255) {
        return QColor();
    }
    return QColor(r, g, b, a);
}

static QRectF stringToRectF(const QString &data)
{
    QStringList numbers = data.split(QLatin1Char(','));
    QPointF origin(numbers.at(0).toDouble(), numbers.at(1).toDouble());
    QSizeF size(numbers.at(2).toDouble(), numbers.at(3).toDouble());
    return QRectF(origin, size);
}

static bool parseGUID(const QString &guidString, unsigned short guid[16])
{
    if (guidString.length() <= 35) {
        return false;
    }

    // Maps bytes to positions in guidString
    const static int indexes[] = {6, 4, 2, 0, 11, 9, 16, 14, 19, 21, 24, 26, 28, 30, 32, 34};

    for (int i = 0; i < 16; i++) {
        int hex1 = hex2int(guidString[indexes[i]].cell());
        int hex2 = hex2int(guidString[indexes[i] + 1].cell());

        if ((hex1 < 0) || (hex2 < 0)) {
            return false;
        }

        guid[i] = hex1 * 16 + hex2;
    }

    return true;
}

// Read next token of abbreviated path data
static bool nextAbbPathToken(AbbPathToken *token)
{
    int *curPos = &token->curPos;
    QString data = token->data;

    while ((*curPos < data.length()) && (data.at(*curPos).isSpace())) {
        (*curPos)++;
    }

    if (*curPos == data.length()) {
        token->type = abtEOF;
        return true;
    }

    QChar ch = data.at(*curPos);

    if (ch.isNumber() || (ch == QLatin1Char('+')) || (ch == QLatin1Char('-'))) {
        int start = *curPos;
        while ((*curPos < data.length()) && (!data.at(*curPos).isSpace()) && (data.at(*curPos) != QLatin1Char(',') && (!data.at(*curPos).isLetter() || data.at(*curPos) == QLatin1Char('e')))) {
            (*curPos)++;
        }
        token->number = data.midRef(start, *curPos - start).toDouble();
        token->type = abtNumber;

    } else if (ch == QLatin1Char(',')) {
        token->type = abtComma;
        (*curPos)++;
    } else if (ch.isLetter()) {
        token->type = abtCommand;
        token->command = data.at(*curPos).cell();
        (*curPos)++;
    } else {
        (*curPos)++;
        return false;
    }

    return true;
}

/**
    Read point (two reals delimited by comma) from abbreviated path data
*/
static QPointF getPointFromString(AbbPathToken *token, bool relative, const QPointF currentPosition)
{
    // TODO Check grammar

    QPointF result;
    result.rx() = token->number;
    nextAbbPathToken(token);
    nextAbbPathToken(token); // ,
    result.ry() = token->number;
    nextAbbPathToken(token);

    if (relative) {
        result += currentPosition;
    }

    return result;
}

/**
    Read point (two reals delimited by comma) from string
*/
static QPointF getPointFromString(const QString &string)
{
    const int commaPos = string.indexOf(QLatin1Char(QLatin1Char(',')));
    if (commaPos == -1 || string.indexOf(QLatin1Char(QLatin1Char(',')), commaPos + 1) != -1) {
        return QPointF();
    }

    QPointF result;
    bool ok = false;
    QStringRef ref = string.midRef(0, commaPos);
    result.setX(QString::fromRawData(ref.constData(), ref.count()).toDouble(&ok));
    if (!ok) {
        return QPointF();
    }

    ref = string.midRef(commaPos + 1);
    result.setY(QString::fromRawData(ref.constData(), ref.count()).toDouble(&ok));
    if (!ok) {
        return QPointF();
    }

    return result;
}

static Qt::FillRule fillRuleFromString(const QString &data, Qt::FillRule def = Qt::OddEvenFill)
{
    if (data == QLatin1String("EvenOdd")) {
        return Qt::OddEvenFill;
    } else if (data == QLatin1String("NonZero")) {
        return Qt::WindingFill;
    }
    return def;
}

/**
    Parse an abbreviated path "Data" description
    \param data the string containing the whitespace separated values

    \see XPS specification 4.2.3 and Appendix G
*/
static QPainterPath parseAbbreviatedPathData(const QString &data)
{
    QPainterPath path;

    AbbPathToken token;

    token.data = data;
    token.curPos = 0;

    nextAbbPathToken(&token);

    // Used by Smooth cubic curve (command s)
    char lastCommand = ' ';
    QPointF lastSecondControlPoint;

    while (true) {
        if (token.type != abtCommand) {
            if (token.type != abtEOF) {
                qCWarning(OkularXpsDebug).nospace() << "Error in parsing abbreviated path data (" << token.type << "@" << token.curPos << "): " << data;
            }
            return path;
        }

        char command = QChar::fromLatin1(token.command).toLower().cell();
        bool isRelative = QChar::fromLatin1(token.command).isLower();
        QPointF currPos = path.currentPosition();
        nextAbbPathToken(&token);

        switch (command) {
        case 'f':
            int rule;
            rule = (int)token.number;
            if (rule == 0) {
                path.setFillRule(Qt::OddEvenFill);
            } else if (rule == 1) {
                // In xps specs rule 1 means NonZero fill. I think it's equivalent to WindingFill but I'm not sure
                path.setFillRule(Qt::WindingFill);
            }
            nextAbbPathToken(&token);
            break;
        case 'm': // Move
            while (token.type == abtNumber) {
                QPointF point = getPointFromString(&token, isRelative, currPos);
                path.moveTo(point);
            }
            break;
        case 'l': // Line
            while (token.type == abtNumber) {
                QPointF point = getPointFromString(&token, isRelative, currPos);
                path.lineTo(point);
            }
            break;
        case 'h': // Horizontal line
            while (token.type == abtNumber) {
                double x = token.number;
                if (isRelative) {
                    x += path.currentPosition().x();
                }
                path.lineTo(x, path.currentPosition().y());
                nextAbbPathToken(&token);
            }
            break;
        case 'v': // Vertical line
            while (token.type == abtNumber) {
                double y = token.number;
                if (isRelative) {
                    y += path.currentPosition().y();
                }
                path.lineTo(path.currentPosition().x(), y);
                nextAbbPathToken(&token);
            }
            break;
        case 'c': // Cubic bezier curve
            while (token.type == abtNumber) {
                QPointF firstControl = getPointFromString(&token, isRelative, currPos);
                QPointF secondControl = getPointFromString(&token, isRelative, currPos);
                QPointF endPoint = getPointFromString(&token, isRelative, currPos);
                path.cubicTo(firstControl, secondControl, endPoint);

                lastSecondControlPoint = secondControl;
            }
            break;
        case 'q': // Quadratic bezier curve
            while (token.type == abtNumber) {
                QPointF point1 = getPointFromString(&token, isRelative, currPos);
                QPointF point2 = getPointFromString(&token, isRelative, currPos);
                path.quadTo(point1, point2);
            }
            break;
        case 's': // Smooth cubic bezier curve
            while (token.type == abtNumber) {
                QPointF firstControl;
                if ((lastCommand == 's') || (lastCommand == 'c')) {
                    firstControl = lastSecondControlPoint + (lastSecondControlPoint + path.currentPosition());
                } else {
                    firstControl = path.currentPosition();
                }
                QPointF secondControl = getPointFromString(&token, isRelative, currPos);
                QPointF endPoint = getPointFromString(&token, isRelative, currPos);
                path.cubicTo(firstControl, secondControl, endPoint);
            }
            break;
        case 'a': // Arc
            // TODO Implement Arc drawing
            while (token.type == abtNumber) {
                /*QPointF rp =*/getPointFromString(&token, isRelative, currPos);
                /*double r = token.number;*/
                nextAbbPathToken(&token);
                /*double fArc = token.number; */
                nextAbbPathToken(&token);
                /*double fSweep = token.number; */
                nextAbbPathToken(&token);
                /*QPointF point = */ getPointFromString(&token, isRelative, currPos);
            }
            break;
        case 'z': // Close path
            path.closeSubpath();
            break;
        }

        lastCommand = command;
    }

    return path;
}

/**
   Parse a "Matrix" attribute string
   \param csv the comma separated list of values
   \return the QTransform corresponding to the affine transform
   given in the attribute

   \see XPS specification 7.4.1
*/
static QTransform attsToMatrix(const QString &csv)
{
    QStringList values = csv.split(QLatin1Char(','));
    if (values.count() != 6) {
        return QTransform(); // that is an identity matrix - no effect
    }
    return QTransform(values.at(0).toDouble(), values.at(1).toDouble(), values.at(2).toDouble(), values.at(3).toDouble(), values.at(4).toDouble(), values.at(5).toDouble());
}

/**
   \return Brush with given color or brush specified by reference to resource
*/
static QBrush parseRscRefColorForBrush(const QString &data)
{
    if (data[0] == QLatin1Char('{')) {
        // TODO
        qCWarning(OkularXpsDebug) << "Reference" << data;
        return QBrush();
    } else {
        return QBrush(hexToRgba(data.toLatin1()));
    }
}

/**
   \return Pen with given color or Pen specified by reference to resource
*/
static QPen parseRscRefColorForPen(const QString &data)
{
    if (data[0] == QLatin1Char('{')) {
        // TODO
        qCWarning(OkularXpsDebug) << "Reference" << data;
        return QPen();
    } else {
        return QPen(hexToRgba(data.toLatin1()));
    }
}

/**
   \return Matrix specified by given data or by referenced dictionary
*/
static QTransform parseRscRefMatrix(const QString &data)
{
    if (data[0] == QLatin1Char('{')) {
        // TODO
        qCWarning(OkularXpsDebug) << "Reference" << data;
        return QTransform();
    } else {
        return attsToMatrix(data);
    }
}

/**
   \return Path specified by given data or by referenced dictionary
*/
static QPainterPath parseRscRefPath(const QString &data)
{
    if (data[0] == QLatin1Char('{')) {
        // TODO
        qCWarning(OkularXpsDebug) << "Reference" << data;
        return QPainterPath();
    } else {
        return parseAbbreviatedPathData(data);
    }
}

/**
   \return The path of the entry
*/
static QString entryPath(const QString &entry)
{
    const QChar slash = QChar::fromLatin1('/');
    const int index = entry.lastIndexOf(slash);
    QString ret = entry.mid(0, index);
    if (index > 0) {
        ret.append(slash);
    }
    if (!ret.startsWith(slash)) {
        ret.prepend(slash);
    }
    return ret;
}

/**
   \return The path of the entry
*/
static QString entryPath(const KZipFileEntry *entry)
{
    return entryPath(entry->path());
}

/**
   \return The absolute path of the \p location, according to \p path if it's non-absolute
*/
static QString absolutePath(const QString &path, const QString &location)
{
    QString retPath;
    if (location.startsWith(QLatin1Char('/'))) {
        // already absolute
        retPath = location;
    } else {
        QUrl u = QUrl::fromLocalFile(path);
        QUrl u2 = QUrl(location);
        retPath = u.resolved(u2).toDisplayString(QUrl::PreferLocalFile);
    }
    // it seems paths & file names can also be percent-encoded
    // (XPS won't ever finish surprising me)
    if (retPath.contains(QLatin1Char('%'))) {
        retPath = QUrl::fromPercentEncoding(retPath.toUtf8());
    }
    return retPath;
}

/**
   Read the content of an archive entry in both the cases:
   a) single file
      + foobar
   b) directory
      + foobar/
        + [0].piece
        + [1].piece
        + ...
        + [x].last.piece

   \see XPS specification 10.1.2
*/
static QByteArray readFileOrDirectoryParts(const KArchiveEntry *entry, QString *pathOfFile = nullptr)
{
    QByteArray data;
    if (entry->isDirectory()) {
        const KArchiveDirectory *relDir = static_cast<const KArchiveDirectory *>(entry);
        QStringList entries = relDir->entries();
        std::sort(entries.begin(), entries.end());
        for (const QString &entry : qAsConst(entries)) {
            const KArchiveEntry *relSubEntry = relDir->entry(entry);
            if (!relSubEntry->isFile()) {
                continue;
            }

            const KZipFileEntry *relSubFile = static_cast<const KZipFileEntry *>(relSubEntry);
            data.append(relSubFile->data());
        }
    } else {
        const KZipFileEntry *relFile = static_cast<const KZipFileEntry *>(entry);
        data.append(relFile->data());
        if (pathOfFile) {
            *pathOfFile = entryPath(relFile);
        }
    }
    return data;
}

/**
   Load the resource \p fileName from the specified \p archive using the case sensitivity \p cs
*/
static const KArchiveEntry *loadEntry(KZip *archive, const QString &fileName, Qt::CaseSensitivity cs)
{
    // first attempt: loading the entry straight as requested
    const KArchiveEntry *entry = archive->directory()->entry(fileName);
    // in case sensitive mode, or if we actually found something, return what we found
    if (cs == Qt::CaseSensitive || entry) {
        return entry;
    }

    QString path;
    QString entryName;
    const int index = fileName.lastIndexOf(QChar::fromLatin1('/'));
    if (index > 0) {
        path = fileName.left(index);
        entryName = fileName.mid(index + 1);
    } else {
        path = QLatin1Char('/');
        entryName = fileName;
    }
    const KArchiveEntry *newEntry = archive->directory()->entry(path);
    if (newEntry->isDirectory()) {
        const KArchiveDirectory *relDir = static_cast<const KArchiveDirectory *>(newEntry);
        QStringList relEntries = relDir->entries();
        std::sort(relEntries.begin(), relEntries.end());
        for (const QString &relEntry : qAsConst(relEntries)) {
            if (relEntry.compare(entryName, Qt::CaseInsensitive) == 0) {
                return relDir->entry(relEntry);
            }
        }
    }
    return nullptr;
}

/**
   \return The name of a resource from the \p fileName
*/
static QString resourceName(const QString &fileName)
{
    QString resource = fileName;
    const int slashPos = fileName.lastIndexOf(QLatin1Char('/'));
    const int dotPos = fileName.lastIndexOf(QLatin1Char('.'));
    if (slashPos > -1) {
        if (dotPos > -1 && dotPos > slashPos) {
            resource = fileName.mid(slashPos + 1, dotPos - slashPos - 1);
        } else {
            resource = fileName.mid(slashPos + 1);
        }
    }
    return resource;
}

static QColor interpolatedColor(const QColor &c1, const QColor &c2)
{
    QColor res;
    res.setAlpha((c1.alpha() + c2.alpha()) / 2);
    res.setRed((c1.red() + c2.red()) / 2);
    res.setGreen((c1.green() + c2.green()) / 2);
    res.setBlue((c1.blue() + c2.blue()) / 2);
    return res;
}

static bool xpsGradientLessThan(const XpsGradient &g1, const XpsGradient &g2)
{
    return qFuzzyCompare(g1.offset, g2.offset) ? g1.color.name() < g2.color.name() : g1.offset < g2.offset;
}

static int xpsGradientWithOffset(const QList<XpsGradient> &gradients, double offset)
{
    int i = 0;
    for (const XpsGradient &grad : gradients) {
        if (grad.offset == offset) {
            return i;
        }
        ++i;
    }
    return -1;
}

/**
   Preprocess a list of gradients.

   \see XPS specification 11.3.1.1
*/
static void preprocessXpsGradients(QList<XpsGradient> &gradients)
{
    if (gradients.isEmpty()) {
        return;
    }

    // sort the gradients (case 1.)
    std::stable_sort(gradients.begin(), gradients.end(), xpsGradientLessThan);

    // no gradient with stop 0.0 (case 2.)
    if (xpsGradientWithOffset(gradients, 0.0) == -1) {
        int firstGreaterThanZero = 0;
        while (firstGreaterThanZero < gradients.count() && gradients.at(firstGreaterThanZero).offset < 0.0) {
            ++firstGreaterThanZero;
        }
        // case 2.a: no gradients with stop less than 0.0
        if (firstGreaterThanZero == 0) {
            gradients.prepend(XpsGradient(0.0, gradients.first().color));
        }
        // case 2.b: some gradients with stop more than 0.0
        else if (firstGreaterThanZero != gradients.count()) {
            QColor col1 = gradients.at(firstGreaterThanZero - 1).color;
            QColor col2 = gradients.at(firstGreaterThanZero).color;
            for (int i = 0; i < firstGreaterThanZero; ++i) {
                gradients.removeFirst();
            }
            gradients.prepend(XpsGradient(0.0, interpolatedColor(col1, col2)));
        }
        // case 2.c: no gradients with stop more than 0.0
        else {
            XpsGradient newGrad(0.0, gradients.last().color);
            gradients.clear();
            gradients.append(newGrad);
        }
    }

    if (gradients.isEmpty()) {
        return;
    }

    // no gradient with stop 1.0 (case 3.)
    if (xpsGradientWithOffset(gradients, 1.0) == -1) {
        int firstLessThanOne = gradients.count() - 1;
        while (firstLessThanOne >= 0 && gradients.at(firstLessThanOne).offset > 1.0) {
            --firstLessThanOne;
        }
        // case 2.a: no gradients with stop greater than 1.0
        if (firstLessThanOne == gradients.count() - 1) {
            gradients.append(XpsGradient(1.0, gradients.last().color));
        }
        // case 2.b: some gradients with stop more than 1.0
        else if (firstLessThanOne != -1) {
            QColor col1 = gradients.at(firstLessThanOne).color;
            QColor col2 = gradients.at(firstLessThanOne + 1).color;
            for (int i = firstLessThanOne + 1; i < gradients.count(); ++i) {
                gradients.removeLast();
            }
            gradients.append(XpsGradient(1.0, interpolatedColor(col1, col2)));
        }
        // case 2.c: no gradients with stop less than 1.0
        else {
            XpsGradient newGrad(1.0, gradients.first().color);
            gradients.clear();
            gradients.append(newGrad);
        }
    }
}

static void addXpsGradientsToQGradient(const QList<XpsGradient> &gradients, QGradient *qgrad)
{
    for (const XpsGradient &grad : gradients) {
        qgrad->setColorAt(grad.offset, grad.color);
    }
}

static void applySpreadStyleToQGradient(const QString &style, QGradient *qgrad)
{
    if (style.isEmpty()) {
        return;
    }

    if (style == QLatin1String("Pad")) {
        qgrad->setSpread(QGradient::PadSpread);
    } else if (style == QLatin1String("Reflect")) {
        qgrad->setSpread(QGradient::ReflectSpread);
    } else if (style == QLatin1String("Repeat")) {
        qgrad->setSpread(QGradient::RepeatSpread);
    }
}

/**
    Read an UnicodeString
    \param raw the raw value of UnicodeString

    \see XPS specification 5.1.4
*/
static QString unicodeString(const QString &raw)
{
    QString ret;
    if (raw.startsWith(QLatin1String("{}"))) {
        ret = raw.mid(2);
    } else {
        ret = raw;
    }
    return ret;
}

XpsHandler::XpsHandler(XpsPage *page)
    : m_page(page)
{
    m_painter = nullptr;
}

XpsHandler::~XpsHandler()
{
}

bool XpsHandler::startDocument()
{
    qCWarning(OkularXpsDebug) << "start document" << m_page->m_fileName;

    XpsRenderNode node;
    node.name = QStringLiteral("document");
    m_nodes.push(node);

    return true;
}

bool XpsHandler::startElement(const QString &nameSpace, const QString &localName, const QString &qname, const QXmlAttributes &atts)
{
    Q_UNUSED(nameSpace)
    Q_UNUSED(qname)

    XpsRenderNode node;
    node.name = localName;
    node.attributes = atts;
    processStartElement(node);
    m_nodes.push(node);

    return true;
}

bool XpsHandler::endElement(const QString &nameSpace, const QString &localName, const QString &qname)
{
    Q_UNUSED(nameSpace)
    Q_UNUSED(qname)

    XpsRenderNode node = m_nodes.pop();
    if (node.name != localName) {
        qCWarning(OkularXpsDebug) << "Name doesn't match";
    }
    processEndElement(node);
    node.children.clear();
    m_nodes.top().children.append(node);

    return true;
}

void XpsHandler::processGlyph(XpsRenderNode &node)
{
    // TODO Currently ignored attributes: CaretStops, DeviceFontName, IsSideways, OpacityMask, Name, FixedPage.NavigateURI, xml:lang, x:key
    // TODO Indices is only partially implemented
    // TODO Currently ignored child elements: Clip, OpacityMask
    // Handled separately: RenderTransform

    QString att;

    m_painter->save();

    // Get font (doesn't work well because qt doesn't allow to load font from file)
    // This works despite the fact that font size isn't specified in points as required by qt. It's because I set point size to be equal to drawing unit.
    float fontSize = node.attributes.value(QStringLiteral("FontRenderingEmSize")).toFloat();
    // qCWarning(OkularXpsDebug) << "Font Rendering EmSize:" << fontSize;
    // a value of 0.0 means the text is not visible (see XPS specs, chapter 12, "Glyphs")
    if (fontSize < 0.1) {
        m_painter->restore();
        return;
    }
    const QString absoluteFileName = absolutePath(entryPath(m_page->fileName()), node.attributes.value(QStringLiteral("FontUri")));
    QFont font = m_page->m_file->getFontByName(absoluteFileName, fontSize);
    att = node.attributes.value(QStringLiteral("StyleSimulations"));
    if (!att.isEmpty()) {
        if (att == QLatin1String("ItalicSimulation")) {
            font.setItalic(true);
        } else if (att == QLatin1String("BoldSimulation")) {
            font.setBold(true);
        } else if (att == QLatin1String("BoldItalicSimulation")) {
            font.setItalic(true);
            font.setBold(true);
        }
    }
    m_painter->setFont(font);

    // Origin
    QPointF origin(node.attributes.value(QStringLiteral("OriginX")).toDouble(), node.attributes.value(QStringLiteral("OriginY")).toDouble());

    // Fill
    QBrush brush;
    att = node.attributes.value(QStringLiteral("Fill"));
    if (att.isEmpty()) {
        QVariant data = node.getChildData(QStringLiteral("Glyphs.Fill"));
        if (data.canConvert<QBrush>()) {
            brush = data.value<QBrush>();
        } else {
            // no "Fill" attribute and no "Glyphs.Fill" child, so show nothing
            // (see XPS specs, 5.10)
            m_painter->restore();
            return;
        }
    } else {
        brush = parseRscRefColorForBrush(att);
        if (brush.style() > Qt::NoBrush && brush.style() < Qt::LinearGradientPattern && brush.color().alpha() == 0) {
            m_painter->restore();
            return;
        }
    }
    m_painter->setBrush(brush);
    m_painter->setPen(QPen(brush, 0));

    // Opacity
    att = node.attributes.value(QStringLiteral("Opacity"));
    if (!att.isEmpty()) {
        bool ok = true;
        double value = att.toDouble(&ok);
        if (ok && value >= 0.1) {
            m_painter->setOpacity(value);
        } else {
            m_painter->restore();
            return;
        }
    }

    // RenderTransform
    att = node.attributes.value(QStringLiteral("RenderTransform"));
    if (!att.isEmpty()) {
        m_painter->setWorldTransform(parseRscRefMatrix(att), true);
    }

    // Clip
    att = node.attributes.value(QStringLiteral("Clip"));
    if (!att.isEmpty()) {
        QPainterPath clipPath = parseRscRefPath(att);
        if (!clipPath.isEmpty()) {
            m_painter->setClipPath(clipPath);
        }
    }

    // BiDiLevel - default Left-to-Right
    m_painter->setLayoutDirection(Qt::LeftToRight);
    att = node.attributes.value(QStringLiteral("BiDiLevel"));
    if (!att.isEmpty()) {
        if ((att.toInt() % 2) == 1) {
            // odd BiDiLevel, so Right-to-Left
            m_painter->setLayoutDirection(Qt::RightToLeft);
        }
    }

    // Indices - partial handling only
    att = node.attributes.value(QStringLiteral("Indices"));
    QList<qreal> advanceWidths;
    if (!att.isEmpty()) {
        QStringList indicesElements = att.split(QLatin1Char(';'));
        for (int i = 0; i < indicesElements.size(); ++i) {
            if (indicesElements.at(i).contains(QStringLiteral(","))) {
                QStringList parts = indicesElements.at(i).split(QLatin1Char(','));
                if (parts.size() == 2) {
                    // regular advance case, no offsets
                    advanceWidths.append(parts.at(1).toDouble() * fontSize / 100.0);
                } else if (parts.size() == 3) {
                    // regular advance case, with uOffset
                    qreal AdvanceWidth = parts.at(1).toDouble() * fontSize / 100.0;
                    qreal uOffset = parts.at(2).toDouble() / 100.0;
                    advanceWidths.append(AdvanceWidth + uOffset);
                } else {
                    // has vertical offset, but don't know how to handle that yet
                    qCWarning(OkularXpsDebug) << "Unhandled Indices element: " << indicesElements.at(i);
                    advanceWidths.append(-1.0);
                }
            } else {
                // no special advance case
                advanceWidths.append(-1.0);
            }
        }
    }

    // UnicodeString
    QString stringToDraw(unicodeString(node.attributes.value(QStringLiteral("UnicodeString"))));
    QPointF originAdvance(0, 0);
    QFontMetrics metrics = m_painter->fontMetrics();
    for (int i = 0; i < stringToDraw.size(); ++i) {
        QChar thisChar = stringToDraw.at(i);
        m_painter->drawText(origin + originAdvance, QString(thisChar));
        const qreal advanceWidth = advanceWidths.value(i, qreal(-1.0));
        if (advanceWidth > 0.0) {
            originAdvance.rx() += advanceWidth;
        } else {
            originAdvance.rx() += metrics.horizontalAdvance(thisChar);
        }
    }
    // qCWarning(OkularXpsDebug) << "Glyphs: " << atts.value("Fill") << ", " << atts.value("FontUri");
    // qCWarning(OkularXpsDebug) << "    Origin: " << atts.value("OriginX") << "," << atts.value("OriginY");
    // qCWarning(OkularXpsDebug) << "    Unicode: " << atts.value("UnicodeString");

    m_painter->restore();
}

void XpsHandler::processFill(XpsRenderNode &node)
{
    // TODO Ignored child elements: VirtualBrush

    if (node.children.size() != 1) {
        qCWarning(OkularXpsDebug) << "Fill element should have exactly one child";
    } else {
        node.data = node.children[0].data;
    }
}

void XpsHandler::processStroke(XpsRenderNode &node)
{
    // TODO Ignored child elements: VirtualBrush

    if (node.children.size() != 1) {
        qCWarning(OkularXpsDebug) << "Stroke element should have exactly one child";
    } else {
        node.data = node.children[0].data;
    }
}

void XpsHandler::processImageBrush(XpsRenderNode &node)
{
    // TODO Ignored attributes: Opacity, x:key, TileMode, ViewBoxUnits, ViewPortUnits
    // TODO Check whether transformation works for non standard situations (viewbox different that whole image, Transform different that simple move & scale, Viewport different than [0, 0, 1, 1]

    QString att;
    QBrush brush;

    QRectF viewport = stringToRectF(node.attributes.value(QStringLiteral("Viewport")));
    QRectF viewbox = stringToRectF(node.attributes.value(QStringLiteral("Viewbox")));
    QImage image = m_page->loadImageFromFile(node.attributes.value(QStringLiteral("ImageSource")));

    // Matrix which can transform [0, 0, 1, 1] rectangle to given viewbox
    QTransform viewboxMatrix = QTransform(viewbox.width() * image.physicalDpiX() / 96, 0, 0, viewbox.height() * image.physicalDpiY() / 96, viewbox.x(), viewbox.y());

    // Matrix which can transform [0, 0, 1, 1] rectangle to given viewport
    // TODO Take ViewPort into account
    QTransform viewportMatrix;
    att = node.attributes.value(QStringLiteral("Transform"));
    if (att.isEmpty()) {
        QVariant data = node.getChildData(QStringLiteral("ImageBrush.Transform"));
        if (data.canConvert<QTransform>()) {
            viewportMatrix = data.value<QTransform>();
        } else {
            viewportMatrix = QTransform();
        }
    } else {
        viewportMatrix = parseRscRefMatrix(att);
    }
    viewportMatrix = viewportMatrix * QTransform(viewport.width(), 0, 0, viewport.height(), viewport.x(), viewport.y());

    brush = QBrush(image);
    brush.setTransform(viewboxMatrix.inverted() * viewportMatrix);

    node.data = QVariant::fromValue(brush);
}

void XpsHandler::processPath(XpsRenderNode &node)
{
    // TODO Ignored attributes: Clip, OpacityMask, StrokeEndLineCap, StorkeStartLineCap, Name, FixedPage.NavigateURI, xml:lang, x:key, AutomationProperties.Name, AutomationProperties.HelpText, SnapsToDevicePixels
    // TODO Ignored child elements: RenderTransform, Clip, OpacityMask
    // Handled separately: RenderTransform
    m_painter->save();

    QString att;
    QVariant data;

    // Get path
    XpsPathGeometry *pathdata = node.getChildData(QStringLiteral("Path.Data")).value<XpsPathGeometry *>();
    att = node.attributes.value(QStringLiteral("Data"));
    if (!att.isEmpty()) {
        QPainterPath path = parseRscRefPath(att);
        delete pathdata;
        pathdata = new XpsPathGeometry();
        pathdata->paths.append(new XpsPathFigure(path, true));
    }
    if (!pathdata) {
        // nothing to draw
        m_painter->restore();
        return;
    }

    // Set Fill
    att = node.attributes.value(QStringLiteral("Fill"));
    QBrush brush;
    if (!att.isEmpty()) {
        brush = parseRscRefColorForBrush(att);
    } else {
        data = node.getChildData(QStringLiteral("Path.Fill"));
        if (data.canConvert<QBrush>()) {
            brush = data.value<QBrush>();
        }
    }
    m_painter->setBrush(brush);

    // Stroke (pen)
    att = node.attributes.value(QStringLiteral("Stroke"));
    QPen pen(Qt::transparent);
    if (!att.isEmpty()) {
        pen = parseRscRefColorForPen(att);
    } else {
        data = node.getChildData(QStringLiteral("Path.Stroke"));
        if (data.canConvert<QBrush>()) {
            pen.setBrush(data.value<QBrush>());
        }
    }
    att = node.attributes.value(QStringLiteral("StrokeThickness"));
    if (!att.isEmpty()) {
        bool ok = false;
        int thickness = att.toInt(&ok);
        if (ok) {
            pen.setWidth(thickness);
        }
    }
    att = node.attributes.value(QStringLiteral("StrokeDashArray"));
    if (!att.isEmpty()) {
        const QStringList pieces = att.split(QLatin1Char(' '), QString::SkipEmptyParts);
        QVector<qreal> dashPattern(pieces.count());
        bool ok = false;
        for (int i = 0; i < pieces.count(); ++i) {
            qreal value = pieces.at(i).toInt(&ok);
            if (ok) {
                dashPattern[i] = value;
            } else {
                break;
            }
        }
        if (ok) {
            pen.setDashPattern(dashPattern);
        }
    }
    att = node.attributes.value(QStringLiteral("StrokeDashOffset"));
    if (!att.isEmpty()) {
        bool ok = false;
        int offset = att.toInt(&ok);
        if (ok) {
            pen.setDashOffset(offset);
        }
    }
    att = node.attributes.value(QStringLiteral("StrokeDashCap"));
    if (!att.isEmpty()) {
        Qt::PenCapStyle cap = Qt::FlatCap;
        if (att == QLatin1String("Flat")) {
            cap = Qt::FlatCap;
        } else if (att == QLatin1String("Round")) {
            cap = Qt::RoundCap;
        } else if (att == QLatin1String("Square")) {
            cap = Qt::SquareCap;
        }
        // ### missing "Triangle"
        pen.setCapStyle(cap);
    }
    att = node.attributes.value(QStringLiteral("StrokeLineJoin"));
    if (!att.isEmpty()) {
        Qt::PenJoinStyle joinStyle = Qt::MiterJoin;
        if (att == QLatin1String("Miter")) {
            joinStyle = Qt::MiterJoin;
        } else if (att == QLatin1String("Bevel")) {
            joinStyle = Qt::BevelJoin;
        } else if (att == QLatin1String("Round")) {
            joinStyle = Qt::RoundJoin;
        }
        pen.setJoinStyle(joinStyle);
    }
    att = node.attributes.value(QStringLiteral("StrokeMiterLimit"));
    if (!att.isEmpty()) {
        bool ok = false;
        double limit = att.toDouble(&ok);
        if (ok) {
            // we have to divide it by two, as XPS consider half of the stroke width,
            // while Qt the whole of it
            pen.setMiterLimit(limit / 2);
        }
    }
    m_painter->setPen(pen);

    // Opacity
    att = node.attributes.value(QStringLiteral("Opacity"));
    if (!att.isEmpty()) {
        m_painter->setOpacity(att.toDouble());
    }

    // RenderTransform
    att = node.attributes.value(QStringLiteral("RenderTransform"));
    if (!att.isEmpty()) {
        m_painter->setWorldTransform(parseRscRefMatrix(att), true);
    }
    if (!pathdata->transform.isIdentity()) {
        m_painter->setWorldTransform(pathdata->transform, true);
    }

    for (const XpsPathFigure *figure : qAsConst(pathdata->paths)) {
        m_painter->setBrush(figure->isFilled ? brush : QBrush());
        m_painter->drawPath(figure->path);
    }

    delete pathdata;

    m_painter->restore();
}

void XpsHandler::processPathData(XpsRenderNode &node)
{
    if (node.children.size() != 1) {
        qCWarning(OkularXpsDebug) << "Path.Data element should have exactly one child";
    } else {
        node.data = node.children[0].data;
    }
}

void XpsHandler::processPathGeometry(XpsRenderNode &node)
{
    XpsPathGeometry *geom = new XpsPathGeometry();

    for (const XpsRenderNode &child : qAsConst(node.children)) {
        if (child.data.canConvert<XpsPathFigure *>()) {
            XpsPathFigure *figure = child.data.value<XpsPathFigure *>();
            geom->paths.append(figure);
        }
    }

    QString att;

    att = node.attributes.value(QStringLiteral("Figures"));
    if (!att.isEmpty()) {
        QPainterPath path = parseRscRefPath(att);
        qDeleteAll(geom->paths);
        geom->paths.clear();
        geom->paths.append(new XpsPathFigure(path, true));
    }

    att = node.attributes.value(QStringLiteral("FillRule"));
    if (!att.isEmpty()) {
        geom->fillRule = fillRuleFromString(att);
    }

    // Transform
    att = node.attributes.value(QStringLiteral("Transform"));
    if (!att.isEmpty()) {
        geom->transform = parseRscRefMatrix(att);
    }

    if (!geom->paths.isEmpty()) {
        node.data = QVariant::fromValue(geom);
    } else {
        delete geom;
    }
}

void XpsHandler::processPathFigure(XpsRenderNode &node)
{
    // TODO Ignored child elements: ArcSegment

    QString att;
    QPainterPath path;

    att = node.attributes.value(QStringLiteral("StartPoint"));
    if (!att.isEmpty()) {
        QPointF point = getPointFromString(att);
        path.moveTo(point);
    } else {
        return;
    }

    for (const XpsRenderNode &child : qAsConst(node.children)) {
        bool isStroked = true;
        att = node.attributes.value(QStringLiteral("IsStroked"));
        if (!att.isEmpty()) {
            isStroked = att == QLatin1String("true");
        }
        if (!isStroked) {
            continue;
        }

        // PolyLineSegment
        if (child.name == QLatin1String("PolyLineSegment")) {
            att = child.attributes.value(QStringLiteral("Points"));
            if (!att.isEmpty()) {
                const QStringList points = att.split(QLatin1Char(' '), QString::SkipEmptyParts);
                for (const QString &p : points) {
                    QPointF point = getPointFromString(p);
                    path.lineTo(point);
                }
            }
        }
        // PolyBezierSegment
        else if (child.name == QLatin1String("PolyBezierSegment")) {
            att = child.attributes.value(QStringLiteral("Points"));
            if (!att.isEmpty()) {
                const QStringList points = att.split(QLatin1Char(' '), QString::SkipEmptyParts);
                if (points.count() % 3 == 0) {
                    for (int i = 0; i < points.count();) {
                        QPointF firstControl = getPointFromString(points.at(i++));
                        QPointF secondControl = getPointFromString(points.at(i++));
                        QPointF endPoint = getPointFromString(points.at(i++));
                        path.cubicTo(firstControl, secondControl, endPoint);
                    }
                }
            }
        }
        // PolyQuadraticBezierSegment
        else if (child.name == QLatin1String("PolyQuadraticBezierSegment")) {
            att = child.attributes.value(QStringLiteral("Points"));
            if (!att.isEmpty()) {
                const QStringList points = att.split(QLatin1Char(' '), QString::SkipEmptyParts);
                if (points.count() % 2 == 0) {
                    for (int i = 0; i < points.count();) {
                        QPointF point1 = getPointFromString(points.at(i++));
                        QPointF point2 = getPointFromString(points.at(i++));
                        path.quadTo(point1, point2);
                    }
                }
            }
        }
    }

    bool closePath = false;
    att = node.attributes.value(QStringLiteral("IsClosed"));
    if (!att.isEmpty()) {
        closePath = att == QLatin1String("true");
    }
    if (closePath) {
        path.closeSubpath();
    }

    bool isFilled = true;
    att = node.attributes.value(QStringLiteral("IsFilled"));
    if (!att.isEmpty()) {
        isFilled = att == QLatin1String("true");
    }

    if (!path.isEmpty()) {
        node.data = QVariant::fromValue(new XpsPathFigure(path, isFilled));
    }
}

void XpsHandler::processStartElement(XpsRenderNode &node)
{
    if (node.name == QLatin1String("Canvas")) {
        m_painter->save();
        QString att = node.attributes.value(QStringLiteral("RenderTransform"));
        if (!att.isEmpty()) {
            m_painter->setWorldTransform(parseRscRefMatrix(att), true);
        }
        att = node.attributes.value(QStringLiteral("Opacity"));
        if (!att.isEmpty()) {
            double value = att.toDouble();
            if (value > 0.0 && value <= 1.0) {
                m_painter->setOpacity(m_painter->opacity() * value);
            } else {
                // setting manually to 0 is necessary to "disable"
                // all the stuff inside
                m_painter->setOpacity(0.0);
            }
        }
    }
}

void XpsHandler::processEndElement(XpsRenderNode &node)
{
    if (node.name == QLatin1String("Glyphs")) {
        processGlyph(node);
    } else if (node.name == QLatin1String("Path")) {
        processPath(node);
    } else if (node.name == QLatin1String("MatrixTransform")) {
        // TODO Ignoring x:key
        node.data = QVariant::fromValue(QTransform(attsToMatrix(node.attributes.value(QStringLiteral("Matrix")))));
    } else if ((node.name == QLatin1String("Canvas.RenderTransform")) || (node.name == QLatin1String("Glyphs.RenderTransform")) || (node.name == QLatin1String("Path.RenderTransform"))) {
        QVariant data = node.getRequiredChildData(QStringLiteral("MatrixTransform"));
        if (data.canConvert<QTransform>()) {
            m_painter->setWorldTransform(data.value<QTransform>(), true);
        }
    } else if (node.name == QLatin1String("Canvas")) {
        m_painter->restore();
    } else if ((node.name == QLatin1String("Path.Fill")) || (node.name == QLatin1String("Glyphs.Fill"))) {
        processFill(node);
    } else if (node.name == QLatin1String("Path.Stroke")) {
        processStroke(node);
    } else if (node.name == QLatin1String("SolidColorBrush")) {
        // TODO Ignoring opacity, x:key
        node.data = QVariant::fromValue(QBrush(QColor(hexToRgba(node.attributes.value(QStringLiteral("Color")).toLatin1()))));
    } else if (node.name == QLatin1String("ImageBrush")) {
        processImageBrush(node);
    } else if (node.name == QLatin1String("ImageBrush.Transform")) {
        node.data = node.getRequiredChildData(QStringLiteral("MatrixTransform"));
    } else if (node.name == QLatin1String("LinearGradientBrush")) {
        const XpsRenderNode *gradients = node.findChild(QStringLiteral("LinearGradientBrush.GradientStops"));
        if (gradients && gradients->data.canConvert<QGradient *>()) {
            QPointF start = getPointFromString(node.attributes.value(QStringLiteral("StartPoint")));
            QPointF end = getPointFromString(node.attributes.value(QStringLiteral("EndPoint")));
            QLinearGradient *qgrad = static_cast<QLinearGradient *>(gradients->data.value<QGradient *>());
            qgrad->setStart(start);
            qgrad->setFinalStop(end);
            applySpreadStyleToQGradient(node.attributes.value(QStringLiteral("SpreadMethod")), qgrad);
            node.data = QVariant::fromValue(QBrush(*qgrad));
            delete qgrad;
        }
    } else if (node.name == QLatin1String("RadialGradientBrush")) {
        const XpsRenderNode *gradients = node.findChild(QStringLiteral("RadialGradientBrush.GradientStops"));
        if (gradients && gradients->data.canConvert<QGradient *>()) {
            QPointF center = getPointFromString(node.attributes.value(QStringLiteral("Center")));
            QPointF origin = getPointFromString(node.attributes.value(QStringLiteral("GradientOrigin")));
            double radiusX = node.attributes.value(QStringLiteral("RadiusX")).toDouble();
            double radiusY = node.attributes.value(QStringLiteral("RadiusY")).toDouble();
            QRadialGradient *qgrad = static_cast<QRadialGradient *>(gradients->data.value<QGradient *>());
            qgrad->setCenter(center);
            qgrad->setFocalPoint(origin);
            // TODO what in case of different radii?
            qgrad->setRadius(qMin(radiusX, radiusY));
            applySpreadStyleToQGradient(node.attributes.value(QStringLiteral("SpreadMethod")), qgrad);
            node.data = QVariant::fromValue(QBrush(*qgrad));
            delete qgrad;
        }
    } else if (node.name == QLatin1String("LinearGradientBrush.GradientStops")) {
        QList<XpsGradient> gradients;
        for (const XpsRenderNode &child : qAsConst(node.children)) {
            double offset = child.attributes.value(QStringLiteral("Offset")).toDouble();
            QColor color = hexToRgba(child.attributes.value(QStringLiteral("Color")).toLatin1());
            gradients.append(XpsGradient(offset, color));
        }
        preprocessXpsGradients(gradients);
        if (!gradients.isEmpty()) {
            QLinearGradient *qgrad = new QLinearGradient();
            addXpsGradientsToQGradient(gradients, qgrad);
            node.data = QVariant::fromValue<QGradient *>(qgrad);
        }
    } else if (node.name == QLatin1String("RadialGradientBrush.GradientStops")) {
        QList<XpsGradient> gradients;
        for (const XpsRenderNode &child : qAsConst(node.children)) {
            double offset = child.attributes.value(QStringLiteral("Offset")).toDouble();
            QColor color = hexToRgba(child.attributes.value(QStringLiteral("Color")).toLatin1());
            gradients.append(XpsGradient(offset, color));
        }
        preprocessXpsGradients(gradients);
        if (!gradients.isEmpty()) {
            QRadialGradient *qgrad = new QRadialGradient();
            addXpsGradientsToQGradient(gradients, qgrad);
            node.data = QVariant::fromValue<QGradient *>(qgrad);
        }
    } else if (node.name == QLatin1String("PathFigure")) {
        processPathFigure(node);
    } else if (node.name == QLatin1String("PathGeometry")) {
        processPathGeometry(node);
    } else if (node.name == QLatin1String("Path.Data")) {
        processPathData(node);
    } else {
        // qCWarning(OkularXpsDebug) << "Unknown element: " << node->name;
    }
}

XpsPage::XpsPage(XpsFile *file, const QString &fileName)
    : m_file(file)
    , m_fileName(fileName)
    , m_pageIsRendered(false)
{
    m_pageImage = nullptr;

    // qCWarning(OkularXpsDebug) << "page file name: " << fileName;

    const KZipFileEntry *pageFile = static_cast<const KZipFileEntry *>(m_file->xpsArchive()->directory()->entry(fileName));

    QXmlStreamReader xml;
    xml.addData(readFileOrDirectoryParts(pageFile));
    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isStartElement() && (xml.name() == QStringLiteral("FixedPage"))) {
            QXmlStreamAttributes attributes = xml.attributes();
            m_pageSize.setWidth(attributes.value(QStringLiteral("Width")).toString().toDouble());
            m_pageSize.setHeight(attributes.value(QStringLiteral("Height")).toString().toDouble());
            break;
        }
    }
    if (xml.error()) {
        qCWarning(OkularXpsDebug) << "Could not parse XPS page:" << xml.errorString();
    }
}

XpsPage::~XpsPage()
{
    delete m_pageImage;
}

bool XpsPage::renderToImage(QImage *p)
{
    if ((m_pageImage == nullptr) || (m_pageImage->size() != p->size())) {
        delete m_pageImage;
        m_pageImage = new QImage(p->size(), QImage::Format_ARGB32);
        // Set one point = one drawing unit. Useful for fonts, because xps specifies font size using drawing units, not points as usual
        m_pageImage->setDotsPerMeterX(2835);
        m_pageImage->setDotsPerMeterY(2835);

        m_pageIsRendered = false;
    }
    if (!m_pageIsRendered) {
        m_pageImage->fill(qRgba(255, 255, 255, 255));
        QPainter painter(m_pageImage);
        renderToPainter(&painter);
        m_pageIsRendered = true;
    }

    *p = *m_pageImage;

    return true;
}

bool XpsPage::renderToPainter(QPainter *painter)
{
    XpsHandler handler(this);
    handler.m_painter = painter;
    handler.m_painter->setWorldTransform(QTransform().scale((qreal)painter->device()->width() / size().width(), (qreal)painter->device()->height() / size().height()));
    QXmlSimpleReader parser;
    parser.setContentHandler(&handler);
    parser.setErrorHandler(&handler);
    const KZipFileEntry *pageFile = static_cast<const KZipFileEntry *>(m_file->xpsArchive()->directory()->entry(m_fileName));
    QByteArray data = readFileOrDirectoryParts(pageFile);
    QBuffer buffer(&data);
    QXmlInputSource source(&buffer);
    bool ok = parser.parse(source);
    qCWarning(OkularXpsDebug) << "Parse result: " << ok;

    return true;
}

QSizeF XpsPage::size() const
{
    return m_pageSize;
}

QFont XpsFile::getFontByName(const QString &absoluteFileName, float size)
{
    // qCWarning(OkularXpsDebug) << "trying to get font: " << fileName << ", size: " << size;

    int index = m_fontCache.value(absoluteFileName, -1);
    if (index == -1) {
        index = loadFontByName(absoluteFileName);
        m_fontCache[absoluteFileName] = index;
    }
    if (index == -1) {
        qCWarning(OkularXpsDebug) << "Requesting unknown font:" << absoluteFileName;
        return QFont();
    }

    const QStringList fontFamilies = m_fontDatabase.applicationFontFamilies(index);
    if (fontFamilies.isEmpty()) {
        qCWarning(OkularXpsDebug) << "The unexpected has happened. No font family for a known font:" << absoluteFileName << index;
        return QFont();
    }
    const QString &fontFamily = fontFamilies[0];
    const QStringList fontStyles = m_fontDatabase.styles(fontFamily);
    if (fontStyles.isEmpty()) {
        qCWarning(OkularXpsDebug) << "The unexpected has happened. No font style for a known font family:" << absoluteFileName << index << fontFamily;
        return QFont();
    }
    const QString &fontStyle = fontStyles[0];
    return m_fontDatabase.font(fontFamily, fontStyle, qRound(size));
}

int XpsFile::loadFontByName(const QString &absoluteFileName)
{
    // qCWarning(OkularXpsDebug) << "font file name: " << absoluteFileName;

    const KArchiveEntry *fontFile = loadEntry(m_xpsArchive, absoluteFileName, Qt::CaseInsensitive);
    if (!fontFile) {
        return -1;
    }

    QByteArray fontData = readFileOrDirectoryParts(fontFile); // once per file, according to the docs

    int result = m_fontDatabase.addApplicationFontFromData(fontData);
    if (-1 == result) {
        // Try to deobfuscate font
        // TODO Use deobfuscation depending on font content type, don't do it always when standard loading fails

        const QString baseName = resourceName(absoluteFileName);

        unsigned short guid[16];
        if (!parseGUID(baseName, guid)) {
            qCWarning(OkularXpsDebug) << "File to load font - file name isn't a GUID";
        } else {
            if (fontData.length() < 32) {
                qCWarning(OkularXpsDebug) << "Font file is too small";
            } else {
                // Obfuscation - xor bytes in font binary with bytes from guid (font's filename)
                const static int mapping[] = {15, 14, 13, 12, 11, 10, 9, 8, 6, 7, 4, 5, 0, 1, 2, 3};
                for (int i = 0; i < 16; i++) {
                    fontData[i] = fontData[i] ^ guid[mapping[i]];
                    fontData[i + 16] = fontData[i + 16] ^ guid[mapping[i]];
                }
                result = m_fontDatabase.addApplicationFontFromData(fontData);
            }
        }
    }

    // qCWarning(OkularXpsDebug) << "Loaded font: " << m_fontDatabase.applicationFontFamilies( result );

    return result; // a font ID
}

KZip *XpsFile::xpsArchive()
{
    return m_xpsArchive;
}

QImage XpsPage::loadImageFromFile(const QString &fileName)
{
    // qCWarning(OkularXpsDebug) << "image file name: " << fileName;

    if (fileName.at(0) == QLatin1Char('{')) {
        // for example: '{ColorConvertedBitmap /Resources/bla.wdp /Resources/foobar.icc}'
        // TODO: properly read a ColorConvertedBitmap
        return QImage();
    }

    QString absoluteFileName = absolutePath(entryPath(m_fileName), fileName);
    const KArchiveEntry *imageFile = loadEntry(m_file->xpsArchive(), absoluteFileName, Qt::CaseInsensitive);
    if (!imageFile) {
        // image not found
        return QImage();
    }

    /* WORKAROUND:
        XPS standard requires to use 96dpi for images which doesn't have dpi specified (in file). When Qt loads such an image,
        it sets its dpi to qt_defaultDpi and doesn't allow to find out that it happend.

        To workaround this I used this procedure: load image, set its dpi to 96, load image again. When dpi isn't set in file,
        dpi set by me stays unchanged.

        Trolltech task ID: 159527.

    */

    QImage image;
    QByteArray data = readFileOrDirectoryParts(imageFile);

    QBuffer buffer(&data);
    buffer.open(QBuffer::ReadOnly);

    QImageReader reader(&buffer);
    image = reader.read();

    image.setDotsPerMeterX(qRound(96 / 0.0254));
    image.setDotsPerMeterY(qRound(96 / 0.0254));

    buffer.seek(0);
    reader.setDevice(&buffer);
    reader.read(&image);

    return image;
}

Okular::TextPage *XpsPage::textPage()
{
    // qCWarning(OkularXpsDebug) << "Parsing XpsPage, text extraction";

    Okular::TextPage *textPage = new Okular::TextPage();

    const KZipFileEntry *pageFile = static_cast<const KZipFileEntry *>(m_file->xpsArchive()->directory()->entry(m_fileName));
    QXmlStreamReader xml;
    xml.addData(readFileOrDirectoryParts(pageFile));

    QTransform matrix = QTransform();
    QStack<QTransform> matrices;
    matrices.push(QTransform());
    bool useMatrix = false;
    QXmlStreamAttributes glyphsAtts;

    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isStartElement()) {
            if (xml.name() == QStringLiteral("Canvas")) {
                matrices.push(matrix);

                QString att = xml.attributes().value(QStringLiteral("RenderTransform")).toString();
                if (!att.isEmpty()) {
                    matrix = parseRscRefMatrix(att) * matrix;
                }
            } else if ((xml.name() == QStringLiteral("Canvas.RenderTransform")) || (xml.name() == QStringLiteral("Glyphs.RenderTransform"))) {
                useMatrix = true;
            } else if (xml.name() == QStringLiteral("MatrixTransform")) {
                if (useMatrix) {
                    matrix = attsToMatrix(xml.attributes().value(QStringLiteral("Matrix")).toString()) * matrix;
                }
            } else if (xml.name() == QStringLiteral("Glyphs")) {
                matrices.push(matrix);
                glyphsAtts = xml.attributes();
            } else if ((xml.name() == QStringLiteral("Path")) || (xml.name() == QStringLiteral("Path.Fill")) || (xml.name() == QStringLiteral("SolidColorBrush")) || (xml.name() == QStringLiteral("ImageBrush")) ||
                       (xml.name() == QStringLiteral("ImageBrush.Transform")) || (xml.name() == QStringLiteral("Path.OpacityMask")) || (xml.name() == QStringLiteral("Path.Data")) || (xml.name() == QStringLiteral("PathGeometry")) ||
                       (xml.name() == QStringLiteral("PathFigure")) || (xml.name() == QStringLiteral("PolyLineSegment"))) {
                // those are only graphical - no use in text handling
            } else if ((xml.name() == QStringLiteral("FixedPage")) || (xml.name() == QStringLiteral("FixedPage.Resources"))) {
                // not useful for text extraction
            } else {
                qCWarning(OkularXpsDebug) << "Unhandled element in Text Extraction start: " << xml.name().toString();
            }
        } else if (xml.isEndElement()) {
            if (xml.name() == QStringLiteral("Canvas")) {
                matrix = matrices.pop();
            } else if ((xml.name() == QStringLiteral("Canvas.RenderTransform")) || (xml.name() == QStringLiteral("Glyphs.RenderTransform"))) {
                useMatrix = false;
            } else if (xml.name() == QStringLiteral("MatrixTransform")) {
                // not clear if we need to do anything here yet.
            } else if (xml.name() == QStringLiteral("Glyphs")) {
                QString att = glyphsAtts.value(QStringLiteral("RenderTransform")).toString();
                if (!att.isEmpty()) {
                    matrix = parseRscRefMatrix(att) * matrix;
                }
                QString text = unicodeString(glyphsAtts.value(QStringLiteral("UnicodeString")).toString());

                // Get font (doesn't work well because qt doesn't allow to load font from file)
                const QString absoluteFileName = absolutePath(entryPath(m_fileName), glyphsAtts.value(QStringLiteral("FontUri")).toString());
                QFont font = m_file->getFontByName(absoluteFileName, glyphsAtts.value(QStringLiteral("FontRenderingEmSize")).toString().toFloat() * 72 / 96);
                QFontMetrics metrics = QFontMetrics(font);
                // Origin
                QPointF origin(glyphsAtts.value(QStringLiteral("OriginX")).toString().toDouble(), glyphsAtts.value(QStringLiteral("OriginY")).toString().toDouble());

                int lastWidth = 0;
                for (int i = 0; i < text.length(); i++) {
                    const int width = metrics.horizontalAdvance(text, i + 1);

                    Okular::NormalizedRect *rect =
                        new Okular::NormalizedRect((origin.x() + lastWidth) / m_pageSize.width(), (origin.y() - metrics.height()) / m_pageSize.height(), (origin.x() + width) / m_pageSize.width(), origin.y() / m_pageSize.height());
                    rect->transform(matrix);
                    textPage->append(text.mid(i, 1), rect);

                    lastWidth = width;
                }

                matrix = matrices.pop();
            } else if ((xml.name() == QStringLiteral("Path")) || (xml.name() == QStringLiteral("Path.Fill")) || (xml.name() == QStringLiteral("SolidColorBrush")) || (xml.name() == QStringLiteral("ImageBrush")) ||
                       (xml.name() == QStringLiteral("ImageBrush.Transform")) || (xml.name() == QStringLiteral("Path.OpacityMask")) || (xml.name() == QStringLiteral("Path.Data")) || (xml.name() == QStringLiteral("PathGeometry")) ||
                       (xml.name() == QStringLiteral("PathFigure")) || (xml.name() == QStringLiteral("PolyLineSegment"))) {
                // those are only graphical - no use in text handling
            } else if ((xml.name() == QStringLiteral("FixedPage")) || (xml.name() == QStringLiteral("FixedPage.Resources"))) {
                // not useful for text extraction
            } else {
                qCWarning(OkularXpsDebug) << "Unhandled element in Text Extraction end: " << xml.name().toString();
            }
        }
    }
    if (xml.error()) {
        qCWarning(OkularXpsDebug) << "Error parsing XpsPage text: " << xml.errorString();
    }
    return textPage;
}

void XpsDocument::parseDocumentStructure(const QString &documentStructureFileName)
{
    qCWarning(OkularXpsDebug) << "document structure file name: " << documentStructureFileName;
    m_haveDocumentStructure = false;

    const KZipFileEntry *documentStructureFile = static_cast<const KZipFileEntry *>(m_file->xpsArchive()->directory()->entry(documentStructureFileName));

    QXmlStreamReader xml;
    xml.addData(documentStructureFile->data());

    while (!xml.atEnd()) {
        xml.readNext();

        if (xml.isStartElement()) {
            if (xml.name() == QStringLiteral("DocumentStructure")) {
                // just a container for optional outline and story elements - nothing to do here
            } else if (xml.name() == QStringLiteral("DocumentStructure.Outline")) {
                qCWarning(OkularXpsDebug) << "found DocumentStructure.Outline";
            } else if (xml.name() == QStringLiteral("DocumentOutline")) {
                qCWarning(OkularXpsDebug) << "found DocumentOutline";
                m_docStructure = new Okular::DocumentSynopsis;
            } else if (xml.name() == QStringLiteral("OutlineEntry")) {
                m_haveDocumentStructure = true;
                QXmlStreamAttributes attributes = xml.attributes();
                int outlineLevel = attributes.value(QStringLiteral("OutlineLevel")).toString().toInt();
                QString description = attributes.value(QStringLiteral("Description")).toString();
                QDomElement synopsisElement = m_docStructure->createElement(description);
                synopsisElement.setAttribute(QStringLiteral("OutlineLevel"), outlineLevel);
                QString target = attributes.value(QStringLiteral("OutlineTarget")).toString();
                int hashPosition = target.lastIndexOf(QLatin1Char('#'));
                target = target.mid(hashPosition + 1);
                // qCWarning(OkularXpsDebug) << "target: " << target;
                Okular::DocumentViewport viewport;
                viewport.pageNumber = m_docStructurePageMap.value(target);
                synopsisElement.setAttribute(QStringLiteral("Viewport"), viewport.toString());
                if (outlineLevel == 1) {
                    // qCWarning(OkularXpsDebug) << "Description: "
                    // << outlineEntryElement.attribute( "Description" );
                    m_docStructure->appendChild(synopsisElement);
                } else {
                    // find the last next highest element (so it this is level 3, we need
                    // to find the most recent level 2 node)
                    QDomNode maybeParentNode = m_docStructure->lastChild();
                    while (!maybeParentNode.isNull()) {
                        if (maybeParentNode.toElement().attribute(QStringLiteral("OutlineLevel")).toInt() == (outlineLevel - 1)) {
                            // we have the right parent
                            maybeParentNode.appendChild(synopsisElement);
                            break;
                        }
                        maybeParentNode = maybeParentNode.lastChild();
                    }
                }
            } else if (xml.name() == QStringLiteral("Story")) {
                // we need to handle Story here, but I have no idea what to do with it.
            } else if (xml.name() == QStringLiteral("StoryFragment")) {
                // we need to handle StoryFragment here, but I have no idea what to do with it.
            } else if (xml.name() == QStringLiteral("StoryFragmentReference")) {
                // we need to handle StoryFragmentReference here, but I have no idea what to do with it.
            } else {
                qCWarning(OkularXpsDebug) << "Unhandled entry in DocumentStructure: " << xml.name().toString();
            }
        }
    }
}

const Okular::DocumentSynopsis *XpsDocument::documentStructure()
{
    return m_docStructure;
}

bool XpsDocument::hasDocumentStructure()
{
    return m_haveDocumentStructure;
}

XpsDocument::XpsDocument(XpsFile *file, const QString &fileName)
    : m_file(file)
    , m_haveDocumentStructure(false)
    , m_docStructure(nullptr)
{
    qCWarning(OkularXpsDebug) << "document file name: " << fileName;

    const KArchiveEntry *documentEntry = file->xpsArchive()->directory()->entry(fileName);
    QString documentFilePath = fileName;
    const QString documentEntryPath = entryPath(fileName);

    QXmlStreamReader docXml;
    docXml.addData(readFileOrDirectoryParts(documentEntry, &documentFilePath));
    while (!docXml.atEnd()) {
        docXml.readNext();
        if (docXml.isStartElement()) {
            if (docXml.name() == QStringLiteral("PageContent")) {
                QString pagePath = docXml.attributes().value(QStringLiteral("Source")).toString();
                qCWarning(OkularXpsDebug) << "Page Path: " << pagePath;
                XpsPage *page = new XpsPage(file, absolutePath(documentFilePath, pagePath));
                m_pages.append(page);
            } else if (docXml.name() == QStringLiteral("PageContent.LinkTargets")) {
                // do nothing - wait for the real LinkTarget elements
            } else if (docXml.name() == QStringLiteral("LinkTarget")) {
                QString targetName = docXml.attributes().value(QStringLiteral("Name")).toString();
                if (!targetName.isEmpty()) {
                    m_docStructurePageMap[targetName] = m_pages.count() - 1;
                }
            } else if (docXml.name() == QStringLiteral("FixedDocument")) {
                // we just ignore this - it is just a container
            } else {
                qCWarning(OkularXpsDebug) << "Unhandled entry in FixedDocument: " << docXml.name().toString();
            }
        }
    }
    if (docXml.error()) {
        qCWarning(OkularXpsDebug) << "Could not parse main XPS document file: " << docXml.errorString();
    }

    // There might be a relationships entry for this document - typically used to tell us where to find the
    // content structure description

    // We should be able to find this using a reference from some other part of the document, but I can't see it.
    const int slashPosition = fileName.lastIndexOf(QLatin1Char('/'));
    const QString documentRelationshipFile = absolutePath(documentEntryPath, QStringLiteral("_rels/") + fileName.mid(slashPosition + 1) + QStringLiteral(".rels"));

    const KZipFileEntry *relFile = static_cast<const KZipFileEntry *>(file->xpsArchive()->directory()->entry(documentRelationshipFile));

    QString documentStructureFile;
    if (relFile) {
        QXmlStreamReader xml;
        xml.addData(readFileOrDirectoryParts(relFile));
        while (!xml.atEnd()) {
            xml.readNext();
            if (xml.isStartElement() && (xml.name() == QStringLiteral("Relationship"))) {
                QXmlStreamAttributes attributes = xml.attributes();
                if (attributes.value(QStringLiteral("Type")).toString() == QLatin1String("http://schemas.microsoft.com/xps/2005/06/documentstructure")) {
                    documentStructureFile = attributes.value(QStringLiteral("Target")).toString();
                } else {
                    qCWarning(OkularXpsDebug) << "Unknown document relationships element: " << attributes.value(QStringLiteral("Type")).toString() << " : " << attributes.value(QStringLiteral("Target")).toString();
                }
            }
        }
        if (xml.error()) {
            qCWarning(OkularXpsDebug) << "Could not parse XPS page relationships file ( " << documentRelationshipFile << " ) - " << xml.errorString();
        }
    } else { // the page relationship file didn't exist in the zipfile
        // this isn't fatal
        qCWarning(OkularXpsDebug) << "Could not open Document relationship file from " << documentRelationshipFile;
    }

    if (!documentStructureFile.isEmpty()) {
        // qCWarning(OkularXpsDebug) << "Document structure filename: " << documentStructureFile;
        // make the document path absolute
        documentStructureFile = absolutePath(documentEntryPath, documentStructureFile);
        // qCWarning(OkularXpsDebug) << "Document structure absolute path: " << documentStructureFile;
        parseDocumentStructure(documentStructureFile);
    }
}

XpsDocument::~XpsDocument()
{
    qDeleteAll(m_pages);
    m_pages.clear();

    if (m_docStructure) {
        delete m_docStructure;
    }
}

int XpsDocument::numPages() const
{
    return m_pages.size();
}

XpsPage *XpsDocument::page(int pageNum) const
{
    return m_pages.at(pageNum);
}

XpsFile::XpsFile()
{
}

XpsFile::~XpsFile()
{
    for (int fontId : qAsConst(m_fontCache)) {
        m_fontDatabase.removeApplicationFont(fontId);
    }
}

bool XpsFile::loadDocument(const QString &filename)
{
    m_xpsArchive = new KZip(filename);
    if (m_xpsArchive->open(QIODevice::ReadOnly) == true) {
        qCWarning(OkularXpsDebug) << "Successful open of " << m_xpsArchive->fileName();
    } else {
        qCWarning(OkularXpsDebug) << "Could not open XPS archive: " << m_xpsArchive->fileName();
        delete m_xpsArchive;
        return false;
    }

    // The only fixed entry in XPS is /_rels/.rels
    const KArchiveEntry *relEntry = m_xpsArchive->directory()->entry(QStringLiteral("_rels/.rels"));
    if (!relEntry) {
        // this might occur if we can't read the zip directory, or it doesn't have the relationships entry
        return false;
    }

    QXmlStreamReader relXml;
    relXml.addData(readFileOrDirectoryParts(relEntry));

    QString fixedRepresentationFileName;
    // We work through the relationships document and pull out each element.
    while (!relXml.atEnd()) {
        relXml.readNext();
        if (relXml.isStartElement()) {
            if (relXml.name() == QStringLiteral("Relationship")) {
                QXmlStreamAttributes attributes = relXml.attributes();
                QString type = attributes.value(QStringLiteral("Type")).toString();
                QString target = attributes.value(QStringLiteral("Target")).toString();
                if (QStringLiteral("http://schemas.openxmlformats.org/package/2006/relationships/metadata/thumbnail") == type) {
                    m_thumbnailFileName = target;
                } else if (QStringLiteral("http://schemas.microsoft.com/xps/2005/06/fixedrepresentation") == type) {
                    fixedRepresentationFileName = target;
                } else if (QStringLiteral("http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties") == type) {
                    m_corePropertiesFileName = target;
                } else if (QStringLiteral("http://schemas.openxmlformats.org/package/2006/relationships/digital-signature/origin") == type) {
                    m_signatureOrigin = target;
                } else {
                    qCWarning(OkularXpsDebug) << "Unknown relationships element: " << type << " : " << target;
                }
            } else if (relXml.name() == QStringLiteral("Relationships")) {
                // nothing to do here - this is just the container level
            } else {
                qCWarning(OkularXpsDebug) << "unexpected element in _rels/.rels: " << relXml.name().toString();
            }
        }
    }
    if (relXml.error()) {
        qCWarning(OkularXpsDebug) << "Could not parse _rels/.rels: " << relXml.errorString();
        return false;
    }

    if (fixedRepresentationFileName.isEmpty()) {
        // FixedRepresentation is a required part of the XPS document
        return false;
    }

    const KArchiveEntry *fixedRepEntry = m_xpsArchive->directory()->entry(fixedRepresentationFileName);
    QString fixedRepresentationFilePath = fixedRepresentationFileName;

    QXmlStreamReader fixedRepXml;
    fixedRepXml.addData(readFileOrDirectoryParts(fixedRepEntry, &fixedRepresentationFileName));

    while (!fixedRepXml.atEnd()) {
        fixedRepXml.readNext();
        if (fixedRepXml.isStartElement()) {
            if (fixedRepXml.name() == QStringLiteral("DocumentReference")) {
                const QString source = fixedRepXml.attributes().value(QStringLiteral("Source")).toString();
                XpsDocument *doc = new XpsDocument(this, absolutePath(fixedRepresentationFilePath, source));
                for (int lv = 0; lv < doc->numPages(); ++lv) {
                    // our own copy of the pages list
                    m_pages.append(doc->page(lv));
                }
                m_documents.append(doc);
            } else if (fixedRepXml.name() == QStringLiteral("FixedDocumentSequence")) {
                // we don't do anything here - this is just a container for one or more DocumentReference elements
            } else {
                qCWarning(OkularXpsDebug) << "Unhandled entry in FixedDocumentSequence: " << fixedRepXml.name().toString();
            }
        }
    }
    if (fixedRepXml.error()) {
        qCWarning(OkularXpsDebug) << "Could not parse FixedRepresentation file:" << fixedRepXml.errorString();
        return false;
    }

    return true;
}

Okular::DocumentInfo XpsFile::generateDocumentInfo() const
{
    Okular::DocumentInfo docInfo;

    docInfo.set(Okular::DocumentInfo::MimeType, QStringLiteral("application/oxps"));

    if (!m_corePropertiesFileName.isEmpty()) {
        const KZipFileEntry *corepropsFile = static_cast<const KZipFileEntry *>(m_xpsArchive->directory()->entry(m_corePropertiesFileName));

        QXmlStreamReader xml;
        xml.addData(corepropsFile->data());
        while (!xml.atEnd()) {
            xml.readNext();
            if (xml.isEndElement()) {
                break;
            }
            if (xml.isStartElement()) {
                if (xml.name() == QStringLiteral("title")) {
                    docInfo.set(Okular::DocumentInfo::Title, xml.readElementText());
                } else if (xml.name() == QStringLiteral("subject")) {
                    docInfo.set(Okular::DocumentInfo::Subject, xml.readElementText());
                } else if (xml.name() == QStringLiteral("description")) {
                    docInfo.set(Okular::DocumentInfo::Description, xml.readElementText());
                } else if (xml.name() == QStringLiteral("creator")) {
                    docInfo.set(Okular::DocumentInfo::Creator, xml.readElementText());
                } else if (xml.name() == QStringLiteral("category")) {
                    docInfo.set(Okular::DocumentInfo::Category, xml.readElementText());
                } else if (xml.name() == QStringLiteral("created")) {
                    QDateTime createdDate = QDateTime::fromString(xml.readElementText(), QStringLiteral("yyyy-MM-ddThh:mm:ssZ"));
                    docInfo.set(Okular::DocumentInfo::CreationDate, QLocale().toString(createdDate, QLocale::LongFormat));
                } else if (xml.name() == QStringLiteral("modified")) {
                    QDateTime modifiedDate = QDateTime::fromString(xml.readElementText(), QStringLiteral("yyyy-MM-ddThh:mm:ssZ"));
                    docInfo.set(Okular::DocumentInfo::ModificationDate, QLocale().toString(modifiedDate, QLocale::LongFormat));
                } else if (xml.name() == QStringLiteral("keywords")) {
                    docInfo.set(Okular::DocumentInfo::Keywords, xml.readElementText());
                } else if (xml.name() == QStringLiteral("revision")) {
                    docInfo.set(QStringLiteral("revision"), xml.readElementText(), i18n("Revision"));
                }
            }
        }
        if (xml.error()) {
            qCWarning(OkularXpsDebug) << "Could not parse XPS core properties:" << xml.errorString();
        }
    } else {
        qCWarning(OkularXpsDebug) << "No core properties filename";
    }

    docInfo.set(Okular::DocumentInfo::Pages, QString::number(numPages()));

    return docInfo;
}

bool XpsFile::closeDocument()
{
    qDeleteAll(m_documents);
    m_documents.clear();

    delete m_xpsArchive;

    return true;
}

int XpsFile::numPages() const
{
    return m_pages.size();
}

int XpsFile::numDocuments() const
{
    return m_documents.size();
}

XpsDocument *XpsFile::document(int documentNum) const
{
    return m_documents.at(documentNum);
}

XpsPage *XpsFile::page(int pageNum) const
{
    return m_pages.at(pageNum);
}

XpsGenerator::XpsGenerator(QObject *parent, const QVariantList &args)
    : Okular::Generator(parent, args)
    , m_xpsFile(nullptr)
{
    setFeature(TextExtraction);
    setFeature(PrintNative);
    setFeature(PrintToFile);
    setFeature(Threaded);
    userMutex();
}

XpsGenerator::~XpsGenerator()
{
}

bool XpsGenerator::loadDocument(const QString &fileName, QVector<Okular::Page *> &pagesVector)
{
    m_xpsFile = new XpsFile();

    m_xpsFile->loadDocument(fileName);
    pagesVector.resize(m_xpsFile->numPages());

    int pagesVectorOffset = 0;

    for (int docNum = 0; docNum < m_xpsFile->numDocuments(); ++docNum) {
        XpsDocument *doc = m_xpsFile->document(docNum);
        for (int pageNum = 0; pageNum < doc->numPages(); ++pageNum) {
            QSizeF pageSize = doc->page(pageNum)->size();
            pagesVector[pagesVectorOffset] = new Okular::Page(pagesVectorOffset, pageSize.width(), pageSize.height(), Okular::Rotation0);
            ++pagesVectorOffset;
        }
    }

    return true;
}

bool XpsGenerator::doCloseDocument()
{
    m_xpsFile->closeDocument();
    delete m_xpsFile;
    m_xpsFile = nullptr;

    return true;
}

QImage XpsGenerator::image(Okular::PixmapRequest *request)
{
    QMutexLocker lock(userMutex());
    QSize size((int)request->width(), (int)request->height());
    QImage image(size, QImage::Format_RGB32);
    XpsPage *pageToRender = m_xpsFile->page(request->page()->number());
    pageToRender->renderToImage(&image);
    return image;
}

Okular::TextPage *XpsGenerator::textPage(Okular::TextRequest *request)
{
    QMutexLocker lock(userMutex());
    XpsPage *xpsPage = m_xpsFile->page(request->page()->number());
    return xpsPage->textPage();
}

Okular::DocumentInfo XpsGenerator::generateDocumentInfo(const QSet<Okular::DocumentInfo::Key> &keys) const
{
    Q_UNUSED(keys);

    qCWarning(OkularXpsDebug) << "generating document metadata";

    return m_xpsFile->generateDocumentInfo();
}

const Okular::DocumentSynopsis *XpsGenerator::generateDocumentSynopsis()
{
    qCWarning(OkularXpsDebug) << "generating document synopsis";

    // we only generate the synopsis for the first file.
    if (!m_xpsFile || !m_xpsFile->document(0)) {
        return nullptr;
    }

    if (m_xpsFile->document(0)->hasDocumentStructure()) {
        return m_xpsFile->document(0)->documentStructure();
    }

    return nullptr;
}

Okular::ExportFormat::List XpsGenerator::exportFormats() const
{
    static Okular::ExportFormat::List formats;
    if (formats.isEmpty()) {
        formats.append(Okular::ExportFormat::standardFormat(Okular::ExportFormat::PlainText));
    }
    return formats;
}

bool XpsGenerator::exportTo(const QString &fileName, const Okular::ExportFormat &format)
{
    if (format.mimeType().inherits(QStringLiteral("text/plain"))) {
        QFile f(fileName);
        if (!f.open(QIODevice::WriteOnly)) {
            return false;
        }

        QTextStream ts(&f);
        for (int i = 0; i < m_xpsFile->numPages(); ++i) {
            Okular::TextPage *textPage = m_xpsFile->page(i)->textPage();
            QString text = textPage->text();
            ts << text;
            ts << QLatin1Char('\n');
            delete textPage;
        }
        f.close();

        return true;
    }

    return false;
}

Okular::Document::PrintError XpsGenerator::print(QPrinter &printer)
{
    QList<int> pageList = Okular::FilePrinter::pageList(printer, document()->pages(), document()->currentPage() + 1, document()->bookmarkedPageList());

    QPainter painter(&printer);

    for (int i = 0; i < pageList.count(); ++i) {
        if (i != 0) {
            printer.newPage();
        }

        const int page = pageList.at(i) - 1;
        XpsPage *pageToRender = m_xpsFile->page(page);
        pageToRender->renderToPainter(&painter);
    }

    return Okular::Document::NoPrintError;
}

const XpsRenderNode *XpsRenderNode::findChild(const QString &name) const
{
    for (const XpsRenderNode &child : children) {
        if (child.name == name) {
            return &child;
        }
    }

    return nullptr;
}

QVariant XpsRenderNode::getRequiredChildData(const QString &name) const
{
    const XpsRenderNode *child = findChild(name);
    if (child == nullptr) {
        qCWarning(OkularXpsDebug) << "Required element " << name << " is missing in " << this->name;
        return QVariant();
    }

    return child->data;
}

QVariant XpsRenderNode::getChildData(const QString &name) const
{
    const XpsRenderNode *child = findChild(name);
    if (child == nullptr) {
        return QVariant();
    } else {
        return child->data;
    }
}

Q_LOGGING_CATEGORY(OkularXpsDebug, "org.kde.okular.generators.xps", QtWarningMsg)

#include "generator_xps.moc"
