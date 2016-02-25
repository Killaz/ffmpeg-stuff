#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define elif else if
#define quit(a) \
	printRusComment(a); \
	printRusComment("; ���������� ���������. ������� ����� ������ ��� �������������.\n"); \
	getch(); \
	return 0;

#define lol() printf("Lol\n")

char format[9] = "";

void printRusComment(char *str) {
	int len = strlen(str);
	char s[800];
//	char *s = (char *) malloc(len * sizeof(char));
	if (s == NULL)
		return (void) -1;
	for (int i = 0; i <= len; i++)
		s[i] = str[i];
	for (int i = 0; i < len; i++) {
		if ((s[i] >= '�'&& s[i] <= '�') || (s[i] >= '�' && s[i] <= '�'))
			s[i] += 192;
		else if (s[i] >= '�' && s[i] <= '�')
			s[i] += 240;
		else if (s[i] == '�')
			s[i] += 57;
		else if (s[i] == '�')
			s[i] += 72;
	}
	printf("%s", s);
//	free(s);
}

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

bool StrClear(char *from, int hmuch) { // �������� �� ����� ������ from hmuch ��������
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
	char c = 0, tmpC, file1[120], *file2 = NULL, str[350], ffpath[90] = "C:\\ffmpeg\\bin\\ffmpeg.exe", param[120] = "-q:v 1 -qmax 10", tmp[800];
	bool u = 0; // skip-parameter             
	time_t t;
//	Init:;
	memset(file1, 0, sizeof(file1));
//	Start:
	system("cls");
	printRusComment("����, ���� ����������, ����� ������� � ���������. ����� \"����������\" ���� � ������� ��� ��������� ����\n");
	Opening:;
	sprintf(tmp, "������� ���� �� ����� (��������������/����� ��� ���������� �� ��� �����������/�����): ");
	printRusComment(tmp);
	read(file1, stdin);
	if (!Analize(file1)) {
		quit("������: �� ������� ���������� ������ �����");
	}
	if (u)
		goto FFtry;
	sprintf(tmp, "����: %s.%s\n(N - ������ �������� ����, Esc - �����, F - ������� ������������ "
	       "ffmpeg'� (���� �� C:\\ffmpeg\\bin), B - ������ ������ ���������� (���� �� -q:v 1), K - F+B:\n", file1, format);
	printRusComment(tmp);
	c = getch();
	if (c == 27) {
		quit("Esc");
	}
	tmpC = c;
	if (c == 'N' || c == 'n')
		goto Opening;		
	if (c == 'b' || c == 'B')
		goto Params;
	if (c != 'f' && c != 'F' && c != 'K' && c != 'k')
		goto FFtry;
//	Prepare:;
	Pathing:;
	sprintf(tmp, "ffmpeg.exe ����� ����� �� ������ \"%s\"?\n", ffpath);
	printRusComment(tmp);
	c = getch();
    printf("%c\n", c);
    if (c != 'y' && c != 'Y' && c != '�' && c != '�' && c != 13) {
    	printRusComment("�������� ���� �� ffmpeg.exe: ");
    	read(ffpath, stdin);
    	goto Pathing;
	}	
	if (tmpC != 'K' && tmpC != 'k' && tmpC != '�' && tmpC != '�')
		goto FFtry;
//	Last_prepare:;
	Params:;
	sprintf(tmp, "������ ��������� �������� ���: %s\n������������ ��� ������? � ������ ������ (�� Y ��� Enter) "
	       "��� ����� ���������� ������ ���� ������ ����������; Esc - �����\n", param);
	printRusComment(tmp);
	c = getch();
	if (c == 27) {
		quit("Esc");
	} else
	if (c == 'n' || c == 'N' || c == '�' || c == '�') {
		printRusComment("������� ����� ������ ����������: ");
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
		quit("������ ��� ������ ffmpeg'� - �������� ������� �����");
	}
	sprintf(str, "%s -i %s.%s -filter_complex \"[0:v]crop=iw/2:ih:iw/2:0\" %s %s-right.%s", ffpath, file1, format, param, file2, format);
	printf("%s\n", str);
	if (system(str)) {
		quit("������ ��� ������ ffmpeg'� - �������� ������� �����");
	}
	if (u)
		goto Opening;
	printRusComment("\n������� U ��� ����������� �������� ��������� � ������ �� �����������, ����� ������ ������� ��� ������\n");
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
