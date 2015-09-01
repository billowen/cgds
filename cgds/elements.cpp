/*
 * This file is part of GDSII.
 *
 * elements.cpp -- The source file which defines the elements of GDSII format.
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

#include "elements.h"


namespace GDS
{

Element::Element()
{
	Tag = RECORD_UNKNOWN;
}

Element::Element(Record_type tag)
{
	Tag = tag;
}

Element::~Element()
{
	
}

std::string Element::type() const
{
	if (Record_name.find(Tag) == Record_name.end())
		return "RECORD_UNKNOWN";
	else
		return Record_name[Tag];
}

Record_type Element::tag() const
{
	return Tag;
}

void Element::set_tag(Record_type tag)
{
	Tag = tag;
}

}

