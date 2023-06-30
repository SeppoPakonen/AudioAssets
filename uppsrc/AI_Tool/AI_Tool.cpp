#include "AI_Tool.h"

#define IMAGECLASS AppImg
#define IMAGEFILE <AI_Tool/App.iml>
#include <Draw/iml_source.h>


GUI_APP_MAIN {
	SetLanguage(GetSystemLNG());
	ToolApp().Run();
}
