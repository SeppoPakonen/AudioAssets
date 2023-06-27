#include "AI_Tool.h"


ArtistEditCtrl::ArtistEditCtrl() {
	CtrlLayout(*this);
	
	add <<= THISBACK(AddArtist);
	save <<= THISBACK(SaveArtist);
	
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
	String dir = app->GetArtistsDir();
	
	RealizeDirectory(dir);
	
	String path = dir + name + ".json";
	
	FileOut fout(path);
	fout << "\n";
	fout.Close();
	
	Data();
	
	int c = artists.GetCount();
	if (c) artists.SetCursor(c-1);
}

void ArtistEditCtrl::Data() {
	String dir = app->GetArtistsDir();
	
	FindFile ff;
	
	String search = dir + "*.json";
	
	int file_i = 0;
	if (ff.Search(search)) do {
		if (ff.IsFile()) {
			String path = ff.GetPath();
			String title = GetFileTitle(path);
			artists.Set(file_i, 0, title);
			file_i++;
		}
	}
	while (ff.Next());
	
	artists.SetCount(file_i);
	
	if (file_i && !artists.IsCursor())
		artists.SetCursor(0);
	
	if (artists.IsCursor())
		DataArtist();
	
}

void ArtistEditCtrl::DataArtist() {
	if (!artists.IsCursor())
		return;
	
	int cursor = artists.GetCursor();
	String title = artists.Get(cursor, 0);
	String dir = app->GetArtistsDir();
	json_path = dir + title + ".json";
	
	Artist& o = app->artist;
	o.Clear();
	LoadFromJsonFile(o, json_path);
	
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
	Artist& o = app->artist;
	o.name						= this->name.GetData();
	o.year_of_birth				= this->year_of_birth.GetData();
	o.year_of_career_begin		= this->year_of_career_begin.GetData();
	o.biography					= this->biography.GetData();
	o.musical_style				= this->musical_style.GetData();
	o.vibe_of_voice				= this->vibe_of_voice.GetData();
	o.acoustic_instruments		= this->acoustic_instruments.GetData();
	o.electronic_instruments	= this->electronic_instruments.GetData();
	
	if (json_path.IsEmpty())
		return;
	
	String json = StoreAsJson(o, true);
	FileOut fout(json_path);
	fout << json;
	fout.Close();
}
