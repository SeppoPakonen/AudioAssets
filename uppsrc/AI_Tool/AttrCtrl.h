#ifndef _AI_Tool_AttrCtrl_h_
#define _AI_Tool_AttrCtrl_h_

struct PatternSnap;

class AttrCtrl : public Ctrl {
	using RectId = Tuple3<Rect, int, int>;
	Vector<RectId> rects;
	RectId sel, pressed;
	
	Vector<bool> active;
	Vector<bool> inherited_active;
public:
	typedef AttrCtrl CLASSNAME;
	AttrCtrl();
	
	void Update();
	void Load();
	void Store();
	void Paint(Draw& d) override;
	void PaintKeys(Draw& d, String title, const Vector<String>& keys, Color clr, int x, int cx, int& cy, float h, Font fnt, int group);
	
	void MouseMove(Point p, dword keyflags) override;
	void LeftDown(Point p, dword keyflags) override;
	void LeftUp(Point, dword keyflags) override;
	void MouseLeave() override;
	
	
	PatternSnap* active_snap = 0;
	Callback WhenUpdate;
};

#endif
