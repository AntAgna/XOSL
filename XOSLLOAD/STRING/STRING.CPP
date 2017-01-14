
extern "C" char GetDigit(int Digit);

char *ltoa(long Value, char *Str, int Base)
{
	char InvString[32];
	char *pStr;
	char *pOrig = Str;

	if (Base == 10 && Value < 0) {
		*Str++ = '-';
		Value = -Value;
	}

	pStr = InvString;
	do {
		*pStr++ = GetDigit(Value % Base);
		Value /= Base;
	} while (Value);

	while (pStr != InvString) {
		*Str++ = *--pStr;
	}
	*Str = '\0';
	return pOrig;
}

char *ultoa(unsigned long Value, char *Str, int Base)
{
	char InvString[32];
	char *pStr;
	char *pOrig = Str;

	pStr = InvString;
	do {
		*pStr++ = GetDigit(Value % Base);
		Value /= Base;
	} while (Value);

	while (pStr != InvString) {
		*Str++ = *--pStr;
	}
	*Str = '\0';
	return pOrig;
}

char *itoa(int Value, char *Str, int Base)
{
	return ltoa((long) Value, Str, Base);
}
