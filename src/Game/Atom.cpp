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

#include "Atom.hpp"

#include "NEngine/NEngine.h"
#include "NEngine/Renderer.h"
#include "NEngine/Sprite.h"
#include "NEngine/Types/Rect.h"

#include "common.hpp"

Atom::Atom(const NE::Sprite* pAtomSprite, const NE::Sprite* pConnectionSprite, const NE::Sprite* pExplosionSprite, const UVec2& initialPosition, int atomType, int connectionType)
    :m_pAtomSprite(pAtomSprite),m_pConnectionSprite(pConnectionSprite),m_pExplosionSprite(pExplosionSprite),
     m_position(initialPosition),m_atomType(atomType),m_connectionType(connectionType),m_lastMovementUpdateTime(0),m_isMoving(false),m_isExploding(false),m_exploded(false),
     m_animationStep(0),m_lastAnimationUpdateTime(0)
{
    updateRefinedPosition();
}

void Atom::updateRefinedPosition()
{
    m_refinedBoardPosition.x = m_position.x * SPRITE_WIDTH;
    m_refinedBoardPosition.y = m_position.y * SPRITE_HEIGHT;
}

bool Atom::draw(const NE::Renderer& renderer, const UVec2& boardOrigin)const
{
    bool bResult = true;
    
    if ( m_exploded )
    {
        return true;
    }
    
    IVec2 screenPosition(boardOrigin.x+m_refinedBoardPosition.x,boardOrigin.y+m_refinedBoardPosition.y);
    
    if ( m_isExploding )
    {
        Rect atomSpriteSource(IVec2(SPRITE_WIDTH*m_animationStep,0),
                            USize2(SPRITE_WIDTH,SPRITE_HEIGHT));
                            
        return renderer.drawSurface(screenPosition,m_pExplosionSprite,atomSpriteSource);
    }
    
    Rect atomSpriteSource(IVec2(SPRITE_WIDTH*m_atomType,0),
                          USize2(SPRITE_WIDTH,SPRITE_HEIGHT));
    
    
    bResult &= renderer.drawSurface(screenPosition,m_pAtomSprite,atomSpriteSource);
    
    for (unsigned int i = 0 ; i < 12 ; i++ )
    {
        int connectionID = powl(2,i);
        if ( (connectionID & m_connectionType) == connectionID )
        {
            Rect connectionSpriteSource(IVec2(SPRITE_WIDTH*i,0),
                                       USize2(SPRITE_WIDTH,SPRITE_HEIGHT));
                                       
            bResult &= renderer.drawSurface(screenPosition,m_pConnectionSprite,connectionSpriteSource);
        }
    }
    
    return bResult;
}

void Atom::update(unsigned int currentTime)
{
    if ( m_exploded )
    {
        return;
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
        }
        
        m_lastMovementUpdateTime = currentTime;
    }
    
    if ( currentTime - m_lastAnimationUpdateTime > 25 && m_isExploding )
    {
        if ( m_animationStep == EXPLOSION_STEP )
        {
            m_isExploding=false;
            m_exploded = true;
        }
        m_animationStep++;
        
        m_lastAnimationUpdateTime = currentTime;
    }
}

void Atom::moveTo(const UVec2& destination)
{
    m_destinationPosition = UVec2(destination.x*SPRITE_WIDTH,destination.y*SPRITE_HEIGHT);
    m_isMoving=true;
}

void Atom::explode()
{
    m_isExploding = true;
    m_isMoving = false;
}
