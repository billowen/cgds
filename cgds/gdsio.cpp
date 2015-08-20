/*
 * This file is part of GDSII.
 *
 * gdsio.cpp -- The source file which define the functions used to convert
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

#include <sstream>
#include "gdsio.h"


bool GDS::readShort(std::ifstream &in, short &data)
{

    Byte buffer[2];
    in.read((char*)buffer, 2);
	if (in.fail())
		return false;
	
	data = buffer[0] << 8 | buffer[1];

    return true;
}

bool GDS::writeShort(std::ofstream &out, short data)
{
    char high, low;
    high = (data & 0xff00) >> 8;
    low = data & 0x00ff;
    out.write(&high, 1);
    out.write(&low, 1);

	return !out.fail();
}

bool GDS::readInteger(std::ifstream &in, int &data)
{
    Byte buffer[4];
    in.read((char*)buffer, 4);
	if (in.fail())
		return false;

    data = buffer[3]
            + (buffer[2] << 8)
            + (buffer[1] << 16)
            + (buffer[0] << 24);

    return true;
 }

bool GDS::writeInteger(std::ofstream &out, int data)
{
    Byte buffer[4];
    buffer[3] = data & 0xff;
    buffer[2] = (data >> 8) & 0xff;
    buffer[1] = (data >> 16) & 0xff;
    buffer[0] = (data >> 24) & 0xff;
    out.write((char *)buffer, 4);

	return !out.fail();
}

bool GDS::readString(std::ifstream &in, int size, std::string &data)
{
	data = "";
    for (int i = 0; i < size; i++)
    {
        char c;
        in.read(&c, 1);
		if (in.fail())
			return false;
        data.push_back(c);

    }
    return true;
}

bool GDS::writeString(std::ofstream &out, std::string data)
{
    out.write(data.c_str(), data.size());
    
    if (data.size() % 2 != 0)
    {
        char c = '\0';
        out.write(&c, 1);
    }

	return !out.fail();
}

bool GDS::readBitarray(std::ifstream &in, short &data)
{
    return readShort(in, data);
}

bool GDS::writeBitarray(std::ofstream &out, short data)
{
    return writeShort(out, data);
}

bool GDS::readDouble(std::ifstream &in, double &data)
{
    unsigned char buffer[8];
    in.read((char*)buffer, 8);
	if (in.fail())
		return false;

    short sign_flag = (buffer[0] & 0x80) ? -1 : 1;
    short exponent = (buffer[0] & 0x7f) - 64;
    long long mantissa = 0;
    for (int i = 1; i < 8; i++)
    {
        mantissa = mantissa << 8;
        //mantissa = mantissa | (0x00000000000000ff & buffer[i]);
        mantissa = mantissa | buffer[i];
    }

	data = mantissa / pow(2.0, 56);
    if (exponent >= 0)
    {
        for (int j = 0; j < exponent; j++)
        {
            data = data * 16;
        }
    }
    else
    {
        for (int j = 0; j < -exponent; j++)
        {
            data = data / 16;
        }
    }
    data = data * sign_flag;

	return true;
}

bool GDS::writeDouble(std::ofstream &out, double data)
{
    unsigned char buffer[8];
    if (data == 0)
    {
        for (int i = 0; i < 8; i++)
        {
            buffer[i] = 0;
        }
        out.write((char*)buffer, 8);
    }
    else
    {
        double mantissa = data >= 0 ? data : -data;
        short exponent = 0;
        while (mantissa >= 1 || mantissa < (1.0 / 16))
        {
            if (mantissa >= 1)
            {
                mantissa /= 16;
                exponent ++;
            }
            else
            {
                mantissa *= 16;
                exponent --;
            }
        }
        exponent += 64;
        buffer[0] = exponent;
        if (data < 0)
            buffer[0] = buffer[0] | 0x80;
        for (int j = 1; j < 8; j++)
        {
            mantissa *= 256;
            buffer[j] = mantissa;
            mantissa = mantissa - int(mantissa);

        }
        out.write((char*)buffer, 8);
    }

	return !out.fail();
}

bool GDS::readByte(std::ifstream &in, GDS::Byte &data)
{
    in.read((char*)&data, 1);
	return !in.fail();
}

bool GDS::writeByte(std::ofstream &out, GDS::Byte data)
{
    out.write((char*)&data, 1);
	return !out.fail();
}

std::string GDS::byteToString(Byte data)
{
    std::stringstream ss;
    ss << std::hex << data;
    return ss.str();
}
