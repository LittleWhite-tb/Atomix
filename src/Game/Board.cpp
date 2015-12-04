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

#include "Board.hpp"

#include "common.hpp"

#include "NEngine/Renderer.h"
#include "NEngine/Sprite.h"
#include "NEngine/Types/Rect.h"

Board::Board(const NE::Sprite* pTilesSprite,const USize2& boardSize)
    :m_size(boardSize),m_tiles(boardSize.height,std::vector<Tile>(boardSize.width,Tile(2))),
     m_pTilesSprite(pTilesSprite)
{
    
}

bool Board::draw(const NE::Renderer& renderer, const UVec2& boardOrigin)
{
    bool bResult = true;
    
    for (size_t y = 0 ; y < m_tiles.size() ; y++ )
    {
        for (size_t x = 0 ; x < m_tiles[y].size() ; x++ )
        {
            if ( m_tiles[y][x].getConsistency() != Tile::TC_Null )
            {
                int spriteIndex = m_tiles[y][x].getType()-1;
                Rect spriteSource(IVec2(SPRITE_WIDTH*spriteIndex,0),
                                  USize2(SPRITE_WIDTH,SPRITE_HEIGHT));
                IVec2 screenPosition(boardOrigin.x+x*SPRITE_WIDTH,boardOrigin.y+y*SPRITE_HEIGHT);
                
                bResult &= renderer.drawSurface(screenPosition,m_pTilesSprite,spriteSource);
            }
        }
    }
    
    return bResult;
}

const Tile& Board::getTile(const UVec2& position)const
{
    assert(position.y < m_tiles.size()) ;
    assert(position.x < m_tiles[0].size());
    
    return m_tiles[position.y][position.x];
}
