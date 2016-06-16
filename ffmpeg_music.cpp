// Russian comments, can be read with FAR. You can downlad files by "git clone https://github.com/Killaz/ffmpeg-stuff.git"

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define elif else if
#define quit(a) \
	printf("%s; �����襭�� �ணࠬ��.\n", a); \
	if (off) { \
		system("shutdown /s /t 120"); \
		printf("�������� �㤥� �몫�祭 �१ 120 ᥪ㭤.\n"); \
	} \
	system("pause"); \
	return 0;
//#define file "%userprofile%/Documents/Tree.txth"

#define printHelp() \
	printf("[-param <string>] or [-param \"<string with spaces>\"] - give parameters to ffmpeg\n" \
	       "[-formats <format_from> <format_to>] - set coding from format_from into format_to\n" \
	       "[-ffpath <string>] or [-ffpath <string with spaces>\"] - set ffmpeg.exe's directory\n" \
	       "[-off] - turns off computer after finishing\n" \
	       "[-u] - skip errors automaticly (automaticly turned on with [-off])\n" \
	       "[-dirs *<string> [string] .. *] - set directories where ffmpeg with these parameters will be started\n" \
	      );

#define lol() printf("Lol\n")

char format1[9] = "mp3", format2[9] = "mp3";

void StrCat(char *where, const char *from) {
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
	return StrCompare(s, format1, strlen(s) - strlen(format1));
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
	bool u = 0, off = 0, user = 1;
	int i, cnt;
	char c = 0, prof[90], file[90], str[250], ffpath[90], param[80] = "-map 0:a -b:a 112k -ac 2";// = "C:\\ffmpeg\\bin\\ffmpeg.exe";
	time_t t, t1;
	FILE *in;
//	Init:;
    t = time(0);
	memset(ffpath, 0, sizeof(ffpath));
	StrCat(ffpath, "C:\\ffmpeg\\bin\\ffmpeg.exe");
	memset(prof, 0, sizeof(prof));
	StrCat(prof, getenv("USERPROFILE"));
	memset(file, 0, sizeof(file));
	sprintf(file, "%s\\Documents\\Tree_%I64d.txth", prof, t);
//	Start:
	system("cls");

	// ������ ��ப� ��ࠬ��஢
	for (i = 1; i < argc; i++) {
		if (strCmp(argv[i], "-u"))
			u = 1;
		elif (strCmp(argv[i], "-param"))
			mergeStrings(argv, param, ++i, argc, '"', '"');
		elif (strCmp(argv[i], "-formats")) {
			mergeStrings(argv, format1, ++i, argc, '"', '"');
			mergeStrings(argv, format2, ++i, argc, '"', '"');
		} elif (strCmp(argv[i], "-user"))
			user = 0, u = 1;
		elif (strCmp(argv[i], "-help")) {
			printHelp();
			return 0;
		} elif (strCmp(argv[i], "-off"))
			off = 1;
		elif (strCmp(argv[i], "-ffpath"))
			mergeStrings(argv, ffpath, ++i, argc, '"', '"');
	}

/*	printf("�ணࠬ�� ��室���� � ����� � ��몮�?\n");
	c = getch();
	if (c == 27) {
		printf("Esc\n");
		quit("����⨥ Esc");
	} elif (c != 'y' && c != 'Y' && c != '�' && c != '�' && c != 13) {
		printf("%c\n", c);
		quit("�� �뫮 ����祭� ���⢥ত����");
	}
	printf("%c\n", c);*/
	Format:;
	if (user) {
		printf("���� ��ॣ������� 䠩�� �� *.%s � *.%s?\n", format1, format2);
		c = getch();
		printf("%c\n", c);
		if (c != 'y' && c != 'Y' && c != '�' && c != '�' && c != 13) {
			printf("������ ���७�� (���ਬ��, \"flac mp3\", \"wma m4a\" ��� ����祪): ");
			scanf("%s %s", format1, format2);
			goto Format;
		}
	}
	Opening:;
	memset(str, 0, sizeof(str));
	StrCat(str, "tree /f /a >> ");
	StrCat(str, file);
	system(str);
	if ((in = fopen(file, "rt")) == NULL) {
		fclose(stdin);
		printf("�⪠���� � ����㯥 � ����� ��� ᮧ����� �६������ 䠩��. ������ 'Y', �᫨ ��� ����� ���� "
		       "� ����� � ����㯮�, � ���� ����� ������� � ��㣮� ��砥.\n");
		if (!user)
			return 1;
		c = getch();
		printf("%c\n", c);
		if (c == 'y' || c == 'Y' || c == '�' || c == '�' || c == 13) {
			memset(file, 0, sizeof(file));
			i = c = 0;
			printf("������ ���� �� �����: ");
			while (c != '\n' /*  Or Esc  */) {
				scanf("%c", &file[i++]);
			}
			if (c == '\n') {
				file[i - 1] = 0;
				sprintf(file, "%s\\Tree_%I64d.txth", file, t);
				goto Opening;
			} else {
				quit("����⨥ Esc");
			}
		} else {
			quit("�� ����祭� ���⢥ত����");
		}
	}
//	Prepare:;
	i = 0;
    while (i < 3) {
    	fscanf(in, "%c", &c);
    	if (c == '\n')
    		i++;
    }
    cnt = 0;
    while (1) {
    	fscanf(in, "%c%*c%*c%*c", &c);
    	if (c != '|' && c != ' ')
    		break;
    	if (read(str, in))
    		cnt++;
    }
    if (user) {
	    printf("�����㦥�� %d 䠩���. ��୮?\n", cnt);
	    c = getch();
	    printf("%c\n", c);
	    if (c != 'y' && c != 'Y' && c != '�' && c != '�' && c != 13) {
	    	printf("�� �뫮 ����祭� ࠧ�襭�� �����\n");
	    	goto Deleting;
		}
	}
	Pathing:;
	if (user) {
		printf("ffmpeg.exe ����� ���� �� ����� \"%s\"?\n", ffpath);
		c = getch();
	    printf("%c\n", c);
	    if (c != 'y' && c != 'Y' && c != '�' && c != '�' && c != 13) {
	    	printf("������ ���� �� ffmpeg.exe: ");
	    	read(ffpath, stdin);
	    	goto Pathing;
		}	
	}
//	Last_prepare:;
	rewind(in);
    i = 0;
    while (i < 3) {
    	fscanf(in, "%c", &c);
    	if (c == '\n')
    		i++;
    }
    memset(str, 0, sizeof(str));
    sprintf(str, "mkdir \"Output %I64d\"", t);
    if (system(str)) {
    	printf("�訡�� �� ᮧ����� ����� ��室��� 䠩���.\n");
    	goto Deleting;
	}
	Params:;
	if (user) {
		printf("����� ��ப� �������樨 �룫廊� ⠪: %s -i \"<filename>.%s\" %s \"Output %I64d\\<filename>.%s\"\n�ᯮ�짮���� ��� ��ப�? "
		       "� ��砥 �⪠�� (�� Y ��� Enter) ��� �㤥� �।������ ����� ᢮� ��ப� ��ࠬ��஢; Esc - ��室\n", ffpath, format1, param, t, format2);
		c = getch();
		if (c == 27) {
			printf("Esc\n");
			goto FullDeleting;
		} else
			printf("%c\n", c);
		if (c == 'n' || c == 'N' || c == '�' || c == '�') {
			printf("������ ����� ��ப� ��ࠬ��஢: ");
			read(param, stdin);
			goto Params;
		}
		printf("�� �몫���� �������� ��᫥ �����襭��? (Esc, N - �몫�祭��)\n");
		c = getch();
		if (c == 'n' || c == 27 || c == 'N' || c == '�' || c == '�')
			off = 1;
	}
	//	Work:;
    i = 0;
    while (1) {
    	fscanf(in, "%c%*c%*c%*c", &c);
    	if (c != '|' && c != ' ') {              //Completed (?)
    		fscanf(in, "%s%s", str, prof);
    		printf("%c%s %s\n", c, str, prof);
    		break;
		}
    	if (read(prof, in)) {                    //prof - name.format
    		system("cls");
    		StrClear(prof, strlen(format1) + 1); //prof - name
    		FFtry:;
    		printf("%d �� %d ��४���஢���, ��稭��� %d:\n", i, cnt, i + 1);
    		memset(str, 0, sizeof(str));
    		sprintf(str, "%s -i \"%s.%s\" %s \"Output %I64d\\%s.%s\"", ffpath, prof, format1, param, t, prof, format2);
    		printf("%s\n", str);
    		if (system(str) && !u) {
    			printf("�訡�� �� ࠡ�� ffmpeg'�. ������ R ��� �����, Esc ��� ��室�, ���� ������� ��� �ய�᪠ � U ��� �ய�᪠ ��� �訡�� ������� ⨯�\n");
    			t1 = time(0);
    			c = 0;
    			while (time(0) - t1 < 20)
	    			if (kbhit()) {
		    			c = getch();
		    			if (c != 27)
		    				printf("%c\n", c);
						if (c == 'r' || c == 'R' || c == '�' || c == '�')
							goto FFtry;
						elif (c == 27) {
							printf("Esc\n������� ��४���஢���� �� ����� ������ 䠩�� � �����?\n");
							c = getch();
							if (c == 'y' || c == 'Y' || c == '�' || c == '�' || c == 13)
								goto FullDeleting;
							else
								goto Deleting;
						} elif (c == 'u' || c == 'U' || c == '�' || c == '�')
							u = 1;
						c = 1;
					}
				if (c == 0)
					u = 1, i++;
	    	} else
    			i++;
    	}
    }
    printf("%d �� %d �뫨 ��४���஢��� ��� �訡�� (��� � �� ���-�����஢�����)\n", i, cnt);
    if (0) {
    	FullDeleting:;
    	sprintf(str, "rmdir \"Output %I64d\" /Q /S", t);
		system(str);
    }
	Deleting:;
	fclose(in);
	memset(str, 0, sizeof(str));
	StrCat(str, "del ");
	StrCat(str, file);
	if (!system(str)) {
		quit("�६���� 䠩� �� 㤠���");
	} else {
		quit("�訡�� �� 㤠����� �६������ 䠩��");
	}
	return 0;
}
