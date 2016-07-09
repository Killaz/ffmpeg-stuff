// Russian comments, can be read with FAR. You can downlad files by "git clone https://github.com/Killaz/ffmpeg-stuff.git"

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "ffmpeg_prog.h"

#define elif else if
#define quit(a) \
	printf("%s; �����襭�� �ணࠬ��.\n", a); \
	system("pause"); \
	return 0;

#define printHelp() \
	printf("[endless] - programm won't close after one picture\n" \
	       "[reverse] - change left with right (needed for Oculus, for example)\n" \
           "[exit] - close thr programm\n" \
           "[help] - print this help\n" \
      );

#define lol() printf("Lol\n")

char format[9] = "";

int main(int argc, char *argv[]) {
	char c = 0, tmp = 0, file1[120], *file2 = NULL, str[350], ffpath[90] = "C:\\ffmpeg\\bin\\ffmpeg.exe", param[120] = "-q:v 1";
	char first = 0; // 1, ����� �� ��ࠬ��஢����祭� ��� 䠩��
	bool u = 0;     // skip-parameter             
	int i;
	bool rev = 0;
	time_t t;
//	Init:;
	memset(file1, 0, sizeof(file1));
//	Start:
	system("cls");
// ������ ��ப� ��ࠬ��஢
	for (i = 1; i < argc; i++) {
		if (StrCmp(argv[i], "endless"))
			u = 1, printf("endless: on\n");
		elif (StrCmp(argv[i], "reverse"))
			rev = 1, printf("reverse: on\n");
		elif (StrCmp(argv[i], "ffpath"))
			if (i < argc - 1)
				StrCat(ffpath, argv[++i]);
			else
				printf("ffpath-parameter is used without path. Ignored.\n");
		elif (Analize(argv[i], format) || argv[i][0] == '"') {
			StrClear(argv[i], strlen(format) + 1);
			if (first == 0)
				StrCat(file1, argv[i]), first = 1;
			elif (first == 1)
				StrCat(file2, argv[i]), first = 2;
			else
				printf("Unknown parameter: %s\n", argv[i]);
		} else      
			printf("Unknown parameter: %s\n", argv[i]);
	}
	if (first == 1) {
		first = 0;
		if (u)
			goto FFtry;
		else
			goto Pathing;
	}

	printf("����, �᫨ ����室���, �㦭� ������� � ����窠��. ����� \"�������\" 䠩� � ���᮫� ��� ����祭�� ���\n");
	Opening:;
	printf("������ ���� �� 䠩�� (��८���⨭��/����� ��� ࠧ������� �� ��� ����ࠦ����/�����): ");
	read(file1, stdin);

	if (StrCmp(file1, "endless")) {
		u ^= 1, printf("endless: %s\n", u ? "on" : "off");
		goto Opening;
	} elif (StrCmp(file1, "exit")) {
		return 0;
	} elif (StrCmp(file1, "help")) {
		printHelp();
		goto Opening;
	} elif (!Analize(file1, format)) {
		printf("Unknown command (or file without type): %s\n", file1);
		goto Opening;
	} else
		StrClear(file1, strlen(format) + 1);

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

	if (file1[0] != '"') {
		file1[strlen(file1) + 1] = 0, format[strlen(format) + 1] = 0,format[strlen(format)] = '"';
		for (i = strlen(file1); i > 0; i--)
			file1[i] = file1[i - 1];
		file1[0] = '"';
	}

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
	
	sprintf(str, "%s -i %s.%s -filter_complex \"[0:v]crop=iw/2:ih:0:0\" %s %s-%s.%s", 
	        ffpath, file1, format, param, file2, rev ? "left" : "right", format);
	printf("%s\n", str);
	NormalRus(str);
	if (system(str)) {
		quit("�訡�� �� ࠡ�� ffmpeg'� - ᮧ����� ��ࢮ�� 䠩��");
	}
	sprintf(str, "%s -i %s.%s -filter_complex \"[0:v]crop=iw/2:ih:iw/2:0\" %s %s-%s.%s",
	        ffpath, file1, format, param, file2, rev ? "right" : "left", format);
	printf("\nSTART: %s\n", str);
	NormalRus(str);
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
