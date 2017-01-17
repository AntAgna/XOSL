
#include "cdrom.h"
#include <fs.h>
#include <mem.h>
    
class CExtDriveParams {
public:
	unsigned short BufferSize;
	unsigned short InfoFlags;
	unsigned long CylinderCount;
	unsigned long HeadCount;
	unsigned long SectorsPerTrack;
	unsigned long TotalSectorCount[2];
	unsigned short BytesPerSector;
	unsigned long EddConfigParams;
	unsigned short DevicePathSignature;
	unsigned char DevicePathInfoLength;
	unsigned char Reserved1[3];
	unsigned char HostBusName[4]; // "ISA" or "PCI"
	unsigned char InterfaceName[8]; // e.g. "ATAPI"
	unsigned char InterfacePath[8];
	unsigned char DevicePath[8];
	unsigned char Reserved2;
	unsigned char Checksum;
};

void printf(const char *,...);

const unsigned long cBaseMemSizeAddr = 0x00000413;
const int SIZE_OF_EDD30 = 8;  // my EDD3.0 and ATAPI driver need 6K mem
const char *cEdd30FileName = "EDD30   BIN";

	enum TBootExtFunc {
		funcInstInt13 = 0, funcUninstInt13, funcInitAtapi, 
		funcSwapDrives, funcSetIoPorts
	};


CCdRom::CCdRom(CFileSystem &FileSystemToUse):
	FileSystem(FileSystemToUse),
	BaseMemSize(*(unsigned short *)cBaseMemSizeAddr),
	BootExtensionAddr(0)
{
}

CCdRom::~CCdRom()
{
}

int CCdRom::Install(short IdePort1, short IdePort2)
{
	if (!BootExtensionAddr) {
		printf("AllocConvMem()\n");
		BootExtensionAddr = AllocConvMem(SIZE_OF_EDD30) << 16;
		printf("LoadBootExtension()\n");
		if (LoadBootExtension() == 0) {
			BootExtensionAddr = 0;
			return -1;
		}
		printf("SetIoPorts\n");
		CallBootExtension(BootExtensionAddr,funcSetIoPorts,IdePort1,IdePort2);
		printf("InitAtapi\n");
		CallBootExtension(BootExtensionAddr,funcInitAtapi,0,0);
		printf("InstInt13\n");
		CallBootExtension(BootExtensionAddr,funcInstInt13,0,0);
	}
	return 0;
}

void CCdRom::Uninstall()
{
	if (BootExtensionAddr) {
		CallBootExtension(BootExtensionAddr,funcUninstInt13,0,0);
		FreeConvMem(SIZE_OF_EDD30);
		BootExtensionAddr = 0;
	}
}


bool CCdRom::IsDriveCdRom(int Drive)
{
	if (!BootExtensionAddr) {
		return false;
	}
	CExtDriveParams ExtDriveParams;
	ExtDriveParams.BufferSize = sizeof(CExtDriveParams);
	if (GetExtDriveParams(Drive,ExtDriveParams) == -1) {
		return false;
	}

	if (ExtDriveParams.DevicePathSignature != 0xbedd) {
		return false;
	}
	if (ExtDriveParams.DevicePathInfoLength != 36) {
		return false;
	}
	
	if (memcmp(ExtDriveParams.InterfaceName,"ATAPI",5) != 0) {
		return false;
	}
	return true;
}

unsigned long CCdRom::AllocConvMem(int KbCount)
{
	BaseMemSize -= KbCount;
	return BaseMemSize << 6;
}

void CCdRom::FreeConvMem(int KbCount)
{
	BaseMemSize += KbCount;
}

int CCdRom::LoadBootExtension()
{
	// TODO: check if this is performed successfully!
	void *DestPtr = (void *)BootExtensionAddr;
	return FileSystem.ReadFile(cEdd30FileName,DestPtr);
}

