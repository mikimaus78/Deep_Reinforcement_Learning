/* *****************************************************************************
 * A.L.E (Arcade Learning Environment)
 * Copyright (c) 2009-2013 by Yavar Naddaf, Joel Veness, Marc G. Bellemare and 
 *   the Reinforcement Learning and Artificial Intelligence Laboratory
 * Released under the GNU General Public License; see License.txt for details. 
 *
 * Based on: Stella  --  "An Atari 2600 VCS Emulator"
 * Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
 *
 * *****************************************************************************
 */
#include "Pitfall.hpp"

#include "../RomUtils.hpp"


PitfallSettings::PitfallSettings() {

    reset();
}


/* create a new instance of the rom */
RomSettings* PitfallSettings::clone() const { 
    
    RomSettings* rval = new PitfallSettings();
    *rval = *this;
    return rval;
}


/* process the latest information from ALE */
void PitfallSettings::step(const System& system) {

    // update the reward
    int score = getDecimalScore(0xD7, 0xD6, 0xD5, &system);
    int reward = score - m_score;
    m_reward = reward;
    m_score = score;

    // update terminal status
    int lives = readRam(&system, 0x80) >> 4;
    // The value at 09xE will be nonzero if we cannot control the player
    int logo_timer = readRam(&system, 0x9E);
    m_terminal = lives == 0 && logo_timer != 0;
}


/* is end of game */
bool PitfallSettings::isTerminal() const {

    return m_terminal;
};


/* get the most recently observed reward */
reward_t PitfallSettings::getReward() const { 

    return m_reward; 
}


/* is an action part of the minimal set? */
bool PitfallSettings::isMinimal(const Action &a) const {

    switch (a) {

        case PLAYER_A_NOOP:
        case PLAYER_A_FIRE:
        case PLAYER_A_UP:
        case PLAYER_A_RIGHT:
        case PLAYER_A_LEFT:
        case PLAYER_A_DOWN:
        case PLAYER_A_UPRIGHT:
        case PLAYER_A_UPLEFT:
        case PLAYER_A_DOWNRIGHT:
        case PLAYER_A_DOWNLEFT:
        case PLAYER_A_UPFIRE:
        case PLAYER_A_RIGHTFIRE:
        case PLAYER_A_LEFTFIRE:
        case PLAYER_A_DOWNFIRE:
        case PLAYER_A_UPRIGHTFIRE:
        case PLAYER_A_UPLEFTFIRE:
        case PLAYER_A_DOWNRIGHTFIRE:
        case PLAYER_A_DOWNLEFTFIRE:
            return true;
        default:
            return false;
    }   
}


/* reset the state of the game */
void PitfallSettings::reset() {
    
    m_reward   = 0;
    m_score    = 2000;
    m_terminal = false;
}
        
/* saves the state of the rom settings */
void PitfallSettings::saveState(Serializer & ser) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putBool(m_terminal);
}

// loads the state of the rom settings
void PitfallSettings::loadState(Deserializer & ser) {
  m_reward = ser.getInt();
  m_score = ser.getInt();
  m_terminal = ser.getBool();
}

ActionVect PitfallSettings::getStartingActions() {
    ActionVect startingActions;
    startingActions.push_back(PLAYER_A_UP);
    return startingActions;
}