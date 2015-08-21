/*
 * This file is part of GDSII.
 *
 * aref.cpp -- The source file which defines the AREF of GDSII format.
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

#include <limits>
#include <assert.h>
#include "aref.h"
#include <sstream>
#include "gdsio.h"
#include "structures.h"

namespace GDS
{

ARef::ARef() :Element(AREF)
{
	Eflags = 0;
	SName = "";
	Strans = 0;
	Row = 0;
	Col = 0;
	Angle = 0;
	Mag = 1;
}

ARef::~ARef()
{
}

std::string ARef::structName() const
{
	return SName;
}

short ARef::row() const
{
	return Row;
}

short ARef::col() const
{
	return Col;
}

std::vector<Point> ARef::xy() const
{
	return Pts;
}

double ARef::angle() const
{
	return Angle;
}

double ARef::mag() const
{
	return Mag;
}

short ARef::strans() const
{
	return Strans;
}

bool ARef::stransFlag(STRANS_FLAG flag) const
{
	return Strans & flag;
}

void ARef::setStructName(std::string name)
{
	SName = name;
}

void ARef::setRowCol(int row, int col)
{
	Row = row;
	Col = col;
}

void ARef::setXY(std::vector<Point> pts)
{
	Pts = pts;
}

void ARef::setAngle(double angle)
{
	Angle = angle;
}

void ARef::setMag(double mag)
{
	Mag = mag;
}

void ARef::setStrans(short strans)
{
	Strans = strans;
}

void ARef::setStrans(STRANS_FLAG flag, bool enable)
{
	Strans = enable ? (Strans | flag) : (Strans & (~flag));
}

int ARef::read(std::ifstream &in, std::string &msg)
{
	bool finished = false;
	while (!finished)
	{
		short record_size;
		Byte record_type, record_dt;
		if (!readShort(in, record_size)
			|| !readByte(in, record_type)
			|| !readByte(in, record_dt))
			return FILE_ERROR;
		switch (record_type)
		{
		case ENDEL:
			finished = true;
			break;
		case EFLAGS:
			if (record_size != 6)
			{
				std::stringstream ss;
				ss << "Wrong record size of EFLAGS (";
				ss << std::hex << record_size << record_type << record_dt;
				ss << ").";
				msg = ss.str();
				return FORMAT_ERROR;
			}
			if (!readShort(in, Eflags))
				return FILE_ERROR;
			break;
		case SNAME:
			if (record_size < 4 || record_size % 2 != 0)
			{
				std::stringstream ss;
				ss << "Wrong record size of SNAME (";
				ss << std::hex << record_size << record_type << record_dt;
				ss << ").";
				msg = ss.str();
				return FORMAT_ERROR;
			}
			if (!readString(in, record_size - 4, SName))
				return FILE_ERROR;
			break;
		case XY:
			if (record_size != 28)
			{
				std::stringstream ss;
				ss << "Wrong record size of XY for AREF (";
				ss << std::hex << record_size << record_type << record_dt;
				ss << "). ";
				msg = ss.str();
				return FORMAT_ERROR;
			}
			for (int i = 0; i < 3; i++)
			{
				int x, y;
				if (!readInteger(in, x)
					|| !readInteger(in, y))
					return FILE_ERROR;
				Pts.push_back(Point(x, y));
			}
			break;
		case STRANS:
			if (record_size != 6)
			{
				std::stringstream ss;
				ss << "Wrong record size of STRANS (";
				ss << std::hex << record_size << record_type << record_dt;
				ss << ").";
				msg = ss.str();
				return FORMAT_ERROR;
			}
			if (!readShort(in, Strans))
				return FILE_ERROR;
			break;
		case COLROW:
			if (record_size != 8)
			{
				std::stringstream ss;
				ss << "Wrong record size of COLROW (";
				ss << std::hex << record_size << record_type << record_dt;
				ss << ").";
				msg = ss.str();
				return FORMAT_ERROR;
			}
			if (!readShort(in, Col)
				|| !readShort(in, Row))
				return FILE_ERROR;
			break;
		case MAG:
			if (record_size != 12)
			{
				std::stringstream ss;
				ss << "Wrong record size of MAG (";
				ss << std::hex << record_size << record_type << record_dt;
				ss << ").";
				msg = ss.str();
				return FORMAT_ERROR;
			}
			if (!readDouble(in, Mag))
				return FILE_ERROR;
			break;
		case ANGLE:
			if (record_size != 12)
			{
				std::stringstream ss;
				ss << "Wrong record size of ANGLE (";
				ss << std::hex << record_size << record_type << record_dt;
				ss << ").";
				msg = ss.str();
				return FORMAT_ERROR;
			}
			if (!readDouble(in, Angle))
				return FILE_ERROR;
			break;
		default:
			break;
		}
	}
	return 0;
}

int ARef::write(std::ofstream &out, std::string &msg)
{
	short record_size;

	// AREF
	record_size = 4;
	writeShort(out, record_size);
	writeByte(out, AREF);
	writeByte(out, NoData);

	record_size = 6;
	writeShort(out, record_size);
	writeByte(out, EFLAGS);
	writeByte(out, Integer_2);
	writeShort(out, Eflags);

	record_size = 4 + SName.size();
	if (record_size % 2 != 0)
		record_size += 1;
	writeShort(out, record_size);
	writeByte(out, SNAME);
	writeByte(out, String);
	writeString(out, SName);

	record_size = 6;
	writeShort(out, record_size);
	writeByte(out, STRANS);
	writeByte(out, Integer_2);
	writeShort(out, Strans);

	record_size = 8;
	writeShort(out, record_size);
	writeByte(out, COLROW);
	writeByte(out, Integer_2);
	writeShort(out, Col);
	writeShort(out, Row);

	record_size = 28;
	writeShort(out, record_size);
	writeByte(out, XY);
	writeByte(out, Integer_4);
	for (int i = 0; i < 3; i++)
	{
		writeInteger(out, Pts[i].x);
		writeInteger(out, Pts[i].y);
	}

	record_size = 12;
	writeShort(out, record_size);
	writeByte(out, ANGLE);
	writeByte(out, Real_8);
	writeDouble(out, Angle);

	record_size = 12;
	writeShort(out, record_size);
	writeByte(out, MAG);
	writeByte(out, Real_8);
	writeDouble(out, Mag);

	record_size = 4;
	writeShort(out, record_size);
	writeByte(out, ENDEL);
	writeByte(out, NoData);

	return out.fail() ? FILE_ERROR : 0;
}

}


