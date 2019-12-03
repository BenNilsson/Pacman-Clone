#pragma once

enum class State
{
	START = 0,
	INTRO,
	PLAYING,
	PAUSED,
	GAMEOVER,
};

class GameState 
{
public:
	
	static State GetState()
	{
		return state;
	}

	static void SetState(State _state) 
	{
		state = _state;
	}

private:

	static State state;
	GameState();
};