/*
 * This file is part of GDSII.
 *
 * boundary.h -- The header file which declare the BOUNDARY in GDSII format.
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

#ifndef BOUNDARY_H
#define BOUNDARY_H
#include "elements.h"

namespace GDS {

/*!
	* \brief Class for 'BOUNDARY' GDSII element
	*
	* GDS syntax:
	*  BOUNDARY
	*  [EFLAGS]
	*  [PLEX]
	*  LAYER
	*  DATATYPE
	*  XY
	*  ENDEL
	*/
class Boundary : public Element {
	short               Eflags;         //< 2 bytes of bit flags. Not support yet.
	short               Layer;
	short               Data_type;
	std::vector<Point>  Pts;

public:
	Boundary();
	virtual ~Boundary();

	short layer() const;
	short dataType() const;
	std::vector<Point> xy() const;
	virtual bool bbox(int &x, int &y, int &w, int &h) const;

	void setLayer(short layer);
	void setDataType(short data_type);
	void setXY(const std::vector<Point> &pts);

	virtual int read(std::ifstream &in, std::string &msg);
	virtual int write(std::ofstream &out, std::string &msg);
};

}

#endif // BOUNDARY_H

