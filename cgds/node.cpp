/*
* This file is part of GDSII.
*
* sref.cpp -- The source file which defines the NODE of GDSII format.
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
#include "node.h"
#include <sstream>
#include "gdsio.h"

namespace GDS
{
Node::Node() :Element(NODE)
{
	Eflags = 0;
	Layer = -1;
	NodeType = -1;
}

Node::~Node()
{

}

int Node::read(std::ifstream &in, std::string &msg)
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
		case NODETYPE:
			if (record_size != 6)
			{
				std::stringstream ss;
				ss << "Wrong record size of NODETYPE (";
				ss << std::hex << record_size << record_type << record_dt;
				ss << ").";
				msg = ss.str();
				return FORMAT_ERROR;
			}
			if (!readShort(in, NodeType))
				return FILE_ERROR;
			break;
		case XY:
		{
			record_size -= 4;
			int num = record_size / 8;
			if (record_size % 8 != 0)
			{
				std::stringstream ss;
				ss << "wrong record size of XY for NODETYPE (";
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
		default:
			break;
		}
	}

	return 0;
}

int Node::write(std::ofstream &out, std::string &msg)
{
	short record_size;

	record_size = 4;
	writeShort(out, record_size);
	writeByte(out, NODE);
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
	writeByte(out, NODETYPE);
	writeByte(out, Integer_2);
	writeShort(out, NodeType);

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