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

#ifndef GAME_HPP
#define GAME_HPP

#include "ResourceBank.hpp"
#include "Cursor.hpp"
#include "Interface.hpp"
#include "GameInfo.hpp"

#include <vector>

namespace NE
{
    class NEngine;
    class Sprite;
}

class Level;
class Atom;

class Game
{
    enum State
    {
        GS_GAME,
        GS_TITLE
    };
    
    
private:
    NE::NEngine* m_pEngine; /*!< Pointer to the NEngine to use */
    
    // Timing variables
    unsigned m_fpsNumber;           /*!< the number of fps (update each second)*/
    unsigned m_fpsCounter;          /*!< the frame counter */
    unsigned m_fpsLastUpdateTime;   /*!< the last update time of the fps number */
    
    unsigned m_lastUpdateTime;      /*!< the last update time of the loop */
    
    bool m_isRunning;
    
    ResourceBank m_resLoader;
    
    std::vector<std::string> m_levelList;

    Interface* m_pInterface;
    Level* m_pLevel;
    Cursor* pCursor;
    Atom* m_pSelectedAtom;
    
    GameInfo m_gameInfo;
    
    unsigned int m_lastTimeDecreaseUpdateTime;
    unsigned int m_lastArrowsUpdateTime;
    unsigned int m_lastButtonTime;
    
    State m_state;
    
    bool render();
    bool update();
    
    void resetGame();
    void loadNextLevel();
    void resetCursor();
    
public:
    Game(NE::NEngine* pEngine);
    ~Game();
    
    /**
     * Some initialisation steps are done here 
     * This function is used mainly to set Game global settings
     */
    bool init();
    
    bool load();
    
    /**
     * Starts the main loop
     * The main loop should render, get the newest event and update the game
     */
    void play();
};

#endif
