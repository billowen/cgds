/*
 * This file is part of GDSII.
 *
 * elements.h -- The header file which declare the elements in GDSII format.
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

#ifndef ELEMENTS_H
#define ELEMENTS_H
#include "tags.h"
#include <vector>
#include <string>
#include <fstream>

namespace GDS {

class Element {
	Record_type Tag;

public:
	Element();
	Element(Record_type tag);
	virtual ~Element();

	Record_type tag() const;
	std::string type() const;

	virtual int read(std::ifstream &in, std::string &msg) = 0;
	virtual int write(std::ofstream &out, std::string &msg) = 0;

protected:
	void setTag(Record_type tag);
};

}
#endif // ELEMENTS_H

