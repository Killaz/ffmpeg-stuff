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

#define VERSION "1.0.0"

#define quit(a) \
	printf("%s; �����襭�� �ணࠬ��.\n", a); \
	system("pause"); \
	return 0;

#define printHelp() \
	printf("ffmpeg_stereo_merge version: " VERSION "\n" \
	       "[-ffpath <string>] or [-ffpath <string with spaces>\"] - set path to ffmpeg's .exe file\n" \
	       "[-formatOut <string>] - force output format" \
	       "[-endless] - programm won't close after one sterepo-picture\n" \
	       "[-noUser] - menu won't be displayed, no questions will be asked\n" \
	       "[-r_first] - first input is content for right eye, then left (not recommended)\n" \
      );

#define lol() printf("Lol\n")

char format_in[15] = "", format_out[15] = "";

int main(int argc, char *argv[]) {
	char c = 0, /*tmp, */file1[350] = "", file2[350] = "", str[2000], ffpath[300] = "C:\\ffmpeg\\bin\\ffmpeg.exe", outFilter[400] = "copy", outParams[400] = "-q:v 1", outDir[300] = "", postfix[30] = "-all";
	bool endless = 0, user = 1, right_first = 1, forced_fout = 0;     // endless-parameter
	int i;
	char files = 0; // 1, �᫨ ���� 䠩� ������ ��� ��ࠬ���; 2, �᫨ ��ன
//	time_t t;
//	Init:;
//	Start:
	system("cls");
// ������ ��ப� ��ࠬ��஢
	for (i = 1; i < argc; i++) {
		if (StrCmp(argv[i], 2, "-endless", "/endless"))
			endless = 1, printf("endless: on\n");
		else if (StrCmp(argv[i], 2, "-ffpath", "/ffpath"))
			StrCat(ffpath, argv[++i]);
		else if (StrCmp(argv[i], 2, "-format_out", "/format_out"))
			StrCat(format_out, argv[++i]);
		else if (StrCmp(argv[i], 2, "-r_first", "/r_first"))
			right_first = 0;
		else if (Analize(argv[i], format_in) || argv[i][0] == '"') {
			StrClear(argv[i], strlen(format_in) + 1);
			if (files == 0)
				StrCat(file1, argv[i]), files = 1;
			else if (files == 1)
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
		printf("����� ��ப� �맮�� �룫廊� ⠪ (endless: %s):\n%s -i file1.format1 -i file2.format1 -filter_complex \"[1:v]pad=iw*2:ih[bg]; [bg][0:v]overlay=w[preout]; [preout] %s\" %s %sfile1%s.%s\n"
		       "������:\n\"path\" / \"p\", �⮡� �������� ���� �� ffmpeg.exe\n"
		       "\"outFilter\" / \"outFilters\" / \"of\", �⮡� �������� ��᫥���� 䨫��� ��室���� 䠩��\n"
		       "\"outParams\" / \"op\", �⮡� �������� ��ࠬ���� ��室���� 䠩��\n"
		       "\"outDir\" / \"od\", �⮡� �������� ����� �뢮��\n"
		       "\"postfix\" / \"pf\", �⮡� �������� ����䨪� ��᫥ ����� ��ࢮ�� 䠩�� � 䠩�� �뢮��\n"
		       "\"formatOut\" / \"fo\" / \"fout\" - �ਭ㤨⥫쭮� �������� ��室���� �ଠ�\n"
		       "\"endless\" / \"el\" - �ணࠬ�� �� �㤥� �����襭� ��⮬���᪨\n"
		       "\"exit\" / \"stop\", �⮡� ��� �� �ணࠬ��\n"
		       "(����� ��ப�) / \"start\" / \"go\", �⮡� ��३� � ����� 䠩���\n",
		       endless ? "on" : "off", ffpath, outFilter, outParams, outDir, postfix, forced_fout ? format_out : "format1");
		c = readWord(str, stdin);
		for (size_t i = 0; i < strlen(str); i++)
			str[i] = tolower(str[i]);
		if (StrCmp(str, 3, "", "go", "start"))
			break;
		else if (StrCmp(str, 2, "exit", "stop")) {
			quit("Exiting");
		} else if (StrCmp(str, 2, "path", "p")) {
			if (c == 10)
				printf("������ ���� �� ffmpeg.exe (������ ᠬ \"ffmpeg.exe\"): ");
			read(ffpath, stdin);
		} else if (StrCmp(str, 3, "outfilter", "of", "outfilters")) {
			if (c == 10)
				printf("������ 䨫���(�) ��室���� 䠩�� (��� ������⢨� 䨫��஢ - \"copy\"): ");
			read(outFilter, stdin);
		} else if (StrCmp(str, 3, "outparams", "op", "outparam")) {
			if (c == 10)
				printf("������ ��ࠬ���� ��室���� 䠩��: ");
			read(outParams, stdin);
		} else if (StrCmp(str, 2, "outdir", "od")/* || StrCmp(str, "od_nr") || StrCmp(str, "outdir_norepair")*/) {
			if (c == 10)
				printf("������ ���� �� ����� �뢮��: ");
			read(outDir, stdin);
			if (outDir[strlen(outDir) - 1] != '\\') {
				outDir[strlen(outDir) + 1] = 0;
				outDir[strlen(outDir)] = '\\';
			}
		} else if (StrCmp(str, 3, "formatout", "fo", "fout")) {
			if (c == 10)
				printf("������ �ଠ� ��室��� 䠩��� (��� �窨; ����� ��ப� - �� ������ �ଠ� �室��� 䠩���): ");
			read(format_out, stdin);
			if (!StrCmp(format_out, ""))
				forced_fout = 1;
			else
				forced_fout = 0, StrCat(format_out, format_in);
		} else if (StrCmp(str, 2, "postfix", "pf")) {
			if (c == 10)
				printf("������ ��ப�, ����� �㤥� �ਯ��뢠���� � ����� ��室���� 䠩�� (����� ��ப� - ��� ᮢ������ � ����ࠦ����� ��� ������ �����): ");
			read(postfix, stdin);
		} else if (StrCmp(str, 2, "endless", "el")) {
			endless = !endless;
		} else
			printf("Can't recognize command: %s\n", str);
		printf("\n");
	}

	Opening:;
	if (files == 2) {
		files = 0;
		goto FFtry;
	}
	printf("\n���, �᫨ ����室���, �㦭� ������� � ����窠��. ����� \"����᪨����\" 䠩�� � ���᮫� ��� ����祭�� ���\n");
	if (files == 1) {
		files = 0;
		goto SecondFile;
	}

	printf("������ ���� �� ����ࠦ���� ��� ������ ����� (��� �������): ");
//	read(file1, stdin);
/*	while (1) {
		c = fgetc(stdin);
		if (c != '\n') {
			ungetc(c, stdin);
			break;
		}
	}*/
	if ((c = readQuote(file1, stdin)) == 2) {
		c = fgetc(stdin);
		while (1) {
			if (c == '"') {
				ungetc(c, stdin);
				break;
			} else if (c == '\n') {
				c = -2;
				break;
			}
		}
	} else if (c == 1)
		c = -2;
	//search of commands:
	if (StrCmp(file1, "endless")) {
		endless = !endless, printf("endless: %s\n", endless ? "on" : "off");
		goto Opening;
	} else if (StrCmp(file1, "exit")) {
		quit("Exiting");
	} else if (StrCmp(file1, "help")) {
		printf("\"exit\" for exit, \"endless\" turns on/off endless mode, \"params\" returns you to parameters change menu\n");
		goto Opening;
	} else if (StrCmp(file1, "params")) {
		goto Params;
	} else if (!Analize(file1, format_in)) {
		printf("Unknown command (or file without type): %s\n", file1);
		goto Opening;
	} else
		StrClear(file1, strlen(format_in) + 1);

	SecondFile:;
	if (c == -2)
		printf("������ ���� �� ����ࠦ���� ��� �ࠢ��� �����: ");
//	read(file2, stdin);
	if (readQuote(file2, stdin) == 2)
		fgetc(stdin);
	StrClear(file2, strlen(format_in) + 1);
	if (!forced_fout)
		StrCopy(format_out, format_in);
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
		if (endless) {
			printf(" �⪠�.\n");
			goto Opening;
		} else
			printf("\n");
	}
	if (endless)
		goto Opening;
	printf("\n������ U ��� �த������� ᮧ����� ��८��� � ⠪��� �� ��ࠬ��ࠬ�, ���� ����� ������� ��� ��室�\n");
	time_t t = time(0);
	while (time(0) - t < 10) {
		if (kbhit()) {
			c = getch();
			if (c == 'u' || c == 'U') {
				endless = 1;
				goto Opening;
			}
			return 0;
		}
	}
	return 0;
}
