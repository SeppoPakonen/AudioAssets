#ifndef _BatchDraftTool_BatchDraftTool_h
#define _BatchDraftTool_BatchDraftTool_h

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#define IMAGECLASS Imgs
#define IMAGEFILE <BatchDraftTool/BatchDraftTool.iml>
#include <Draw/iml_header.h>

#define LAYOUTFILE <BatchDraftTool/BatchDraftTool.lay>
#include <CtrlCore/lay.h>

class BatchDraftTool : public TopWindow {
	MenuBar		menu;
	Splitter	hsplit;
	ArrayCtrl	files;
	DocEdit     edit;
	
	TimeCallback tc;
	String dir, activepath;
	bool active_exists = false;
	bool next_file_on_enter = false;
	
public:
	typedef BatchDraftTool CLASSNAME;
	BatchDraftTool();
	~BatchDraftTool() {StoreThis(); SaveFile();}
	
	void StoreThis() {StoreToFile(*this, ConfigFile("app.bin"));}
	void LoadThis() {LoadFromFile(*this, ConfigFile("app.bin"));}
	void Serialize(Stream& s) {s % dir;}
	void DataDirectory();
	void NextFile();
	void DataFile();
	void SaveFile();
	void LoadFile();
	void EndCursor();
	void BatchCreate();
	bool HasDir() const;
	
};

#endif
