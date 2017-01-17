#ifndef BypassH
#define BypassH

#include <newdefs.h>
#include <ptab.h>
   
	   
class CBypass {
public:
	CBypass();
	~CBypass();

	void Execute(const char *ErrorMsg = NULL);
private:
	TPartTable MBR;

	void ReadMBR();
	int ReadVolumeLabel(int Drive, unsigned long Sector, char *VolumeLabel);

	void RawBoot(int BootIndex);

	void CheckPassword();

};

#endif
