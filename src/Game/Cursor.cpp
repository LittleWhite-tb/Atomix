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


#include "Cursor.hpp"

#include "NEngine/Renderer.h"
#include "NEngine/Types/Rect.h"
#include "common.hpp"

#include <cassert>

Cursor::Cursor(const NE::Sprite* pSprite)
    :m_pSprite(pSprite),m_position(0,0),
     m_lastAnimationUpdateTime(0),m_animationStep(0),m_lastMovementUpdateTime(0),m_isMoving(false),m_endMovement(false),m_state(Cursor::MOVE)
{
    assert(pSprite);
    
    updateRefinedPosition();
}

void Cursor::updateRefinedPosition()
{
    m_refinedBoardPosition.x = m_position.x * SPRITE_WIDTH;
    m_refinedBoardPosition.y = m_position.y * SPRITE_HEIGHT;
}

void Cursor::toogle()
{
    if ( m_state == Cursor::SELECTION )
    {
        m_state = Cursor::MOVE;
    }
    else
    {
        m_state = Cursor::SELECTION;
    }
}

bool Cursor::draw(const NE::Renderer& renderer, const UVec2& boardOrigin)
{
    if ( m_state == Cursor::DISABLED )
    {
        return true;
    }
    
    IVec2 screenPosition(boardOrigin.x+m_refinedBoardPosition.x,
                         boardOrigin.y+m_refinedBoardPosition.y);
    
    Rect spriteSource(IVec2(0,0),
                      USize2(SPRITE_WIDTH,SPRITE_HEIGHT));
    
    if ( m_state == Cursor::SELECTION )
    {
        spriteSource.position.x = (1 + m_animationStep) * SPRITE_WIDTH;
    }
    
    return renderer.drawSurface(screenPosition,m_pSprite,spriteSource);
}

void Cursor::update(unsigned int currentTime)
{
    if ( m_state == Cursor::DISABLED )
    {
        return;
    }
    
    if ( currentTime - m_lastAnimationUpdateTime > 50 )
    {
        m_animationStep++;
        m_animationStep = m_animationStep % 3;
        
        m_lastAnimationUpdateTime = currentTime;
    }
    
    if ( currentTime - m_lastMovementUpdateTime > 10 && m_isMoving )
    {
        int toGoX = m_destinationPosition.x - m_refinedBoardPosition.x;
        int toGoY = m_destinationPosition.y - m_refinedBoardPosition.y;
        int move = 0;
        
        if ( toGoX != 0 )
        {
            if ( toGoX > 0 )
            {
                move = std::min(MOVE_STEP,toGoX);
            }
            else
            {
                move = std::min(MOVE_STEP,-toGoX);
                move = -move;
                
            }
            
            m_refinedBoardPosition.x += move;
        }
        else if ( toGoY != 0 )
        {
            if ( toGoY > 0 )
            {
                move = std::min(MOVE_STEP,toGoY);
            }
            else
            {
                move = std::min(MOVE_STEP,-toGoY);
                move = -move;
            }
            
            m_refinedBoardPosition.y += move;
        }
        else
        {
            m_position.x = m_refinedBoardPosition.x/SPRITE_WIDTH;
            m_position.y = m_refinedBoardPosition.y/SPRITE_HEIGHT;
            m_isMoving = false;
            m_endMovement = true;
        }
        
        m_lastMovementUpdateTime = currentTime;
    }
}

void Cursor::moveTo(const UVec2& destination)
{
    m_destinationPosition = UVec2(destination.x*SPRITE_WIDTH,destination.y*SPRITE_HEIGHT);
    m_isMoving = true;
}

void Cursor::reset(const UVec2& newPosition)
{
    m_state = Cursor::MOVE;
    m_isMoving = false;
    m_position = newPosition;
    updateRefinedPosition();
}
