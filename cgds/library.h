/*
 * This file is part of GDSII.
 *
 * library.h -- The header file which declare the library in GDSII format.
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

#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include "structures.h"

namespace GDS 
{
class Techfile;

class Library {
	short           Version;
	short           Mod_year;
	short           Mod_month;
	short           Mod_day;
	short           Mod_hour;
	short           Mod_minute;
	short           Mod_second;
	short           Acc_year;
	short           Acc_month;
	short           Acc_day;
	short           Acc_hour;
	short           Acc_minute;
	short           Acc_second;
	std::string     Lib_name;
	double          DBUnit_in_meter;
	double          DBUnit_in_userunit;

	std::vector<std::shared_ptr<Structure> > Cells;

		
public:
	Library();
	~Library();

	void init();

	size_t size() const;
	std::shared_ptr<Structure> get(int index);
	std::shared_ptr<Structure> get(std::string name);
	/*!
		* Add a new structure into library. If there is a structure existed in library which
		* have the same name, it will cause the failure of process.
		* 
		* \param [in] name		The name of the new structure.
		*
		* \return	The pointer to the new structure. If failed to add the new structure, the
		*			return value will be nullptr.
		*/
    std::shared_ptr<Structure> Add(std::string name);
	/*!
		* Delete a structure in the library.
		*
		* \param [in] name			Name of structure.
		*/
    void Del(std::string name);
    void BuildCellLinks(bool del_dirty_links = false);
    void CollectLayers(Techfile &tech_file);

	int read(std::ifstream &in, std::string &msg);
	int write(std::ofstream &out, std::string &msg);
};
}

#endif // LIBRARY_H

