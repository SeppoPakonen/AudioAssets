#ifndef _AI_Tool_Data_h_
#define _AI_Tool_Data_h_

struct Artist {
	String name;
	int year_of_birth = 0;
	int year_of_career_begin = 0;
	String biography;
	String musical_style;
	String vibe_of_voice;
	String acoustic_instruments;
	String electronic_instruments;
	
	void Clear() {
		name.Clear();
		year_of_birth = 0;
		year_of_career_begin = 0;
		biography.Clear();
		musical_style.Clear();
		vibe_of_voice.Clear();
		acoustic_instruments.Clear();
		electronic_instruments.Clear();
	}
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("year_of_birth", year_of_birth)
			("year_of_career_begin", year_of_career_begin)
			("biography", biography)
			("musical_style", musical_style)
			("vibe_of_voice", vibe_of_voice)
			("acoustic_instruments", acoustic_instruments)
			("electronic_instruments", electronic_instruments);
	}
};

struct Story {
	int year = 0;
	String title;
	String meaning;
	String devices;
	String emotion;
	String references;
	String structure;
	String history;
	String storyline;
	String implications;
	
	void Clear() {
		year = 0;
		title.Clear();
		meaning.Clear();
		devices.Clear();
		emotion.Clear();
		references.Clear();
		structure.Clear();
		history.Clear();
		storyline.Clear();
		implications.Clear();
	}
	void Jsonize(JsonIO& json) {
		json
			("year", year)
			("title", title)
			("meaning", meaning)
			("devices", devices)
			("emotion", emotion)
			("references", references)
			("structure", structure)
			("history", history)
			("storyline", storyline)
			("implications", implications)
			;
	}
};

struct LinePattern {
	String text;
	Index<String> pronouns;
	String explanation;
	Index<String> elements;
	Index<String> interactions;
	Index<String> interactions_with;
	
	Index<String> moral_interactions;
	Index<String> acting_styles;
	Index<String> tones;
	Index<String> voiceover;
	Index<String> comedic_scenarios;
	
	Index<String> dramatic_scenarios;
	Index<String> comedic_attributes;
	Index<String> humor_attributes;
	String type_of_sentence;
	
	
	void Jsonize(JsonIO& json) {
		json
			("text", text)
			("pronouns", pronouns)
			("explanation", explanation)
			("elements", elements)
			("interactions", interactions)
			("interactions_with", interactions_with)
			
			("moral_interactions", moral_interactions)
			("acting_styles", acting_styles)
			("tones", tones)
			("voiceover", voiceover)
			("comedic_scenarios", comedic_scenarios)
			
			("dramatic_scenarios", dramatic_scenarios)
			("comedic_attributes", comedic_attributes)
			("humor_attributes", humor_attributes)
			("type_of_sentence", type_of_sentence)
			;
	}
};

struct Pattern {
	String artist;
	String title;
	String part;
	Array<LinePattern> lines;
	
	
	
	void Clear() {
		lines.Clear();
	}
	void Jsonize(JsonIO& json) {
		json
			("artist", artist)
			("title", title)
			("part", part)
			("pattern", lines)
			;
	}
};


#endif
