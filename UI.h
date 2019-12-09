#pragma once
#include <sstream>
#include "S2D/S2D.h"


class UI_Texture2D
{
public:
	S2D::Texture2D* Texture;
	S2D::Vector2 Position;

	UI_Texture2D(S2D::Vector2 _position, S2D::Texture2D _texture);
};

class UI_String
{
public:
	string Text;
	S2D::Vector2 Position;

	UI_String(S2D::Vector2 _position, string _text);
private:
	std::stringstream stream;
};
