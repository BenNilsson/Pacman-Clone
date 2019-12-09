#include "UI.h"

UI_String::UI_String(S2D::Vector2 _position, string _text)
{
	Text = _text;
	stream.str(_text);
	Position = _position;
}

UI_Texture2D::UI_Texture2D(S2D::Vector2 _position, S2D::Texture2D _texture)
{
	Texture = &_texture;
	Position = _position;
}
