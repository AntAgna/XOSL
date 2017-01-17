/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <about.h>
#include <loader.h>
void printf(const char *,...);

extern unsigned char *SplashLogo;

static const char *StrXoslTitle = "Extended Operating System Loader";
static const char *StrCopyright = "Copyright \xa9 1999/2000, Geurt Vos";
static const char *StrHttpXosl = "http://www.xosl.org";



static const char *StrLogoInfo = "XOSL logo created by:";
static const char *StrLogoCreator = "Mark Monciardini";
static const char *StrHttpLogo = "http://www.designsbymark.com";
/*
static const char *StrPartInfo = "Ranish Partition Manager";
static const char *StrPartCreator = "Created by: Mikhail Ranish";
static const char *StrPartHttp = "http://www.ranish.com/part";

static const char *StrDesignInfo = "Additional design initial version:";
static const char *StrDesign = "Dan Duskin";


static const char *StrThanksInfo1 = "Thanks to:";
static const char *StrThanksInfo2 = "Mikhail Ranish, Pavel Rezny";
static const char *StrThanksInfo3 = "and everyone else who has";
static const char *StrThanksInfo4 = "helped me with the development";
static const char *StrThanksInfo5 = "of XOSL.";
*/
static const char *StrWarranty1 = "XOSL comes with";
static const char *StrWarranty2 = "ABSOLUTELY NO WARRANTY";
    
struct {
	const char *Text;
	int TextColor;
} ListBoxContents[] = {
	"XOSL logo created by:",17,
	"Mark Monciardini",17,
	"http://www.designsbymark.com",22,
	"",17,
	"Smart Boot Manager",17,
	"http://btmgr.sourceforge.net",22,
	"",17,
	"Ranish Partition Manager",17,
	"Created by: Mikhail Ranish",17,
	"http://www.ranish.com/part",22,
	"",17,
	"Additional design initial version:",17,
	"Dan Duskin",17,
	"",17,
	"Thanks to:",17,
	"Mikhail Ranish, Pavel Rezny",17,
	"and everyone else who has",17,
	"helped me with the development",17,
	"of XOSL.",17
};


CAbout::CAbout(CLoader &LoaderToUse, CScreen &ScreenToUse, CXOSLData &XoslDataToUse):
	Loader(LoaderToUse), Screen(ScreenToUse), XoslData(XoslDataToUse)
{
	Initialized = false;
	Form = NULL;
}

CAbout::~CAbout()
{
}

void CAbout::Show()
{
	if (!Initialized) {
		Initialize();
		CenterWindow();
		if (XoslData.GetGraphics()->Font9) {
			RealignText();
		}
	}

	if (!Form->Visible) {
		Form->Show();
		Form->FocusControl(CloseBtn);
	}
	else
		Screen.SetFocusWnd(Form);
}

void CAbout::Hide()
{
	if (Form && Form->Visible)
		Form->Hide();
}

int CAbout::IsVisible()
{
	if (!Form) {
		return false;
	}
	return Form->Visible;
}

int CAbout::GotFocus()
{
	if (!Form) {
		return false;
	}
	return Screen.GetFocusWnd() == Form;
}

void CAbout::Initialize()
{
	CreateControls();
	InitializeControls();
	ConnectEventHandlers();
	InstallControls();
	Initialized = true;
}

void CAbout::CreateControls()
{
	int lLeft;

	Form = new CForm("About XOSL",FORM_NORMAL,true,200,10,469,308,false);
	Image = new CImage(SplashLogo,218,146,1,7,7,220,148,true);

	lLeft = 8 + ((218 - Graph->GetTextWidth(StrXoslTitle,STYLE_BOLD)) >> 1);
	XoslTitle = new CLabel(StrXoslTitle,STYLE_BOLD,true,17,lLeft,160,true);
	lLeft = 8 + ((218 - Graph->GetTextWidth(StrCopyright,STYLE_REGULAR)) >> 1);
	Copyright = new CLabel(StrCopyright,STYLE_REGULAR,true,17,lLeft,176,true);
	lLeft = 8 + ((218 - Graph->GetTextWidth(StrHttpXosl,STYLE_REGULAR)) >> 1);
	HttpXosl = new CLabel(StrHttpXosl,STYLE_REGULAR,true,22,lLeft,192,true);

	Warranty1 = new CLabel(StrWarranty1,STYLE_REGULAR,true,17,8,252,true);
	Warranty2 = new CLabel(StrWarranty2,STYLE_REGULAR,true,17,8,268,true);

	InfoListBox = new CListBox(1,false,232,8,225,217,true,this);
	ScrollBar = new CScrollBar(0,0,0,false,438,9,215,true,InfoListBox);

	CloseBtn = new CButton("Close",307,240,75,25,true,this);
}

void CAbout::InitializeControls()
{
	int Index;

	InfoListBox->SetColumn(0,204,"");
	InfoListBox->SetBackgroundColor(19);
	for (Index = 0; Index < 19; ++Index) {
		InfoListBox->AddRow();
		InfoListBox->AddItem(Index,0,ListBoxContents[Index].Text);
		InfoListBox->SetRowStyle(Index,ListBoxContents[Index].TextColor,STYLE_REGULAR);
	}
	InfoListBox->SetScrollBar(ScrollBar);
}

void CAbout::ConnectEventHandlers()
{
	CloseBtn->OnClick((TWndOnClick)CloseBtnClick);
}


void CAbout::InstallControls()
{
	Form->AddControl(Image);
	Form->AddControl(XoslTitle);
	Form->AddControl(Copyright);
	Form->AddControl(HttpXosl);
	Form->AddControl(Warranty1);
	Form->AddControl(Warranty2);
	Form->AddControl(InfoListBox);
	Form->AddControl(ScrollBar);
	Form->AddControl(CloseBtn);

	Screen.AddWindow(Form);
}

void CAbout::CenterWindow()
{
	if (Form) {
		Screen.CenterWindow(Form);
	}
}

void CAbout::RealignText()
{
	int Left, Top;

	if (!Form) {
		return;
	}
	XoslTitle->SetVisible(false);
	Copyright->SetVisible(false);
	HttpXosl->SetVisible(false);

	XoslTitle->GetPosition(Left,Top);
	Left = 8 + ((218 - Graph->GetTextWidth(StrXoslTitle,STYLE_BOLD)) >> 1);
	XoslTitle->SetPosition(Left,Top);
	Copyright->GetPosition(Left,Top);
	Left = 8 + ((218 - Graph->GetTextWidth(StrCopyright,STYLE_REGULAR)) >> 1);
	Copyright->SetPosition(Left,Top);
	HttpXosl->GetPosition(Left,Top);
	Left = 8 + ((218 - Graph->GetTextWidth(StrHttpXosl,STYLE_REGULAR)) >> 1);
	HttpXosl->SetPosition(Left,Top);
	

	XoslTitle->SetVisible(true);
	Copyright->SetVisible(true);
	HttpXosl->SetVisible(true);
}


void CAbout::CloseBtnClick(CAbout &About)
{
	About.Hide();
	About.Loader.Focus();
}
