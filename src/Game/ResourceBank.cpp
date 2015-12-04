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

#include "ResourceBank.hpp"

#include "NEngine/SpriteLoader.h"
#include "Font.hpp"

#include <cassert>
#include <stdexcept>

ResourceBank::ResourceBank(NE::SpriteLoader* pSpriteLoader)
    :m_pSpriteLoader(pSpriteLoader)
{
    assert(pSpriteLoader);
}

ResourceBank::~ResourceBank()
{
    for ( FontBank::const_iterator itFont = m_fonts.begin() ; itFont != m_fonts.end() ; ++itFont )
    {
        delete itFont->second;
    }
}

const NE::Sprite* ResourceBank::getSprite(const std::string& name)
{
    SpritesBank::const_iterator itSprite = m_sprites.find(name);
    if ( itSprite == m_sprites.end() ) // Not found, tries to load it
    {
        const NE::Sprite* pNewSprite = m_pSpriteLoader->loadSpriteFromFile(name);
        if ( pNewSprite == NULL )
        {
            throw std::runtime_error("Fail to get sprite, giving up (try to download the game again)");
        }
        
        m_sprites[name] = pNewSprite;
        
        return pNewSprite;
    }
    else
    {
        return itSprite->second;
    }
}

void ResourceBank::loadFont(const std::string& name, const USize2& letterSize, unsigned char startingLetter)
{
    Font* pNewFont = new Font(this->getSprite(name),letterSize,startingLetter);
    if ( pNewFont == NULL )
    {
        throw std::runtime_error("Fail to load font, giving up (try to download the game again)");
    }
    
    m_fonts[name] = pNewFont;
}

Font* ResourceBank::getFont(const std::string& name)
{
    FontBank::const_iterator itFont = m_fonts.find(name);
    if ( itFont == m_fonts.end() ) // Not found :(
    {
        throw std::runtime_error("Fail to get font : did you load it before ?");
    }
    else
    {
        return itFont->second;
    }
}
