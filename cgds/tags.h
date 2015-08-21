/*
 * This file is part of GDSII.
 *
 * tags.h -- The header file which defines some constants variables.
 *
 * Copyright (c) 2015 Kangpeng Shao <billowen035@gmail.com>
 *
 * GDSII is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at you option) any later version.
 *
 * GDSII is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABLILTY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GDSII. If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef TAGS_H
#define TAGS_H
#include <map>
#include <string>

namespace GDS {

typedef unsigned char Byte;

enum IO_ERROR_CODE
{
	FILE_ERROR = 1,
	FORMAT_ERROR
};

struct Point
{
	int x, y;
	Point(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	Point() 
	{
		x = 0;
		y = 0;
	}
};

enum STRANS_FLAG
{
	REFLECTION = 0x8000,
	ABS_MAG    = 0x0004,
	ABS_ANGLE  = 0x0002,
};

enum Record_type : Byte
{
    HEADER       = 0x00,
    BGNLIB       = 0x01,
    LIBNAME      = 0x02,
    UNITS        = 0x03,
    ENDLIB       = 0x04,
    BGNSTR       = 0x05,
    STRNAME      = 0x06,
    ENDSTR       = 0x07,
    BOUNDARY     = 0x08,
    PATH         = 0x09,
    SREF         = 0x0a,
    AREF         = 0x0b,
    TEXT         = 0x0c,
    LAYER        = 0x0d,
    DATATYPE     = 0x0e,
    WIDTH        = 0x0f,
    XY           = 0x10,
    ENDEL        = 0x11,
    SNAME        = 0x12,
    COLROW       = 0x13,
	NODE		 = 0x15,

    TEXTTYPE     = 0x16,
    PRESENTATION = 0x17,

    STRING       = 0x19,
    STRANS       = 0x1a,
    MAG          = 0x1b,
    ANGLE        = 0x1c,

    PATHTYPE     = 0x21,

    EFLAGS       = 0x26,
	NODETYPE	 = 0x2a,

    PLEX         = 0x2f,
    BGNEXTN      = 0x30,
    ENDEXTN      = 0x31,

    RECORD_UNKNOWN     = 0x79,
};

static std::map<int, std::string> Record_name = {
    { 0x00, "HEADER" },
    { 0x01, "BGNLIB" },
    { 0x02, "LIBNAME" },
    { 0x03, "UNITS" },
    { 0x04, "ENDLIB" },
    { 0x05, "BGNSTR" },
    { 0x06, "STRNAME" },
    { 0x07, "ENDSTR" },
    { 0x08, "BOUNDARY" },
    { 0x09, "PATH" },
    { 0x0a, "SREF" },
    { 0x0b, "AREF" },
    { 0x0c, "TEXT" },
    { 0x0d, "LAYER" },
    { 0x0e, "DATATYPE" },
    { 0x0f, "WIDTH" },
    { 0x10, "XY" },
    { 0x11, "ENDEL" },
    { 0x12, "SNAME" },
    { 0x13, "COLROW" },
	{ 0x15, "NODE" },

    { 0x16, "TEXTTYPE" },
    { 0x17, "PRESENTATION"},

    { 0x19, "STRING" },
    { 0x1a, "STRANS" },
    { 0x1b, "MAG" },
    { 0x1c, "ANGLE" },

    { 0x21, "PATHTYPE" },

    { 0x26, "EFLAGS" },
	{ 0x2a, "NODETYPE" },

    { 0x2f, "PLEX" },
    { 0x30, "BGNEXTN" },
    { 0x31, "ENDEXTN" },
};

enum Data_type : Byte
{
    NoData    = 0x00,
    BitArray  = 0x01,
    Integer_2 = 0x02,
    Integer_4 = 0x03,
    Real_4    = 0x04,
    Real_8    = 0x05,
    String    = 0x06,
    DATA_UNKNOWN =0x79,
};

}
#endif // TAGS_H

