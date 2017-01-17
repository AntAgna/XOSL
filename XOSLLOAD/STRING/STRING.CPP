
extern "C" char GetDigit(int Digit);

char *itoa(long Value, char *Str, int Base)
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
