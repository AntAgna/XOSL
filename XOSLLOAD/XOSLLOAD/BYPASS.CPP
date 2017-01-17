
#include <Bypass.h>
#include <disk.h>
#include <text.h>

#include <fat16.h>
#include <fat32.h>

#include <ctype.h>
#include <mem.h>

#include <mbrpass.h>
#include <encpwd.h>

#include <string.h>

#define BootJump() (__emit__(0xea,0x00,0x7c,0x00,0x00))
#define KEY_ENTER 0x1c0d 
#define IPL_ADDR ( (void *)0x00007c00 )

void printf(const char *format, ...)
{
	unsigned short *argl;
	char buf[32];

	for (argl = &((unsigned short *)&format)[2]; *format; ++format)
		switch (*format) {
			case '\n':
				PutS("\r\n");
				break;
			case '%':
				switch (*++format) {
					case 'c':
						PutCh(*argl++);
						break;
					case 'd':
						itoa(*argl++,buf,DEC);
						PutS(buf);
						break;
					case 'x':
						itoa(*argl++,buf,HEX);
						PutS(buf);
						break;
					case 's':
						PutS((const char *)*(long *)argl);
						argl += 2;
						break;
					case 'l':
						if (format[1] == 'd') {
							++format;
							itoa(*(unsigned long *)argl++,buf,DEC);
							PutS(buf);
							break;
						}
						if (format[1] == 'x') {
							++format;
							itoa(*(unsigned long *)argl++,buf,HEX);
							PutS(buf);
							break;
						}
					default:
						PutCh('%');
						--format;
						break;
				}
				break;
			default:
				PutCh(*format);
				break;
		}
}






CBypass::CBypass()
{
	
}

CBypass::~CBypass()
{

}

void CBypass::Execute(const char *ErrorMsg)
{
	int Index;
	char VolumeLabel[16];
	const char *FSName;
	unsigned short Key;

	if (ErrorMsg) {
		printf("CRITICAL ERROR: %s\n\n",ErrorMsg);
	}
	printf("-- XOSL Bypass --\n\n");
	ReadMBR();
	CheckPassword();
	for (Index = 0; Index < 4; ++Index) {
		if (MBR.Table[Index].SectorCount) {
			FSName = CPartList::GetFSName(MBR.Table[Index].FSType);
			if (ReadVolumeLabel(0x80,MBR.Table[Index].StartSector,VolumeLabel) != -1) {
				printf("%d - %s (%s)\n",Index + 1,FSName,VolumeLabel);
			}
			else {
				printf("%d - %s\n",Index + 1,FSName);
			}
		}
		else {
			printf("%d - <empty>\n",Index + 1);
		}
	}
	printf("\nF - Boot from floppy\n\nSelect partition to boot --> ");
	while (((Key = tolower(GetCh() & 0x00ff)) < '1' || Key > '4') && Key != 'f');
	printf("%c\n\n",Key);

	RawBoot(Key);

}

void CBypass::ReadMBR()
{
	CDisk Disk;

	Disk.Map(0x80,0);
	Disk.Read(0,&MBR,1);
}


int CBypass::ReadVolumeLabel(int Drive, unsigned long Sector, char *VolumeLabel)
{
	char BootRecord[512];
	TBootFAT16 *BootFAT16 = (TBootFAT16 *)BootRecord;
	TBootFAT32 *BootFAT32 = (TBootFAT32 *)BootRecord;
	CDisk Disk;

	// check return values -> in case the partition table is corrupted.
	if (Disk.Map(Drive,Sector) == -1)
		return -1;
	if (Disk.Read(0,BootRecord,1) == -1)
		return -1;

	if (memcmp(BootFAT16->FSID,"FAT1",4) == 0) {
		CPartList::CreateVolumeLabel(BootFAT16->Label,VolumeLabel);
		return 0;
	}
	else {
		if (memcmp(BootFAT32->FSID,"FAT3",4) == 0) {
			CPartList::CreateVolumeLabel(BootFAT32->Label,VolumeLabel);
			return 0;
		}
	}
	return -1;
}


void CBypass::CheckPassword()
{
	CMBRPassword *MBRPassword;
	char PasswordStr[64];
	int PasswordIndex;
	unsigned short Key;

	MBRPassword = (CMBRPassword *)&MBR;
	if (!MBRPassword->Password) {
		return;
	}

	for (;;) {
		printf("Enter XOSL password: ");

		for (PasswordIndex = 0; (Key = GetCh()) != KEY_ENTER && PasswordIndex < 63; ++PasswordIndex) {
			printf("*");
			PasswordStr[PasswordIndex] = (char)Key;
		}
		PasswordStr[PasswordIndex] = '\0';
		if (EncodePassword(PasswordStr) == MBRPassword->Password) {
			printf("\n\n");
			return;
		}
		printf("\nInvalid password!\n\n");
	}
}


void CBypass::RawBoot(int BootIndex)
// f  : boot from floppy
// 1-4: boot partition
{
	int Index;
	CDisk Disk;

	if (BootIndex != 'f') {
		BootIndex -= '1';
		for (Index = 0; Index < 4; ++Index)
			MBR.Table[Index].Activated = 0;
		MBR.Table[BootIndex].Activated = 0x80;
		Disk.Map(0x80,0);
		Disk.Write(0,&MBR,1);
		Disk.Read(MBR.Table[BootIndex].StartSector,IPL_ADDR,1);
	}
	else {
		Disk.Map(0x00,0);
		while (Disk.Read(0,IPL_ADDR,1) == -1) {
			printf("Unable to read from floppy drive\n");
			printf("Press any key to retry...\n");
			GetCh();
		}
	}
	BootJump();
}

