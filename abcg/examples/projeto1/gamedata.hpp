#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class Input { DownR, UpR, Down, Up };
enum class State { Playing, Gol, Player2Win, Player1Win };
enum class BallState { Playing, BallStop };

struct GameData {
  State m_state{State::Playing};
  BallState m_ballState{BallState::BallStop};
  std::bitset<4> m_input;  // [fire, up, down, left, right]
};

#endif