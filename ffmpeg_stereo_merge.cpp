// Russian comments, can be read with FAR. You can downlad files by "git clone https://github.com/Killaz/ffmpeg-stuff.git"

#include <stdio.h>
#include <conio.h>
// For getch()
#include <string.h>
// For strlen()
#include <stdlib.h>
// For system()
#include <time.h>
// For time()
#include <ctype.h>

#include "ffmpeg_prog.h"

#define elif else if
#define quit(a) \
	printf("%s; �����襭�� �ணࠬ��.\n", a); \
	system("pause"); \
	return 0;

#define printHelp() \
	printf("[-ffpath <string>] or [-ffpath <string with spaces>\"] - set path to ffmpeg's .exe file\n" \
	       "[-formatOut <string>] - force output format" \
	       "[-endless] - programm won't close after one sterepo-picture\n" \
	       "[-noUser] - menu won't be displayed, no questions will be asked\n" \
      );

#define lol() printf("Lol\n")

char format_in[15] = "", format_out[15] = "";

int main(int argc, char *argv[]) {
	char c = 0, tmp, file1[350] = "", file2[350] = "", str[2000], ffpath[300] = "C:\\ffmpeg\\bin\\ffmpeg.exe", outFilter[400] = "copy", outParams[400] = "-q:v 1", outDir[300] = "", postfix[30] = "-all";
	bool u = 0, user = 1, right_first = 1, forced_fout = 0;     // skip-parameter
	int i;
	char files = 0; // 1, �᫨ ���� 䠩� ������ ��� ��ࠬ���; 2, �᫨ ��ன
	time_t t;
//	Init:;
//	Start:
	system("cls");
// ������ ��ப� ��ࠬ��஢
	for (i = 1; i < argc; i++) {
		if (StrCmp(argv[i], "-endless") || StrCmp(argv[i], "/endless"))
			u = 1, printf("endless: on\n");
		elif (StrCmp(argv[i], "-ffpath") || StrCmp(argv[i], "/ffpath"))
			StrCat(ffpath, argv[++i]);
		elif (StrCmp(argv[i], "-format_out") || StrCmp(argv[i], "/format_out"))
			StrCat(format_out, argv[++i]);
		elif (StrCmp(argv[i], "-r_first") || StrCmp(argv[i], "/r_first"))
			right_first = 0;
		elif (Analize(argv[i], format_in) || argv[i][0] == '"') {
			StrClear(argv[i], strlen(format_in) + 1);
			if (files == 0)
				StrCat(file1, argv[i]), files = 1;
			elif (files == 1)
				StrCat(file2, argv[i]), files = 2;
			else
				printf("Unknown parameter: %s (input files already: %s and %s)\n", argv[i], file1, file2);
		} else      
			printf("Unknown parameter: %s\n", argv[i]);
	}
	if (StrCmp(format_out, ""))
		StrCat(format_out, format_in);
	Params:;
	while(user) {
		printf("����� ��ப� �맮�� �룫廊� ⠪:\n%s -i file1.format1 -i file2.format1 -filter_complex \"[1:v]pad=iw*2:ih[bg]; [bg][0:v]overlay=w[preout]; [preout] %s\" %s %sfile1%s.format2\n"
		       "������:\n\"path\" / \"p\", �⮡� �������� ���� �� ffmpeg.exe\n"
		       "\"outFilter\" / \"outFilters\" / \"of\", �⮡� �������� ��᫥���� 䨫��� ��室���� 䠩��\n"
		       "\"outParams\" / \"op\", �⮡� �������� ��ࠬ���� ��室���� 䠩��\n"
		       "\"outDir\" / \"od\", �⮡� �������� ����� �뢮��\n"
		       "\"postfix\" / \"pf\", �⮡� �������� ����䨪� ��᫥ ����� ��ࢮ�� 䠩�� � 䠩�� �뢮��\n"
		       "\"exit\" / \"stop\", �⮡� ��� �� �ணࠬ��\n"
		       "\"formatOut\" / \"fo\" / \"fout\" - �ਭ㤨⥫쭮� �������� ��室���� �ଠ�\n"
		       "(����� ��ப�) / \"start\" / \"go\", �⮡� ��३� � ����� 䠩���\n",
		       ffpath, outFilter, outParams, outDir, postfix);
		c = readWord(str, stdin);
		for (size_t i = 0; i < strlen(str); i++)
			str[i] = tolower(str[i]);
		if (StrCmp(str, "") || StrCmp(str, "go") || StrCmp(str, "start"))
			break;
		elif (StrCmp(str, "exit") || StrCmp(str, "stop")) {
			quit("Exiting");
		} elif (StrCmp(str, "path") || StrCmp(str, "p")) {
			if (c == 10)
				printf("������ ���� �� ffmpeg.exe (������ ᠬ \"ffmpeg.exe\"): ");
			read(ffpath, stdin);
		} elif (StrCmp(str, "outfilter") || StrCmp(str, "of") || StrCmp(str, "outfilters")) {
			if (c == 10)
				printf("������ 䨫���(�) ��室���� 䠩�� (��� ������⢨� 䨫��஢ - \"copy\"): ");
			read(outFilter, stdin);
		} elif (StrCmp(str, "outparams") || StrCmp(str, "op")) {
			if (c == 10)
				printf("������ ��ࠬ���� ��室���� 䠩��: ");
			read(outParams, stdin);
		} elif (StrCmp(str, "outdir") || StrCmp(str, "od")/* || StrCmp(str, "od_nr") || StrCmp(str, "outdir_norepair")*/) {
			if (c == 10)
				printf("������ ���� �� ����� �뢮��: ");
			read(outDir, stdin);
			if (outDir[strlen(outDir) - 1] != '\\') {
				outDir[strlen(outDir) + 1] = 0;
				outDir[strlen(outDir)] = '\\';
			}
		} elif (StrCmp(str, "formatout") || StrCmp(str, "fo") || StrCmp(str, "fout")) {
			if (c == 10)
				printf("������ �ଠ� ��室��� 䠩��� (��� �窨; ����� ��ப� - �� ������ �ଠ� �室��� 䠩���): ");
			read(format_out, stdin);
			if (!StrCmp(format_out, ""))
				forced_fout = 1;
			else
				forced_fout = 0, StrCat(format_out, format_in);
		} elif (StrCmp(str, "postfix") || StrCmp(str, "pf")) {
			if (c == 10)
				printf("������ ��ப�, ����� �㤥� �ਯ��뢠���� � ����� ��室���� 䠩�� (����� ��ப� - ��� ᮢ������ � ����ࠦ����� ��� ������ �����): ");
			read(postfix, stdin);
		} else
			printf("Can't recognize command: %s\n", str);
		printf("\n");
	}

	Opening:;
	if (files == 2) {
		files = 0;
		goto FFtry;
	}
	printf("���, �᫨ ����室���, �㦭� ������� � ����窠��. ����� \"����᪨����\" 䠩�� � ���᮫� ��� ����祭�� ���\n");
	if (files == 1) {
		files = 0;
		goto SecondFile;
	}

	printf("������ ���� �� ����ࠦ���� ��� ������ ����� (��� �������): ");
	read(file1, stdin);

	//search of commands:
	if (StrCmp(file1, "endless")) {
		u = !u, printf("endless: %s\n", u ? "on" : "off");
		goto Opening;
	} elif (StrCmp(file1, "exit")) {
		quit("Exiting");
	} elif (StrCmp(file1, "help")) {
		printf("\"exit\" for exit, \"endless\" turns on/off endless mode, \"params\" returns you to parameters change menu\n");
		goto Opening;
	} elif (StrCmp(file1, "params")) {
		goto Params;
	} elif (!Analize(file1, format_in)) {
		printf("Unknown command (or file without type): %s\n", file1);
		goto Opening;
	} else
		StrClear(file1, strlen(format_in) + 1);

	SecondFile:;
	printf("������ ���� �� ����ࠦ���� ��� �ࠢ��� �����: ");
	read(file2, stdin);
	StrClear(file2, strlen(format_in) + 1);
	if (!forced_fout)
		StrCat(format_out, format_in);
/*	printf("�����:\n1: %s.%s\n2: %s.%s\n(N - ������ ������� ���, Esc - ��室, Entrer / y - ����� ᮢ��饭��, \n",
	       right_first ? file1 : file2, format_in, right_first ? file2 : file1, format_in);
	c = getch();
	if (c == 27) {
		quit("Esc");
	}
	tmp = c;
	if (c == 'N' || c == 'n')
		goto Opening;
	if (c == 'B' || c == 'b')
		goto Params;
	if (c != 'F' && c != 'f' && c != 'K' && c != 'k')
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
//	Work:;*/
	FFtry:;
	if (file1[0] != '"') {
		file1[strlen(file1) + 1] = 0, format_in[strlen(format_in) + 1] = 0, format_in[strlen(format_in)] = '"';
		for (i = strlen(file1); i > 0; i--)
			file1[i] = file1[i - 1];
		file1[0] = '"';
		files = -1;                      // flag
	}
	if (file2[0] != '"') {
		if (files != -1) {
			printf("��� ������ ���� ���� ��� � ����窠�, ���� ��� ��� ���. �⪠�.\n");
			goto Opening;
		}
		file2[strlen(file2) + 1] = 0;
		for (i = strlen(file2); i > 0; i--)
			file2[i] = file2[i - 1];
		file2[0] = '"';
	}
	files = 0;

	printf("file1 = %s\n", file1);
	printf("file2 = %s\n", file2);
	                                                                                                                //colorlevels=rimin=0.082:gimin=0.082:bimin=0.082:rimax=0.9:gimax=0.9:bimax=0.9
	sprintf(str, "%s -i %s.%s -i %s.%s -filter_complex \"[1:v]pad=iw*2:ih[bg]; [bg][0:v]overlay=w[preout]; [preout] %s\" %s %s%s%s.%s",
	              ffpath, right_first ? file1 : file2, format_in, right_first ? file2 : file1, format_in, outFilter, outParams, outDir, right_first ? file1 : file2, postfix, format_out);
	printf("%s\n", str);
	NormalRus(str);
	if (system(str)) {
		printf("�訡�� �� ࠡ�� ffmpeg'�.");
		if (u) {
			printf(" �⪠�.\n");
			goto Opening;
		} else
			printf("\n");
	}
	if (u)
		goto Opening;
/*	printf("\n������ U ��� �த������� ᮧ����� ��८��� � ⠪��� �� ��ࠬ��ࠬ�, ���� ����� ������� ��� ��室�\n");
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
	}*/
	return 0;
}
