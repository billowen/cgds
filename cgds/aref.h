/*
 * This file is part of GDSII.
 *
 * aref.h -- The header file which declare the AREF in GDSII format.
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

#ifndef AREF_H
#define AREF_H
#include <memory>
#include "elements.h"

namespace GDS {
class Structure;
/*!
	* \brief Class for 'AREF' GDSII element
	*
	* GDS syntax:
	* GDS syntax:
	*  AREF
	*  [EFLAGS]
	*  [PLEX]
	*  SNAME
	*  [STRANS]
	*  COLROW
	*  XY
	*  ENDEL
	*/
class ARef : public Element {
	short               Eflags;
	std::string         SName;
	short               Strans;
	short               Row, Col;
	std::vector<Point>  Pts;
	double              Angle;
	double              Mag;
	std::weak_ptr<Structure> ReferTo;

public:
	ARef();
	virtual ~ARef();

	std::string structName() const;
	short row() const;
	short col() const;
	std::vector<Point> xy() const;
	double angle() const;
	double mag() const;
	short strans() const;
	bool stransFlag(STRANS_FLAG flag) const;
	virtual bool bbox(int &x, int &y, int &w, int &h) const;
    std::shared_ptr<Structure> reference() { return ReferTo.lock(); }

	void setStructName(std::string name);
	void setRowCol(int row,  int col);
	void setXY(std::vector<Point> pts);
	void setAngle(double angle);
	void setMag(double mag);
	void setStrans(short strans);
	void setStrans(STRANS_FLAG flag, bool enable = true);
	void setReference(std::shared_ptr<Structure> ref);

	virtual int read(std::ifstream &in, std::string &msg);
	virtual int write(std::ofstream &out, std::string &msg);
};

}

#endif // AREF_H

