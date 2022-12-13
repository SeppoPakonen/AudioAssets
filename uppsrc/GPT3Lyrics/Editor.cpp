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
	qualifier.Add("dream-like");
	qualifier.Add("story-like");
	qualifier.Add("meme-like");
	qualifier.Add("fighting-like");
	qualifier.Add("news-like");
	genre.Add("");
	genre.Add("rock");
	genre.Add("funk-rock");
	genre.Add("rap");
	genre.Add("pop");
	genre.Add("traditional");
	genre.Add("dance");
	genre.Add("metal");
	genre.Add("RnB");
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
	audience.Add("young adults");
	audience.Add("young adult women");
	audience.Add("young adult men");
	audience.Add("old people");
	audience.Add("small children");
	audience.Add("children");
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
	attributes.Add("unfriendly");
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
	attributes.Add("blissful");
	attributes.Add("funny");
	attributes.Add("punishing");
	attributes.Add("colourful");
	attributes.Add("adventurous");
	attributes.Add("action-packed");
	attributes.Add("lovely");
	attributes.Add("cliffhanger");
	attributes.Add("cute");
	attributes.Add("melancholic");
	attributes.Add("hateful");
	attributes.Add("desperate");
	attributes.Add("hopeful");
	attributes.Add("hopeless");
	attributes.Add("sophisticated");
	attributes.Add("painful");
	attributes.Add("happy");
	attributes.Add("tense");
	attributes.Add("competitive");
	attributes.Add("exaggerated");
	attributes.Add("historical");
	attributes.Add("sad");
	attributes.Add("understanding");
	attributes.Add("wise");
	attributes.Add("supernatural");
	attributes.Add("space-themed");
	attributes.Add("sleepy");
	attributes.Add("soothing");
	attributes.Add("cautionary");
	attributes.Add("sarcastic");
	attributes.Add("nostalgic");
	attributes.Add("comedic");
	attributes.Add("tired");
	attributes.Add("euphoric");
	attributes.Add("clueless");
	attributes.Add("twisted");
	attributes.Add("bossy");
	attributes.Add("cheerful");
	attributes.Add("sport");
	attributes.Add("wishful");
	attributes.Add("unnerving");
	attributes.Add("liberating");
	attributes.Add("humble");
	attributes.Add("generous");
	attributes.Add("donating");
	attributes.Add("moronic");
	attributes.Add("loathing");
	attributes.Add("proud");
	attributes.Add("dark humorous");
	attributes.Add("lustful");
	attributes.Add("protective");
	attributes.Add("populous");
	attributes.Add("assembling");
	attributes.Add("existentialistic");
	attributes.Add("direct");
	attributes.Add("regretting");
	attributes.Add("honest");
	attributes.Add("unbelieveable");
	attributes.Add("vulgar");
	attributes.Add("careless");
	attributes.Add("confrontational");
	attributes.Add("thankful");
	attributes.Add("confident");
	attributes.Add("wistful"); // longing
	attributes.Add("bereft"); // longing
	attributes.Add("metaphorical about life");
	attributes.Add("metaphorical about death");
	attributes.Add("metaphorical about family");
	attributes.Add("metaphorical about happy family event");
	attributes.Add("metaphorical about happy event with friends");
	attributes.Add("metaphorical about romantic relationship");
	attributes.Add("metaphorical about bad relationship");
	attributes.Add("metaphorical about divorce");
	attributes.Add("metaphorical about sex");
	attributes.Add("metaphorical about god");
	attributes.Add("metaphorical about stalker");
	
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
	String dir = home + DIR_SEPS "AudioAssets" DIR_SEPS "music";
	LOG("Finding files from: " + dir);
	
	Index<String> subdirs;
	subdirs.Add("Release");
	subdirs.Add("Composed");
	subdirs.Add("Unfinished");
	
	Time now = GetSysTime();
	int begin_year = 2001;
	int end_year = now.year;
	
	for (String sd : subdirs) {
		int idx = 0;
		for (int year = begin_year; year <= end_year; year++) {
			if (year <= 2002 && sd == "Release")
				continue;
			
			FindFile ff;
			if (ff.Search(dir + DIR_SEPS + sd + DIR_SEPS + IntStr(year) + "/*.tg")) {
				do {
					LOG(idx << ": " << ff.GetPath());
					String relpath = ff.GetPath().Mid(home.GetCount() + 1);
					
					Song& song = songlist.GetAdd(relpath);
					song.subdir = sd;
					song.year = year;
					song.name = GetFileName(relpath);
					song.name = song.name.Left(song.name.GetCount()-3);
					
					idx++;
				}
				while (ff.Next());
			}
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
	
	edit.subdir.SetData(song.subdir);
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

String Editor::GetXmlFile() const {
	Index<String> dirs;
	dirs.Add(GetExeFolder());
	dirs.Add(GetCurrentDirectory());
	dirs.Add(GetDataFile(""));
	
	for (String dir : dirs) {
		String file = AppendFileName(dir, "songlist.xml");
		if (FileExists(file))
			return file;
	}
	
	#ifdef flagDEBUG
	return AppendFileName(GetDataFile(""), "songlist.xml");
	#else
	return AppendFileName(GetExeFolder(), "songlist.xml");
	#endif
}

void Editor::RealizeXmlPath() {
	if (xml_path.IsEmpty())
		xml_path = GetXmlFile();
}

void Editor::LoadThis() {
	RealizeXmlPath();
	LOG("Loading file " << xml_path);
	String xml = LoadFile(xml_path);
	LoadFromXML(*this, xml);
}

void Editor::StoreThis() {
	RealizeXmlPath();
	String xml = StoreAsXML(*this);
	LOG("Storing file " << xml_path);
	FileOut fout(xml_path);
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
		("subdir", subdir)
		("topic", topic)
		("genre", genre)
		("audience", audience)
		("qualifier", qualifier)
		("attributes", attributes)
	;
}
