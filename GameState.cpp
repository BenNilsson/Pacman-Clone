#include "GameState.h"

void GameState::SetState(GameState::State _state)
{
	state = _state;
}

GameState::State GameState::GetState()
{
	return state;
}
