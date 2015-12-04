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

#include "Interface.hpp"

#include "NEngine/Renderer.h"
#include "NEngine/Sprite.h"
#include "Font.hpp"

#include "GameInfo.hpp"

Interface::Interface(Font* pFont, Font* pHugeFont, Font* pGreenFont, const NE::Sprite* pSchemeFrame)
    :m_pFont(pFont),m_pHugeFont(pHugeFont),m_pGreenFont(pGreenFont),
    m_pSchemeFrame(pSchemeFrame)
{
}
    
bool Interface::draw(const NE::Renderer& renderer, const UVec2& interfaceOrigin, const GameInfo& gameInfo)
{
    bool bResult = true;
    
    std::string scoreString = this->toString(gameInfo.score);
    
    bResult &= m_pFont->draw(renderer,gameInfo.playerName,IVec2(interfaceOrigin.x+8,interfaceOrigin.y+16));
    bResult &= m_pFont->draw(renderer,scoreString,IVec2(interfaceOrigin.x+(80-m_pFont->getStringSize(scoreString).width),interfaceOrigin.y+32));
    bResult &= m_pFont->draw(renderer,"LEVEL   " + this->toString(gameInfo.levelNumber),IVec2(interfaceOrigin.x+8,interfaceOrigin.y+100));
    bResult &= m_pFont->draw(renderer,"TIME",IVec2(interfaceOrigin.x+interfaceOrigin.x+((m_pSchemeFrame->getSize().width-16)-m_pFont->getStringSize("TIME").width)/2,interfaceOrigin.y+200));

    unsigned int nbMins = gameInfo.remainingTime/60;
    unsigned int nbSeconds = gameInfo.remainingTime%60;
    std::string timeString = this->toString(nbMins);
    timeString = timeString + " : ";
    if ( nbSeconds < 10 )
    {
        timeString = timeString + "0";
    }
    timeString = timeString + this->toString(nbSeconds);
    
    bResult &= m_pHugeFont->draw(renderer, timeString ,IVec2(interfaceOrigin.x+16,interfaceOrigin.y+232));
    
    bResult &= renderer.drawSurface(IVec2(interfaceOrigin.x,interfaceOrigin.y+480-m_pSchemeFrame->getSize().height),m_pSchemeFrame);
    
    bResult &= m_pGreenFont->draw(renderer,gameInfo.schemeName,IVec2(interfaceOrigin.x+((m_pSchemeFrame->getSize().width-16)-m_pGreenFont->getStringSize(gameInfo.schemeName).width)/2,interfaceOrigin.y+480-m_pSchemeFrame->getSize().height+20));
    
    return bResult;
}

