/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <image.h>
#include <graph.h>

CImage::CImage(const void *Image, int ImageWidth, int ImageHeight, int Border,
		 int Left, int Top, int Width, int Height, int Visible):
		 CControl(Left,Top,Width,Height,Visible,false,false,NULL)
{
	this->Image = Image;
	this->Border = Border;
	this->ImageWidth = ImageWidth;
	this->ImageHeight = ImageHeight;
	if (!Image)
		Color = 19;
}

CImage::~CImage()
{
}

void CImage::SetImage(const void *Image, int ImageWidth, int ImageHeight)
{
	this->Image = Image;
	this->ImageWidth = ImageWidth;
	this->ImageHeight = ImageHeight;
	Refresh();
}

void CImage::SetBorder(int Border)
{
	if (Border != this->Border) {
		this->Border = Border;
		Refresh();
	}
}

void CImage::SetColor(int Color)
{
	this->Color = Color;
}

void CImage::Draw(long, long, long, long)
{
	if (Image)
		if (Border) {
			Graph->PutImage(1,1,ImageWidth,ImageHeight,Image);
			Graph->Rectangle(0,0,Width,Height,17);
		}
		else
			Graph->PutImage(0,0,ImageWidth,ImageHeight,Image);
	else {
		Graph->Bar(0,0,Width,Height,Color);
		if (Border)
			Graph->Rectangle(0,0,Width,Height,17);
	}
}