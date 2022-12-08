#include "GPT3Lyrics.h"


Editor::Editor() {
	Title("GPT3 Lyrics");
	Sizeable().MaximizeBox().MinimizeBox();
	Maximize();
	
	qualifier.Add("");
	qualifier.Add("metaphoric");
	qualifier.Add("popular");
	qualifier.Add("surreal");
	qualifier.Add("realistic");
	qualifier.Add("story-like");
	qualifier.Add("meme-like");
	genre.Add("");
	genre.Add("rock");
	genre.Add("rap");
	genre.Add("pop");
	genre.Add("traditional");
	genre.Add("dance");
	genre.Add("metal");
	audience.Add("");
	audience.Add("all ages");
	audience.Add("everyone");
	audience.Add("everybody");
	audience.Add("men");
	audience.Add("women");
	audience.Add("men and women");
	audience.Add("women and men");
	audience.Add("men but not women");
	audience.Add("women but not men");
	audience.Add("teens");
	audience.Add("teen girls");
	audience.Add("teen boys");
	audience.Add("old people");
	attributes.Add("");
	attributes.Add("realistic");
	attributes.Add("imaginary");
	attributes.Add("easy");
	attributes.Add("soft");
	attributes.Add("hard");
	attributes.Add("difficult");
	attributes.Add("slightly sexual");
	attributes.Add("very sexual");
	attributes.Add("romantic");
	attributes.Add("friendly");
	attributes.Add("joyful");
	attributes.Add("violent");
	attributes.Add("dark");
	attributes.Add("scary");
	attributes.Add("disorienting");
	attributes.Add("horrifying");
	attributes.Add("unexpected");
	attributes.Add("surprising");
	attributes.Add("exciting");
	attributes.Add("thrilling");
	attributes.Add("wholesome");
	attributes.Add("empowering");
	attributes.Add("beautiful");
	attributes.Add("rewarding");
	attributes.Add("relaxing");
	attributes.Add("metaforical about life");
	attributes.Add("metaforical about death");
	attributes.Add("metaforical about family");
	attributes.Add("metaforical about happy family event");
	attributes.Add("metaforical about happy event with friends");
	attributes.Add("metaforical about romantic relationship");
	attributes.Add("metaforical about bad relationship");
	attributes.Add("metaforical about divorce");
	attributes.Add("metaforical about sex");
	attributes.Add("metaforical about god");
	attributes.Add("metaforical about stalker");
	
	for (String s : qualifier) edit.qualifier.Add(s);
	for (String s : genre) edit.genre.Add(s);
	for (String s : audience) edit.audience.Add(s);
	for (String s : attributes) edit.attrlist.Add(s);
	
	CtrlLayout(edit);
	
	AddFrame(menu);
	Add(hsplit.SizePos());
	
	hsplit.Horz().SetPos(2000);
	hsplit << songs << edit;
	
	songs.AddIndex();
	songs.AddColumn("Year");
	songs.AddColumn("Name");
	songs.ColumnWidths("1 3");
	songs.WhenAction << THISBACK(SongData);
	
	edit.topic <<= THISBACK(DataChanged);
	edit.genre <<= THISBACK(DataChanged);
	edit.audience <<= THISBACK(DataChanged);
	edit.qualifier <<= THISBACK(DataChanged);
	edit.attributes <<= THISBACK(DataChanged);
	edit.attrlist <<= THISBACK(AttributeListChanged);
	
	menu.Sub("App", [this](Bar& b){
		b.Add("Find all songs", THISBACK(FindSongs)).Key(K_CTRL_F);
		b.Add("Save", THISBACK(StoreThis)).Key(K_CTRL_S);
	});
	
	LoadThis();
	Data();
}

Editor::~Editor() {
	StoreThis();
}

void Editor::FindSongs() {
	String home = GetHomeDirectory();
	String dir = home + DIR_SEPS "AudioAssets" DIR_SEPS "music" DIR_SEPS "Composed";
	LOG("Finding files from: " + dir);
	
	Time now = GetSysTime();
	int begin_year = 2001;
	int end_year = now.year;
	
	int idx = 0;
	for (int year = begin_year; year <= end_year; year++) {
		FindFile ff;
		
		if (ff.Search(dir + DIR_SEPS + IntStr(year) + "/*.tg")) {
			do {
				LOG(idx << ": " << ff.GetPath());
				String relpath = ff.GetPath().Mid(home.GetCount() + 1);
				
				Song& song = songlist.GetAdd(relpath);
				song.year = year;
				song.name = GetFileName(relpath);
				song.name = song.name.Left(song.name.GetCount()-3);
				
				idx++;
			}
			while (ff.Next());
		}
	}
	
	SortByValue(songlist, Song());
	
	StoreThis();
	Data();
}

void Editor::Data() {
	for(int i = 0; i < songlist.GetCount(); i++) {
		Song& song = songlist[i];
		songs.Set(i, 0, songlist.GetKey(i));
		songs.Set(i, 1, song.year);
		songs.Set(i, 2, song.name);
	}
	
	if (!songs.IsCursor())
		songs.SetCursor(0);
	
	SongData();
}

void Editor::SongData() {
	int cursor = songs.GetCursor();
	if (cursor < 0) return;
	
	String key = songs.Get(cursor, 0);
	Song& song = songlist.Get(key);
	
	edit.year.SetData(song.year);
	edit.name.SetData(song.name);
	edit.topic.SetData(song.topic);
	
	int i;
	i = genre.Find(song.genre); if (i >= 0) edit.genre.SetIndex(i); else edit.genre.SetData(Value());
	i = audience.Find(song.audience); if (i >= 0) edit.audience.SetIndex(i); else edit.audience.SetData(Value());
	i = qualifier.Find(song.qualifier); if (i >= 0) edit.qualifier.SetIndex(i); else edit.qualifier.SetData(Value());
	i = attributes.Find(song.attributes); if (i >= 0) edit.attrlist.SetIndex(i); else edit.attrlist.SetData(Value());
	edit.attributes.SetData(song.attributes);
	
}

void Editor::DataChanged() {
	int cursor = songs.GetCursor();
	if (cursor < 0) return;
	
	String key = songs.Get(cursor, 0);
	Song& song = songlist.Get(key);
	
	//song.year  = edit.year.GetData();
	//song.name  = edit.name.GetData();
	song.topic = edit.topic.GetData();
	song.genre = edit.genre.GetData();
	song.audience = edit.audience.GetData();
	song.qualifier = edit.qualifier.GetData();
	song.attributes = edit.attributes.GetData();
	
}

void Editor::AttributeListChanged() {
	String a = edit.attributes.GetData();
	if (a == "" || attributes.Find(a) >= 0)
		edit.attributes.SetData(edit.attrlist.GetData());
	DataChanged();
}

void Editor::LoadThis() {
	String file = GetDataFile("songlist.xml");
	String xml = LoadFile(file);
	LoadFromXML(*this, xml);
}

void Editor::StoreThis() {
	String file = GetDataFile("songlist.xml");
	String xml = StoreAsXML(*this);
	LOG("Storing file " << file);
	FileOut fout(file);
	fout << xml;
	fout.Close();
}

void Editor::Xmlize(XmlIO& xml) {
	xml
		("songlist", songlist)
	;
}












String Song::GetPath() const {
	String home = GetHomeDirectory();
	String path = home + DIR_SEPS "AudioAssets" DIR_SEPS "music" DIR_SEPS "Composed" DIR_SEPS + IntStr(year) + DIR_SEPS + name;
	return path;
}

void Song::Xmlize(XmlIO& xml) {
	xml
		("year", year)
		("name", name)
		("topic", topic)
		("genre", genre)
		("audience", audience)
		("qualifier", qualifier)
		("attributes", attributes)
	;
}
