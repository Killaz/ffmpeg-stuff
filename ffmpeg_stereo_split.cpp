#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define elif else if
#define quit(a) \
	printf("%s; �����襭�� �ணࠬ��.\n", a); \
	system("pause"); \
	return 0;
//#define file "%userprofile%/Documents/Tree.txth"
#define lol() printf("Lol\n")

typedef unsigned char uchar;

char format[9] = "";

void StrCat(char *where, char *from) {
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

bool StrCompare(char *where, char *what, int from) {
	int len = strlen(what), len1 = strlen(where);
	for (int i = 0; i < len; i++) {
		if (from + i >= len1 || where[from + i] != what[i])
			return 0;
	}
	return 1;
}

bool StrClear(char *from, int hmuch) { // ��१��� �� ���� ��ப� from hmuch ᨬ�����
	int len = strlen(from);
	if (len < hmuch)
		return 0;
	for (int i = len - hmuch; i < len; i++)
		from[i] = 0;
	return 1;
}

bool Analize(char *from) {
	memset(format, 0, sizeof(format));
	for (int i = strlen(from) - 1; i > 0; i--)
		if (from[i] == '.') {
			StrCat(format, from + i + 1);
			StrClear(from, strlen(format) + 1);
			return 1;
		}
	return 0;
}

bool read(char *s, FILE *f) {
	int i = 0;
	while (1) {
		fscanf(f, "%c", &s[i]);
		if (s[i] == '\n') {
			s[i] = 0;
			break;
		} else
			i++;
	}
	return StrCompare(s, format, strlen(s) - strlen(format));
}

int main() {
	char c = 0, tmp, file1[120], *file2 = NULL, str[350], ffpath[90] = "C:\\ffmpeg\\bin\\ffmpeg.exe", param[120] = "-q:v 1";
	bool u = 0; // skip-parameter             
	time_t t;
//	Init:;
	memset(file1, 0, sizeof(file1));
//	Start:
	system("cls");
	printf("����, �᫨ ����室���, �㦭� ������� � ����窠��. ����� \"�������\" 䠩� � ���᮫� ��� ����祭�� ���\n");
	Opening:;
	printf("������ ���� �� 䠩�� (��८���⨭��/����� ��� ࠧ������� �� ��� ����ࠦ����/�����): ");
	read(file1, stdin);
	if (!Analize(file1)) {
		quit("�訡��: �� 㤠���� �ᯮ����� �ଠ� 䠩��");
	}
	if (u)
		goto FFtry;
	printf("����: %s.%s\n(N - ������ ������� ����, Esc - ��室, F - 㪠���� �ᯮ������� "
	       "ffmpeg'� (�᫨ �� C:\\ffmpeg\\bin), B - ������ ��ப� ��ࠬ��஢ (�᫨ �� -q:v 1), K - F+B:\n", file1, format);
	c = getch();
	if (c == 27) {
		quit("Esc");
	}
	tmp = c;
	if (c == 'N' || c == 'n' || c == '�' || c == '�')
		goto Opening;		
	if (c == 'B' || c == 'b' || c == '�' || c == '�')
		goto Params;
	if (c != 'F' && c != 'f' && c != '�' && c != '�' && c != 'K' && c != 'k' && c != '�' && c != '�')
		goto FFtry;
//	Prepare:;
	Pathing:;
	printf("ffmpeg.exe ����� ���� �� ����� \"%s\"?\n", ffpath);
	c = getch();
    printf("%c\n", c);
    if (c != 'y' && c != 'Y' && c != '�' && c != '�' && c != 13) {
    	printf("������ ���� �� ffmpeg.exe: ");
    	read(ffpath, stdin);
    	goto Pathing;
	}	
	if (tmp != 'K' && tmp != 'k' && tmp != '�' && tmp != '�')
		goto FFtry;
//	Last_prepare:;
	Params:;
	printf("����� ��ࠬ���� �룫��� ⠪: %s\n�ᯮ�짮���� ��� ��ப�? � ��砥 �⪠�� (�� Y ��� Enter) "
	       "��� �㤥� �।������ ����� ᢮� ��ப� ��ࠬ��஢; Esc - ��室\n", param);
	c = getch();
	if (c == 27) {
		quit("Esc");
	} else
		printf("%c\n", c);
	if (c == 'n' || c == 'N' || c == '�' || c == '�') {
		printf("������ ����� ��ப� ��ࠬ��஢: ");
		read(param, stdin);
		goto Params;
	}
//	Work:;
	FFtry:;
	memset(str, 0, sizeof(str));
	if (StrCompare(file1, "-all", strlen(file1) - 4)) {
		if ((file2 = (char *) malloc((strlen(file1) - 4) * sizeof(char))) == NULL)
			file2 = file1;
		else {
			file2[0] = 0;
			StrCatN(file2, file1, 0, strlen(file1) - 4);
		}
	} else
		file2 = file1;
	sprintf(str, "%s -i %s.%s -filter_complex \"[0:v]crop=iw/2:ih:0:0\" %s %s-left.%s", ffpath, file1, format, param, file2, format);
	printf("%s\n", str);
	if (system(str)) {
		quit("�訡�� �� ࠡ�� ffmpeg'� - ᮧ����� ��ࢮ�� 䠩��");
	}
	sprintf(str, "%s -i %s.%s -filter_complex \"[0:v]crop=iw/2:ih:iw/2:0\" %s %s-right.%s", ffpath, file1, format, param, file2, format);
	printf("%s\n", str);
	if (system(str)) {
		quit("�訡�� �� ࠡ�� ffmpeg'� - ᮧ����� ��ࢮ�� 䠩��");
	}
	if (u)
		goto Opening;
	printf("\n������ U ��� �த������� ᮧ����� ��८��� � ⠪��� �� ��ࠬ��ࠬ�, ���� ����� ������� ��� ��室�\n");
	t = time(0);
	while (time(0) - t < 10) {
		if (kbhit()) {
			c = getch();
			if (c == 'u' || c == 'U' || c == '�' || c == '�') {
				u = 1;
				goto Opening;
			}
			return 0;
		}
	}
	return 0;
}
