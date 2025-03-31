#ifndef _ChordTool_ChordTool_h
#define _ChordTool_ChordTool_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define LAYOUTFILE <ChordTool/ChordTool.lay>
#include <CtrlCore/lay.h>

#define IMAGEFILE <ChordTool/ChordTool.iml>
#include <Draw/iml_header.h>

class ChordTool : public WithChordToolLayout<TopWindow> {
public:
	ChordTool();
};

#endif
