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

#ifndef LEVELLOADER_HPP
#define LEVELLOADER_HPP

#include "FileReader.hpp"

#include "NEngine/Types/Size2.h"

#include <vector>

struct AtomData
{
    UVec2 position;
    
    int type;
    int connection;
};

class LevelLoader
{
    typedef std::vector< std::vector< int > > IntArray;
    typedef std::vector< AtomData > AtomDataArray;
    typedef std::vector< std::vector< std::pair<int, int> > > IntPairArray;
    
private:

    FileReader m_file;
    
    std::string m_backgroundName;
    std::string m_tileThemeName;
    USize2 m_size;
    
    unsigned int m_availableTime; // In seconds
    
    IntArray m_tilesID;
    AtomDataArray m_atomsData;
    
    std::string m_schemeName;
    IntPairArray m_schemeID;

    bool readData();
    
    bool readSize(USize2& size);
    bool readIntArray(const USize2& size, IntArray& array);
    
    bool readAtoms(AtomDataArray& array);
    bool readScheme(const USize2& size, IntPairArray& array);
    
public:
    LevelLoader(const std::string& levelPath);
    
    unsigned int getAvailableTime()const { return m_availableTime; }
    
    const std::string& getBackgroundName()const { return m_backgroundName; }
    const std::string& getTileThemeName()const { return m_tileThemeName; }
    const std::string& getSchemeName()const { return m_schemeName; }
    
    const USize2& getLevelSize() { return m_size; }
    const IntArray& getTilesID()const { return m_tilesID; }
    const AtomDataArray& getAtoms()const { return m_atomsData; }
    const IntPairArray& getSchemeID()const { return m_schemeID; }
};

#endif
