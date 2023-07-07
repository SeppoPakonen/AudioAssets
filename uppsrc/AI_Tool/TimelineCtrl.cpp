#include "AI_Tool.h"

TimelineCtrl::TimelineCtrl() {
	CtrlLayout(*this);
	
	leftsplit.Vert() << albumlist << songlist;
	
	
	albumlist.AddColumn(t_("Artist"));
	albumlist.AddColumn(t_("Title"));
	albumlist.AddColumn(t_("Year"));
	songlist.AddColumn(t_("Title"));
	songlist.AddColumn(t_("Project name"));
	
	add_album <<= THISBACK(AddAlbum);
	add_song <<= THISBACK(AddSong);
	album_title <<= THISBACK(OnValueChange);
	album_artist <<= THISBACK(OnValueChange);
	album_date <<= THISBACK(OnValueChange);
	song_title <<= THISBACK(OnValueChange);
	song_prj_name <<= THISBACK(OnValueChange);
	songlist.WhenBar << THISBACK(SongBar);
	albumlist <<= THISBACK(DataAlbum);
	songlist <<= THISBACK(DataSong);
	
}

void TimelineCtrl::Data() {
	Database& db = Database::Single();
	
	for(int i = 0; i < db.timeline.albums.GetCount(); i++) {
		Timeline::Album& a = db.timeline.albums[i];
		albumlist.Set(i, 0, a.artist);
		albumlist.Set(i, 1, a.title);
		albumlist.Set(i, 2, a.date);
	}
	albumlist.SetCount(db.timeline.albums.GetCount());
	
	int cursor = 0; // max(0, db.GetActiveScoreGroupIndex());
	if (cursor >= 0 && cursor < db.timeline.albums.GetCount())
		albumlist.SetCursor(cursor);
	
	if (albumlist.IsCursor())
		DataAlbum();
}

void TimelineCtrl::DataAlbum() {
	Database& db = Database::Single();
	
	if (!albumlist.IsCursor())
		return;
	
	int cursor = albumlist.GetCursor();
	Timeline::Album& a = db.timeline.albums[cursor];
	
	album_title.SetData(a.title);
	album_artist.SetData(a.artist);
	album_date.SetData(a.date);
	
	for(int i = 0; i < a.songs.GetCount(); i++) {
		Timeline::Song& s = a.songs[i];
		songlist.Set(i, 0, s.title);
		songlist.Set(i, 1, s.prj_name);
	}
	songlist.SetCount(a.songs.GetCount());
	
	cursor = 0;
	if (cursor >= 0 && cursor < songlist.GetCount())
		songlist.SetCursor(cursor);
	
	if (songlist.IsCursor())
		DataSong();
}

void TimelineCtrl::DataSong() {
	Database& db = Database::Single();
	
	if (!albumlist.IsCursor())
		return;
	
	int acursor = albumlist.GetCursor();
	int scursor = songlist.GetCursor();
	Timeline::Album& a = db.timeline.albums[acursor];
	Timeline::Song& s = a.songs[scursor];
	
	song_title.SetData(s.title);
	song_prj_name.SetData(s.prj_name);
	
}

void TimelineCtrl::AddAlbum() {
	Database& db = Database::Single();
	Timeline& tl = db.timeline;
	tl.albums.Add();
	
	Data();
	
	albumlist.SetCursor(albumlist.GetCount()-1);
}

void TimelineCtrl::AddSong() {
	Database& db = Database::Single();
	
	if (!albumlist.IsCursor())
		return;
	
	int cursor = albumlist.GetCursor();
	Timeline::Album& a = db.timeline.albums[cursor];
	a.songs.Add();
	
	DataAlbum();
	
	songlist.SetCursor(songlist.GetCount()-1);
}

void TimelineCtrl::RemoveSong() {
	Database& db = Database::Single();
	
	if (!albumlist.IsCursor())
		return;
	
	int acursor = albumlist.GetCursor();
	Timeline::Album& a = db.timeline.albums[acursor];
	
	int scursor = songlist.GetCursor();
	
	if (scursor >= 0 && scursor < a.songs.GetCount()) {
		a.songs.Remove(scursor);
		DataAlbum();
		scursor--;
		if (scursor >= 0 && scursor < songlist.GetCursor())
			songlist.SetCursor(scursor);
	}
}

void TimelineCtrl::OnValueChange() {
	Database& db = Database::Single();
	
	if (!albumlist.IsCursor())
		return;
	
	int acursor = albumlist.GetCursor();
	Timeline::Album& a = db.timeline.albums[acursor];
	
	a.artist = album_artist.GetData();
	a.title = album_title.GetData();
	a.date = album_date.GetData();
	
	albumlist.Set(acursor, 0, a.artist);
	albumlist.Set(acursor, 1, a.title);
	albumlist.Set(acursor, 2, a.date);
	
	if (songlist.IsCursor()) {
		int scursor = songlist.GetCursor();
		Timeline::Song& s = a.songs[scursor];
		
		s.title = song_title.GetData();
		s.prj_name = song_prj_name.GetData();
		
		songlist.Set(scursor, 0, s.title);
		songlist.Set(scursor, 1, s.prj_name);
	}
	
}

void TimelineCtrl::SongBar(Bar& bar) {
	bar.Add(t_("Add"), THISBACK(AddSong));
	bar.Add(t_("Remove"), THISBACK(RemoveSong));
	
}
