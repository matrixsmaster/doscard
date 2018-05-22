#include <string>
#include "VM86Wrapper.h"

static VM86Wrapper* g_VM;
static char* shadow;

using namespace std;

VM86Wrapper::VM86Wrapper(uint32_t base_addr) :
		membase(base_addr)
{
}

VM86Wrapper::~VM86Wrapper()
{
	if (vm) delete vm;
}

int VM86Wrapper::PullChar()
{
	if (!bufout.empty()) {
		int r = bufout.front();
		bufout.pop_front();
		return r;
	}
	return 0;
}

void VM86Wrapper::PushChar(int v)
{
	if (!vm || stopped) return;
	bufin.push_back(v);
}

bool VM86Wrapper::Loop()
{
	if (!started) {
		if (vm) delete vm;
		vm = new VM86(membase);
		started = vm;
		stopped = false;
	}

	if (!vm || stopped) return false;

	while (!bufin.empty()) {
		vm->PushEvent(bufin.front());
		bufin.pop_front();
	}

	for (cycles = 0; cycles < VM86W_MINCYCLES; cycles++) {
		stopped = vm->FullStep();
		if (stopped) break;
	}

	string out = vm->PullTextOutput();
	for (auto &c : out) bufout.push_back(c);
	if (use_intscreen) UpdateScreen();

	return stopped;
}

void VM86Wrapper::Reset()
{
	if (vm) vm->Reset();
	bufin.clear();
	bufout.clear();
	if (use_intscreen) Clear();
	started = false; //this line foces VM to be re-created (and as side effect, not saved)
	stopped = false;
	cycles = 0;
}

bool VM86Wrapper::SetGeometry(int x, int y)
{
	if (x < 80 || y < 20 || x > 120 || y > 50) return false;
	use_intscreen = true;
	intscreen_mod = false;
	bufdim = Pnt2D(x, y);
	Clear();
	return true;
}

string VM86Wrapper::GetLine(int n)
{
	if (n < 0 || n >= (int)intscreen.size()) return string();
	if (n == bufdim.y - 1) intscreen_mod = false;
	return intscreen[n];
}

void VM86Wrapper::Clear()
{
	intscreen.clear();
	intscreen.resize(bufdim.y);
	for (auto &l : intscreen) l.resize(bufdim.x, ' ');
	escbuffer.clear();
	escaped = false;
	curpos_hold = Pnt2D();
	curpos_wanted = Pnt2D();
	scroll_wnd.x = 0;
	scroll_wnd.y = bufdim.y - 1;
}

bool VM86Wrapper::ProcessANSIEscape(std::string in, std::string &out)
{
	out.clear();
	if (in.empty()) return false;

	int l = in.length();
	char first = in.at(0);
	char last = in.at(l - 1);

	if (in == "[c") //Report Device Code
		out = "[?1;00c";

	else if (in == "[5n") //Report Device OK
		out = "[0n";

//	else if (in == "[6n") //Report Cursor Position
//		out = '[' + to_string(curpos_wanted.y + 1) + ';' + to_string(curpos_wanted.x + 1) + 'R';

	else if (in == "[?25h") //Show Cursor
		;

	else if (in == "[?25l") //Hide Cursor
		;

	else if (in == "[s")
		curpos_hold = curpos_wanted;

	else if (in == "[u")
		curpos_wanted = curpos_hold;

	else if (in == "[H" || in == "[f" || in == "[;H" || in == "[;f")
		curpos_wanted = Pnt2D();

	else if (in == "[2J" || in == "c") //Clear screen or Reset device
		Clear();

	else if (in == "[=7h")
		lwrapping = true;

	else if (in == "[=7l")
		lwrapping = false;

	else if (in == "[K") {
		for (int i = curpos_wanted.x; i < bufdim.x; i++) intscreen[curpos_wanted.y][i] = ' ';

	}
	else if (first == '[' && last >= 'A' && last <= 'D') {
		int x = 1;
		char tmp = ' ';
		sscanf(in.c_str(), "[%d%c", &x, &tmp); //ignore result
		switch (last) {
		case 'A': curpos_wanted.y -= x; break;
		case 'B': curpos_wanted.y += x; break;
		case 'C': curpos_wanted.x += x; break;
		case 'D': curpos_wanted.x -= x; break;
		}
		CheckScroll();

	}
	else if (l >= 5 && first == '[' && (last == 'H' || last == 'f')) { //Set cursor position
		char s = ' ';
		int c = 1, r = 1;
		if (sscanf(in.c_str(), "[%d;%d%c", &r, &c, &s) == 3) {
			curpos_wanted = Pnt2D(--c, --r);
		}

	}
	else if (l >= 3 && first == '[' && last == 'm') { //Set attributes
		;

	}
	else if (first == '[' && last == 'r') {
		int s = 1, e = bufdim.y;
		sscanf(in.c_str(), "[%d;%d", &s, &e); //ignore result
		scroll_wnd.x = --s;
		scroll_wnd.y = --e;
		curpos_wanted = Pnt2D(s, 0);

	}
	else {
		if (first == '[' && isalpha(last)) {
			printf("ERROR: Unknown ANSI escape sequence '%s'\n", in.c_str());
			return true;
		}
		return false;
	}

//	_MESSAGE("ANSI esc '%s' processed, out: %s\n", in.c_str(), out.c_str());
	if (!out.empty()) out = '\x1b' + out;
	return true;
}

void VM86Wrapper::CheckScroll()
{
	if (curpos_wanted.x < 0) curpos_wanted.x = 0;
	if (curpos_wanted.y < 0) curpos_wanted.y = 0;
	if (curpos_wanted.x >= bufdim.x) {
		if (lwrapping) {
			curpos_wanted.x = 0;
			curpos_wanted.y++;
		}
		else
			curpos_wanted.x = bufdim.x - 1;
	}

	while (curpos_wanted.y > scroll_wnd.y) {
		string nws;
		nws.resize(bufdim.x, ' ');
		if (!scroll_wnd.x && scroll_wnd.y == bufdim.y - 1) {
			intscreen.pop_front();
			intscreen.push_back(nws);
		}
		else {
			for (int j = scroll_wnd.x; j < scroll_wnd.y; j++)
				intscreen[j] = (j == bufdim.y - 1) ? nws : intscreen[j + 1];
			intscreen[scroll_wnd.y] = nws;
		}
		curpos_wanted.y--;
	}
}

void VM86Wrapper::UpdateScreen()
{
	string r, er;
	int qty = GetCharsPending();
	for (int i = 0; i < qty; i++) {
		intscreen_mod = true;
		int c = PullChar();

		if (escaped) {
			escbuffer += c;
			if (ProcessANSIEscape(escbuffer, er)) {
				r += er;
				rbuffer += er;
				escbuffer.clear();
				escaped = false;
			}

		}
		else
			switch (c) {
			case 0x1B: //Escape
				escaped = true;
				break;

			case '\r': //Carriage return
				curpos_wanted.x = 0;
				break;

			case '\n': //New line
				curpos_wanted.x = 0;
				curpos_wanted.y++;
				CheckScroll();
				break;

			default:
				//it's possible to fill in entire screen without triggering scrolling by the last character
				CheckScroll();
				intscreen[curpos_wanted.y][curpos_wanted.x] = c;
				curpos_wanted.x++;
			}
	}
}

void VM86_Start(uint32_t base_addr)
{
	g_VM = new VM86Wrapper(base_addr);
	g_VM->SetGeometry(VM86W_GEOMW,VM86W_GEOMH);
	shadow = (char*)malloc(VM86W_GEOMW*VM86W_GEOMH);
	memset(shadow,0,VM86W_GEOMW*VM86W_GEOMH);
}

int VM86_FullStep()
{
	if (g_VM->Loop()) return -1;
	if (!g_VM->IsScreenModified()) return 0;

	int cnt = 0;
	char x = 0;
	for (int i = 0; i < VM86W_GEOMH; i++) {
		string s = g_VM->GetLine(i);
		for (int j = 0; j < s.length(); j++)
			if (s.at(j) != shadow[i*VM86W_GEOMW+j]) {
				shadow[i*VM86W_GEOMW+j] = s.at(j);
				if (!cnt) x = s.at(j);
				cnt++;
			}
	}

	if (cnt == 1) {
		Pnt2D c = g_VM->GetCursorPos();
		return 0x01000000 | (c.y << 16) | (c.x << 8) | x;
	}
	else return 1;
}

char* VM86_GetShadowBuf()
{
	return shadow;
}

void VM86_Stop()
{
	delete g_VM;
	g_VM = NULL;
	free(shadow);
	shadow = NULL;
}
