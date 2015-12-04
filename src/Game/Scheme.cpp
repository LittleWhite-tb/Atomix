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

#include "Scheme.hpp"

#include "NEngine/Renderer.h"
#include "NEngine/Sprite.h"
#include "NEngine/Types/Rect.h"

#include "Level.hpp"
#include "Atom.hpp"

#include "common.hpp"

Scheme::Scheme(const NE::Sprite* pSchemeSprite, const NE::Sprite* pSchemeConnectionSprite, const SchemeTable& scheme, const std::string& name)
    :m_name(name),m_scheme(scheme),m_pSchemeSprite(pSchemeSprite),m_pSchemeConnectionSprite(pSchemeConnectionSprite)
{
}
    
bool Scheme::draw(const NE::Renderer& renderer, const UVec2& boardOrigin)
{
    bool bResult = true;
    
    UVec2 schemeOrigin(boardOrigin.x + (8*SCHEME_SPRITE_WIDTH - SCHEME_SPRITE_WIDTH * m_scheme[0].size())/2,
                       boardOrigin.y + 32 + (8*SCHEME_SPRITE_HEIGHT - SCHEME_SPRITE_HEIGHT * m_scheme.size())/2); // 48 to avoid scheme title
    
    for ( unsigned int y = 0 ; y < m_scheme.size() ; y++ )
    {
        for ( unsigned int x = 0 ; x < m_scheme[y].size() ; x++ )
        {
            if ( m_scheme[y][x].second != 0 )
            {
                Rect atomSpriteSource(IVec2(SCHEME_SPRITE_WIDTH*m_scheme[y][x].first,0),
                                      USize2(SCHEME_SPRITE_WIDTH,SCHEME_SPRITE_HEIGHT));
                IVec2 screenPosition(schemeOrigin.x+x*SCHEME_SPRITE_WIDTH,schemeOrigin.y+y*SCHEME_SPRITE_HEIGHT);
                
                bResult &= renderer.drawSurface(screenPosition,m_pSchemeSprite,atomSpriteSource);
                
                for (unsigned int i = 0 ; i < 12 ; i++ )
                {
                    int connectionID = powl(2,i);
                    if ( (connectionID & m_scheme[y][x].second) == connectionID )
                    {
                        Rect connectionSpriteSource(IVec2(SCHEME_SPRITE_WIDTH*i,0),
                                                   USize2(SCHEME_SPRITE_WIDTH,SCHEME_SPRITE_HEIGHT));
                                                   
                        bResult &= renderer.drawSurface(screenPosition,m_pSchemeConnectionSprite,connectionSpriteSource);
                    }
                }
            }
        }
    }
    
    return bResult;
}

bool Scheme::checkVictory(const USize2& boardSize, const AtomList& atoms)
{
    for ( unsigned int boardY = 0 ; boardY < boardSize.height ; boardY++ )
    {
        for ( unsigned int boardX = 0 ; boardX < boardSize.width ; boardX++ )
        {
            bool validation = true;
            for ( unsigned int schemeY = 0 ; schemeY < m_scheme.size() && validation ; schemeY++ )
            {
                for ( unsigned int schemeX = 0 ; schemeX < m_scheme[schemeY].size() && validation ; schemeX++ )
                {
                    const Atom* pAtom = Level::getAtom(atoms,UVec2(boardX+schemeX,boardY+schemeY));
                    if ( boardX+schemeX >= boardSize.width-1 || boardY+schemeY >= boardSize.height-1 ) // Check board boundaries
                    {
                        validation=false;
                    }
                    if ( pAtom == NULL )
                    {
                        if ( m_scheme[schemeY][schemeX] != std::pair<int,int>(0,0) ) // Check for empty part in scheme
                        {
                            validation = false;
                        }
                    }
                    else
                    {
                        // Check for atom correspondency
                        if ( pAtom->match(m_scheme[schemeY][schemeX].first,m_scheme[schemeY][schemeX].second) == false )
                        {
                            validation = false;
                        }
                    }
                }
            }
            
            if ( validation == true ) // We validated all the Scheme
            {
                return true;
            }
        }
    }
    
    return false;
}
