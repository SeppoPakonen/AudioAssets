#ifndef _AI_Tool_AI_Tool_h
#define _AI_Tool_AI_Tool_h

#include <CtrlLib/CtrlLib.h>
#include <CodeEditor/CodeEditor.h>
#define TFILE <AI_Tool/AI_Tool.t>
#include <Core/t.h>
using namespace Upp;

#include "AttrCtrl.h"

#define IMAGECLASS AppImg
#define IMAGEFILE <AI_Tool/App.iml>
#include <Draw/iml_header.h>

#define LAYOUTFILE <AI_Tool/App.lay>
#include <CtrlCore/lay.h>

#include "Optimizer.h"
#include "Data.h"
#include "PlotCtrl.h"
#include "PatternCtrl.h"
#include "ScoreCtrl.h"
#include "AttrScoreCtrl.h"
#include "ProductionCtrl.h"
#include "ReverseCtrl.h"
#include "TimelineCtrl.h"
#include "App.h"

#endif
