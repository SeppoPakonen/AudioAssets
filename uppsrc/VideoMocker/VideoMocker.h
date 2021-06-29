#ifndef _VideoMocker_VideoMocker_h
#define _VideoMocker_VideoMocker_h

#include <CtrlLib/CtrlLib.h>
#include <plugin/jpg/jpg.h>
using namespace Upp;

#include "Project.h"


#define LAYOUTFILE <VideoMocker/VideoMocker.lay>
#include <CtrlCore/lay.h>

#define IMAGEFILE <VideoMocker/VideoMocker.iml>
#include <Draw/iml_header.h>

class SidePanel : public WithSidePanelLayout<TopWindow> {
public:
	typedef SidePanel CLASSNAME;
	SidePanel();
	
};

class RendererCtrl : public Ctrl {
public:
	void Paint(Draw& d) override;
	
};

class TimelineCtrl : public Ctrl {
public:
	void Paint(Draw& d) override;
	
};


class VideoMocker : public TopWindow {
	MenuBar menu;
	Splitter vsplit, hsplit;
	SidePanel panel;
	RendererCtrl renderer;
	TimelineCtrl timeline;
	
	VideoMockup::Project prj;
	VideoMockup::ImageFrame* img_frame = 0;
	VideoMockup::SubtitleFrame* sub_frame = 0;
	VideoMockup::LyricsFrame* lyr_frame = 0;
	
	void MainMenu(Bar& bar);
	void NewFile();
	void SaveFile();
	void SaveAsFile();
	void OpenFile();
	void DataChanged();
	void Data();
	
public:
	typedef VideoMocker CLASSNAME;
	VideoMocker();
	
};

#endif
