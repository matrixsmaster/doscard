#ifndef VM86W_H_
#define VM86W_H_

#define VM86W_MINCYCLES 1
#define VM86W_GEOMW 80
#define VM86W_GEOMH 25

#ifdef __cplusplus
#include "VM86.h"

struct Pnt2D {
	int x = 0, y = 0;

	Pnt2D() {}
	Pnt2D(int nv) : x(nv), y(nv) {}
	Pnt2D(int nx, int ny) : x(nx), y(ny) {}
};

class VM86Wrapper
{
private:
	VM86 * vm = NULL;
	uint32_t membase;
	bool started = false;
	bool stopped = false;
	unsigned cycles = 0;
	double timeslice = 0.01;
	std::deque<int> bufin, bufout;

	bool use_intscreen = false;
	std::deque<std::string> intscreen;
	bool intscreen_mod = false;
	Pnt2D bufdim;
	std::string escbuffer, rbuffer;
	Pnt2D curpos_hold, curpos_wanted;
	bool escaped = false;
	bool lwrapping = true;
	Pnt2D scroll_wnd;

	void Clear();
	bool ProcessANSIEscape(std::string in, std::string &out);
	void CheckScroll();
	void UpdateScreen();

public:
	VM86Wrapper(uint32_t base_addr);
	virtual ~VM86Wrapper();

	int GetCharsPending() { return bufout.size(); }
	int PullChar();
	void PushChar(int v);
	bool Loop();
	void Reset();
	bool IsStarted() { return started; }
	bool IsStopped() { return stopped; }
	unsigned GetLastCycles() { return cycles; }
	bool SetGeometry(int x, int y);
	std::string GetLine(int n);
	bool IsScreenModified() { return intscreen_mod; }
	Pnt2D GetCursorPos() { return curpos_wanted; }
};

extern "C" {
#endif

	void VM86_Start(uint32_t base_addr);

	int VM86_FullStep();

	char* VM86_GetShadowBuf();

	void VM86_Stop();

#ifdef __cplusplus
}
#endif

#endif /* VM86W_H_ */
