/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <trackbar.h>
#include <graph.h>
#include <key.h>

#define TRACKBAR_HEIGHT 15

CTrackBar::CTrackBar(int Min, int Max, int Value, int Left, int Top, int Width, int Visible, void *HandlerClass):
		CAnimatedControl(Left,Top,Width,TRACKBAR_HEIGHT,Visible,false,HandlerClass)
{
	this->Min = Min;
	this->Max = Max;
	this->Value = Value;
	SetSliderPos();
	ButtonDown = false;
	TrackBarChange = NULL;
}

CTrackBar::~CTrackBar()
{
}

void CTrackBar::SetMin(int Min)
{
	this->Min = Min;
	SetSliderPos();
	Refresh();
}

void CTrackBar::SetMax(int Max)
{
	this->Max = Max;
	SetSliderPos();
	Refresh();
}

void CTrackBar::SetValue(int Value)
{
	if (Value > Max)
		Value = Max;
	if (Value < Min)
		Value = Min;
	if (Value != this->Value) {
		this->Value = Value;
		SetSliderPos();
		Refresh();
		if (TrackBarChange && HandlerClass)
			TrackBarChange(HandlerClass,Value);
	}
}

int CTrackBar::GetValue()
{
	return Value;
}

void CTrackBar::Draw(long, long, long, long)
{
	Graph->HLine(4,4,Width - 9,18);
	Graph->VLine(4,5,4,18);
	if (Enabled && MouseIsOver) {
		Graph->HLine(5,5,Width - 11,17);
		Graph->PutPixel(5,6,17);
		Graph->PutPixel(5,7,17);

		Graph->HLine(6,8,Width - 10,20);
		Graph->VLine(Width - 6,5,3,20);

		Graph->Bar(6,6,Width - 12,2,21);

	}
	else {
		Graph->Bar(5,5,Width - 10,4,21);
	}

	Graph->HLine(4,9,Width - 8,21);
	Graph->VLine(Width - 5,4,5,21);
	if (GotFocus)
		Graph->Rectangle(0,0,Width,Height,18);
	DrawSlider();
}

void CTrackBar::DrawSlider()
{
	Graph->HLine(SliderPos,3,4,21);
	Graph->VLine(SliderPos,4,7,21);
	if (Enabled && MouseIsOver) {
		Graph->HLine(SliderPos,11,5,17);
		Graph->VLine(SliderPos + 4,3,8,17);
		Graph->VLine(SliderPos + 1,4,6,20);
		Graph->PutPixel(SliderPos + 2,4,20);
		Graph->VLine(SliderPos + 2,5,5,19);
		Graph->HLine(SliderPos + 1,10,3,18);
		Graph->VLine(SliderPos + 3,4,6,18);
	}
	else {
		Graph->HLine(SliderPos,11,5,18);
		Graph->VLine(SliderPos + 4,3,8,18);
		Graph->Bar(SliderPos + 1,4,3,7,19);
	}
}

void CTrackBar::SetSliderPos()
{
	SliderPos = 4 + ((long)(Value - Min) * (Width - 13)) / (Max - Min);
}

void CTrackBar::SetMetrics(int Width, int Height)
{
	CControl::SetMetrics(Width,Height);
	SetValue(Value);
}

int CTrackBar::MouseDown(int MouseX, int MouseY)
{
	int Status;

	Status = CControl::MouseDown(MouseX,MouseY);
	if (Status != -1) {
		ButtonDown = true;
		MouseMove(MouseX,MouseY);
	}
	return Status;
}

int CTrackBar::MouseMove(int X, int Y)
{
	int Status;

	Status = CAnimatedControl::MouseMove(X,Y);

	if (ButtonDown)
		SetValue(((long)(X - Left - 4) * (Max - Min)) / (Width - 13) + Min);

	return Status;
}

void CTrackBar::MouseUp()
{
	ButtonDown = false;
}

void CTrackBar::OnChange(TTrackBarChange TrackBarChange)
{
	this->TrackBarChange = TrackBarChange;
}

void CTrackBar::KeyPress(unsigned short Key)
{
	switch (Key) {
		case KEY_NONE:
			break;
		case KEY_LEFT:
		case KEY_LEFT_EXT:
			SetValue(Value - 1);
			break;
		case KEY_RIGHT:
		case KEY_RIGHT_EXT:
			SetValue(Value + 1);
			break;
		case KEY_HOME:
		case KEY_HOME_EXT:
			SetValue(Min);
			break;
		case KEY_END:
		case KEY_END_EXT:
			SetValue(Max);
			break;
		case KEY_PAGEUP:
		case KEY_PU_EXT:
		case KEY_CTRLLEFT:
		case KEY_CTRLL_EXT:
			SetValue(Value - 8);
			break;
		case KEY_PAGEDOWN:
		case KEY_PD_EXT:
		case KEY_CTRLRIGHT:
		case KEY_CTRLR_EXT:
			SetValue(Value + 8);
			break;
		default:
			break;
	}
}
