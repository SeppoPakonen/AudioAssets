#include "AI_Tool.h"

Color HSVToRGB(double H, double S, double V) {
	double r = 0, g = 0, b = 0;
	
	H *= 360.;
	
	if (S == 0)
	{
		r = V;
		g = V;
		b = V;
	}
	else
	{
		int i;
		double f, p, q, t;

		if (H == 360)
			H = 0;
		else
			H = H / 60;

		i = (int)trunc(H);
		f = H - i;

		p = V * (1.0 - S);
		q = V * (1.0 - (S * f));
		t = V * (1.0 - (S * (1.0 - f)));

		switch (i)
		{
		case 0:
			r = V;
			g = t;
			b = p;
			break;

		case 1:
			r = q;
			g = V;
			b = p;
			break;

		case 2:
			r = p;
			g = V;
			b = t;
			break;

		case 3:
			r = p;
			g = q;
			b = V;
			break;

		case 4:
			r = t;
			g = p;
			b = V;
			break;

		default:
			r = V;
			g = p;
			b = q;
			break;
		}

	}
	
	DUMP(r);
	DUMP(g);
	DUMP(b);
	RGBA rgb;
	rgb.r = r * 255;
	rgb.g = g * 255;
	rgb.b = b * 255;
	rgb.a = 255;
	
	return rgb;
}



Plotter::Plotter() {
	AddFrame(InsetFrame());
	
}

void Plotter::Paint(Draw& d) {
	Size sz = GetSize();
	
	d.DrawRect(sz, White());
	
	Color txt_clr = Black();
	Font fnt = Monospace(10);
	
	
	
	Grouplist& g = Database::Single().groups;
	
	int y = sz.cy-15;
	int c = g.scorings.GetCount();
	
	values.SetCount(c);
	draw_group.SetCount(c);
	vert_lines.SetCount(0);
	
	String part_key;
	int vert_x = 0;
	if (whole_song) {
		if (!pattern)
			return;
		for (auto& v : values) v.SetCount(0);
		for(int i = 0; i < pattern->parts.GetCount(); i++) {
			String key = pattern->parts[i];
			int j = pattern->unique_parts.Find(key);
			ASSERT(j >= 0);
			PartScore& part = pattern->unique_parts[j];
			
			// Accumulate values
			int c0 = min(c, part.values.GetCount());
			for(int j = 0; j < c0; j++) {
				auto& to = values[j];
				const auto& from = part.values[j];
				for (auto& v : from)
					to.Add(v);
			}
			
			// Get positions of vertical lines
			vert_x += part.len;
			vert_lines.Add(vert_x);
			
			// Get key string for the whole song
			if (i)
				part_key << ", ";
			part_key << g.Translate(key);
		}
	}
	else {
		if (!part)
			return;
		part->Realize();
		this->values <<= part->values;
		part_key = this->part_key;
	}
	
	{
		d.DrawText(3,3,part_key,fnt,txt_clr);
		d.DrawText(3,13, !mode ? "absolute value" : "accumulated value",fnt,txt_clr);
	}
	
	// Accumulated value mode
	if (mode == 1) {
		for (auto& vv : this->values) {
			double a = 0;
			for (auto& v : vv) {
				a += v;
				v = a;
			}
		}
	}
	
	// Don't draw zero-lines
	for(int i = 0; i < draw_group.GetCount(); i++) {
		bool& b = draw_group[i];
		b = false;
		auto& vv = values[i];
		for (auto& v : vv) {
			if (v != 0) {
				b = true;
				break;
			}
		}
	}
	
	int limit = 0;
	for(int i = 0; i < c; i++) {
		if (!draw_group[i])
			continue;
		
		const Grouplist::ScoringType& s = g.scorings[i];
		String str =
			g.Translate(s.klass) + ": " +
			g.Translate(s.axes0) + " / " +
			g.Translate(s.axes1);
		
		double h = (double)i / c;
		Color clr = HSVToRGB(h, 1, 0.5);
		Size sz = GetTextSize(str, fnt);
		d.DrawRect(RectC(5, y, sz.cx, sz.cy), White());
		d.DrawText(5, y, str, fnt, clr);
		y-= 11;
		
		for (auto& v : values[i])
			limit = max(limit, abs(v));
	}
	if (!limit)
		limit = 1;
	
	int h_2 = sz.cy / 2;
	d.DrawLine(0, h_2, sz.cx-1, h_2, 1, Black());
	
	for(int i = 0; i < c; i++) {
		if (!draw_group[i])
			continue;
		
		auto& vv = values[i];
		if (vv.GetCount() < 2)
			continue;
		
		double h = (double)i / c;
		Color clr = HSVToRGB(h, 1, 0.9);
		polyline.SetCount(0);
		double cx = (double)sz.cx / (vv.GetCount()-1);
		double x = 0;
		for (auto& v : vv) {
			Point& pt = polyline.Add();
			double fy = (v + limit) / (2.0 * limit);
			pt.x = x;
			pt.y = (1.0 - fy) * sz.cy;
			x += cx;
		}
		d.DrawPolyline(polyline, 3, clr);
	}
	
	if (vert_lines.GetCount()) {
		double cx = (double)sz.cx / (vert_x-1);
		for(int i = 0; i < vert_lines.GetCount(); i++) {
			int x = cx * vert_lines[i];
			d.DrawLine(x, 0, x, sz.cy, 1, Black());
		}
	}
}

void Plotter::LeftDown(Point p, dword keyflags) {
	mode = (mode + 1) % 2;
	Refresh();
}

void Plotter::MouseWheel(Point p, int zdelta, dword keyflags) {
	Database& d = Database::Single();
	focused_type = (focused_type + (zdelta > 0 ? +1 : -1)) % d.groups.scorings.GetCount();
	Refresh();
}






PlotCtrl::PlotCtrl() {
	AddFrame(InsetFrame());
	Add(plotter.HSizePos().VSizePos(0,30));
	Add(choices.BottomPos(0,30).HSizePos());
	
	
	choices.Add(t_("Mood: joyful/melancholic"));
	choices.Add(t_("Mood: playful/serious"));
	choices.Add(t_("Mood: uplifting/heavy"));
	choices.Add(t_("Mood: lighthearted/somber"));
	choices.Add(t_("Mood: humorous/dramatic"));
	choices.Add(t_("Social: authoritarian/liberatrian"));
	choices.Add(t_("Economic: liberal/conservative"));
	choices.Add(t_("Culture: individualism/collective"));
	choices.Add(t_("Human strength: strong/weak"));
	choices.Add(t_("Motivation: rewarding/punishing"));
	choices.Add(t_("Sexualization: sexual/non-sexual"));
	choices.Add(t_("Attitude: hopeful/despair"));
	choices.Add(t_("Attitude: optimistic/pessimistic"));
	choices.Add(t_("Attitude: open/closed"));
	choices.Add(t_("Beliefs: spiritual/secular"));
	choices.Add(t_("Expectations: perfection/acceptance"));
	choices.SetIndex(0);
	
}
