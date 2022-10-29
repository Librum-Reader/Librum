// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0;
// -*-
//
// Class: length
//
// Part of KVIEWSHELL
//
// SPDX-FileCopyrightText: 2005 Stefan Kebekus
// SPDX-FileCopyrightText: 2006 Wilfried Huss
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef _length_h_
#define _length_h_

#include <math.h>
#include <cmath>

class QString;

#define mm_per_cm 10.0
#define mm_per_m 1000.0
#define mm_per_inch 25.4
#define mm_per_TeXPoint (2540.0 / 7227.0)
#define mm_per_bigPoint (25.4 / 72.0)
#define mm_per_pica (25.4 / 6.0)
#define mm_per_didot (25.4 * 0.0148)
#define mm_per_cicero (25.4 * 0.178)
#define mm_per_scaledPoint (25.4 / (72.27 * 65536.0))

/** @short Represents a phyical length

    This class is used to represent a physical length. Its main purpose
    it to help in the conversion of units, and to avoid confusion
    about units. To avoid misunderstandings, there is no default
    constructor so that this class needs to be explicitly initialized
    with one of the functions below.

    @warning Lengths are stored internally in mm. If you convert to
    or from any other unit, expect floating point round-off errors.

    @author Stefan Kebekus <kebekus@kde.org>
    @version 1.0.0
*/

class Length
{
public:
    /** constructs a 'length = 0mm' object */
    Length()
    {
        length_in_mm = 0;
    }

    /** sets the length in millimeters */
    void setLength_in_mm(double l)
    {
        length_in_mm = l;
    }

    /** sets the length in centimeters */
    void setLength_in_cm(double l)
    {
        length_in_mm = l * mm_per_cm;
    }

    /** sets the length in meters */
    void setLength_in_m(double l)
    {
        length_in_mm = l * mm_per_m;
    }

    /** sets the length in inches */
    void setLength_in_inch(double l)
    {
        length_in_mm = l * mm_per_inch;
    }

    /** sets the length in TeX points */
    void setLength_in_TeXPoints(double l)
    {
        length_in_mm = l * mm_per_TeXPoint;
    }

    /** sets the length in big points (1/72 of an inch) */
    void setLength_in_bigPoints(double l)
    {
        length_in_mm = l * mm_per_bigPoint;
    }

    /** sets the length in picas (1/6 of an inch) */
    void setLength_in_pica(double l)
    {
        length_in_mm = l * mm_per_pica;
    }

    /** sets the length in didots (0.0148 inches) */
    void setLength_in_didot(double l)
    {
        length_in_mm = l * mm_per_didot;
    }

    /** sets the length in ciceros (0.178 inches) */
    void setLength_in_cicero(double l)
    {
        length_in_mm = l * mm_per_cicero;
    }

    /** sets the length in scaled points (1 scaled point = 65536 TeX points) */
    void setLength_in_scaledPoints(double l)
    {
        length_in_mm = l * mm_per_scaledPoint;
    }

    /** sets the length (@param l ) in pixels. The parameter @param res is the
       resolution of the used device in DPI. */
    void setLength_in_pixel(int l, double res)
    {
        setLength_in_inch(l / res);
    }

    /** @returns the length in millimeters */
    double getLength_in_mm() const
    {
        return length_in_mm;
    }

    /** @returns the length in centimeters */
    double getLength_in_cm() const
    {
        return length_in_mm / mm_per_cm;
    }

    /** @returns the length in meters */
    double getLength_in_m() const
    {
        return length_in_mm / mm_per_m;
    }

    /** @returns the length in inches */
    double getLength_in_inch() const
    {
        return length_in_mm / mm_per_inch;
    }

    /** @returns the length in TeX points */
    double getLength_in_TeXPoints() const
    {
        return length_in_mm / mm_per_TeXPoint;
    }

    /** @returns the length in big points (1/72 of an inch) */
    double getLength_in_bigPoints() const
    {
        return length_in_mm / mm_per_bigPoint;
    }

    /** @returns the length in picas (1/6 of an inch) */
    double getLength_in_pica() const
    {
        return length_in_mm / mm_per_pica;
    }

    /** @returns the length in didots (0.0148 inches) */
    double getLength_in_didot() const
    {
        return length_in_mm / mm_per_didot;
    }

    /** @returns the length in ciceros (0.178 inches) */
    double getLength_in_cicero() const
    {
        return length_in_mm / mm_per_cicero;
    }

    /** @returns the length in scaled points (1 scaled point = 65536 TeX points)
     */
    double getLength_in_scaledPoints() const
    {
        return length_in_mm / mm_per_scaledPoint;
    }

    /** @returns the length in pixel. The parameter @param res is the resolution
       of the used device in DPI. */
    int getLength_in_pixel(double res) const
    {
        return int(getLength_in_inch() * res);
    }

    /** @returns true is lengths differ by no more than 2mm */
    bool isNearlyEqual(const Length o) const
    {
        return fabs(length_in_mm - o.getLength_in_mm()) <= 2.0;
    }

    /** Comparison of two lengthes */
    bool operator>(const Length o) const
    {
        return (length_in_mm > o.getLength_in_mm());
    }

    bool operator<(const Length o) const
    {
        return (length_in_mm < o.getLength_in_mm());
    }

    /** Comparison of two lengthes */
    bool operator>=(const Length o) const
    {
        return (length_in_mm >= o.getLength_in_mm());
    }

    bool operator<=(const Length o) const
    {
        return (length_in_mm <= o.getLength_in_mm());
    }

    /** Ratio of two lengthes

    @warning There is no safeguared to prevent you from division by
    zero. If the length in the denominator is near 0.0, a floating point
    exception may occur.

    @returns the ratio of the two lengthes as a double
    */
    double operator/(const Length o) const
    {
        return (length_in_mm / o.getLength_in_mm());
    }

    /** Sum of two lengthes

    @returns the sum of the lengthes as a Length
    */
    Length operator+(const Length o) const
    {
        Length r;
        r.length_in_mm = length_in_mm + o.length_in_mm;
        return r;
    }

    /** Difference of two lengthes

    @returns the difference of the lengthes as a Length
    */
    Length operator-(const Length o) const
    {
        Length r;
        r.length_in_mm = length_in_mm - o.length_in_mm;
        return r;
    }

    /** Division of a length

    @warning There is no safeguared to prevent you from division by
    zero. If the number in the denominator is near 0.0, a floating point
    exception may occur.

    @returns a fraction of the original length as a Length
    */
    Length operator/(const double l) const
    {
        Length r;
        r.length_in_mm = length_in_mm / l;
        return r;
    }

    /** Multiplication of a length

    @returns a multiplied length as a Length
    */
    Length operator*(const double l) const
    {
        Length r;
        r.length_in_mm = length_in_mm * l;
        return r;
    }

    /** This method converts a string that gives a distance in one of the
    commonly used units, such as "12.3mm", "12 inch" or "15 didot" to
    millimeters. For a complete list of supported units, see the
    static lists that are hardcoded in "units.cpp".

    If the conversion is not possible *ok is set to "false" and an
    undefined value is returned. If the unit could not be recognized,
    an error message is printed via kdError(). Otherwise, *ok is set
    to true.

    It is possible in rare circumstances that ok is set to true
    although the string is malformed.

    It is fine to set ok to 0. */
    static float convertToMM(const QString& distance, bool* ok = nullptr);

private:
    /** Length in millimeters */
    double length_in_mm;
};

#undef mm_per_cm
#undef mm_per_m
#undef mm_per_inch
#undef mm_per_TeXPoint
#undef mm_per_bigPoint
#undef mm_per_pica
#undef mm_per_didot
#undef mm_per_cicero
#undef mm_per_scaledPoint

#endif
