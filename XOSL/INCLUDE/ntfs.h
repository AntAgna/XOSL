#ifndef NTFSH
#define NTFSH



struct TBootNTFS {
	char Jump[3];
	char SystemId[8]; // "NTFS"
}


#endif
