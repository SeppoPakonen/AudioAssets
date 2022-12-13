#ifndef _GPT3Lyrics_Editor_h_
#define _GPT3Lyrics_Editor_h_

#define LAYOUTFILE <GPT3Lyrics/Editor.lay>
#include <CtrlCore/lay.h>


struct Artist : Moveable<Artist> {
	String name;
	String description;
	String message;
	String brand_adjectives;
	String artist_adjectives;
	String music_adjectives;
	String genre_adjectives;
	String production_adjectives;
	String music_video_adjectives;
	String global_example;
	double global_listeners = 0;
	String local_example;
	double local_listeners = 0;

	void Xmlize(XmlIO& xml);
};

struct Song : Moveable<Song> {
	String name;
	int year;
	
	String topic;
	String genre;
	String subdir;
	String audience;
	String qualifier;
	String attributes;
	
	String GetPath() const;
	
	void Xmlize(XmlIO& xml);
	bool operator()(const Song& a, const Song& b) const {if (a.year != b.year) return a.year < b.year; return a.name < b.name;}
};

class Editor : public TopWindow {
	TabCtrl tabs;
	
	struct ArtistCtrl : WithArtistForm<ParentCtrl> {
		typedef ArtistCtrl CLASSNAME;
		Editor* e = 0;
		
		void Init();
		void Data();
		void DataChanged();
		void DataItem();
		void Add();
		void Remove();
	} a;
	
	struct Header {
		typedef Header CLASSNAME;
		Editor* e = 0;
		Splitter hsplit;
		ArrayCtrl songs;
		WithSongForm<ParentCtrl> edit;
		
		void Init();
		void Data();
		void SongData();
		void DataChanged();
		void AttributeListChanged();
	} h;
	
	struct Statistics : ParentCtrl {
		Editor* e = 0;
		
		void Init();
		void Data();
	} s;
	
	struct SongFinder : ParentCtrl {
		Editor* e = 0;
		
		void Init();
		void Data();
	} f;
	
	struct DataMining : ParentCtrl {
		Editor* e = 0;
		
		void Init();
		void Data();
	} d;
	
	MenuBar menu;
	String xml_path;
	
	ArrayMap<String,Song> songlist;
	Array<Artist> artistlist;
	
	Index<String> qualifier;
	Index<String> genre;
	Index<String> audience;
	Index<String> attributes;
	
public:
	typedef Editor CLASSNAME;
	Editor();
	~Editor();
	
	void MenuBar(Bar& bar);
	void Data();
	void FindSongs();
	void RealizeXmlPath();
	void LoadThis();
	void StoreThis();
	void Xmlize(XmlIO& xml);
	
	String GetXmlFile() const;
	
};


#endif
