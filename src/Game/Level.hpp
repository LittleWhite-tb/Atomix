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

#ifndef LEVEL_HPP
#define LEVEL_HPP

namespace NE
{
    class Sprite;
}

#include "ResourceBank.hpp"
#include "Board.hpp"
#include "Scheme.hpp"
#include "Atom.hpp"

class Level
{
private:
    unsigned int m_availableTime;

    const NE::Sprite* m_pBackGroundSprite;

    Board* m_pBoard;
    AtomList m_atoms;
    Scheme* m_pScheme;

public:
    Level(ResourceBank* pResourceBank,const std::string& name);
    ~Level();
    
    unsigned int getAvailableTime()const { return m_availableTime; }
    
    bool drawBackground(const NE::Renderer& renderer);
    bool draw(const NE::Renderer& renderer, const UVec2& gameBoardOrigin, const UVec2& schemeBoardOrigin);
    
    const Board* getBoard()const { return m_pBoard; }
    const AtomList& getAtoms()const { return m_atoms; }
    const Scheme* getScheme()const { return m_pScheme; }
    
    bool isEnded()const;
    
    static bool checkAtom(const AtomList& atoms, const UVec2& position);
    bool checkAtom(const UVec2& position);
    static const Atom* getAtom(const AtomList& atoms, const UVec2& position);
    static Atom* getAtom(AtomList& atoms, const UVec2& position);
    Atom* getAtom(const UVec2& position);
};

#endif
