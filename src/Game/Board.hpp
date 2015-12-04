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

#ifndef BOARD_HPP
#define BOARD_HPP

#include "Tile.hpp"

#include "NEngine/Types/Size2.h"

#include <vector>

namespace NE
{
    class Sprite;
    class Renderer;
}
class BoardTileSetter;

class Board
{
private:
    USize2 m_size;
    std::vector< std::vector< Tile > > m_tiles;
    
    const NE::Sprite* m_pTilesSprite;
    
    friend class BoardTileSetter;
    
public:
    Board(const NE::Sprite* pTilesSprite, const USize2& boardSize);

    const Tile& getTile(const UVec2& position)const;
    const USize2& getSize()const { return m_size; }

    bool draw(const NE::Renderer& renderer, const UVec2& boardOrigin);
};

#endif
