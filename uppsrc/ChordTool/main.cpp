#include "ChordTool.h"

#define IMAGEFILE <ChordTool/ChordTool.iml>
#include <Draw/iml_source.h>

ChordTool::ChordTool()
{
	CtrlLayout(*this, "Window title");
}

GUI_APP_MAIN
{
	ChordTool().Run();
}
