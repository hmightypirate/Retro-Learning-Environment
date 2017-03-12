#include "../RomUtils.hpp"
#include <iomanip>

#include "../RomUtils.hpp"
#include "SuperMarioKart.hpp"

#include "RleSystem.hxx"
#include "RleException.h"

using namespace rle;

SuperMarioKartSettings::SuperMarioKartSettings() {
    reset();

    // TODO
    minimalActions = {  JOYPAD_NOOP,
                        JOYPAD_LEFT,
                        JOYPAD_RIGHT,
                        JOYPAD_R,
                        JOYPAD_A,
                        JOYPAD_B,
                        JOYPAD_Y,
                        JOYPAD_X,
                        JOYPAD_L,
                        JOYPAD_LEFT | JOYPAD_A,
                        JOYPAD_RIGHT | JOYPAD_A,
                        JOYPAD_LEFT | JOYPAD_X,
                        JOYPAD_RIGHT | JOYPAD_X,
                        JOYPAD_RIGHT | JOYPAD_Y,
                        JOYPAD_LEFT | JOYPAD_Y,
                        JOYPAD_RIGHT | JOYPAD_R,
                        JOYPAD_LEFT | JOYPAD_R,
                        JOYPAD_RIGHT | JOYPAD_B,
                        JOYPAD_LEFT | JOYPAD_B,
                        JOYPAD_LEFT | JOYPAD_B | JOYPAD_L,
                        JOYPAD_RIGHT | JOYPAD_B | JOYPAD_L
    };
}

/* create a new instance of the rom */
RomSettings* SuperMarioKartSettings::clone() const {
    RomSettings* rval = new SuperMarioKartSettings();
    *rval = *this;
    return rval;
}

/* process the latest information from RLE */
void SuperMarioKartSettings::step(const RleSystem& system) {

  // update the player posiiton
  reward_t playerPosition = getDecimalScore(0x1ef8, &system);

  //Selecting reward score strategy as the diff with the previous position
  reward_t playerScore = playerPosition;

  //Reward is positive if the player wins one position 
  m_reward = m_reward - playerPosition;
  
  m_score = playerScore;

  current_lives = getDecimalScore(0x0154, &system);

  if (current_lives < m_lives)
    {
      //Reached a terminal state
      m_terminal = true
    }
}

/* reset the state of the game */
void StreetFighterIITurboHyperFightingSettings::reset() {
    m_reward   = 0;
    m_score    = 0;
    m_terminal = false;
    m_wins    = 0;
    o_wins    = 0;
    m_health  = 0;
    o_health  = 0;
    match_ended = false;
    // Avoid resetting m_speed and m_difficulty as the remain across resets.
}

/* saves the state of the rom settings */
void StreetFighterIITurboHyperFightingSettings::saveState( Serializer & ser ) {
    ser.putInt(m_reward);
    ser.putBool(m_terminal);
    ser.putInt(m_lives);
}

// loads the state of the rom settings
void StreetFighterIITurboHyperFightingSettings::loadState( Deserializer & des ) {
    m_reward = des.getInt();
    m_terminal = des.getBool();
    m_lives = des.getInt();
}

ActionVect StreetFighterIITurboHyperFightingSettings::getStartingActions(const RleSystem& system){
    int i, num_of_nops(100);
    ActionVect startingActions;
    // wait for intro to end
    for(i = 0; i<num_of_nops; i++){
        startingActions.push_back(JOYPAD_NOOP);
    }

    // second animation
    startingActions.push_back(JOYPAD_B);

    // Selecting 1 player mode
    startingActions.push_back(JOYPAD_B);

    // Selecting race mode
    startingActions.push_back(JOYPAD_B);

    // Selecting velocity class FIXME: add selection capability here
    startingActions.push_back(JOYPAD_B);

    // Validating options
    startingActions.push_back(JOYPAD_B);

    //Selecting driver
    startingActions.push_back(JOYPAD_B);

    //Validate
    startingActions.push_back(JOYPAD_B);

    for(i = 0; i<5*num_of_nops; i++){
        startingActions.push_back(JOYPAD_NOOP);
    }

    // Selecting circuit
    startingActions.push_back(JOYPAD_B);
    
    return startingActions;
}

