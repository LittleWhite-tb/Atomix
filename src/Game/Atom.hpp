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

#ifndef ATOM_HPP
#define ATOM_HPP

#include "NEngine/Types/Vec2.h"

#include <vector>

namespace NE
{
    class Sprite;
    class Renderer;
}

class Atom
{
private:
    const NE::Sprite* m_pAtomSprite;
    const NE::Sprite* m_pConnectionSprite;
    const NE::Sprite* m_pExplosionSprite;

    UVec2 m_position;
    UVec2 m_refinedBoardPosition;
    
    int m_atomType;
    int m_connectionType;
    
    unsigned int m_lastMovementUpdateTime;
    UVec2 m_destinationPosition;
    bool m_isMoving;
    bool m_isExploding;
    bool m_exploded;
    
    unsigned int m_animationStep;
    unsigned int m_lastAnimationUpdateTime;
    
    void updateRefinedPosition();
    
public:
    Atom(const NE::Sprite* pAtomSprite, const NE::Sprite* pConnectionSprite, const NE::Sprite* pExplosionSprite, const UVec2& initialPosition, int atomType, int connectionType);
    
    bool draw(const NE::Renderer& renderer, const UVec2& boardOrigin)const;
    void update(unsigned int currentTime);
    
    void explode();
    bool isExploding()const { return m_isExploding; }
    bool hasExploded()const { return m_exploded; }
    
    const UVec2& getPosition()const { return m_position; }
    
    void moveTo(const UVec2& destination);
    
    bool match(int atomType, int connectionType)const
    {
        return atomType == m_atomType && connectionType == m_connectionType;
    }
};
typedef std::vector<Atom> AtomList;

#endif
