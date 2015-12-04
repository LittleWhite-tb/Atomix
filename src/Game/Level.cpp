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

#include "Level.hpp"

#include "NEngine/Renderer.h"
#include "NEngine/Sprite.h"

#include "Level/LevelLoader.hpp"
#include "BoardTileSetter.hpp"

#include "common.hpp"

#include <cassert>

Level::Level(ResourceBank* pResourceBank, const std::string& name)
{
    LevelLoader levelLoader(name);
    
    // Background
    m_pBackGroundSprite = pResourceBank->getSprite(BACKGROUND_SPRITE_PATH + levelLoader.getBackgroundName() + ".bmp");
    
    USize2 levelSize = levelLoader.getLevelSize();
    m_pBoard = new Board(pResourceBank->getSprite(TILES_SPRITE_PATH + levelLoader.getTileThemeName() + ".bmp"),levelSize);
    
    const std::vector< std::vector< int> >& tilesID = levelLoader.getTilesID();
    for ( unsigned int y = 0 ; y < levelSize.height ; y++ )
    {
        for ( unsigned int x = 0 ; x < levelSize.width ; x++ )
        {
            BoardTileSetter::setTile(*m_pBoard,UVec2(x,y),tilesID[y][x]);
        }
    }
    
    const std::vector<AtomData>& atomData = levelLoader.getAtoms();
    
    for ( std::vector<AtomData>::const_iterator itAD = atomData.begin() ; itAD != atomData.end() ; ++itAD )
    {
        const Tile& t = m_pBoard->getTile(itAD->position);
        if ( t.getConsistency() != Tile::TC_Empty)
        {
            NEWarning << "Atom not on empty tile (at " << itAD->position << ")\n";
            assert(t.getConsistency() == Tile::TC_Empty); // Check if an atom is not on empty Cell
        }
        
        m_atoms.push_back(Atom(pResourceBank->getSprite(ATOMS_SPRITE_PATH),
                               pResourceBank->getSprite(CONNECTIONS_SPRITE_PATH),
                               pResourceBank->getSprite(EXPLOSION_SPRITE_PATH),
                               itAD->position,itAD->type,itAD->connection));
    }
    
    m_pScheme = new Scheme(pResourceBank->getSprite(SCHEME_ATOMS_SPRITE_PATH),pResourceBank->getSprite(SCHEME_CONNECTIONS_SPRITE_PATH),levelLoader.getSchemeID(),levelLoader.getSchemeName());
    
    m_availableTime = levelLoader.getAvailableTime();
}

Level::~Level()
{
    delete m_pScheme;
    delete m_pBoard;
}

bool Level::drawBackground(const NE::Renderer& renderer)
{
    return renderer.drawSurface(IVec2(0,0),m_pBackGroundSprite);
}

bool Level::draw(const NE::Renderer& renderer, const UVec2& gameBoardOrigin, const UVec2& schemeBoardOrigin)
{
    bool bResult = true;
    
    m_pBoard->draw(renderer,gameBoardOrigin);
    for ( std::vector<Atom>::const_iterator itAtom = m_atoms.begin() ; itAtom != m_atoms.end() ; ++itAtom )
    {
        itAtom->draw(renderer,gameBoardOrigin);
    }
    m_pScheme->draw(renderer,schemeBoardOrigin);
    
    return bResult;
}

bool Level::isEnded()const
{
    return m_pScheme->checkVictory(m_pBoard->getSize(),m_atoms);
}

bool Level::checkAtom(const AtomList& atoms, const UVec2& position)
{
    return getAtom(atoms,position) != NULL;
}

bool Level::checkAtom(const UVec2& position)
{
    return getAtom(m_atoms,position) != NULL;
}

const Atom* Level::getAtom(const AtomList& atoms, const UVec2& position)
{
    for ( unsigned int i = 0 ; i < atoms.size() ; i++)
    {
        if ( atoms[i].getPosition() == position )
        {
            return &atoms[i];
        }
    }
    
    return NULL;
}

Atom* Level::getAtom(AtomList& atoms, const UVec2& position)
{
    for ( unsigned int i = 0 ; i < atoms.size() ; i++)
    {
        if ( atoms[i].getPosition() == position )
        {
            return &atoms[i];
        }
    }
    
    return NULL;
}

Atom* Level::getAtom(const UVec2& position)
{
    return Level::getAtom(m_atoms,position);
}
