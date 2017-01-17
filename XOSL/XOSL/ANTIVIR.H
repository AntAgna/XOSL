#ifndef AntiVirH
#define AntiVirH

#include <newdefs.h>
#include <fs.h>
#include <key.h>

class CAntiVirus {
public:
	CAntiVirus(CFileSystem &FileSystemToUse);
	~CAntiVirus();

	void Sweep();

private:
	CFileSystem &FileSystem;

	void ClearScreen();


	void GetRealMBR(char *RealMBR);
	int GetDiskMBR(char *DiskMBR);

	void DisplayError();
	void DisplayWarning();
	void FixMBR(const char *DiskMBR);
	void DisplayResult();

	void Reboot();

};

#endif
