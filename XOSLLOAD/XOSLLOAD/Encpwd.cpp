/*
 * Extended Operating System Loader (XOSL)
 * Copyright (c) 1999 by Geurt Vos
 *
 * This code is distributed under GNU General Public License (GPL)
 *
 * The full text of the license can be found in the GPL.TXT file,
 * or at http://www.gnu.org
 */

unsigned long EncodePassword(const char *Password)
{
	int Index;
	unsigned long Code;

	if (!*Password)
		return 0;
	Code = 91369; // random prime number
	for (Index = 0; Password[Index]; ++Index)
		Code = (Password[Index] ^ Code) + ((Password[Index] << 2) + 251) * ((Code >> 1) + 93);
	return Code;
}
