#include <Core/Core.h>

using namespace Upp;

void GoDir(String dir, String prefix, Index<String>& files) {
	FindFile sub;
	sub.Search(AppendFileName(dir, "*"));
	do {
		String n = sub.GetName();
		if (n.Left(1) == ".")
			continue;
		String ext = GetFileExt(n);
		if (ext == ".upp")
			continue;
		if (sub.IsDirectory())
			GoDir(sub.GetPath(), prefix + n + "/", files);
		else
			files.Add(prefix + n);
	}
	while (sub.Next());
}

CONSOLE_APP_MAIN
{
	String dir = CommandLine()[0];
	
	FindFile ff;
	ff.Search(AppendFileName(dir, "*"));
	do {
		if (!ff.IsDirectory())
			continue;
		String title = ff.GetName();
		if (title.Left(1) == ".")
			continue;
		String subdir = AppendFileName(dir, title);
		String upp = AppendFileName(subdir, title + ".upp");
		String upp_content = "file\n";
		
		Index<String> files;
		GoDir(subdir, "", files);
		
		for(int i = 0; i < files.GetCount(); i++) {
			String n = files[i];
			upp_content << "\t\"" << n;
			if (i+1 == files.GetCount())
				upp_content << "\";\n";
			else
				upp_content << "\",\n";
		}
		LOG(">>>>> " << upp << "\n" << upp_content << "\n\n");
		FileOut fout(upp);
		fout << upp_content;
	}
	while (ff.Next());
}
