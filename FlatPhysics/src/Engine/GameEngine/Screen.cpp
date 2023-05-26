#include "Screen.h"

void Screen::DimScreen(float alpha)
{
	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), ColorAlpha(BLACK, alpha));
}

void Button::SetButton(const char *str, float width, float height, int fontSize, float elevation, float x, float y) {
	_width = width;
	_height = height;
	_x = x;
	_y = y;

	// Core attributes
	_elevation = elevation;
	dynamic_elevation = elevation;
	original_y_pos = y;

	// top rectangle
	top_rect = { x, y, width, height };

	// bottom rectangle
	bottom_rect = { x, y, width, elevation };

	// text
	font = LoadFont("arial.ttf");
	this->fontSize = fontSize;
	text = str;

}
void Button::SetPosition(float x, float y) {
	_x = x;
	_y = y;
	original_y_pos = y;

	// top rectangle
	top_rect = { x, y, _width, _height };

	// bottom rectangle
	bottom_rect = { x, y, _width, _elevation };
}
void Button::draw() {
	// elevation Logic
	top_rect.y = original_y_pos - dynamic_elevation;


	bottom_rect.y = top_rect.y;
	bottom_rect.height = top_rect.height + dynamic_elevation;


	DrawRectangleRounded(bottom_rect, 12, 100, bottom_color);

	DrawRectangleRounded(top_rect, 12, 100, top_color);
	
	Vector2 StringSize = MeasureTextEx(font, text, fontSize, 3);
	DrawTextEx(font, text, { top_rect.x + top_rect.width / 2 - StringSize.x / 2, top_rect.y + top_rect.height / 2 - StringSize.y / 2 }, fontSize, 3, text_color);
}