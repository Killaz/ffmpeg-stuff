#ifndef FFMPEG_PROG_H
#define FFMPEG_PROG_H

void StrCat(char *where, const char *from) {
	int i = 0, j = 0;
	while (where[i] != 0)
		i++;
	do {
		where[i++] = from[j++];
	} while (from[j] != 0);
}

void StrCatN(char *where, char *from, int start, int finish) {
	int i = 0, j = start;
	while (where[i] != 0)
		i++;
	while (from[j] != 0 && j < finish)
		where[i++] = from[j++];
	where[i] = 0;
}

// �ࠢ������ �ப� s1 � ��ப�� s2
bool StrCmp(const char *s1, const char *s2) {
	int len = strlen(s1);
	if (len != (int) strlen(s2))
		return 0;
	for (int i = 0; i < len; i++)
		if (s1[i] != s2[i])
			return 0;
	return 1;
}

// �ࠢ������ ��ப� where � ��ப�� from,��稭�� � from-⮣� ᨬ����
bool StrCompare(const char *where, const char *what, int from) {
	int len = strlen(what), len1 = strlen(where);
	for (int i = 0; i < len; i++) {
		if (from + i >= len1 || where[from + i] != what[i])
			return 0;
	}
	return 1;
}

// ��१��� �� ���� ��ப� from hmuch ᨬ�����
bool StrClear(char *from, int hmuch) {
	int len = strlen(from);
	if (len < hmuch)
		return 0;
	for (int i = len - hmuch; i < len; i++)
		from[i] = 0;
	return 1;
}

bool read(char *s, FILE *f) {
	int i = 0;
	if (f == NULL)
		return 0;
	else
	while (1) {
		if (fscanf(f, "%c", &s[i]) < 1)
			break;
		if (s[i] == '\n') {
			s[i] = 0;
			break;
		} else
			i++;
	}
	if (i == 0)
		return 0;
	else
		return 1;
}

// �����頥� 1, �᫨ ���� �ଠ� 䠩��, � 0, �᫨ ��� ���.
// � ��ࢮ� ��砥 ������� ��ப� format �� ����� �ଠ�.
bool Analize(char *from, char *format) {
	memset(format, 0, sizeof(format));
	for (int i = strlen(from) - 1; i > 0; i--)
		if (from[i] == '.') {
			StrCat(format, from + i + 1);
			return 1;
		}
	return 0;
}

void NormalRus(char *s) {
	for (size_t i = 0; i < strlen(s); i++) {
		if ((s[i] >= '�' && s[i] <= '�') || (s[i] >= '�' && s[i] <= '�'))
			s[i] -= 192;
		else if (s[i] >= '�' && s[i] <= '�')
			s[i] -= 240;
		else if (s[i] == '�')
			s[i] -= 57;
		else if (s[i] == '�')
			s[i] -= 72;
	}
}
#endif
