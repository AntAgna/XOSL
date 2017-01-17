#ifndef MBRPassH
#define MBRPassH

class CMBRPassword {
public:
	char IPL[436];
	unsigned long Password;
	char Reserved[6];
	char PartTable[16 * 4];
	unsigned short MagicNumber;
};

#endif
