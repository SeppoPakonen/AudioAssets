#ifndef _GPT3Lyrics_Editor_h_
#define _GPT3Lyrics_Editor_h_

#define LAYOUTFILE <GPT3Lyrics/Editor.lay>
#include <CtrlCore/lay.h>


struct Artist : Moveable<Artist> {
	String name;
	String description;
	String message;
	String global_example;
	double global_listeners = 0;
	String local_example;
	double local_listeners = 0;

	String pos_brand_adjectives;
	String pos_artist_adjectives;
	String pos_music_adjectives;
	String pos_genre_adjectives;
	String pos_production_adjectives;
	String pos_music_video_adjectives;
	
	String neg_brand_adjectives;
	String neg_artist_adjectives;
	String neg_music_adjectives;
	String neg_genre_adjectives;
	String neg_production_adjectives;
	String neg_music_video_adjectives;
	
	String pop_brand_adjectives;
	String pop_artist_adjectives;
	String pop_music_adjectives;
	String pop_genre_adjectives;
	String pop_production_adjectives;
	String pop_music_video_adjectives;
	
	String rare_brand_adjectives;
	String rare_artist_adjectives;
	String rare_music_adjectives;
	String rare_genre_adjectives;
	String rare_production_adjectives;
	String rare_music_video_adjectives;
	
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
