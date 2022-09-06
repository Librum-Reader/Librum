// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0; -*-
//
// pageSize.h
//
// Part of KVIEWSHELL - A framework for multipage text/gfx viewers
//
// SPDX-FileCopyrightText: 2002-2005 Stefan Kebekus
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef PAGESIZE_H
#define PAGESIZE_H

#include "simplePageSize.h"

#include <QObject>

class QString;
class QStringList;

/* \brief This class represents physical page sizes.

The main difference to the SimplePageSize class are the following.

- This class knows about standard page sizes and accepts page sizes in
  various formats, e.g. as a string "DIN A4", or by specifying the
  page width and height. Several units (inch, millimeters,
  centimeters) are possible.

- It is made sure that page width an height are always in a reasonable
  range, which is currently set to 5cm .. 50cm

- The default constructor provides a locale-depending default.

@author Stefan Kebekus <kebekus@kde.org>
@version 1.0.0
*/

class pageSize : public QObject, public SimplePageSize
{
    Q_OBJECT

public:
    /** \brief Default constructor, initializes the pageSize with a
        reasonable default

        The default chosen depends on the locale. At the moment, A4 size
        is chosen for countries with metric measurement system, and US
        letter otherwise.
    */
    pageSize();

    /** \brief Initializes the pageSize with a SimplePageSize. */
    explicit pageSize(const SimplePageSize &);

    pageSize(const pageSize &) = delete;
    pageSize &operator=(const pageSize &) = delete;

    /** \brief List of standard pageSizes

    This method returns the names of standard pageSizes,
    e.g. "A4". These can be used, e.g., by a QComboBox to let the user
    choose known sizes. The returned list is also a list of all possible
    return values of the formatName() method explained below. If you
    call pageSizeNames() more than once, it is guaranteed that the
    same list of strings will be returned.

    @returns QStringList that contains
    */
    QStringList pageSizeNames();

    /** \brief Set page size by name.

    Acceptable strings are

    (1) a name from the list returned by pageSizeNames(), such as "DIN
        A4"

    (2) a string like "500x300", which describes a page of width 500mm
        and height 300mm.

    (3) a string like "3in, 4 cm". A number of different units,
        including "in", "mm" and "cm", and a few typographical units are
        recognized

    If the name is not of these types, and error message is printed to
    stderr using kError() and a default value, which depends on the
    locale, is set.

    In any case, the values will be trimmed so as not to exceed the
    minima/maxima of 5cm and 50cm, respectively. If the page size found
    matches one of the standard sizes by an error of no more than 2mm,
    the standard page size will be set. The signal sizeChanged() will
    always be emitted.

    @param name string that represents the page size

    @returns 'True', if the parameter could be parsed, and 'false'
    otherwise.
    */
    bool setPageSize(const QString &name);

    /** \brief Set page size from width and height strings

    Sets the page size to "width" and "height", given in the associated
    units. Currently, "mm", "cm" and "in" are supported. If a unit is
    not recognized, "mm" is silently assumed, and error message is
    printed to stderr using kError(). If the page size set matches one
    of the standard sizes by an error of no more than 2mm, the standard
    page size will be set.  If width or height does not contain a
    number, the result is an undefined value. However, it is guaranteed
    in any case that both width and height are between the minimal and
    maximal possible values, i.e. in the range 5..50 cm. If the newly
    set value differs from the old value by more that 2mm for width or
    height, the signal sizeChanged() will be emitted

    @param width string that represents the page width as a number,
    e.g., " 300 "

    @param widthUnits units for the width string. Currently "mm", "cm"
    and "in" are allowed.

    @param height string that represents the page height as a number,
    e.g., " 300 "

    @param heightUnits units for the height string. Currently "mm", "cm"
    and "in" are allowed.
    */
    void setPageSize(const QString &width, const QString &widthUnits, const QString &height, const QString &heightUnits);

    /** \brief Set page size

    Sets the page size to "width" and "height", given in millimeter.  If
    the page size set matches one of the standard sizes by an error of
    no more than 2mm, the standard page size will be set.  Values are
    trimmed so that both width and height are between the minimal and
    maximal possible values, i.e. in the range 5..50 cm. If the newly
    set value differs from the old value by more that 2mm for width or
    height, the signal sizeChanged() will be emitted

    @param width_in_mm page width in mm

    @param height_in_mm page height in mm
    */
    void setPageSize(double width_in_mm, double height_in_mm);
    using SimplePageSize::setPageSize;

    /** \brief Preferred unit for the current page size

    @returns The name of the unit, one of "cm", "mm" or "in", which is
        most commonly used with the current page format. For instance,
        US Letter and US Legal are best given in inches, to avoid very
        odd numbers. If the page format is unknown, returns a guess
        based on the current locale. */
    QString preferredUnit() const;

    /** \brief Returns the page width as a string

    @param unit The unit in which the page width shall be returned. This
    must be one of "cm", "mm" or "in".

    @returns a string containing a number, e.g. "3.1415", which gives the page
    width in the given unit.  If the unit is not recognized, the string "--" is returned.
    */
    QString widthString(const QString &unit) const;

    /** \brief Returns the page height as a string

    @param unit The unit in height the page width shall be
    returned. This must be one of "cm", "mm" or "in".

    @returns a string containing a number which gives the page height in
    the given unit. If the unit is not recognized, the string "--" is
    returned.
    */
    QString heightString(const QString &unit) const;

    /** \brief Returns a name for the page size, if this is a standard
        size

        @warning This method does not take care of orientation, e.g. it
        will return "DIN A4" if the page size is either 210x297 or
        297x210.

        @returns A name for the current page size, if the format has a
        name, or QString() otherwise. If the result is not
        QString(), it is guaranteed to be one of the strings
        returned by the pageSizeNames() method.
    */
    QString formatName() const;

    /** \brief Returns an number for the page size, if this is a
        standard size

        @warning This method does not take care of orientation, e.g. it
        will return the same value if the page size is either 210x297 or
        297x210.

        @returns If the current format is one of the standard sizes, a
        non-negative integer is returned, which is an index to the
        QStringList returned by the pageSizeNames() method. If the
        current format is none of the standard sizes, -1 is returned.
    */
    int formatNumber() const
    {
        return currentSize;
    }

    /** \brief Gets orientation for standard sizes

    If the pageSize is one of the standard sizes, i.e. formatNumber() !=
    -1, this method can be used to get the orientation. If the pageSize
    is not a standard size, this method prints an error message stderr
    using kError().

    @returns 0 for 'portrait', or 1 for 'landscape'. If the size is none
    of the standard sizes, an undefined value is returned.
    */
    int getOrientation() const;

    /** \brief Returns a string that can be read by setPageSize(QString)

    @returns This method returns a string like "210x297". The numbers
    are page width and height in millimeters. The setPageSize(QString)
    method will understand this output.
    */
    QString serialize() const;

    /** \brief Returns a textual description of the page size. */
    QString description() const;

public Q_SLOTS:
    /** \brief Sets orientation

    If the pageSize is one of the standard sizes, i.e. formatNumber() !=
    -1, this method can be used to set the orientation. If the pageSize
    is not a standard size, this method prints an error message stderr
    using kError() and does nothing.

    @param orient 0 sets 'portrait orientation', 1 sets 'landscape'
   */
    void setOrientation(int orient);

Q_SIGNALS:
    /** \brief Signal emitted when the page sizes changes

    emitted to indicate that the size changed. Not emitted immediately
    after construction, when the constructor sets the default
    size.

    @param t a pointer to this
    */
    void sizeChanged(const SimplePageSize &t);

private:
    /** Makes sure that pageWidth and pageHeight are in the permissible
        range and not, e.g., negative. */
    void rectifySizes();

    /** Tries to find one of the known sizes which matches pageWidth and
        pageHeight, with an error margin of 2 millimeters. If found, the
        value of 'currentsize' is set to point to the known size, and
        pageWidth and pageHeight are set to the correct values for that
        size. Otherwise, currentSize is set to -1 to indicate "custom
        size". Note: this method does not take care of orientation,
        e.g. the method will set 'currentsize' to point to "DIN A4" if
        either the page size is 210x297 or 297x210. */
    void reconstructCurrentSize();

    /** Gives a default value for currentSize, which depends on the
        locale. In countries with metric system, this will be "DIN A4",
        in countries with the imperial system, "US Letter". */
    int defaultPageSize();

    /** Permissible range: 0--#Size_of_array staticList, or -1 to
        indicate a "user defined setting". Other values may lead to a
        segfault. */
    int currentSize;
};

#endif
