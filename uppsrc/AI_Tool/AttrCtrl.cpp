#include "AI_Tool.h"

/*
Vector<String> elements;
Vector<String> interactions;
Vector<String> with;
Vector<String> moral_interactions;
Vector<String> acting_styles;
Vector<String> tones;
Vector<String> voiceover_tones;
Vector<String> comedic_scenarios;
Vector<String> dramatic_scenarios;
Vector<String> types_of_sentences;
Vector<String> comedic_sentences;
Vector<String> humorous_expressions;
*/


AttrCtrl::AttrCtrl() {
	sel.b = -1;
	sel.c = -1;
	pressed.b = -1;
	pressed.c = -1;
	
	Grouplist& g = Database::Single().groups;
	active.SetCount(g.group_count * g.group_limit, false);
	inherited_active.SetCount(g.group_count * g.group_limit, false);
	
	//for (bool& b : active) b = Randomf() < 0.15;
	
}

void AttrCtrl::Load() {
	Grouplist& g = Database::Single().groups;
	if (!active_snap)
		return;
	
	for (bool& b : active) b = false;
	for (const SnapAttr& a : active_snap->attributes) {
		int id = a.group * g.group_limit + a.item;
		ASSERT(id >= 0 && id < active.GetCount());
		if (id >= 0 && id < active.GetCount())
			active[id] = true;
	}
	
	for (bool& b : inherited_active) b = false;
	PatternSnap* owner = active_snap->owner;
	while (owner) {
		for (const SnapAttr& a : owner->attributes) {
			int id = a.group * g.group_limit + a.item;
			ASSERT(id >= 0 && id < inherited_active.GetCount());
			if (id >= 0 && id < inherited_active.GetCount())
				inherited_active[id] = true;
		}
		owner = owner->owner;
	}
}

void AttrCtrl::Store() {
	Grouplist& g = Database::Single().groups;
	if (!active_snap)
		return;
	
	active_snap->attributes.Clear();
	
	int id = 0;
	for (bool& b : active) {
		if (b) {
			SnapAttr attr;
			attr.item = id % g.group_limit;
			attr.group = id / g.group_limit;
			active_snap->attributes.Add(attr);
		}
		id++;
	}
}

void AttrCtrl::Paint(Draw& d) {
	Grouplist& g = Database::Single().groups;
	Color bg = Color(203, 203, 203);
	Size sz = GetSize();
	
	d.DrawRect(sz, bg);
	
	
	/*
	
	12 Contrast and Unexpected Elements
	15 Interactions
	24 (Interactions) with
	24 Moral interactions
	9  Acting Styles
	16 Tones
	19 Voiceover Tones
	17 Types of comedic scenarios
	19 Types of dramatic scenarios
	3  Types of sentences
	25 Comedic sentences
	23 Humorous expression
	*/
	
	int tab;
	int div = 7;
	int cy, cx = sz.cx / div;
	float h;
	
	int font_h = 0.75 * (float)sz.cy / (g.acting_styles.GetCount() + g.tones.GetCount() + g.dramatic_scenarios.GetCount());
	Font fnt = SansSerif(font_h);
	
	rects.SetCount(0);
	
	cy = 0;
	h = (float)sz.cy / 40; //(1 + pronouns.GetCount());
	tab = 0;
	PaintKeys(d, t_("Pronouns"), g.pronouns, g.pronouns_clr, tab * cx, cx, cy, h, fnt, g.PRONOUNS);
	
	
	cy = 0;
	h = (float)sz.cy / (2 + g.moral_interactions.GetCount() + g.moral_interaction_modes.GetCount());
	tab++;
	PaintKeys(d, t_("Moral interactions"), g.moral_interactions, g.moral_interactions_clr, tab * cx, cx, cy, h, fnt, g.MORAL_IA);
	PaintKeys(d, t_("Moral interactions mode"), g.moral_interaction_modes, g.moral_interaction_modes_clr, tab * cx, cx, cy, h, fnt, g.MORAL_IA_MODE);
	
	cy = 0;
	h = (float)sz.cy / (3 + g.types_of_sentences.GetCount() + g.elements.GetCount() + g.moral_interactions.GetCount());
	tab++;
	PaintKeys(d, t_("Types of sentences"), g.types_of_sentences, g.types_of_sentences_clr, tab * cx, cx, cy, h, fnt, g.TYPES_OF_SENT);
	PaintKeys(d, t_("Contrast and Unexpected Elements"), g.elements, g.elements_clr, tab * cx, cx, cy, h, fnt, g.ELEMENTS);
	PaintKeys(d, t_("Religiously moral"), g.religious_moral_interactions, g.religious_moral_interactions_clr, tab * cx, cx, cy, h, fnt, g.RELMORAL_IA);
	
	cy = 0;
	tab++;
	h = (float)sz.cy / (2 + g.interactions.GetCount() + g.with.GetCount());
	PaintKeys(d, t_("Interactions"), g.interactions, g.interactions_clr, tab * cx, cx, cy, h, fnt, g.INTERACTIONS);
	PaintKeys(d, t_("(Interactions) with"), g.with, g.with_clr, tab * cx, cx , cy, h, fnt, g.WITH);
	
	cy = 0;
	tab++;
	h = (float)sz.cy / (3 + g.acting_styles.GetCount() + g.tones.GetCount() + g.dramatic_scenarios.GetCount());
	PaintKeys(d, t_("Acting Styles"), g.acting_styles, g.acting_styles_clr, tab * cx, cx, cy, h, fnt, g.ACTING_STYLES);
	PaintKeys(d, t_("Tones"), g.tones, g.tones_clr, tab * cx, cx, cy, h, fnt, g.TONES);
	PaintKeys(d, t_("Types of dramatic scenarios"), g.dramatic_scenarios, g.dramatic_scenarios_clr, tab * cx, cx, cy, h, fnt, g.DRAMATIC_SCEN);
	
	cy = 0;
	tab++;
	h = (float)sz.cy / (2 + g.voiceover_tones.GetCount() + g.comedic_sentences.GetCount());
	PaintKeys(d, t_("Voiceover Tones"), g.voiceover_tones, g.voiceover_tones_clr, tab * cx, cx, cy, h, fnt, g.VOICEOVER_TONES);
	PaintKeys(d, t_("Comedic sentences"), g.comedic_sentences, g.comedic_sentences_clr, tab * cx, cx, cy, h, fnt, g.COMEDIC_SENT);
	
	cy = 0;
	tab++;
	h = (float)sz.cy / (2 + g.comedic_scenarios.GetCount() + g.humorous_expressions.GetCount());
	PaintKeys(d, t_("Comedic scenarios"), g.comedic_scenarios, g.comedic_scenarios_clr, tab * cx, cx, cy, h, fnt, g.COMEDIC_SCEN);
	PaintKeys(d, t_("Humorous expressions"), g.humorous_expressions, g.humorous_expressions_clr, tab * cx, cx, cy, h, fnt, g.HUMOROUS_EXPR);
	
	
}

void AttrCtrl::PaintKeys(Draw& d, String title, const Vector<String>& keys, Color clr, int x, int cx, int& cy, float h, Font fnt, int group) {
	Grouplist& g = Database::Single().groups;
	int x0 = x;
	int y0 = cy;
	int x1 = x + cx;
	int ht = keys.GetCount() * h;
	int y1 = cy + ht;
	const int off = 1;
	
	
	
	for(int i = 0; i <= keys.GetCount(); i++) {
		int y2 = y0 + i * h;
		String txt;
		Rect r = RectC(x0,y2,cx,h+1);
		d.Clip(r);
		Color bg_clr, stripe_clr;
		bool stripes = false;
		Font txt_fnt = fnt;
		int xoff = 4;
		if (!i) {
			bg_clr = Black(); //clr;
			txt = title;
			txt_fnt.Bold();
			Size sz = GetTextSize(txt, txt_fnt);
			xoff = (cx - sz.cx) / 2;
		}
		else {
			int j = i-1;
			const String& key = keys[j];
			int id = group * g.group_limit + j;
			bool active = this->active[id];
			bool inherited_active = this->inherited_active[id];
			
			// Mouse hovers
			if (group == pressed.b && j == pressed.c)
				bg_clr = Blend(clr, White(), 0.8*255);
			else if (group == sel.b && j == sel.c)
				bg_clr = clr;
			else if (active)
				bg_clr = Blend(clr, GrayColor(64), 0.3*255);
			else if (inherited_active) {
				bg_clr = Blend(clr, GrayColor(64), 0.97*255);
				stripe_clr = Blend(clr, GrayColor(64), 0.3*255);
				stripes = true;
			}
			else
				bg_clr = Blend(clr, GrayColor(64), 0.97*255);
			txt = key;
			
			RectId& rid = rects.Add();
			rid.a = r;
			rid.b = group;
			rid.c = j;
		}
		
		d.DrawRect(r, bg_clr);
		if (stripes) {
			int sw = 2;
			for (int x = x0; x <= x1; x += sw * 2) {
				Rect r = RectC(x, y2, sw, h+1);
				d.DrawRect(r, stripe_clr);
			}
		}
		
		// Capitalize first letter
		txt = ToUpper(txt.Left(1)) + txt.Mid(1);
		
		d.DrawText(xoff+ x0 + off + 1, y2 + off + 1, txt, txt_fnt, Black());
		d.DrawText(xoff+ x0 + off, y2 + off, txt, txt_fnt, White());
		d.End();
	}
	
	
	Color line_clr = GrayColor();
	d.DrawLine(x0, y0, x0, y1, 1, line_clr);
	d.DrawLine(x1, y0, x1, y1, 1, line_clr);
	for(int i = 0; i <= keys.GetCount() + 1; i++) {
		int y2 = y0 + i * h;
		d.DrawLine(x0, y2, x1, y2, 1, line_clr);
	}
	
	cy += (1 + keys.GetCount()) * h;
}

void AttrCtrl::MouseMove(Point p, dword keyflags) {
	bool found = false;
	for(RectId& rid : rects) {
		if (rid.a.Contains(p)) {
			bool r = sel.b != rid.b || sel.c != rid.c;
			sel = rid;
			if (r)
				Update();
			found = true;
			break;
		}
	}
	if (!found) {
		bool r = sel.b != -1;
		sel.b = -1;
		sel.c = -1;
		if (r)
			Update();
	}
}

void AttrCtrl::MouseLeave() {
	if (sel.b >= 0) {
		sel.b = -1;
		sel.c = -1;
		Update();
	}
}

void AttrCtrl::LeftDown(Point p, dword keyflags) {
	Grouplist& g = Database::Single().groups;
	for(RectId& rid : rects) {
		if (rid.a.Contains(p)) {
			SnapAttr a;
			a.group = rid.b;
			a.item = rid.c;
			pressed = rid;
			int id = a.group * g.group_limit + a.item;
			if (id >= 0 && id < active.GetCount()) {
				bool& b = active[id];
				if (active_snap) {
					b = !b;
					if (!b) {
						int i = active_snap->attributes.Find(a);
						ASSERT(i >= 0);
						active_snap->attributes.Remove(i);
					}
					else {
						active_snap->attributes.Add(a);
					}
					Update();
				}
			}
			break;
		}
	}
}

void AttrCtrl::LeftUp(Point, dword keyflags) {
	pressed.b = -1;
	pressed.c = -1;
	Update();
}

void AttrCtrl::Update() {
	Refresh();
	WhenUpdate();
}
