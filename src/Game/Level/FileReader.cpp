/**
Atomix is a remake of the eponym game from Atari ST / Amiga
The program is developped under an event that took place on internet with the goal to make a game in a weekend
Copyright (C) 2013  Alexandre "LittleWhite" LAURENT

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

website: http://www.developpez.net/forums/d1360495/applications/developpement-2d-3d-jeux/projets/we-jeux-3-atomix/
e-mail: lw.demoscene@gmail.com
**/

#include "FileReader.hpp"

#include <ios>

FileReader::FileReader(const std::string& filePath)
    :m_file(filePath.c_str())
{
    if ( !m_file.is_open() )
    {
        throw std::ios_base::failure("Fail to find file " + filePath);
    }
}
    
bool FileReader::readLine(std::string& line)
{
    bool bResult = false;
    
    bResult = std::getline(m_file,line);
    if ( line.size() == 0 ) // Jump \n
    {
        bResult = std::getline(m_file,line);
    }
    return bResult;
}
