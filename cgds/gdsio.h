/*
 * This file is part of GDSII.
 *
 * gdsio.h -- The header file which declare the functions used to convert
 *            binary data from GDSII files.
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

#ifndef GDSIO_H
#define GDSIO_H
#include <fstream>
#include <string>
#include "tags.h"

namespace GDS {

/*
 * 2-Byte Signed Integer    ---- short
 * 4-Byte Signed Integer    ---- int
 * 4-Byte Real              ---- float
 * 8-Byte Real              ---- double
 * ASCII String             ---- std::string
 * Bit Array                ---- short
 **/

bool readByte(std::ifstream &in, Byte &data);
bool readShort(std::ifstream &in, short &data);
bool readInteger(std::ifstream &in, int &data);
bool readDouble(std::ifstream &in, double &data);
bool readString(std::ifstream &in, int size, std::string &data);
bool readBitarray(std::ifstream &in, short &data);

bool writeByte(std::ofstream &out, Byte data);
bool writeShort(std::ofstream &out, short data);
bool writeInteger(std::ofstream &out, int data);
bool writeFloat(std::ofstream &out, float data);
bool writeDouble(std::ofstream &out, double data);
bool writeString(std::ofstream &out, std::string data);
bool writeBitarray(std::ofstream &out, short data);

std::string byteToString(Byte data);


}

#endif // GDSIO_H

