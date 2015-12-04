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

#include "NEngine/NEngineFactory.h"
#include "NEngine/NEngine.h"
#include "NEngine/Window.h"
#include "NEngine/Types/Vec2.h"

#include "Game/Game.hpp"

int main(int argc, char** argv)
{
    // Starting NEngine
    NE::NEngine* pNE = NE::NEngineFactory::createNEngine(NE::SDL_ENGINE);
    if ( pNE->init() == false )
    {
        return 1;
    }
    
    {
        if ( pNE->getWindow()->createWindow(USize2(640,480),32,false,"Atomix") )
        {
            Game g(pNE);
            if ( g.init() )
            {
                if ( g.load() )
                {
                    g.play();
                }
            }

            pNE->getWindow()->destroyWindow();
        }
    }
    
    // Stopping NEngine
    pNE->stop();
    delete pNE;
    
    NE::NEngine::setLogger(NULL);
    
    return 0;
}
