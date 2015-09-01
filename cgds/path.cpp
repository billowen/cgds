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

short Path::data_type() const
{
	return Data_type;
}

int Path::width() const
{
	return Width;
}

short Path::path_type() const
{
	return Path_type;
}

std::vector<Point> Path::xy() const
{
	return Pts;
}

void Path::set_layer(short layer)
{
	Layer = layer;
}

void Path::set_data_type(short data_type)
{
	Data_type = data_type;
}

void Path::set_width(int width)
{
	Width = width;
}

void Path::set_path_type(short type)
{
	Path_type = type;
}

void Path::set_xy(const std::vector<Point> &pts)
{
	Pts.clear();
	Pts = pts;
}

bool Path::bbox(int &x, int &y, int &w, int &h) const
{
	assert(Width > 0 && Pts.size() >= 2);
	if (Width == 0 || Pts.size() < 2)
		return false;

	int llx = GDS_MAX_INT;
	int lly = GDS_MAX_INT;
	int urx = GDS_MIN_INT;
	int ury = GDS_MIN_INT;
	for (auto pt : Pts)
	{
		llx = pt.x < llx ? pt.x : llx;
		lly = pt.y < lly ? pt.y : lly;
		urx = pt.x > urx ? pt.x : urx;
		ury = pt.y > ury ? pt.y : ury;
	}
	// extend the path
	for (size_t i = 1; i < Pts.size(); i++)
	{
		if (Pts[i - 1].x == Pts[i].x)
		{
			llx = (Pts[i].x - Width / 2) < llx ? (Pts[i].x - Width / 2) : llx;
			urx = (Pts[i].x + Width / 2) > urx ? (Pts[i].x + Width / 2) : urx;
		}
		else
		{
			lly = (Pts[i].y - Width / 2) < lly ? (Pts[i].y - Width / 2) : lly;
			ury = (Pts[i].y + Width / 2) > ury ? (Pts[i].y + Width / 2) : ury;
		}
	}
	// extend the end point
	if (Path_type > 0)
	{
		if (Pts[0].x == Pts[1].x)
		{
			if (Pts[0].y < Pts[1].y)
				lly = (Pts[0].y - Width / 2) < lly ? (Pts[0].y - Width / 2) : lly;
			else
				ury = (Pts[0].y + Width / 2) > ury ? (Pts[0].y + Width / 2) : ury;
		}
		else
		{
			if (Pts[0].x < Pts[1].x)
				llx = (Pts[0].x - Width / 2) < llx ? (Pts[0].x - Width / 2) : llx;
			else
				urx = (Pts[0].x + Width / 2) > urx ? (Pts[0].x + Width / 2) : urx;
		}
		size_t n = Pts.size();
		if (Pts[n - 2].x == Pts[n - 1].x)
		{
			if (Pts[n - 1].y < Pts[n - 2].y)
				lly = (Pts[n - 1].y - Width / 2) < lly ? (Pts[n - 1].y - Width / 2) : lly;
			else
				ury = (Pts[n - 1].y + Width / 2) > ury ? (Pts[n - 1].y + Width / 2) : ury;
		}
		else 
		{
			if (Pts[n - 1].x < Pts[n - 2].x)
				llx = (Pts[n - 1].x - Width / 2) < llx ? (Pts[n - 1].x - Width / 2) : llx;
			else
				urx = (Pts[n - 1].x + Width / 2) > urx ? (Pts[n - 1].x + Width / 2) : urx;
		}
	}
	x = llx;
	y = lly;
	w = urx - llx;
	h = ury - lly;

	return true;
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

    record_size = 4 + short(8 * Pts.size());
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

