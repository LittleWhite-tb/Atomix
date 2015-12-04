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

#include "LevelLoader.hpp"

#include "FileReader.hpp"

#include "NEngine/NEngine.h"

#include <stdexcept>
#include <cassert>

LevelLoader::LevelLoader(const std::string& levelPath)
    :m_file(levelPath)
{
    if ( !readData() )
    {
        throw std::logic_error("Fail to load level '"+levelPath+"'");
    }
}
    
bool LevelLoader::readData()
{
    bool bResult = true;
    
    bResult &= m_file.readLine(m_backgroundName);
    bResult &= m_file.readLine(m_tileThemeName);
    bResult &= m_file.readValue<unsigned>(m_availableTime);
    
    bResult &= readSize(m_size);
    
    if ( bResult )
    {
        m_tilesID.resize(m_size.height,std::vector<int>(m_size.width,0));
        bResult &= readIntArray(m_size,m_tilesID);
    }
    else
    {
        NEError << "Fail to load level size\n";
        return false;
    }
    
    bResult &= readAtoms(m_atomsData);
    if ( bResult == false )
    {
        NEError << "Fail to load atoms\n";
        return false;
    }
    
    bResult &= m_file.readLine(m_schemeName);
    if ( bResult == false )
    {
        NEError << "Fail to scheme name\n";
        return false;
    }
    
    USize2 schemeSize;
    bResult &= readSize(schemeSize);
    
    if ( bResult )
    {
        m_schemeID.resize(schemeSize.height,std::vector<std::pair<int,int> >(schemeSize.width,std::pair<int,int>(0,0)));
        bResult &= readScheme(schemeSize,m_schemeID);
    }
    else
    {
        NEError << "Fail to load scheme size\n";
        return false;
    }
    
    return bResult;    
}

bool LevelLoader::readSize(USize2& size)
{
    bool bResult = true;
    
    bResult &= m_file.readValue<unsigned>(size.width);
    bResult &= m_file.readValue<unsigned>(size.height);
    
    return bResult;
}

bool LevelLoader::readIntArray(const USize2& size, IntArray& array )
{
    bool bResult = true;
    
    assert(array.size() == size.height);
    assert(array[0].size() == size.width);
    
    for ( unsigned int y = 0 ; y < size.height ; y++ )
    {
        for (unsigned int x = 0 ; x < size.width ; x++ )
        {
            bResult &= m_file.readValue<int>(array[y][x]);
            if (bResult == false )
            {
                NEError << "Fail to read array at " << x << ";" << "y\n";
                return false;
            }
        }
    }
    
    return true;
}

bool LevelLoader::readAtoms(AtomDataArray& array)
{
    bool bResult = true;
    
    unsigned nbAtoms = 0;
    bResult &= m_file.readValue<unsigned>(nbAtoms);
    
    if ( bResult == false )
    {
        NEError << "Fail to read number of atoms";
        return false;
    }
    if ( nbAtoms <= 1 )
    {
        NEError << "Invalid number of atoms declared in level file";
        return false;
    }
    
    for ( unsigned int i = 0 ; i < nbAtoms ; i++ )
    {
        AtomData atom;
        
        bResult &= m_file.readValue<int>(atom.type);
        bResult &= m_file.readValue<int>(atom.connection);
        bResult &= m_file.readValue<unsigned>(atom.position.x);
        bResult &= m_file.readValue<unsigned>(atom.position.y);
        
        array.push_back(atom);
    }
    
    return bResult;
}

bool LevelLoader::readScheme(const USize2& size, IntPairArray& array)
{
    bool bResult = true;
    
    assert(array.size() == size.height);
    assert(array[0].size() == size.width);
    
    for ( unsigned int y = 0 ; y < size.height ; y++ )
    {
        for (unsigned int x = 0 ; x < size.width ; x++ )
        {
            bResult &= m_file.readValue<int>(array[y][x].first);
            bResult &= m_file.readValue<int>(array[y][x].second);
            if (bResult == false )
            {
                NEError << "Fail to read array at " << x << ";" << "y\n";
                return false;
            }
        }
    }
    
    return true;
}
