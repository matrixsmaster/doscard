#include <cstdio>
#include <vector>
#include <string>
#include <cstring>
#include <locale>
#include <assert.h>

#define DELIMS ",\r\n"

using namespace std;

struct Pin {
	int no;
	string name;
	char type;
	string label;
};

static const struct {
	string tok;
	char x;
} typetab[] = {
	{ "input", 'I' },
	{ "output", 'O' },
	{ "i/o", 'B' },
	{ "power", 'W' },
	{ "", '\0' }
};

int main(int argc, char* argv[])
{
	assert(argc == 3);
	
	FILE* fi = fopen(argv[1],"r");
	assert(fi);
	FILE* fo = fopen(argv[2],"w");
	assert(fo);
	
	int fsm = 0;
	char buf[2048];
	vector<Pin> chip;
	
	while (fgets(buf,sizeof(buf),fi)) {
		if (!fsm) {
			fsm++; //skip first line
			continue;
		}
		
		fsm = 1;
		Pin cur;
		char* tok = strtok(buf,DELIMS);
		do {
			string s = tok;
			if (!s.empty() && s[0] == '\"' && s.back() == '\"')
				s = s.substr(1,s.length()-2);
			//printf("token: '%s'\n",s.c_str());
			
			switch (fsm) {
			case 1: cur.no = atoi(s.c_str()); break;
			case 2: cur.name = s; break;
			case 3:
				for (int i = 0; typetab[i].x; i++) {
					string _s(s);
					for (auto &_x : _s) _x = tolower(_x);
					if (_s == typetab[i].tok) {
						cur.type = typetab[i].x;
						break;
					}
				}
				break;
			case 4: cur.label = s; break;
			case 5: 
				if (!s.empty()) cur.label = s;
				chip.push_back(cur);
				break;
			}
			
			tok = strtok(NULL,DELIMS);
			fsm++;
		} while (tok);
	}
	fclose(fi);
	
	printf("%lu pins read\n",chip.size());
	
	string pname = basename(argv[1]);
	auto pos = pname.find_first_of(".");
	assert(pos > 0);
	pname = pname.substr(0,pos);
	
	fprintf(fo,"EESchema-LIBRARY Version 2.3\n");
	fprintf(fo,"#\n# %s\n#\n",pname.c_str());
	fprintf(fo,"DEF %s U 0 40 Y Y 1 F N\n",pname.c_str());
	fprintf(fo,"F0 \"U\" 0 0 60 H V C CNN\n");
	fprintf(fo,"F1 \"%s\" 0 0 60 H V C CNN\n",pname.c_str());
	fprintf(fo,"F2 \"\" 0 0 60 H V C CNN\n");
	fprintf(fo,"F3 \"\" 0 0 60 H V C CNN\n");
	fprintf(fo,"DRAW\n");
	
	unsigned div = (chip.size() > 24 && chip.size() % 4 == 0)? 4 : 2;
	int vspc = 100;
	int hspc = 1000;
	int skip = 3;
	size_t n = 0;
	int cx = 0;
	int cy = -skip * vspc;
	int size = 200;
	
//	int pg = static_cast<int>(chip.size()) / div;
//	fprintf(fo,"S %d %d %d %d 0 1 0 N",size,0,(-cy+size)*2+vspc*pg,-vspc*pg+cy);
	
	for (unsigned i = 0; i < div; i++) {
		char dir;
		switch (i) {
		case 0: dir = 'R'; break;
		case 1: dir = (div > 2)? 'U' : 'L'; break;
		case 2: dir = 'L'; break;
		case 3: dir = 'D'; break;
		}
		
		for (unsigned j = 0; j < chip.size()/div; j++,n++) {
			string full;
			if (chip[n].label.empty()) full = chip[n].name;
			else full = chip[n].name + "/" + chip[n].label;
			
			fprintf(fo,"X %s %d %d %d %d %c 50 50 1 1 %c\n",full.c_str(),chip[n].no,cx,cy,size,dir,chip[n].type);
			
			switch (dir) {
			case 'R': cy -= vspc; break;
			case 'L': cy += vspc; break;
			case 'U': cx += vspc; break;
			case 'D': cx -= vspc; break;
			}
		}
		
		switch (dir) {
		case 'R':
			cy += vspc;
			if (div > 2) {
				cx += skip * vspc + size;
				cy -= skip * vspc + size;
			} else
				cx += hspc + size;
			break;
			
		case 'U':
			cx -= vspc;
			cx += skip * vspc + size;
			cy += skip * vspc + size;
			break;
			
		case 'L':
			cy -= vspc;
			cx -= skip * vspc + size;
			cy += skip * vspc + size;
			break;
		}
	}
	
	fprintf(fo,"ENDDRAW\n");
	fprintf(fo,"ENDDEF\n");
	fprintf(fo,"#\n#End Library\n");
	
	fclose(fo);
	return 0;
}
