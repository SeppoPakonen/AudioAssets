#include "AI_Tool.h"


CompositionEditCtrl::CompositionEditCtrl() {
	CtrlLayout(*this);
	
	add <<= THISBACK(AddComposition);
	
	year <<= THISBACK(SaveComposition);
	title <<= THISBACK(SaveComposition);
	tempo <<= THISBACK(SaveComposition);
	beat <<= THISBACK(SaveComposition);
	melody <<= THISBACK(SaveComposition);
	chord_progression <<= THISBACK(SaveComposition);
	key_and_mode <<= THISBACK(SaveComposition);
	texture <<= THISBACK(SaveComposition);
	structure <<= THISBACK(SaveComposition);
	genre_style <<= THISBACK(SaveComposition);
	
	compositions.AddColumn("Composition");
	compositions.NoHeader();
	compositions.WhenAction << THISBACK(DataComposition);
}

void CompositionEditCtrl::AddComposition() {
	String composition_name;
	bool b = EditTextNotNull(
		composition_name,
		"Composition's name",
		"Please write composition's name",
		0
	);
	if (b) {
		NewComposition(composition_name);
	}
}

void CompositionEditCtrl::NewComposition(const String& name) {
	Database::Single().CreateComposition(name);
	
	Data();
	
	int c = compositions.GetCount();
	if (c) compositions.SetCursor(c-1);
}

void CompositionEditCtrl::Data() {
	Database& db = Database::Single();
	for(int i = 0; i < db.compositions.GetCount(); i++) {
		Composition& a = db.compositions[i];
		compositions.Set(i, 0, a.file_title);
	}
	compositions.SetCount(db.compositions.GetCount());
	
	if (db.compositions.GetCount() && !compositions.IsCursor())
		compositions.SetCursor(0);
	
	if (compositions.IsCursor())
		DataComposition();
	
}

void CompositionEditCtrl::DataComposition() {
	if (!compositions.IsCursor())
		return;
	
	int cursor = compositions.GetCursor();
	String title = compositions.Get(cursor, 0);
	
	Database& db = Database::Single();
	Composition& o = db.compositions[cursor];
	active_composition = &o;
	
	this->year						.SetData(o.year);
	this->title						.SetData(o.title);
	this->tempo						.SetData(o.tempo);
	this->beat						.SetData(o.beat);
	this->melody					.SetData(o.melody);
	this->chord_progression			.SetData(o.chord_progression);
	this->key_and_mode				.SetData(o.key_and_mode);
	this->texture					.SetData(o.texture);
	this->structure					.SetData(o.structure);
	this->genre_style				.SetData(o.genre_style);
	
}

void CompositionEditCtrl::SaveComposition() {
	if (!active_composition)
		return;
	
	Composition& o = *active_composition;
	o.year						= this->year.GetData();
	o.title						= this->title.GetData();
	o.tempo						= this->tempo.GetData();
	o.beat						= this->beat.GetData();
	o.melody					= this->melody.GetData();
	o.chord_progression			= this->chord_progression.GetData();
	o.key_and_mode				= this->key_and_mode.GetData();
	o.texture					= this->texture.GetData();
	o.structure					= this->structure.GetData();
	o.genre_style				= this->genre_style.GetData();
	
}
