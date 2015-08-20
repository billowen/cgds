/*
 * This file is part of GDSII.
 *
 * structure.cpp -- The source file which defines the structure of GDSII format.
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

#include <iostream>
#include <limits>
#include <algorithm>
#include <sstream>
#include "structures.h"
#include "boundary.h"
#include "path.h"
#include "gdsio.h"
#include <ctime>

namespace GDS
{

Structure::Structure()
{
	Struct_name = "";

	time_t now = time(0);
	tm *ltm = localtime(&now);
	Mod_year = ltm->tm_year + 1900;
	Mod_month = ltm->tm_mon + 1;
	Mod_day = ltm->tm_mday;
	Mod_hour = ltm->tm_hour + 1;
	Mod_minute = ltm->tm_min + 1;
	Mod_second = ltm->tm_sec + 1;
	Acc_year = ltm->tm_year + 1900;
	Acc_month = ltm->tm_mon + 1;
	Acc_day = ltm->tm_mday;
	Acc_hour = ltm->tm_hour + 1;
	Acc_minute = ltm->tm_min + 1;
	Acc_second = ltm->tm_sec + 1;
}

Structure::Structure(std::string name)
{
	Struct_name = name;

	time_t now = time(0);
	tm *ltm = localtime(&now);
	Mod_year = ltm->tm_year + 1900;
	Mod_month = ltm->tm_mon + 1;
	Mod_day = ltm->tm_mday;
	Mod_hour = ltm->tm_hour + 1;
	Mod_minute = ltm->tm_min + 1;
	Mod_second = ltm->tm_sec + 1;
	Acc_year = ltm->tm_year + 1900;
	Acc_month = ltm->tm_mon + 1;
	Acc_day = ltm->tm_mday;
	Acc_hour = ltm->tm_hour + 1;
	Acc_minute = ltm->tm_min + 1;
	Acc_second = ltm->tm_sec + 1;
}

Structure::~Structure()
{
	Elements.clear();
}

std::string Structure::name() const
{
	return Struct_name;
}

size_t Structure::size() const
{
	return Elements.size();
}

std::weak_ptr<Element> Structure::get(int index) const
{
	if (index < 0 || index >= Elements.size())
		return std::shared_ptr<Element>();
	else
		return Elements[index];
}

void Structure::add(std::shared_ptr<Element> new_element)
{
	if (new_element.get() == nullptr)
		return;
	bool existed = false;
	for (auto e : Elements)
	{
		if (new_element.get() == e.get())
		{
			existed = true;
			break;
		}
	}
	if (!existed)
		Elements.push_back(new_element);
			
}

int Structure::read(std::ifstream &in, std::string &msg)
{
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
		case ENDSTR:
			if (record_size != 4)
			{
				std::stringstream ss;
				ss << "Wrong record size of ENDSTR (";
				ss << std::hex << record_size << record_type << record_dt;
				ss << ").";
				msg = ss.str();
				return FORMAT_ERROR;
			}
			finished = true;
			break;
		case STRNAME:
			if (record_size < 4 || record_size % 2 != 0)
			{
				std::stringstream ss;
				ss << "Wrong record size of STRNAME (";
				ss << std::hex << record_size << record_type << record_dt;
				ss << ").";
				msg = ss.str();
				return FORMAT_ERROR;
			}
			if (!readString(in, record_size - 4, Struct_name))
				return FILE_ERROR;
			break;
		/*case TEXT:
		{
			if (record_size != 4)
			{
				std::stringstream ss;
				ss << "wrong record size of TEXT (";
				ss << std::hex << record_size << record_type << data_type;
				ss << ").";
				std::string msg = ss.str();
				throw FormatError(msg);
			}
			Text *e = new Text(this);
			e->read(in);
			Contents.push_back(e);
			break;
		}*/
		case BOUNDARY:
		{
			if (record_size != 4)
			{
				std::stringstream ss;
				ss << "Wrong record size of BOUNDARY (";
				ss << std::hex << record_size << record_type << record_dt;
				ss << ").";
				msg = ss.str();
				return FORMAT_ERROR;
			}
			std::shared_ptr<Boundary> e = std::make_shared<Boundary>();
			int error_code = e->read(in, msg);
			if (error_code > 0)
				return error_code;
			Elements.push_back(e);
			break;
		}
		case PATH:
		{
			if (record_size != 4)
			{
				std::stringstream ss;
				ss << "Wrong record size of PATH (";
				ss << std::hex << record_size << record_type << record_dt;
				ss << ").";
				msg = ss.str();
				return FORMAT_ERROR;
			}
			std::shared_ptr<Path> e = std::make_shared<Path>();
			int error_code = e->read(in, msg);
			if (error_code > 0)
				return error_code;
			Elements.push_back(e);
			break;
		}
		/*case SREF:
		{
			if (record_size != 4)
			{
				std::stringstream ss;
				ss << "wrong record size of SREF (";
				ss << std::hex << record_size << record_type << data_type;
				ss << ").";
				std::string msg = ss.str();
				throw FormatError(msg);
			}
#ifdef _DEBUG_LOG
            {
                std::stringstream ss;
                ss << std::dec << " " << record_size << " " << Record_name[record_type] << " " << data_type << std::endl;
                log->write(ss.str());
            }
#endif
			SRef *e = new SRef(this);
			e->read(in);
			Contents.push_back(e);
			break;
		}
		case AREF:
		{
			if (record_size != 4)
			{
				std::stringstream ss;
				ss << "wrong record size of AREF (";
				ss << std::hex << record_size << record_type << data_type;
				ss << ").";
				std::string msg = ss.str();
				throw FormatError(msg);
			}
#ifdef _DEBUG_LOG
            {
                std::stringstream ss;
                ss << std::dec << " " << record_size << " " << Record_name[record_type] << " " << data_type << std::endl;
                log->write(ss.str());
            }
#endif
			ARef *e = new ARef(this);
			e->read(in);
			Contents.push_back(e);
			break;
		}*/
		default:
			break;
		}
	}
	return true;
}

int Structure::write(std::ofstream &out, std::string &msg)
{
	short record_size;

	record_size = 28;
	writeShort(out, record_size);
	writeByte(out, BGNSTR);
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

	record_size = 4 + Struct_name.size();
	if (record_size % 2 != 0)
		record_size += 1;
	writeShort(out, record_size);
	writeByte(out, STRNAME);
	writeByte(out, String);
	writeString(out, Struct_name);

	if (out.fail())
		return FILE_ERROR;

	for (auto e : Elements)
	{
		int err_code = e->write(out, msg);
		if (err_code > 0)
			return err_code;
	}

	record_size = 4;
	writeShort(out, record_size);
	writeByte(out, ENDSTR);
	writeByte(out, NoData);

	return out.fail() ? FILE_ERROR : 0;
}


}




