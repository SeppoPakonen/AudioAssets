#include "WebsiteGenerator.h"

void FindRecursively(String dir, Vector<String>& files) {
	String pattern = AppendFileName(dir, "*");
	
	FindFile ff;
	if (ff.Search(pattern)) {
		do {
			String path = ff.GetPath();
			String title = GetFileName(path);
			if (title.Left(1) != ".") {
				if (ff.IsDirectory())
					FindRecursively(path, files);
				else if (GetFileExt(path) == ".tg")
					files << path;
			}
		}
		while (ff.Next());
	}
	
}

Generator::Generator() {
	author = "Seppo Pakonen";
	if (0) {
		sitemap_dir = "/home/sblo/sppp/";
		export_dir = "/home/sblo/sppp/AudioAssets/";
		if (0) {
			sitemap_root_addr = root_addr = "http://127.0.0.1";
		}
		else {
			sitemap_root_addr = "http://seppopakonen.nethub.fi/";
			root_addr = "http://seppopakonen.nethub.fi/AudioAssets";
		}
	}
	else {
		sitemap_dir = "/home/sblo/upphub/sppp.github.io/";
		export_dir = "/home/sblo/upphub/AudioAssets/docs/";
		if (0) {
			sitemap_root_addr = root_addr = "http://127.0.0.1";
		}
		else {
			sitemap_root_addr = "https://sppp.github.io/";
			root_addr = "https://sppp.github.io/AudioAssets";
		}
	}
	description = "Music composed by " + author;
	more_url = "https://github.com/sppp/AudioAssets";
	more_msg = "GitHub page";
	song_dir_to_root = "../";
	song_dir = AppendFileName(export_dir, "song");
	upphub_dir = GetHomeDirFile("upphub");
	src_tg_dir = upphub_dir + DIR_SEPS + "tmp" + DIR_SEPS + "converted";
	keywords =
		"finnish composer, finnish guitarist, sydämen asialla, pms, en tahtonut tapella, seppo p, suomalainen säveltäjä, "
		"oululainen säveltäjä, artisti, kitaristi, oululainen kitaristi, nordic composer, popular, vaikeat ajat, prio, "
		"pohjantähti, pohjantahti, hei tyttö, suomihiphop, mikseri.net, rakkauden aika, sana, ilmaista musiikkia, "
		"free music, libre, tabulatuuri, tabulatuureja, kitaratabulatuuri, tabeja, aloittelija, kitaransoitto ohje, "
		"kitaraopettaja, basso-opettaja, rumpuopettaja, rumputabulatuuri, bassotabulatuuri, ilmainen kitaranopetus, "
		"bassonopetus, free guitar teaching, free bass teaching, oululainen kitaranopetus, online kitaranopetus";
}

String Generator::GetCleanName(const Data& d) {
	String s;
	for(int i = 0; i < d.filename.GetCount(); i++) {
		int chr = d.filename[i];
		if (IsAlNum(chr))
			s.Cat(chr);
	}
	return s;
}

String Generator::GetTitleName(const Data& d) {
	String title = ToUpper(d.filename.Left(1)) + d.filename.Mid(1);
	int i = title.ReverseFind(" - ");
	if (i >= 0 && i == title.GetCount() - 4) {
		int v = title[i+3] - 'a' + 1;
		title = title.Left(i + 1);
		if      (v == 1) title << "(1st version)";
		else if (v == 2) title << "(2nd version)";
		else if (v == 3) title << "(3rd version)";
		else title << "(" << IntStr(v) << "th version)";
	}
	return title;
}

String Generator::GetRoundedDate(const Data& d) {
	String timestr_rounded;
	if (d.date_vis == 0) {
		timestr_rounded = "year " + IntStr(d.date.year);
	}
	else if (d.date_vis == 1) {
		if (d.date.month >= 12 || d.date.month < 4)
			timestr_rounded << "winter ";
		else if (d.date.month >= 3 && d.date.month < 6)
			timestr_rounded << "spring ";
		else if (d.date.month >= 6 && d.date.month < 9)
			timestr_rounded << "summer ";
		else
			timestr_rounded << "autumn ";
		timestr_rounded << IntStr(d.date.year);
	}
	else if (d.date_vis == 2) {
		timestr_rounded << Format("%Month ", d.date.month) << IntStr(d.date.year);
	}
	else {
		timestr_rounded = Format("%d.%d.%d",
		(int)d.date.day,
		(int)d.date.month,
		(int)d.date.year);
	}
	return timestr_rounded;
}

void Generator::Export() {
	RealizeDirectory(song_dir);
	
	String dir = upphub_dir + "/AudioAssets/music/Seppo Pakonen";
	if (!DirectoryExists(dir))
		return;
	
	files.Clear();
	FindRecursively(dir, files);
	
	data.Clear();
	String out = GetDataFile("pubinfo.txt");
	if (LoadFile(out) == "") {
		for(String path : files) {
			String title = GetFileTitle(path);
			String dir = GetFileDirectory(path);
			int year = StrInt(GetFileName(dir.Left(dir.GetCount()-1)));
			Data& d = data.Add();
			d.filename = title;
			d.date = Date(year, 1, 1);
		}
		Sort(data, Data());
		StoreAsJsonFile(data, out, true);
		LOG("No pubinfo data found. Exported template. Exiting early.");
		return;
	}
	
	LoadFromJsonFile(data, out);
	Sort(data, Data());
	
	
	int idx = 0;
	for(Data& d : data) {
		String path = dir + DIR_SEPS + IntStr(d.date.year) + DIR_SEPS + d.filename + ".tg";
		if (!(FileExists(path))) {
			LOG("File doesn't exist: " << path);
			SetExitCode(1);
			return;
		}
		d.last_edit = FileGetTime(path);
		d.idx = idx++;
	}
	
	// Get the recently edited file list
	Sort(data, DataEditSort());
	fresh_list.Clear();
	for(int i = 0; i < 4; i++)
		fresh_list << data[i].idx;
	
	// Restore default sorting
	Sort(data, Data());
	
	Time now = GetSysTime();
	String tmpl_path = GetDataFile("song_template") + DIR_SEPS + "index.htm";
	String tmpl = LoadFile(tmpl_path);
	
	String front_html;
	
	
	String lastmod = Format("%04d-%02d-%02d`T%02d:%02d:%02d`+00:00",
		(int)now.year,
		(int)now.month,
		(int)now.day,
		(int)now.hour,
		(int)now.minute,
		(int)now.second);
	
	String sitemap_txt_out = AppendFileName(sitemap_dir, "audioassets-sitemap.txt");
	String sitemap_out = AppendFileName(sitemap_dir, "audioassets-sitemap.xml");
	FileOut sitemap_txt_fout(sitemap_txt_out);
	FileOut sitemap_fout(sitemap_out);
	
	sitemap_txt_fout << sitemap_root_addr + "\n";
    sitemap_txt_fout << root_addr + "\n";
    
	sitemap_fout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?><?xml-stylesheet type=\"text/xsl\" href=\"" + sitemap_root_addr + "main-sitemap.xsl\"?>\n<urlset xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:image=\"http://www.google.com/schemas/sitemap-image/1.1\" xsi:schemaLocation=\"http://www.sitemaps.org/schemas/sitemap/0.9 http://www.sitemaps.org/schemas/sitemap/0.9/sitemap.xsd http://www.google.com/schemas/sitemap-image/1.1 http://www.google.com/schemas/sitemap-image/1.1/sitemap-image.xsd\" xmlns=\"http://www.sitemaps.org/schemas/sitemap/0.9\">\n";
	sitemap_fout << "<url>\n";
    sitemap_fout << "\t<loc>" + sitemap_root_addr + "</loc>\n";
    sitemap_fout << "\t<lastmod>" + lastmod + "</lastmod>\n";
    sitemap_fout << "</url>\n";
	sitemap_fout << "<url>\n";
    sitemap_fout << "\t<loc>" + root_addr + "</loc>\n";
    sitemap_fout << "\t<lastmod>" + lastmod + "</lastmod>\n";
    sitemap_fout << "</url>\n";

	idx = 0;
	for(Data& d : data) {
		String s = tmpl;
		String song_title = GetTitleName(d);
		String clean_name = GetCleanName(d);
		String title = song_title + " tabulature - by Seppo Pakonen";
		String page_url = GetUrl(d);
		String tg_url = GetTGUrl(d);
		String timestr = Format("%04d-%02d-%02d %02d:%02d:%02d",
			(int)d.last_edit.year,
			(int)d.last_edit.month,
			(int)d.last_edit.day,
			(int)d.last_edit.hour,
			(int)d.last_edit.minute,
			(int)d.last_edit.second);
		
		String timestr_rounded = GetRoundedDate(d);
		
		String description;
		
		
		s.Replace("${TITLE}", title);
		s.Replace("${SONGTITLE}", song_title);
		s.Replace("${SONGDIRTOROOT}", song_dir_to_root);
		s.Replace("${ROOTADDR}", root_addr);
		s.Replace("${DESCRIPTION}", description);
		s.Replace("${MOREURL}", more_url);
		s.Replace("${MOREMSG}", more_msg);
		s.Replace("${TG_FILE_URL}", tg_url);
		s.Replace("${AUTHOR}", author);
		s.Replace("${TABCOUNT}", IntStr(data.GetCount()));
		s.Replace("${DATETIME}", timestr);
		s.Replace("${DATETIME_ROUNDED}", timestr_rounded);
		s.Replace("${KEYWORDS}", keywords);
		
		s.Replace("${CUR_YEAR}", IntStr(now.year));
		
		for(int i = 0; i < 2; i++) {
			int next = (idx + 1 + i) % data.GetCount();
			const Data& f = data[next];
			
			String is = IntStr(i);
			String sugg_img = song_dir_to_root + "images/tab-default.jpg";
			String sugg_url = GetUrl(f);
			String sugg_year = IntStr(f.date.year);
			String sugg_title = GetTitleName(f);
			
			s.Replace("${SUGG_TITLE_" + is + "}", sugg_title);
			s.Replace("${SUGG_URL_" + is + "}", sugg_url);
			s.Replace("${SUGG_IMG_" + is + "}", sugg_img);
			s.Replace("${SUGG_YEAR_" + is + "}", sugg_year);
		}
		
		for(int i = 0; i < 4; i++) {
			String is = IntStr(i);
			const Data& f = data[fresh_list[i]];
			
			String fresh_url = GetUrl(f);
			String fresh_year = IntStr(f.date.year);
			String fresh_img = song_dir_to_root + "images/tab-default.jpg";
			String fresh_title = GetTitleName(f);
			String fresh_rounded = GetRoundedDate(f);
			
			s.Replace("${FRESH_TITLE_" + is + "}", fresh_title);
			s.Replace("${FRESH_URL_" + is + "}", fresh_url);
			s.Replace("${FRESH_IMG_" + is + "}", fresh_img);
			s.Replace("${FRESH_YEAR_" + is + "}", fresh_year);
			s.Replace("${FRESH_DATE_ROUNDED_" + is + "}", fresh_rounded);
		}
		
		String html_path = AppendFileName(song_dir, clean_name + ".html");
		//LOG("Writing: " << html_path);
		FileOut fout(html_path);
		fout << s;
		fout.Close();
		
		// Copy tg
		String src_tg = AppendFileName(src_tg_dir, d.filename + ".tg");
		if (!FileExists(src_tg)) {
			LOG("error: file doesn't exist: " << src_tg);
			return;
		}
		String dst_tg = song_dir + DIR_SEPS + GetCleanName(d) + ".tg";
		if (!FileCopy(src_tg, dst_tg)) {
			LOG("error: couldn't copy file");
			LOG("       from " << src_tg);
			LOG("       to   " << dst_tg);
			return;
		}
		
		
		
		// Front page
		{
			String s;
			s << "		   <div class=\"item first\" style=\"width: 293px; float: left; \">\n"
			  << "	         <div class=\"inside\">\n"
			  << "	      <figure class=\"img-polaroid figure_video\">\n"
			  << "	        <a class=\"various iframe lightbox-video\" href=\"${ITEM_URL}\">\n"
			  << "	          <img src=\"${ITEM_IMG}\" alt=\"\">\n"
			  << "	          <span class=\"video_icon\"></span>\n"
			  << "	                    <span class=\"video_title\">${ITEM_YEAR}</span>\n"
			  << "	                  </a>\n"
			  << "	      </figure>\n"
			  << "	      <p class=\"\">\n"
			  << "	         ${ITEM_TITLE} <br>\n"
			  << "	            Published on ${DATE_ROUNDED} </p>\n"
			  << "	              </div>\n"
			  << "	       </div>\n\n\n";
			
			String year = IntStr(d.date.year);
			String img = "images/tab-default.jpg";
			
			s.Replace("${ITEM_URL}", page_url);
			s.Replace("${ITEM_IMG}", img);
			s.Replace("${ITEM_YEAR}", year);
			s.Replace("${ITEM_TITLE}", song_title);
			s.Replace("${DATE_ROUNDED}", timestr_rounded);
			
			
			front_html = s + "\n\n" + front_html;
		}
		
		// Sitemap
		{
			sitemap_txt_fout << page_url + "\n";
			
			sitemap_fout << "<url>\n";
	        sitemap_fout << "\t<loc>" + page_url + "</loc>\n";
	        sitemap_fout << "\t<lastmod>" + lastmod + "</lastmod>\n";
	        sitemap_fout << "</url>\n";
		}
       
		idx++;
	}
	
	sitemap_fout << "</urlset>\n";
	
	
	
	
	{
		String tmpl_path = GetDataFile("site_template") + DIR_SEPS + "index.html";
		String s = LoadFile(tmpl_path);
		
		s.Replace("${KEYWORDS}", keywords);
		s.Replace("${TITLE}", description);
		s.Replace("${SONGDIRTOROOT}", song_dir_to_root);
		s.Replace("${ROOTADDR}", root_addr);
		s.Replace("${DESCRIPTION}", description);
		s.Replace("${MOREURL}", more_url);
		s.Replace("${MOREMSG}", more_msg);
		s.Replace("${AUTHOR}", author);
		s.Replace("${TABCOUNT}", IntStr(data.GetCount()));
		s.Replace("${CUR_YEAR}", IntStr(now.year));
		s.Replace("${CONTENT}", front_html);
		
		String html_path = AppendFileName(export_dir, "index.html");
		LOG("Writing " << html_path);
		FileOut fout(html_path);
		fout << s;
		fout.Close();
	}
	
	
}

CONSOLE_APP_MAIN {
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	Generator().Export();
	
}
