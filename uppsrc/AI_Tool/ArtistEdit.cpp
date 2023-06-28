#include "AI_Tool.h"


ArtistEditCtrl::ArtistEditCtrl() {
	CtrlLayout(*this);
	
	add <<= THISBACK(AddArtist);
	
	name <<= THISBACK(SaveArtist);
	year_of_birth <<= THISBACK(SaveArtist);
	year_of_career_begin <<= THISBACK(SaveArtist);
	biography <<= THISBACK(SaveArtist);
	musical_style <<= THISBACK(SaveArtist);
	vibe_of_voice <<= THISBACK(SaveArtist);
	acoustic_instruments <<= THISBACK(SaveArtist);
	electronic_instruments <<= THISBACK(SaveArtist);
	
	artists.AddColumn("Artist");
	artists.NoHeader();
	artists.WhenAction << THISBACK(DataArtist);
}

void ArtistEditCtrl::AddArtist() {
	String artist_name;
	bool b = EditTextNotNull(
		artist_name,
		"Artist's name",
		"Please write artist's name",
		0
	);
	if (b) {
		NewArtist(artist_name);
	}
}

void ArtistEditCtrl::NewArtist(const String& name) {
	Database::Single().CreateArtist(name);
	
	Data();
	
	int c = artists.GetCount();
	if (c) artists.SetCursor(c-1);
}

void ArtistEditCtrl::Data() {
	Database& db = Database::Single();
	for(int i = 0; i < db.artists.GetCount(); i++) {
		Artist& a = db.artists[i];
		artists.Set(i, 0, a.file_title);
	}
	artists.SetCount(db.artists.GetCount());
	
	if (db.artists.GetCount() && !artists.IsCursor())
		artists.SetCursor(0);
	
	if (artists.IsCursor())
		DataArtist();
	
}

void ArtistEditCtrl::DataArtist() {
	if (!artists.IsCursor())
		return;
	
	int cursor = artists.GetCursor();
	String title = artists.Get(cursor, 0);
	
	Database& db = Database::Single();
	Artist& o = db.artists[cursor];
	active_artist = &o;
	
	this->name						.SetData(o.name);
	this->year_of_birth				.SetData(o.year_of_birth);
	this->year_of_career_begin		.SetData(o.year_of_career_begin);
	this->biography					.SetData(o.biography);
	this->musical_style				.SetData(o.musical_style);
	this->vibe_of_voice				.SetData(o.vibe_of_voice);
	this->acoustic_instruments		.SetData(o.acoustic_instruments);
	this->electronic_instruments	.SetData(o.electronic_instruments);
	
}

void ArtistEditCtrl::SaveArtist() {
	if (!active_artist)
		return;
	
	Artist& o = *active_artist;
	o.name						= this->name.GetData();
	o.year_of_birth				= this->year_of_birth.GetData();
	o.year_of_career_begin		= this->year_of_career_begin.GetData();
	o.biography					= this->biography.GetData();
	o.musical_style				= this->musical_style.GetData();
	o.vibe_of_voice				= this->vibe_of_voice.GetData();
	o.acoustic_instruments		= this->acoustic_instruments.GetData();
	o.electronic_instruments	= this->electronic_instruments.GetData();
	
}
