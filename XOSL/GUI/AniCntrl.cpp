
#include <AniCntrl.h>


bool CAnimatedControl::Animate = true;
		
		
CAnimatedControl::CAnimatedControl(int Left, int Top, int Width, int Height, int Visible, int OnTop, void *HandlerClass):
	CControl(Left,Top,Width,Height,Visible,OnTop,true,HandlerClass)
{
	MouseIsOver = false;
}

CAnimatedControl::~CAnimatedControl()
{
}

int CAnimatedControl::MouseMove(int X, int Y)
{
	if (!Enabled) {
		return -1;
	}
	CControl::MouseMove(X,Y);
	
	if (MouseX >= Left && MouseX <= Right && MouseY >= Top && MouseY <= Bottom) {
		if (Animate && !MouseIsOver) {
			MouseIsOver = true;
			MouseOver();
		}
//		return 0;
		return -1;
	}
	else {
		if (MouseIsOver) {
			MouseIsOver = false;
			MouseOut();
		}
	}
	return -1;
}

void CAnimatedControl::MouseOver()
{
	Refresh();
}

void CAnimatedControl::MouseOut()
{
	Refresh();
}

