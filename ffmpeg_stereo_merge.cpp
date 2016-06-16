// Russian comments, can be read with FAR. You can downlad files by "git clone https://github.com/Killaz/ffmpeg-stuff.git"

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

#define printHelp() \
	printf("[endless] - programm won't close after two pictures\n" \
           "[exit] - close thr programm\n" \
           "[help] - print this help\n" \
      );

#define lol() printf("Lol\n")

char format[9] = "";

void StrCat(char *where, char *from) {
	int i = 0, j = 0;
	while (where[i] != 0)
		i++;
	do {
		where[i++] = from[j++];
	} while (from[j] != 0);
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

// �ࠢ������ �ப� s1 � ��ப�� s2
bool strCmp(const char *s1, const char *s2) {
	int len = strlen(s1);
	if (len != (int) strlen(s2))
		return 0;
	for (int i = 0; i < len; i++)
		if (s1[i] != s2[i])
			return 0;
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

// �����頥� 1, �᫨ ���� �ଠ� 䠩��, � , �᫨ ��� ���.
// � ��ࢮ� ��砥 ������� ��ப� format �� ����� �ଠ�.
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

// ��⠥� ��ப� �� ��७�� ��ப�
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

// ������� ��ப� �� args[] � where, ��稭�� � from-⮣� � �����稢�� len-�� ������⮬ args[],
// �᫨ ��� ��室���� ����� ���⮢� � ������ ᨬ������, ���� ���� ᨬ��� - �� ���⮢�. from �������.
bool mergeStrings(char **args, char *where, int &from, int len, char startSymb, char finishSymb) {
	if (from >= len)
		return 0;
	where[0] = 0;
	if (args[from][0] != startSymb) {
		StrCat(where, args[from]);
		return 1;
	}
	StrCat(where, &args[from++][1]);
	for (; from < len; from++) {
		int lenWhere = strlen(where);
		if (where[lenWhere - 1] == finishSymb) {
			StrClear(where, 1);
			return 1;
		}
		where[lenWhere++] = ' ';
		where[lenWhere++] = 0;
		StrCat(where, args[from]);
	}
	where[strlen(where) - 1] = 0;
	return 0;
}

int main(int argc, char *argv[]) {
	char c = 0, tmp, file1[120], file2[120], str[350], ffpath[90] = "C:\\ffmpeg\\bin\\ffmpeg.exe", param[120] = "-q:v 1";
	bool u = 0;     // skip-parameter
	int i;
	char first = 0; // 1, �᫨ ���� 䠩� ������ ��� ��ࠬ���; 2, �᫨ ��ன
	time_t t;
//	Init:;
	memset(file1, 0, sizeof(file1));
	memset(file2, 0, sizeof(file2));
//	Start:
	system("cls");
// ������ ��ப� ��ࠬ��஢
	for (i = 1; i < argc; i++) {
		if (strCmp(argv[i], "endless"))
			u = 1, printf("endless: on\n");
		elif (StrCmp(argv[i]. "ffpath"))
			if (i < argc - 1)
				StrCat(ffpath, argv[++i]);
			else
				printf("ffpath-parameter is used without path. Ignored.\n");
		elif (Analize(argv[i]) || argv[i][0] == '"') {
			if (first == 0)
				StrCat(file1, argv[i]), first = 1;
			elif (first == 1)
				StrCat(file2, argv[i]), first = 2;
			else
				printf("Unknown parameter: %s\n", argv[i]);
		} else      
			printf("Unknown parameter: %s\n", argv[i]);
	}
	printf("���, �᫨ ����室���, �㦭� ������� � ����窠��. ����� \"����᪨����\" 䠩�� � ���᮫� ��� ����祭�� ���\n");
	Opening:;
	
	if (first == 2) {
		first = 0;
		goto FFtry;
	}
	if (first == 1) {
		first = 0;
		goto SecondFile;
	}

	printf("������ ���� �� ����ࠦ���� ��� ������ ����� (��� �������): ");
	read(file1, stdin);

	//search of commands:
	if (strCmp(file1, "endless")) {
		u ^= 1, printf("endless: %s\n", u ? "on" : "off");
		goto Opening;
	} elif (strCmp(file1, "exit")) {
		return 0;
	} elif (strCmp(file1, "help")) {
		printHelp();
		goto Opening;
	} elif (!Analize(file1)) {
		printf("Unknown command (or file without type): %s\n", file1);
		goto Opening;
	}

	SecondFile:;
	printf("������ ���� �� ����ࠦ���� ��� �ࠢ��� �����: ");
	read(file2, stdin);
	StrClear(file2, strlen(format) + 1);
	if (u)
		goto FFtry;
	printf("�����:\n1: %s.%s\n2: %s.%s\n(N - ������ ������� ���, Esc - ��室, F - 㪠���� �ᯮ������� ffmpeg'� (�᫨ �� C:\\ffmpeg\\bin), \n"
	       "B - ������ ��ப� ��ࠬ��஢ (�᫨ �� -q:v 1), K - F+B:\n", file1, format, file2, format);
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
	if (file1[0] != '"') {
		file1[strlen(file1) + 1] = 0, format[strlen(format) + 1] = 0,format[strlen(format)] = '"';
		for (i = strlen(file1); i > 0; i--)
			file1[i] = file1[i - 1];
		file1[0] = '"';
		first = -1;                      // flag
	}
	if (file2[0] != '"') {
		if (first != -1) {
			printf("��� ������ ���� ���� ��� � ����窠�, ���� ��� ��� ���. �⪠�.\n");
			goto Opening;
		}
		file2[strlen(file2) + 1] = 0;
		for (i = strlen(file2); i > 0; i--)
			file2[i] = file2[i - 1];
		file2[0] = '"';
	}
	first = 0;

	printf("file1 = %s\n", file1);
	printf("file2 = %s\n", file2);
	sprintf(str, "%s -i %s.%s -i %s.%s -filter_complex \"[0:v]pad=iw*2:ih[bg]; [bg][1:v]overlay=w,colorlevels=rimin=0.082:"
	             "gimin=0.082:bimin=0.082:rimax=0.9:gimax=0.9:bimax=0.9\" %s %s-all.%s", ffpath, file2, format, file1, format, param, file1, format);
	printf("%s\n", str);
	if (system(str)) {
		printf("�訡�� �� ࠡ�� ffmpeg'�.");
		if (u) {
			printf(" �⪠�.");
			goto Opening;
		} else
			printf("\n");
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
