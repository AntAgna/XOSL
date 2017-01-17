/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

#include <key.h>
#include <string.h>

CKeyboard::CKeyboard()
{
	CtrlKey = false;
	AltKey = false;
}

CKeyboard::~CKeyboard()
{
}


void CKeyboard::GetKeyStatus(bool &Ctrl, bool &Alt)
{
	Ctrl = CtrlKey;
	Alt = AltKey;
}

bool CKeyboard::IsFuncKey(int KeyCode, bool &Shift, bool &Ctrl, bool &Alt)
{
	Shift = Alt = Ctrl = false;
	if (KeyCode >= KEY_F1 && KeyCode <= KEY_F10) {
		return true;
	}
	if (KeyCode >= KEY_SHIFT_F1 && KeyCode <= KEY_SHIFT_F10) {
		Shift = true;
		return true;
	}
	if (KeyCode >= KEY_ALT_F1 && KeyCode <= KEY_ALT_F10) {
		Alt = true;
		return true;
	}
	if (KeyCode >= KEY_CTRL_F1 && KeyCode <= KEY_CTRL_F10) {
		Ctrl = true;
		return true;
	}
	return false;
}

void CKeyboard::GetKeyName(int KeyCode, char *KeyName)
{
	int TempKey;

	TempKey = LastKey;
	LastKey = KeyCode;
	GetKeyName(KeyName);
	LastKey = TempKey;
}

void CKeyboard::GetKeyName(char *KeyName)
{
	unsigned short ScanCode = LastKey >> 8;
	unsigned short AsciiCode = LastKey & 0xff;
	int Index;

	CtrlKey = false;
	AltKey = false;
	/* scan the special keys list */
	for (Index = 0; Index < SPECIAL_KEY_COUNT; ++Index)
		if (SpecialKeyList[Index].ScanCode == (unsigned char)ScanCode) {
			if (SpecialKeyList[Index].NormalAscii == (unsigned char)AsciiCode) {
				strcpy(KeyName,SpecialKeyList[Index].KeyName);
				return;
			}
			else
				if (SpecialKeyList[Index].CtrlAscii == (unsigned char)AsciiCode) {
					strcpy(KeyName,"Ctrl-");
					strcat(KeyName,SpecialKeyList[Index].KeyName);
					CtrlKey = true;
					return;
				}
				else
					if (SpecialKeyList[Index].AltAscii == (unsigned char)AsciiCode) {
						strcpy(KeyName,"Alt-");
						strcat(KeyName,SpecialKeyList[Index].KeyName);
						AltKey = true;
						return;
					}
		}
	if (ScanCode >= DEFAULT_KEY_COUNT) {
		strcpy(KeyName,"Undefined");
		return;
	}
	if (KeyList[ScanCode].NormalAscii == (unsigned char)AsciiCode)
		strcpy(KeyName,KeyList[ScanCode].KeyName);
	else
		if (KeyList[ScanCode].CtrlAscii == (unsigned char)AsciiCode) {
			strcpy(KeyName,"Ctrl-");
			strcat(KeyName,KeyList[ScanCode].KeyName);
			CtrlKey = true;
		}
		else
			if (KeyList[ScanCode].AltAscii == (unsigned char)AsciiCode) {
				strcpy(KeyName,"Alt-");
				strcat(KeyName,KeyList[ScanCode].KeyName);
				AltKey = true;
			}
			else
				strcpy(KeyName,(const char *)&AsciiCode);
}

/*
 * list of special keys, which share scan codes
 * with 'default' keys. This list is scanned first.
 */
CKeyboard::TSpecialKeyListEntry CKeyboard::SpecialKeyList[SPECIAL_KEY_COUNT] = {
{0x54,0x00,-1,-1,"Shift-F1"}, // Shift is not supported by TKeyListEntry
{0x55,0x00,-1,-1,"Shift-F2"}, // though it should have supported it...
{0x56,0x00,-1,-1,"Shift-F3"},
{0x57,0x00,-1,-1,"Shift-F4"},
{0x58,0x00,-1,-1,"Shift-F5"},
{0x59,0x00,-1,-1,"Shift-F6"},
{0x5a,0x00,-1,-1,"Shift-F7"},
{0x5b,0x00,-1,-1,"Shift-F8"},
{0x5c,0x00,-1,-1,"Shift-F9"},
{0x5d,0x00,-1,-1,"Shift-F10"},
{0x5f,0x80,0x02,-1,"Sleep"} // Windows 98 Power Kbd Sleep shared with Ctrl-F2
};
		typedef struct {
			char ScanCode;
			char NormalAscii;
			char CtrlAscii;
			char AltAscii;
			const char *KeyName;
		} TSpecialKeyListEntry;


// Use scan code as index in KeyList
CKeyboard::TKeyListEntry CKeyboard::KeyList[DEFAULT_KEY_COUNT] = {
{-1,0x00,-1,"Break"}, // Break (Ctrl only)
{0x1b,-1,-1,"Escape"}, // Escape
{-1,-1,-1,NULL}, // 1 !
{-1,0x00,-1,"2"}, // 2 @
{-1,-1,-1,NULL}, // 3 #
{-1,-1,-1,NULL}, // 4 $
{-1,-1,-1,NULL}, // 5 %
{-1,0x1e,-1,"6"}, // 6 ^
{-1,-1,-1,NULL}, // 7 &
{-1,-1,-1,NULL}, // 8 *
{-1,-1,-1,NULL}, // 9 (
{-1,-1,-1,NULL}, // 0 )
{-1,0x1f,-1,"-"}, // - _
{-1,-1,-1,NULL}, // = +
{0x08,0x7f,-1,"Backspace"}, // backspace
{0x09,-1,-1,"Tab"}, // Tab
{-1,0x11,0x00,"Q"}, // q Q
{-1,0x17,0,"W"}, // w W
{-1,0x05,0,"E"}, // e E
{-1,0x12,0,"R"}, // r R
{-1,0x14,0,"T"}, // t T
{-1,0x19,0,"Y"}, // y Y
{-1,0x15,0,"U"}, // u U
{-1,0x09,0,"I"}, // i I
{-1,0x0f,0,"O"}, // o O
{-1,0x10,0,"P"}, // p P
{-1,0x1b,-1,"["}, // [ {
{-1,0x1d,-1,"]"}, // ] }
{0x0d,0x0a,-1,"Enter"}, // Enter
{-1,-1,-1,"Undefined"}, // Undefined key
{-1,0x01,0,"A"}, // a A
{-1,0x13,0,"S"}, // s S
{-1,0x04,0,"D"}, // d D
{-1,0x06,0,"F"}, // f F
{-1,0x07,0,"G"}, // g G
{-1,0x08,0,"H"}, // h H
{-1,0x0a,0,"J"}, // j J
{-1,0x0b,0,"K"}, // k K
{-1,0x0c,0,"L"}, // l L
{-1,-1,-1,NULL}, // ; :
{-1,-1,-1,NULL}, // ' "
{-1,-1,-1,NULL}, // ` ~
{-1,-1,-1,"Undefined"}, // Undefined key
{-1,0x1c,-1,"\\"}, // \ |
{-1,0x1a,0,"Z"}, // z Z
{-1,0x18,0,"X"}, // x X
{-1,0x03,0,"C"}, // c C
{-1,0x16,0,"V"}, // v V
{-1,0x02,0,"B"}, // b B
{-1,0x0e,0,"N"}, // n N
{-1,0x0d,0,"M"}, // m M
{-1,-1,-1,NULL}, // , <
{-1,-1,-1,NULL}, // . >
{-1,-1,-1,NULL}, // .
{-1,-1,-1,"Undefined"}, // Undefined key
{0x2a,-1,-1,"Keypad *"}, // Keypad *
{-1,-1,-1,"Undefined"}, // Undefined key
{0x20,-1,-1,"Space"}, // space
{-1,-1,-1,"Undefined"}, // Undefined key
{0x00,-1,-1,"F1"}, // F1
{0x00,-1,-1,"F2"}, // F2
{0x00,-1,-1,"F3"}, // F3
{0x00,-1,-1,"F4"}, // F4
{0x00,-1,-1,"F5"}, // F5
{0x00,-1,-1,"F6"}, // F6
{0x00,-1,-1,"F7"}, // F7
{0x00,-1,-1,"F8"}, // F8
{0x00,-1,-1,"F9"}, // F9
{0x00,-1,-1,"F10"}, // F10
{-1,-1,-1,"Undefined"}, // Undefined key
{-1,-1,-1,"Undefined"}, // Undefined key
{0x00,-1,-1,"Home"}, // Home
{0x00,-1,-1,"Up"}, // Cursor Up
{0x00,-1,-1,"Page Up"}, // Page Up
{0x2d,-1,-1,"Keypad -"}, // Keypad -
{0x00,-1,-1,"Left"}, // Cursor left
{-1,-1,-1,NULL}, // Keypad 5
{0x00,-1,-1,"Right"}, // Cursor right
{0x2b,-1,-1,"Keypad +"}, // Keypard +
{0x00,-1,-1,"End"}, // End
{0x00,-1,-1,"Down"}, // Cursor down
{0x00,-1,-1,"Page Down"}, // Page Down
{0x00,-1,-1,"Insert"}, // Insert
{0x00,-1,-1,"Delete"}, // Delete
{-1,-1,-1,"Undefined"}, // Undefined key
{-1,-1,-1,"Undefined"}, // Undefined key
{-1,-1,-1,"Undefined"}, // Undefined key
{-1,-1,-1,"Undefined"}, // Undefined key
{-1,-1,-1,"Undefined"}, // Undefined key
{-1,-1,-1,"Undefined"}, // Undefined key
{-1,-1,-1,"Undefined"}, // Undefined key
{-1,-1,-1,"Undefined"}, // Undefined key
{-1,-1,-1,"Undefined"}, // Undefined key
{-1,-1,-1,"Undefined"}, // Undefined key
{-1,0x00,-1,"F1"}, // F1 (Ctrl only)
{-1,0x00,-1,"F2"}, // F2 (Ctrl only)
//{0x80,0x02,-1,"Sleep"}, // Windows 98 Power Kbd Sleep button also on 0x5f
{-1,0x00,-1,"F3"}, // F3 (Ctrl only)
{-1,0x00,-1,"F4"}, // F4 (Ctrl only)
{-1,0x00,-1,"F5"}, // F5 (Ctrl only)
{-1,0x00,-1,"F6"}, // F6 (Ctrl only)
{-1,0x00,-1,"F7"}, // F7 (Ctrl only)
{-1,0x00,-1,"F8"}, // F8 (Ctrl only)
{-1,0x00,-1,"F9"}, // F9 (Ctrl only)
{-1,0x00,-1,"F10"}, // F10 (Ctrl only)
{-1,-1,0x00,"F1"}, // F1 (Alt only)
{-1,-1,0x00,"F2"}, // F2 (Alt only)
{-1,-1,0x00,"F3"}, // F3 (Alt only)
{-1,-1,0x00,"F4"}, // F4 (Alt only)
{-1,-1,0x00,"F5"}, // F5 (Alt only)
{-1,-1,0x00,"F6"}, // F6 (Alt only)
{-1,-1,0x00,"F7"}, // F7 (Alt only)
{-1,-1,0x00,"F8"}, // F8 (Alt only)
{-1,-1,0x00,"F9"}, // F9 (Alt only)
{-1,-1,0x00,"F10"}, // F10 (Alt only)
{-1,0x00,-1,"SysRq"}, // SysRq (Ctrl only)
{-1,0x00,-1,"Left"}, // Left (Ctrl only)
{-1,0x00,-1,"Right"}, // Ctrl (Ctrl only)
{-1,0x00,-1,"End"}, // End (Ctrl only)
{-1,0x00,-1,"Page Down"}, // Page Down (Ctrl only)
{-1,0x00,-1,"Home"}, // Home (Ctrl only)
{-1,-1,0x00,"1"}, // 1 (Alt only)
{-1,-1,0x00,"2"}, // 2 (Alt only)
{-1,-1,0x00,"3"}, // 3 (Alt only)
{-1,-1,0x00,"4"}, // 4 (Alt only)
{-1,-1,0x00,"5"}, // 5 (Alt only)
{-1,-1,0x00,"6"}, // 6 (Alt only)
{-1,-1,0x00,"7"}, // 7 (Alt only)
{-1,-1,0x00,"8"}, // 8 (Alt only)
{-1,-1,0x00,"9"}, // 9 (Alt only)
{-1,-1,0x00,"0"}, // 0 (Alt only)
{-1,-1,0x00,"-"}, // - (Alt only)
{-1,-1,0x00,"="}, // = (Alt only)
{-1,0x00,-1,"Page Up"} // Page Up (Ctrl only)
};

class CKeyDesc {
public:
	unsigned short KeyCode;
	unsigned short NewKeyCode;
	const char *KeyName;
	const char *ShortKeyName;
	bool SpecialKey;
};

CKeyDesc USKeyList[] =  {
	{0x011b,0x011b,"Escape","esc",true},
	{0x3b00,0x3b00,"F1","F1",true},
	{0x3c00,0x3c00,"F2","F2",true},
	{0x3d00,0x3d00,"F3","F3",true},
	{0x3e00,0x3e00,"F4","F4",true},
	{0x3f00,0x3f00,"F5","F5",true},
	{0x4000,0x4000,"F6","F6",true},
	{0x4100,0x4100,"F7","F7",true},
	{0x4200,0x4200,"F8","F8",true},
	{0x4300,0x4300,"F9","F9",true},
	{0x4400,0x4400,"F10","F10",true},
	{0x2960,0x2960,"`","`",false},
	{0x0231,0x0231,"1","1",false},
	{0x0332,0x0332,"2","2",false},
	{0x0433,0x0433,"3","3",false},
	{0x0533,0x0434,"4","4",false},
	{0x0633,0x0435,"5","5",false},
	{0x0733,0x0436,"6","6",false},
	{0x0833,0x0437,"7","7",false},
	{0x0933,0x0438,"8","8",false},
	{0x0a33,0x0439,"9","9",false},
	{0x0b33,0x0430,"0","0",false},
	{0x0c2d,0x0d3d,"-","-",false},
	{0x0d3d,0x0d3d,"=","=",false},
	{0x0e08,0x0e08,"Backspace","bs",true},
	{0x0f09,0x0f09,"Tab","tab",true},
	{0x1071,0x1071,"q","q",false},
	{0x1177,0x1177,"w","w",false},
	{0x1265,0x1265,"e","e",false},
	{0x1372,0x1372,"r","r",false},
	{0x1474,0x1474,"t","t",false},
	{0x1579,0x1579,"y","y",false},
	{0x1675,0x1675,"u","u",false},
	{0x1769,0x1769,"i","i",false},
	{0x186f,0x186f,"o","o",false},
	{0x1970,0x1970,"p","p",false},
	{0x1a5b,0x1a5b,"[","[",false},
	{0x1b5d,0x1b5d,"]","]",false},
	{0x1c0d,0x1c0d,"Enter","ret",true},
	{0x1e61,0x1e61,"a","a",false},
	{0x1f73,0x1f73,"s","s",false},
	{0x2064,0x2064,"d","d",false},
	{0x2166,0x2166,"f","f",false},
	{0x2267,0x2267,"g","g",false},
	{0x2368,0x2368,"h","h",false},
	{0x246a,0x246a,"j","j",false},
	{0x256b,0x256b,"k","k",false},
	{0x266c,0x266c,"l","l",false},
	{0x273b,0x273b,";",";",false},
	{0x2827,0x2827,"'","'",false},
	{0x2b5c,0x2b5c,"\\","\\",false},
	{0x565c,0x565c,"\\","\\",false},
	{0x2c7a,0x2c7a,"z","z",false},
	{0x2d78,0x2d78,"x","x",false},
	{0x2e63,0x2e63,"c","c",false},
	{0x2f76,0x2f76,"v","v",false},
	{0x3062,0x3062,"b","b",false},
	{0x316e,0x316e,"n","n",false},
	{0x326d,0x326d,"m","m",false},
	{0x332c,0x332c,",",",",false},
	{0x342e,0x342e,".",".",false},
	{0x352f,0x352f,"/","/",false},
	{0x3920,0x3920,"Space"," ",false},
	{0x372a,0x372a,"Keypad *","*",false},
    {0x4a2d,0x4a2d,"Keypad -","-",false},
	{0x4700,0x4700,"Home","hm",true},
	{0x4800,0x4800,"Cursor Up","cu",true},
	{0x4900,0x4900,"Page Up","pu",true},
	{0x4b00,0x4b00,"Cursor Left","cl",true},
	{0x4c00,0x4c00,"Keypad 5","k5",true},
	{0x4d00,0x4d00,"Cursor Right","cr",true},
	{0x4e2b,0x4e2b,"Keypad +","+",false},
	{0x4f00,0x4f00,"End","end",true},
	{0x5000,0x5000,"Cursor Down","cd",true},
	{0x5100,0x5100,"Page Down","pd",true},
	{0x5200,0x5200,"Insert","ins",true},
	{0x5300,0x5300,"Delete","del",true},
	// shift
	{0x5400,0x5400,"Shift-F1","CF1",true},
	{0x5500,0x5500,"Shift-F2","CF2",true},
	{0x5600,0x5600,"Shift-F3","CF3",true},
	{0x5700,0x5700,"Shift-F4","CF4",true},
	{0x5800,0x5800,"Shift-F5","CF5",true},
	{0x5900,0x5900,"Shift-F6","CF6",true},
	{0x5a00,0x5a00,"Shift-F7","CF7",true},
	{0x5b00,0x5b00,"Shift-F8","CF8",true},
	{0x5c00,0x5c00,"Shift-F9","CF9",true},
	{0x5d00,0x5d00,"Shift-F10","CF10",true},
	{0x297e,0x297e,"~","~",false},
	{0x0221,0x0221,"!","!",false},
	{0x0340,0x0340,"@","@",false},
	{0x0423,0x0423,"#","#",false},
	{0x0524,0x0524,"$","$",false},
	{0x0625,0x0625,"%","%",false},
	{0x075e,0x075e,"^","^",false},
	{0x0826,0x0826,"&","&",false},
	{0x092a,0x092a,"*","*",false},
	{0x0a28,0x0a28,"(","(",false},
	{0x0b29,0x0b29,")",")",false},
	{0x0c5f,0x0c5f,"_","_",false},
	{0x0d2b,0x0d2b,"+","+",false},
	{0x0f00,0x0f00,"Shift-Tab","Stab",true},
	{0x1051,0x1051,"Q","Q",false},
	{0x1157,0x1157,"W","W",false},
	{0x1245,0x1245,"E","E",false},
	{0x1352,0x1352,"R","R",false},
	{0x1451,0x1451,"T","T",false},
	{0x1559,0x1559,"Y","Y",false},
	{0x1655,0x1655,"U","U",false},
	{0x1749,0x1749,"I","I",false},
	{0x184f,0x184f,"O","O",false},
	{0x1950,0x1950,"P","P",false},
	{0x1a7b,0x1a7b,"{","{",false},
	{0x1b7d,0x1b7d,"}","}",false},
	{0x1e41,0x1e41,"A","A",false},
	{0x1f53,0x1f53,"S","S",false},
	{0x2044,0x2044,"D","D",false},
	{0x2146,0x2146,"F","F",false},
	{0x2247,0x2247,"G","G",false},
	{0x2348,0x2348,"H","H",false},
	{0x244a,0x244a,"J","J",false},
	{0x254b,0x254b,"K","K",false},
	{0x264c,0x264c,"L","L",false},
	{0x273a,0x273a,":",":",false},
	{0x2822,0x2822,"\"","\"",false},
	{0x2b7c,0x2b7c,"|","|",false},
	{0x567c,0c567c,"|","|",false},
	{0x2c5a,0x2c5a,"Z","Z",false},
	{0x2d58,0x2d58,"X","X",false},
	{0x2e43,0x2e43,"C","C",false},
	{0x2f56,0x2f56,"V","V",false},
	{0x3042,0x3042,"B","B",false},
	{0x314e,0x314e,"N","N",false},
	{0x324d,0x324d,"M","M",false},
	{0x333c,0x333c,"<","<",false},
	{0x343e,0x3e3e,">",">",false},
	{0x353f,0x353f,"?","?",false},
	{0x4737,0x4737,"Keypad 7","7",false},
	{0x4838,0x4837,"Keypad 8","8",false},
	{0x4939,0x4939,"Keypad 9","9",false},
	{0x4b34,0x4b34,"Keypad 4","4",false},
	{0x4c35,0x4c25,"Keypad 5","5",false},
	{0x4d36,0x4d36,"Keypad 6","6",false},
	{0x4f31,0x4f31,"Keypad 1","1",false},
	{0x5032,0x5032,"Keypad 2","2",false},
	{0x5133,0x5133,"Keypad 3","3",false},
	{0x5230,0x5230,"Keypad 0","0",false},
	{0x532e,0x532e,"Keypad .",".",false},

	// Ctrl
	{0x5e00,0x5e00,"Ctrl-F1","CF1",true},
	{0x5f00,0x5f00,"Ctrl-F2","CF2",true},
	{0x6000,0x6000,"Ctrl-F3","CF3",true},
	{0x6100,0x6100,"Ctrl-F4","CF4",true},
	{0x6200,0x6200,"Ctrl-F5","CF5",true},
	{0x6300,0x6300,"Ctrl-F6","CF6",true},
	{0x6400,0x6400,"Ctrl-F7","CF7",true},
	{0x6500,0x6500,"Ctrl-F8","CF8",true},
	{0x6600,0x6600,"Ctrl-F9","CF9",true},
	{0x6700,0x6700,"Ctrl-F10","CF10",true},
	{0x7200,0x7200,"Ctrl-SysRq","Csreq",true},
	{0x0300,0x0300,"Ctrl-2","C2",true},
	{0x071e,0x071e,"Ctrl-6","C6",true},
	{0x0c1f,0x0c1f,"Ctrl--","C-",true}, 
	{0x0e7f,0x0e7f,"Ctrl-Backspace","Cbs",true},
	{0x1011,0x1011,"Ctrl-Q","CQ",true},
	{0x1117,0x1117,"Ctrl-W","CW",true},
	{0x1205,0x1205,"Ctrl-E","CE",true},
	{0x1312,0x1312,"Ctrl-R","CR",true},
	{0x1414,0x1414,"Ctrl-T","CT",true},
	{0x1519,0x1519,"Ctrl-Y","CY",true},
	{0x1615,0x1615,"Ctrl-U","CU",true},
	{0x1700,0x1700,"Ctrl-I","CI",true},
	{0x180f,0x180f,"Ctrl-O","CO",true},
	{0x1910,0x1910,"Ctrl-P","CP",true},
	{0x1a1b,0x1a1b,"Ctrl-[","C[",true},
	{0x1b19,0x1b19,"Ctrl-]","C]",true},
	{0x1c0a,0x1c0a,"Ctrl-Enter","Cret,true"},
	{0x1e01,0x1e01,"Ctrl-A","CA",true},
	{0x1f13,0x1f13,"Ctrl-S","CS",true},
	{0x2004,0x2004,"Ctrl-D","CD",true},
	{0x2106,0x2106,"Ctrl-F","CF",true},
	{0x2207,0x2207,"Ctrl-G","CG",true},
	{0x2308,0x2308,"Ctrl-H","CH",true},
	{0x2400,0x2400,"Ctrl-J","CJ",true},
	{0x250b,0x250b,"Ctrl-K","CK",true},
	{0x260c,0x260c,"Ctrl-L","CL",true},
	{0x2b1c,0x2b1c,"Ctrl-\\","C\\",true},
	{0x561c,0x561c,"Ctrl-\\","C\\",true},
	{0x2c1a,0x2c1a,"Ctrl-Z","CZ",true},
	{0x2d18,0x2d18,"Ctrl-X","CX",true},
	{0x2e03,0x2e03,"Ctrl-C","CC",true},
	{0x2f16,0x2f16,"Ctrl-V","CV",true},
	{0x3002,0x3002,"Ctrl-B","CB",true},
	{0x310e,0x310e,"Ctrl-N","CN",true},
	{0x320d,0c320d,"Ctrl-M","CM",true},
	{0x7700,0x7700,"Ctrl-Home","Chm",true},
	{0x8400,0x8400,"Ctrl-Page Up","Cpu",true},
	{0x7300,0x7300,"Ctrl-Left","Ccl",true},
	{0x7400,0x7400,"Ctrl-Right","Ccr",true},
	{0x7500,0x7500,"Ctrl-End","Cend",true},
	{0x7600,0x7600,"Ctrl-Page Down","Cpd",true},
	// Alt
	{0x0100,0x0100,"Alt-Escape","Aesc",true},
	{0x5400,0x5400,"Alt-F1","AF1",true},
	{0x5500,0x5500,"Alt-F2","AF2",true},
	{0x5600,0x5600,"Alt-F3","AF3",true},
	{0x5700,0x5700,"Alt-F4","AF4",true},
	{0x5800,0x5800,"Alt-F5","AF5",true},
	{0x5900,0x5900,"Alt-F6","AF6",true},
	{0x5a00,0x5a00,"Alt-F7","AF7",true},
	{0x5b00,0x5b00,"Alt-F8","AF8",true},
	{0x5c00,0x5c00,"Alt-F9","AF9",true},
	{0x5d00,0x5d00,"Alt-F10","AF10",true},
	{0x2900,0x2900,"Alt-`","A`",true},
	{0x7800,0x7800,"Alt-1","A1",true},
	{0x7900,0x7900,"Alt-2","A2",true},
	{0x7a00,0x7a00,"Alt-3","A3",true},
	{0x7b00,0x7b00,"Alt-4","A4",true},
	{0x7c00,0x7c00,"Alt-5","A5",true},
	{0x7d00,0x7d00,"Alt-6","A6",true},
	{0x7e00,0x7e00,"Alt-7","A7",true},
	{0x7f00,0x7f00,"Alt-8","A8",true},
	{0x8000,0x8000,"Alt-9","A9",true},
	{0x8100,0x8100,"Alt-0","A0",true},
	{0x8200,0x8200,"Alt--","A-",true},
	{0x8300,0x8300,"Alt-=","A=",true},
	{0x1000,0x1000,"Alt-Q","AQ",true},
	{0x1100,0x1100,"Alt-W","AW",true},
	{0x1200,0x1200,"Alt-E","AE",true},
	{0x1300,0x1300,"Alt-R","AR",true},
	{0x1400,0x1400,"Alt-T","AT",true},
	{0x1500,0x1500,"Alt-Y","AY",true},
	{0x1600,0x1600,"Alt-U","AU",true},
	{0x1700,0x1700,"Alt-I","AI",true},
	{0x1800,0x1800,"Alt-O","AO",true},
	{0x1900,0x1900,"Alt-P","AP",true},
	{0x1a00,0x1a00,"Alt-[","A[",true},
	{0x1b00,0x1b00,"Alt-]","A]",true},
	{0x1c00,0x1c00,"Alt-Enter","Aret",true},
	{0x1e00,0x1e00,"Alt-A","AA",true},
	{0x1f00,0x1f00,"Alt-S","AS",true},
	{0x2000,0x2000,"Alt-D","AD",true},
	{0x2100,0x2100,"Alt-F","AF",true},
	{0x2200,0x2200,"Alt-G","AG",true},
	{0x2300,0x2300,"Alt-H","AH",true},
	{0x2400,0x2400,"Alt-J","AJ",true},
	{0x2500,0x2500,"Alt-K","AK",true},
	{0x2600,0x2600,"Alt-L","AL",true},
	{0x2700,0x2700,"Alt-;","A;",true},
	{0x2800,0x2800,"Alt-'","A'",true},
	{0x2b00,0x2b00,"Alt-\\","A\\",true},
	{0x5600,0x5600,"Alt-\\","A\\",true},
	{0x2c00,0x2c00,"Alt-Z","AZ",true},
	{0x2d00,0x2d00,"Alt-X","AX",true},
	{0x2eE0,0x2e00,"Alt-C","AC",true},
	{0x2f00,0x2f00,"Alt-V","AV",true},
	{0x3000,0x3000,"Alt-B","AB",true},
	{0x3100,0x3100,"Alt-N","AN",true},
	{0x3200,0x3200,"Alt-M","AM",true},
	{0x3300,0x3300,"Alt-,","A,",true},
	{0x3400,0x3400,"Alt-.","A.",true},
	{0x3500,0x3500,"Alt-/","A/",true}

};



