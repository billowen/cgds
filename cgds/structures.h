/*
 * This file is part of GDSII.
 *
 * struct.h -- The header file which declare the structure of GDSII format.
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

#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include "elements.h"

namespace GDS {

	class Structure {
		std::string     Struct_name;
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

		std::vector<std::shared_ptr<Element> > Elements;
	public:
		Structure();
		Structure(std::string name);
		~Structure();

		std::string name() const;
		size_t size() const;
		std::weak_ptr<Element> get(int index) const;

		void add(std::shared_ptr<Element> new_element);

		int read(std::ifstream &in, std::string &msg);
		int write(std::ofstream &out, std::string &msg);
	};

}



#endif // STRUCTURES_H

