/*
 * This file is part of GDSII.
 *
 * path.cpp -- The source file which defines the PATH of GDSII format.
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
#include "path.h"
#include <sstream>
#include <algorithm>
#include "gdsio.h"

namespace GDS
{

Path::Path() :Element(PATH)
{
	Eflags = 0;
	Layer = -1;
	Data_type = -1;
	Width = 0;
	Path_type = 0;
}

Path::~Path()
{

}

short Path::layer() const
{
	return Layer;
}

short Path::dataType() const
{
	return Data_type;
}

int Path::width() const
{
	return Width;
}

short Path::pathType() const
{
	return Path_type;
}

std::vector<Point> Path::xy() const
{
	return Pts;
}

void Path::setLayer(short layer)
{
	Layer = layer;
}

void Path::setDataType(short data_type)
{
	Data_type = data_type;
}

void Path::setWidth(int width)
{
	Width = width;
}

void Path::setPathType(short type)
{
	Path_type = type;
}

void Path::setXY(const std::vector<Point> &pts)
{
	Pts.clear();
	Pts = pts;
}

int Path::read(std::ifstream &in, std::string &msg)
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
		case LAYER:
			if (record_size != 6)
			{
				std::stringstream ss;
				ss << "Wrong record size of LAYER (";
				ss << std::hex << record_size << record_type << record_dt;
				ss << ").";
				msg = ss.str();
				return FORMAT_ERROR;
			}
			if (!readShort(in, Layer))
				return FILE_ERROR;
			break;
		case DATATYPE:
			if (record_size != 6)
			{
				std::stringstream ss;
				ss << "Wrong record size of DATATYPE (";
				ss << std::hex << record_size << record_type << record_dt;
				ss << ").";
				msg = ss.str();
				return FORMAT_ERROR;
			}
			if (!readShort(in, Data_type))
				return FILE_ERROR;
			break;
		case XY:
		{
			record_size -= 4;
			int num = record_size / 8;
			if (record_size % 8 != 0 || num < 2)
			{
				std::stringstream ss;
				ss << "wrong record size of XY for PATH (";
				ss << std::hex << record_size << record_type << record_dt;
				ss << "). ";
				msg = ss.str();
				return FORMAT_ERROR;
			}
			for (int i = 0; i < num; i++)
			{
				int x, y;
				if (!readInteger(in, x)
					|| !readInteger(in, y))
					return FILE_ERROR;
				Pts.push_back(Point(x, y));
			}
			break;
		}
		case WIDTH:
			if (record_size != 8)
			{
				std::stringstream ss;
				ss << "Wrong record size of WIDTH (";
				ss << std::hex << record_size << record_type << record_dt;
				ss << "). ";
				msg = ss.str();
				return FORMAT_ERROR;
			}
			if (!readInteger(in, Width))
				return FILE_ERROR;
			break;
		case PATHTYPE:
			if (record_size != 6)
			{
				std::stringstream ss;
				ss << "Wrong record size of PATHTYPE (";
				ss << std::hex << record_size << record_type << record_dt;
				ss << "). ";
				msg = ss.str();
				return FORMAT_ERROR;
			}
			if (!readShort(in, Path_type))
				return FILE_ERROR;
			break;
		default:
			break;
		}
	}

	return 0;
}

int Path::write(std::ofstream &out, std::string &msg)
{
	short record_size;

	record_size = 4;
	writeShort(out, record_size);
	writeByte(out, PATH);
	writeByte(out, NoData);

	record_size = 6;
	writeShort(out, record_size);
	writeByte(out, EFLAGS);
	writeByte(out, Integer_2);
	writeShort(out, Eflags);

	record_size = 6;
	writeShort(out, record_size);
	writeByte(out, LAYER);
	writeByte(out, Integer_2);
	writeShort(out, Layer);

	record_size = 6;
	writeShort(out, record_size);
	writeByte(out, DATATYPE);
	writeByte(out, Integer_2);
	writeShort(out, Data_type);

	record_size = 8;
	writeShort(out, record_size);
	writeByte(out, WIDTH);
	writeByte(out, Integer_4);
	writeInteger(out, Width);

	record_size = 6;
	writeShort(out, record_size);
	writeByte(out, PATHTYPE);
	writeByte(out, Integer_2);
	writeShort(out, Path_type);

	record_size = 4 + 8 * Pts.size();
	writeShort(out, record_size);
	writeByte(out, XY);
	writeByte(out, Integer_4);
	for (auto p : Pts)
	{
		writeInteger(out, p.x);
		writeInteger(out, p.y);
	}

	record_size = 4;
	writeShort(out, record_size);
	writeByte(out, ENDEL);
	writeByte(out, NoData);

	return out.fail() ? FILE_ERROR : 0;
}
}

