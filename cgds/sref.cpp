/*
 * This file is part of GDSII.
 *
 * sref.cpp -- The source file which defines the SREF of GDSII format.
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

#include <assert.h>
#include "sref.h"
#include <sstream>
#include "gdsio.h"
#include "structures.h"

namespace GDS
{

SRef::SRef() :Element(SREF)
{
	Eflags = 0;
	SName = "";
	Strans = 0;
	Angle = 0;
	Mag = 1;
	//ReferTo = std::shared_ptr<Structure>();
}

SRef::~SRef()
{
}

std::string SRef::structName() const
{
	return SName;
}

Point SRef::xy() const
{
	return Pt;
}

double SRef::angle() const
{
	return Angle;
}

double SRef::mag() const
{
	return Mag;
}

short SRef::strans() const
{
	return Strans;
}

bool SRef::stransFlag(STRANS_FLAG flag) const
{
	return Strans & flag;
}

void SRef::setStructName(std::string name)
{
	SName = name;
}

void SRef::setXY(Point pt)
{
	Pt = pt;
}

void SRef::setAngle(double angle)
{
	Angle = angle;
}

void SRef::setMag(double mag)
{
	Mag = mag;
}

void SRef::setStrans(short strans)
{
	Strans = strans;
}

void SRef::setStrans(STRANS_FLAG flag, bool enable)
{
	Strans = enable ? (Strans | flag) : (Strans & (~flag));
}

void SRef::setReference(std::shared_ptr<Structure> ref)
{
	ReferTo = ref;
}

int SRef::read(std::ifstream &in, std::string &msg)
{
	msg = "";
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
			if (record_size != 12)
			{
				std::stringstream ss;
				ss << "Wrong record size of XY for SREF (";
				ss << std::hex << record_size << record_type << record_dt;
				ss << "). ";
				msg = ss.str();
				return FORMAT_ERROR;
			}
			int x, y;
			if (!readInteger(in, x)
				|| !readInteger(in, y))
				return FILE_ERROR;
			Pt = Point(x, y);
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

int SRef::write(std::ofstream &out, std::string &msg)
{
	short record_size;

	record_size = 4;
	writeShort(out, record_size);
	writeByte(out, SREF);
	writeByte(out, NoData);

	record_size = 6;
	writeShort(out, record_size);
	writeByte(out, EFLAGS);
	writeByte(out, Integer_2);
	writeShort(out, Eflags);

	record_size = 6;
	writeShort(out, record_size);
	writeByte(out, STRANS);
	writeByte(out, Integer_2);
	writeShort(out, Strans);

	record_size = 4 + SName.size();
	if (record_size % 2 != 0)
		record_size += 1;
	writeShort(out, record_size);
	writeByte(out, SNAME);
	writeByte(out, String);
	writeString(out, SName);

	record_size = 12;
	writeShort(out, record_size);
	writeByte(out, XY);
	writeByte(out, Integer_4);
	writeInteger(out, Pt.x);
	writeInteger(out, Pt.y);

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

