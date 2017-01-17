#ifndef StrTableH
#define StrTableH

class CLoaderStringTable {
public:
	const char *FormTitle;
	const char *Version;
	const char *Copyright;
	const char *URL;
	const char *ChooseOS;
	const char *BootError;
	const char *BootBtnText;
	const char *SetupBtnText;
	const char *PreferBtnText;
	const char *AboutBtnText;
	const char *EnterPassword;
	const char *InvalidPassword;
	const char *SetupPassTitle;
	const char *PreferPassTitle;
	const char *BootPassTitle;
	const char *TimerMinPre;
	const char *TimerMinPost;
	const char *TimerSecPre;
	const char *TimerSecPost;
	const char *EndTimer;
};


class CXoslStringTable {
public:
//	CXoslStringTable(CFileSystem &FileSystem);
//	~CXoslStringTable();

	static CLoaderStringTable Loader;

};

extern CXoslStringTable *StringTable;

#endif
