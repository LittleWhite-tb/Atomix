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

#ifndef CURSOR_HPP
#define CURSOR_HPP

#include "NEngine/Types/Vec2.h"

namespace NE
{
    class Sprite;
    class Renderer;
}

class Cursor
{
public:
    enum State
    {
        MOVE,
        SELECTION,
        DISABLED
    };
    
private:
    const NE::Sprite* m_pSprite;

    UVec2 m_position;
    UVec2 m_refinedBoardPosition;
    
    unsigned int m_lastAnimationUpdateTime;
    unsigned int m_animationStep;
    
    unsigned int m_lastMovementUpdateTime;
    UVec2 m_destinationPosition;
    bool m_isMoving;
    bool m_endMovement;
    
    State m_state;
    
    void updateRefinedPosition();
    
public:

    Cursor(const NE::Sprite* pSprite);
    
    const UVec2& getPosition()const { return m_position; }
    State getState()const { return m_state; }
    
    void select() { m_state = SELECTION; }
    void release() { m_state = MOVE; }
    void disable() { m_state = DISABLED; }
        
    bool draw(const NE::Renderer& renderer, const UVec2& boardOrigin);
    void update(unsigned int currentTime);
    
    void moveTo(const UVec2& destination);
    
    void toogle();
    
    void reset(const UVec2& newPosition);
    
    bool isMoving()const { return m_isMoving; }
    bool atEndMovement()const { return m_endMovement; }
    void resetMovement() { m_endMovement = false; }
};

#endif
