/*
* This file is part of GDSII.
*
* text.h -- The header file which declare the NODE in GDSII format.
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

#ifndef NODE_H
#define NODE_H
#include "elements.h"

namespace GDS {

/*!
* \brief Class for 'Node' GDSII element
*
* GDS syntax:
*  NODE
*  [EFLAGS]
*  [PLEX]
*  LAYER
*  NODETYPE
*  XY
*  ENDEL
*/
class Node: public Element
{
public:
	Node();
	virtual ~Node();

	short layer() const { return Layer; }
	short nodeType() const { return NodeType; }
	std::vector<Point> xy() const { return Pts; }

	void setLayer(short layer) { Layer = layer; }
	void setNodeType(short node_type) { NodeType = node_type; }
	void setXY(std::vector<Point> xy) { Pts = xy; }

	virtual int read(std::ifstream &in, std::string &msg);
	virtual int write(std::ofstream &out, std::string &msg);

private:
	short               Eflags;         //< 2 bytes of bit flags. Not support yet.
	short               Layer;
	short               NodeType;
	std::vector<Point>  Pts;
};

}

#endif // TEXT_H

