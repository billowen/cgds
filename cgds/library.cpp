/*
 * This file is part of GDSII.
 *
 * library.cpp -- The source file which defines the library of GDSII format.
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
#include "library.h"
#include "tags.h"
#include "gdsio.h"
#include <sstream>
#include <ctime>
#include "elements.h"
#include "aref.h"
#include "sref.h"

namespace GDS 
{

Library::Library()
{
	init();
}

Library::~Library()
{
	Cells.clear();
}

void Library::init()
{
	Lib_name = "";

	time_t now = time(0);
	tm *ltm    = localtime(&now);
	Mod_year   = ltm->tm_year + 1900;
	Mod_month  = ltm->tm_mon + 1;
	Mod_day    = ltm->tm_mday;
	Mod_hour   = ltm->tm_hour + 1;
	Mod_minute = ltm->tm_min + 1;
	Mod_second = ltm->tm_sec + 1;
	Acc_year   = ltm->tm_year + 1900;
	Acc_month  = ltm->tm_mon + 1;
	Acc_day    = ltm->tm_mday;
	Acc_hour   = ltm->tm_hour + 1;
	Acc_minute = ltm->tm_min + 1;
	Acc_second = ltm->tm_sec + 1;

	Version = 0;
	DBUnit_in_meter = 1e-9;
	DBUnit_in_userunit = 1e-3;

	Cells.clear();
}

size_t Library::size() const
{
	return Cells.size();
}

std::shared_ptr<Structure> Library::get(int index)
{
	if (index < 0 || (size_t)index >= Cells.size())
		return std::shared_ptr<Structure>();
	return Cells[index];
}

std::shared_ptr<Structure> Library::add(std::string name)
{
	std::shared_ptr<Structure> ret = std::make_shared<Structure>();
	for (auto e : Cells)
	{
		if (!e)
			continue;

		if (e->name() == name)
		{
			ret = e;
		}
	}
	if (!ret)
	{
		std::shared_ptr<Structure> new_item = std::make_shared<Structure>(name);
		Cells.push_back(new_item);
		ret = new_item;
	}

	return ret;
}

std::shared_ptr<Structure> Library::get(std::string name)
{
	for (auto e : Cells)
	{
		if (!e)
			continue;
		if (e->name() == name)
		{
			return e;
		}
	}
	return std::shared_ptr<Structure>();
}

void Library::del(std::string name)
{
	for (size_t i = 0; i < size(); i++)
	{
		auto node = Cells[i];
		if (!node)
			continue;
		if (node->name() == name)
		{
			Cells.erase(Cells.begin() + i);
			break;
		}
	}
}

void Library::buildCellLinks(bool del_dirty_links)
{
	for (auto cell : Cells)
	{
		for (size_t i = 0; i < cell->size(); i++)
		{
			auto element = cell->get(i);
			if (element->tag() == SREF)
			{
				auto temp = std::dynamic_pointer_cast<SRef>(element);
				std::string sname = temp->structName();
				auto source_cell = get(sname);
				temp->setReference(source_cell);
				source_cell->addReferBy(cell);
			}
			else if (element->tag() == AREF)
			{
				auto temp = std::dynamic_pointer_cast<ARef>(element);
				std::string sname = temp->structName();
				auto source_cell = get(sname);
				temp->setReference(source_cell);
				source_cell->addReferBy(cell);
			}
		}
	}
}

int Library::read(std::ifstream &in, std::string &msg)
{
	msg = "";
	init();
	// read HEADER
	short record_size;
	Byte record_type, record_dt;
	if (!readShort(in, record_size)
		|| !readByte(in, record_type)
		|| !readByte(in, record_dt))
		return FILE_ERROR;
	if (record_type != HEADER)
	{
		std::stringstream ss;
		ss << "Unexpected tag where HEADER are expected (";
		ss << std::hex << record_size << record_type << record_dt;
		ss << ").";
		msg = ss.str();
		return FORMAT_ERROR;
	}
	if (record_size != 6)
	{
		std::stringstream ss;
		ss << "wrong record size of HEADER (";
		ss << std::hex << record_size << record_type << record_dt;
		ss << ").";
		msg = ss.str();
		return FORMAT_ERROR;
	}
	if (!readShort(in, Version))
		return FILE_ERROR;

	// read BGNLIB
	if (!readShort(in, record_size)
		|| !readByte(in, record_type)
		|| !readByte(in, record_dt))
		return FILE_ERROR;
	if (record_type != BGNLIB)
	{
		std::stringstream ss;
		ss << "Unexpected tag where BGNLIB are expected (";
		ss << std::hex << record_size << record_type << record_dt;
		ss << ").";
		msg = ss.str();
		return FORMAT_ERROR;
	}
	if (record_size != 28)
	{
		std::stringstream ss;
		ss << "Wrong record size of BGNLIB (";
		ss << std::hex << record_size << record_type << record_dt;
		ss << ").";
		msg = ss.str();
		return FORMAT_ERROR;
	}

	if (!readShort(in, Mod_year)
		|| !readShort(in, Mod_month)
		|| !readShort(in, Mod_day)
		|| !readShort(in, Mod_hour)
		|| !readShort(in, Mod_minute)
		|| !readShort(in, Mod_second)
		|| !readShort(in, Acc_year)
		|| !readShort(in, Acc_month)
		|| !readShort(in, Acc_day)
		|| !readShort(in, Acc_hour)
		|| !readShort(in, Acc_minute)
		|| !readShort(in, Acc_second))
		return FILE_ERROR;

	while (1)
	{
		if (!readShort(in, record_size)
			|| !readByte(in, record_type)
			|| !readByte(in, record_dt))
			return FILE_ERROR;

		bool finished = false;
		switch (record_type)
		{
		case ENDLIB:
			finished = true;
			break;
		case LIBNAME:
			if (record_size < 4 || record_size % 2 != 0)
			{
				std::stringstream ss;
				ss << "Wrong record size of " + Record_name[record_type] + " (";
				ss << std::hex << record_size << record_type << record_dt;
				ss << ").";
				msg = ss.str();
				return FORMAT_ERROR;
			}
			if (!readString(in, record_size - 4, Lib_name))
				return FILE_ERROR;
			break;
		case UNITS:
			if (record_size != 20)
			{
				std::stringstream ss;
				ss << "Wrong record size of UNITS (";
				ss << std::hex << record_size << record_type << record_dt;
				ss << ").";
				msg = ss.str();
				return FORMAT_ERROR;
			}
			if (!readDouble(in, DBUnit_in_userunit)
				|| !readDouble(in, DBUnit_in_meter))
				return FILE_ERROR;
			break;
		case BGNSTR:
		{
			if (record_size != 28)
			{
				std::stringstream ss;
				ss << "Wrong record size of BGNSTR (";
				ss << std::hex << record_size << record_type << record_dt;
				ss << ").";
				msg = ss.str();
				return FORMAT_ERROR;
			}
			std::shared_ptr<Structure> node = std::make_shared<Structure>();
			int error_code = node->read(in, msg);
			if (error_code > 0)
				return error_code;
			Cells.push_back(node);
			break;
		}
		default:
			break;
		}
		if (finished)
			break;
	}

	return 0;
}

int Library::write(std::ofstream &out, std::string &msg)
{
	int record_size;

	record_size = 6;
	writeShort(out, record_size);
	writeByte(out, HEADER);
	writeByte(out, Integer_2);
	writeShort(out, Version);

	record_size = 28;
	writeShort(out, record_size);
	writeByte(out, BGNLIB);
	writeByte(out, Integer_2);
	writeShort(out, Mod_year);
	writeShort(out, Mod_month);
	writeShort(out, Mod_day);
	writeShort(out, Mod_hour);
	writeShort(out, Mod_minute);
	writeShort(out, Mod_second);
	writeShort(out, Acc_year);
	writeShort(out, Acc_month);
	writeShort(out, Acc_day);
	writeShort(out, Acc_hour);
	writeShort(out, Acc_minute);
	writeShort(out, Acc_second);

	record_size = 4 + Lib_name.size();
	if (record_size % 2 != 0)
		record_size += 1;
	writeShort(out, record_size);
	writeByte(out, LIBNAME);
	writeByte(out, String);
	writeString(out, Lib_name);

	record_size = 20;
	writeShort(out, record_size);
	writeByte(out, UNITS);
	writeByte(out, Real_8);
	writeDouble(out, DBUnit_in_userunit);
	writeDouble(out, DBUnit_in_meter);

	for (auto e : Cells)
	{
		int err_code = e->write(out, msg);
		if (err_code > 0)
			return err_code;
	}

	record_size = 4;
	writeShort(out, record_size);
	writeByte(out, ENDLIB);
	writeByte(out, NoData);

	return out.fail() ? FILE_ERROR : 0;
}

}


