#ifndef _AI_Tool_AttrCtrl_h_
#define _AI_Tool_AttrCtrl_h_

class AttrCtrl : public Ctrl {
	Vector<String> pronouns;
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
	Color pronouns_clr;
	Color elements_clr;
	Color interactions_clr;
	Color with_clr;
	Color moral_interactions_clr;
	Color acting_styles_clr;
	Color tones_clr;
	Color voiceover_tones_clr;
	Color comedic_scenarios_clr;
	Color dramatic_scenarios_clr;
	Color types_of_sentences_clr;
	Color comedic_sentences_clr;
	Color humorous_expressions_clr;
	
	enum {
		PRONOUNS,
		ELEMENTS,
		INTERACTIONS,
		WITH,
		MORAL_IA,
		ACTING_STYLES,
		TONES,
		VOICEOVER_TONES,
		COMEDIC_SCEN,
		DRAMATIC_SCEN,
		TYPES_OF_SENT,
		COMEDIC_SENT,
		HUMOROUS_EXPR,
		group_count
	};
	
	using RectId = Tuple3<Rect, int, int>;
	Vector<RectId> rects;
	int sel_group = -1;
	int sel_i = -1;
	
	Vector<bool> active;
	static const int group_limit = 30;
	
public:
	typedef AttrCtrl CLASSNAME;
	AttrCtrl();
	
	void Paint(Draw& d) override;
	void PaintKeys(Draw& d, String title, const Vector<String>& keys, Color clr, int x, int cx, int& cy, float h, Font fnt, int group);
	
	void MouseMove(Point p, dword keyflags) override;

};

#endif
