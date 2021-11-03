#include "Pipe.h"

void Pipe::Render()
{
	//RenderBoundingBox();
}

void Pipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - width / 2;
	t = y;
	r = l + width;
	b = t + height;

}
