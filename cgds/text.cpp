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

#include "text.h"
#include <sstream>
#include "gdsio.h"

namespace GDS
{

Text::Text() :Element(TEXT)
{
	Eflags = 0;
	Layer = -1;
	Text_type = -1;
	Presentation = 0;
	Strans = 0;
	String = "";
}

Text::~Text()
{

}

short Text::layer() const
{
	return Layer;
}

short Text::textType() const
{
	return Text_type;
}

short Text::presentation() const
{
	return Presentation;
}

short Text::strans() const
{
	return Strans;
}

Point Text::xy() const
{
	return Pt;
}

std::string Text::string() const
{
	return String;
}

void Text::setLayer(short layer)
{
	Layer = layer;
}

void Text::setTextType(short text_type)
{
	Text_type = text_type;
}

void Text::setPresentation(short presentation)
{
	Presentation = presentation;
}

void Text::setStrans(short strans)
{
	Strans = strans;
}

void Text::setXY(Point pt)
{
	Pt = pt;
}

void Text::setString(std::string string)
{
	String = string;
}

int Text::read(std::ifstream &in, std::string &msg)
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
		case TEXTTYPE:
			if (record_size != 6)
			{
				std::stringstream ss;
				ss << "Wrong record size of TEXTTYPE (";
				ss << std::hex << record_size << record_type << record_dt;
				ss << ").";
				msg = ss.str();
				return FORMAT_ERROR;
			}
			if (!readShort(in, Text_type))
				return FILE_ERROR;
			break;
		case XY:
		{
			if (record_size != 12)
			{
				std::stringstream ss;
				ss << "Wrong record size of XY for TEXT (";
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
		}
		case PRESENTATION:
			if (record_size != 6)
			{
				std::stringstream ss;
				ss << "Wrong record size of PRESENTATION (";
				ss << std::hex << record_size << record_type << record_dt;
				ss << ").";
				msg = ss.str();
				return FORMAT_ERROR;
			}
			if (!readShort(in, Presentation))
				return FILE_ERROR;
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
		case STRING:
			if (record_size < 4 || record_size % 2 != 0)
			{
				std::stringstream ss;
				ss << "Wrong record size of STRING (";
				ss << std::hex << record_size << record_type << record_dt;
				ss << ").";
				msg = ss.str();
				return FORMAT_ERROR;
			}
			if (!readString(in, record_size - 4, String))
				return FILE_ERROR;
			break;
		default:
			break;
		}
	}
	return 0;
}

int Text::write(std::ofstream &out, std::string &msg)
{
	short record_size;

	record_size = 4;
	writeShort(out, record_size);
	writeByte(out, TEXT);
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
	writeByte(out, TEXTTYPE);
	writeByte(out, Integer_2);
	writeShort(out, Text_type);

	record_size = 6;
	writeShort(out, record_size);
	writeByte(out, PRESENTATION);
	writeByte(out, Integer_2);
	writeShort(out, Presentation);

	record_size = 6;
	writeShort(out, record_size);
	writeByte(out, STRANS);
	writeByte(out, Integer_2);
	writeShort(out, Strans);

	record_size = 12;
	writeShort(out, record_size);
	writeByte(out, XY);
	writeByte(out, Integer_4);
	writeInteger(out, Pt.x);
	writeInteger(out, Pt.y);

	record_size = 4 + String.size();
	if (record_size % 2 != 0)
		record_size += 1;
	writeShort(out, record_size);
	writeByte(out, STRING);
	writeByte(out, GDS::String);
	writeString(out, String);

	record_size = 4;
	writeShort(out, record_size);
	writeByte(out, ENDEL);
	writeByte(out, NoData);

	return out.fail() ? FILE_ERROR : 0;
}

}


