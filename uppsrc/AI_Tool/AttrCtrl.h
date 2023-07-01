#ifndef _AI_Tool_AttrCtrl_h_
#define _AI_Tool_AttrCtrl_h_

struct PatternSnap;

class AttrCtrl : public Ctrl {
	using RectId = Tuple3<Rect, int, int>;
	Vector<RectId> group_title_rects;
	Vector<RectId> entry_rects;
	RectId sel, pressed;
	
	Vector<bool> active;
	Vector<bool> inherited_active;
	
	Vector<Vector<int>> group_grid;
	bool invalidate_group_grid = false;
	
	void RealizeTemp();
	
public:
	typedef AttrCtrl CLASSNAME;
	AttrCtrl();
	
	void Update();
	void Load();
	void Store();
	void Paint(Draw& d) override;
	void PaintKeys(Draw& d, int group_i, int x, int cx, int& y, float lineh, Font fnt);
	void ContextMenu(Bar& bar, Point pt);
	void AddGroup();
	void AddEntry(int group);
	
	void MouseMove(Point p, dword keyflags) override;
	void LeftDown(Point p, dword keyflags) override;
	void LeftUp(Point, dword keyflags) override;
	void RightDown(Point p, dword keyflags) override;
	void MouseLeave() override;
	void Layout() override {invalidate_group_grid = true;}
	
	
	PatternSnap* active_snap = 0;
	Callback WhenUpdate;
};

#endif
