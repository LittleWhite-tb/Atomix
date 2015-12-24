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


#include "Game.hpp"

#include "NEngine/NEngine.h"
#include "NEngine/Window.h"
#include "NEngine/Renderer.h"
#include "NEngine/Time.h"
#include "NEngine/SpriteLoader.h"
#include "NEngine/InputManager.h"
#include "NEngine/SoundEngine.h"
#include "NEngine/Exceptions/FileNotFoundException.h"
#include "NEngine/Types/Colour.h"

#include "Level.hpp"
#include "BoardTileSetter.hpp"

#include "common.hpp"

#include <cassert>

Game::Game(NE::NEngine* pEngine)
    :m_pEngine(pEngine),
     m_fpsNumber(0),m_fpsCounter(0),m_fpsLastUpdateTime(0),m_lastUpdateTime(0),m_isRunning(true),
     m_resLoader(pEngine->getSpriteLoader()),
     m_pLevel(NULL),pCursor(NULL),m_pSelectedAtom(NULL),
     m_lastTimeDecreaseUpdateTime(0),m_lastArrowsUpdateTime(0),m_lastButtonTime(0),m_state(GS_TITLE)
{
    assert(pEngine);

    m_levelList.push_back("./data/levels/l1.txt");
    m_levelList.push_back("./data/levels/l2.txt");
    m_levelList.push_back("./data/levels/l3.txt");
    m_levelList.push_back("./data/levels/l4.txt");
    m_levelList.push_back("./data/levels/l5.txt");
    m_levelList.push_back("./data/levels/l6.txt");
    m_levelList.push_back("./data/levels/l7.txt");
    m_levelList.push_back("./data/levels/l8.txt");
    m_levelList.push_back("./data/levels/l9.txt");
    m_levelList.push_back("./data/levels/l10.txt");
    m_levelList.push_back("./data/levels/l11.txt");
    m_levelList.push_back("./data/levels/l12.txt");
    m_levelList.push_back("./data/levels/l13.txt");
    
    m_gameInfo.playerName = "PLAYER 1";
    m_gameInfo.score = 0;
    m_gameInfo.levelNumber = 0;
}

Game::~Game()
{
    /*
    for ( std::vector<Level*>::const_iterator itLevel = m_levels.begin() ; itLevel != m_levels.end() ; ++itLevel )
    {
        delete *itLevel;
    }
    */
    delete pCursor;
}

bool Game::init()
{
    // Magenta pixels as transparancy (as often in old games)
    m_pEngine->getSpriteLoader()->setTransparencyColour(Colour(255,0,255));
    
    return true;
}

bool Game::load()
{
    try
    {
        // Loading generic sprites
        m_resLoader.getSprite(SCHEME_FRAME_SPRITE_PATH);
        
        m_resLoader.getSprite(ATOMS_SPRITE_PATH);
        m_resLoader.getSprite(CONNECTIONS_SPRITE_PATH);
        
        m_resLoader.getSprite(CURSOR_SPRITE_PATH);
        m_resLoader.getSprite(EXPLOSION_SPRITE_PATH);
        
        m_resLoader.getSprite(SCHEME_ATOMS_SPRITE_PATH);
        m_resLoader.getSprite(SCHEME_CONNECTIONS_SPRITE_PATH);
        
        m_resLoader.getSprite(TITLE_SPRITE_PATH);
        
        // Fonts
        m_resLoader.loadFont(FONT_SPRITE_PATH,USize2(16,16),' ');
        m_resLoader.loadFont(HUGE_FONT_SPRITE_PATH,USize2(32,32),' ');
        m_resLoader.loadFont(GREEN_FONT_SPRITE_PATH,USize2(16,16),' ');
    }
    catch (std::exception& e)
    {
        NEError << e.what() + std::string("\n");
        return false;
    }
    
    // Load musics
    bool bResult = true;
    bool bMusicResult = true;
    
    try
    {
        m_pEngine->getSoundEngine()->load(MUSIC_PATH);
    }
    catch (FileNotFoundException e)
    {
        NEError << e.what() << "\n";
        bMusicResult = false;
    }
    
    bResult &= m_pEngine->getSoundEngine()->load(MOVE_SOUND_PATH);
    bResult &= m_pEngine->getSoundEngine()->load(COLLISION_SOUND_PATH);
    if ( bResult == false )
    {
        return false;
    }
    
    m_pInterface = new Interface(m_resLoader.getFont(FONT_SPRITE_PATH), 
                                 m_resLoader.getFont(HUGE_FONT_SPRITE_PATH),
                                 m_resLoader.getFont(GREEN_FONT_SPRITE_PATH),
                                 m_resLoader.getSprite(SCHEME_FRAME_SPRITE_PATH));
    pCursor = new Cursor(m_resLoader.getSprite(CURSOR_SPRITE_PATH));
    
    for ( std::vector<std::string>::const_iterator itLevelName = m_levelList.begin() ; itLevelName != m_levelList.end() ; ++itLevelName )
    {
        try
        {
            Level* newLevel = new Level(&m_resLoader,*itLevelName);
            if ( newLevel != NULL )
            {
                //m_levels.push_back(newLevel);
            }
        }
        catch (std::exception& e)
        {
            NEError << e.what() << "\n";
        }
    }
    /*
    if ( m_levels.size() == 0 )
    {
        return false;
    }*/
    
    
    if ( bMusicResult )
    {
        m_pEngine->getSoundEngine()->play(MUSIC_PATH,true);
    }
    
    return true;
}

void Game::play()
{
    bool bResult = true;

    // Main loop
    while ( m_isRunning && // If the game is still active
            m_pEngine->getInputManager()->needEscape() == false &&  // If no button to quit have been used
            m_pEngine->getWindow()->needWindowClosure() == false &&  // or window cross
            bResult == true ) // or a problem occured
    {
        // Display game
        bResult &= this->render();
        
        // This solve a problem in Windows, because the events have to be updated in the same thread than the video.
        unsigned int actualTime = m_pEngine->getTime()->getTime();
        //if ( actualTime - m_lastUpdateTime > 25 ) // All 25ms
        {
            bResult &= this->update();
            m_lastUpdateTime = m_pEngine->getTime()->getTime();
        }

        // FPS management
        m_fpsCounter++;
        if ( actualTime - m_fpsLastUpdateTime > 1000 ) // After one second
        {
            if ( m_pSelectedAtom == NULL || !m_pSelectedAtom->isExploding() )
            {
                m_gameInfo.remainingTime--;
            }
            
            m_fpsNumber= m_fpsCounter;
            m_fpsCounter = 0;
            m_fpsLastUpdateTime = m_pEngine->getTime()->getTime();

            NELog(0) << "FPS: " << m_fpsNumber << "\n";
        }
    }
}

bool Game::render()
{
    m_pEngine->getRenderer()->clearScreen(Colour(0,0,0,255));
    
    if ( m_state == Game::GS_TITLE )
    {
        m_pEngine->getRenderer()->drawSurface(IVec2(0,0),m_resLoader.getSprite(TITLE_SPRITE_PATH));
    }
    else
    {
        UVec2 gameBoardOrigin(BORDER_WIDTH+30,32);
        UVec2 schemeBoardOrigin(10,480-m_resLoader.getSprite(SCHEME_FRAME_SPRITE_PATH)->getSize().height);
        
        m_pLevel->drawBackground(*m_pEngine->getRenderer());
        m_pInterface->draw(*m_pEngine->getRenderer(),UVec2(0,0),m_gameInfo);
        m_pLevel->draw(*m_pEngine->getRenderer(),gameBoardOrigin,schemeBoardOrigin);
        if ( m_pSelectedAtom )
        {
            if ( m_pSelectedAtom->isExploding() )
            {
                m_pSelectedAtom->draw(*m_pEngine->getRenderer(),gameBoardOrigin);
            }
        }
        

        pCursor->draw(*m_pEngine->getRenderer(),gameBoardOrigin);
    }
    
    // Render something here :D
    if ( m_pEngine->getRenderer()->updateWindow() == false )
    {
        NEError << "Fail to draw on the screen\n";
    }

    return true;
}

bool Game::update()
{
    m_pEngine->getInputManager()->update();

    unsigned int currentTime = m_pEngine->getTime()->getTime();
    NE::InputManager::ArrowsDirection directions =m_pEngine->getInputManager()->getDirectionsPressed(0);
    
    if ( m_state == Game::GS_TITLE )
    {
        if ( m_pEngine->getInputManager()->getButtonState(0,NE::InputManager::INPUT_A) )
        {
            m_state = Game::GS_GAME;
            resetGame();
        }
    }
    else if ( m_state == Game::GS_GAME )
    {
    
        pCursor->update(currentTime);
        if ( m_pSelectedAtom != NULL )
        {
            m_pSelectedAtom->update(currentTime);
        }
        
        if ( m_pLevel->isEnded() )
        {
            pCursor->disable();
            // Do the explosion sequence
            const AtomList& atoms = m_pLevel->getAtoms();
            if ( m_pSelectedAtom == NULL || !m_pSelectedAtom->isExploding() )
            {
                bool found = false;
                for ( unsigned int i = 0 ; i < atoms.size() && !found ; i++ )
                {
                    if ( !atoms[i].isExploding() && !atoms[i].hasExploded() )
                    {
                        m_pSelectedAtom = m_pLevel->getAtom(atoms[i].getPosition());
                        m_pSelectedAtom->explode();
                        
                        m_gameInfo.score+=500;
                        
                        found = true;
                    }
                }
                
                if ( found == false ) // Everything exploded
                {
                    if ( m_gameInfo.remainingTime != 0 ) // Decrease time counter and increase score
                    {
                        if ( currentTime - m_lastTimeDecreaseUpdateTime > 25 )
                        {
                            m_gameInfo.score+=5;
                            m_gameInfo.remainingTime--;
                            
                            m_lastTimeDecreaseUpdateTime = currentTime;
                        }
                    }
                    else
                    {
                        if ( m_gameInfo.levelNumber == m_levelList.size() )
                        {
                            NELog(0) << "You finished the game with a score of : " << m_gameInfo.score << "\n";
                            m_state = Game::GS_TITLE;
                            return true;
                        }
                    
                        this->loadNextLevel();
                    }
                }
            }
            
            return true;
        }
        else if ( m_gameInfo.remainingTime == 0 )
        {
            m_state = Game::GS_TITLE;
            return true;
        }
        
        // Handle movement
        if ( !pCursor->isMoving() )
        {
            if ( currentTime - m_lastArrowsUpdateTime > 100 )
            {
                UVec2 cursorPosition = pCursor->getPosition();
                IVec2 direction(0,0);
                switch (directions) // Get direction info
                {
                    case NE::InputManager::AD_UP:
                        direction.y=-1;
                        break;
                    case NE::InputManager::AD_DOWN:
                        direction.y=1;
                        break;
                    case NE::InputManager::AD_LEFT:
                        direction.x=-1;
                        break;
                    case NE::InputManager::AD_RIGHT:
                        direction.x=1;
                        break;
                }
                
                if ( pCursor->getState() == Cursor::MOVE ) // Just moving cursor
                {
                    UVec2 destination = cursorPosition;
                    destination.x += direction.x;
                    destination.y += direction.y;

                    if ( destination.x < m_pLevel->getBoard()->getSize().width &&
                         destination.y < m_pLevel->getBoard()->getSize().height )
                    {
                        if ( m_pLevel->getBoard()->getTile(UVec2(cursorPosition.x+direction.x,cursorPosition.y+direction.y)).getConsistency() != Tile::TC_Null )
                        {
                            pCursor->moveTo(destination);
                        }
                    }
                }
                else
                {
                    UVec2 destination = cursorPosition;
                    UVec2 nextDestination = cursorPosition;
                    do
                    {
                        if ( m_pLevel->checkAtom(nextDestination) == true )
                        {
                            if ( !(nextDestination == cursorPosition) )
                            {
                                break;
                            }
                            if ( direction == IVec2(0,0) )
                            {
                                break;
                            }
                        }
                        
                        destination = nextDestination;
                        
                        nextDestination.x += direction.x;
                        nextDestination.y += direction.y;
                    }while ( m_pLevel->getBoard()->getTile(nextDestination).getConsistency() == Tile::TC_Empty);
                    
                    if ( !(cursorPosition == destination) )
                    {
                        m_pEngine->getSoundEngine()->play(MOVE_SOUND_PATH,false);
                    }
                    
                    pCursor->moveTo(destination);
                    m_pSelectedAtom->moveTo(destination);
                }
                
                m_lastArrowsUpdateTime = currentTime;
            }
            
            // Handle button
            if ( m_pEngine->getInputManager()->getButtonState(0,NE::InputManager::INPUT_A) &&
                 currentTime - m_lastButtonTime > 500 )
            {
                if ( m_pLevel->checkAtom(pCursor->getPosition()) )
                {
                    pCursor->toogle();
                    if ( pCursor->getState() == Cursor::MOVE )
                    {
                        m_pSelectedAtom = NULL;
                    }
                    else
                    {
                        m_pSelectedAtom = m_pLevel->getAtom(pCursor->getPosition());
                    }
                    m_lastButtonTime = currentTime;
                }
            }
            
            /*
            // Handle sound of end movement
            if ( pCursor->atEndMovement() )
            {
                m_pEngine->getSoundEngine()->play(COLLISION_SOUND_PATH,false);
                pCursor->resetMovement();
            }*/
        }
    }

    return true;
}

void Game::loadNextLevel()
{
    m_pSelectedAtom = NULL;
    
    if ( m_pLevel != NULL )
    {
        delete m_pLevel;
    }
    
    m_pLevel = new Level(&m_resLoader,m_levelList[m_gameInfo.levelNumber]);
    m_gameInfo.remainingTime = m_pLevel->getAvailableTime();
    m_gameInfo.schemeName = m_pLevel->getScheme()->getName();
    
    m_gameInfo.levelNumber++;
    
    this->resetCursor();
}

void Game::resetCursor()
{
    // Find first not NULL tile
    bool stop = false;
    UVec2 cursorResetPosition;
    USize2 boardSize = m_pLevel->getBoard()->getSize();
    for ( unsigned int y = 0 ; y < boardSize.height && !stop ; y++ )
    {
        for (unsigned int x = 0 ; x < boardSize.width && !stop ; x++ )
        {
            if ( m_pLevel->getBoard()->getTile(UVec2(x,y)).getConsistency() != Tile::TC_Null )
            {
                cursorResetPosition.x = x;
                cursorResetPosition.y = y;
                
                stop = true;
            }
        }
    }
    pCursor->reset(cursorResetPosition);
}

void Game::resetGame()
{
    m_gameInfo.levelNumber = 0;
    m_gameInfo.score = 0;
    
    this->loadNextLevel();
}
