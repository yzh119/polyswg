#ifndef pswglib
#define pswglib
#include <iostream>
#include <fstream>
#include <string>
#include "md2html4cpp/transform.h"
#include <boost/date_time/gregorian/greg_month.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/filesystem.hpp>
using namespace std;
const int lineLength = 1000;
/* Print information about pswg(version etc).
 * Use 'pswg' to show it
 */
void printWelcomeInfo() {
	cout << "polyswg: a light static website generator." << endl;
	cout << "Input 'pswg -h' for more information." << endl;
}

/* Print help information about pswg.
 * Use 'pswg -h' to show it
 */
void printHelpInfo() {
	cout << "Commands:" << endl;
	cout << "	pswg -a YOUR_POST_FILENAME: add a post for your website." << endl;
	cout << "	pswg -g: generate HTML files for your website." << endl;
	cout << "	pswg -d: upload your HTML files to your server." << endl;
	cout << "	pswg -i: initialize your website." << endl;
	cout << "	pswg -s: start server, you can browse your website at http://0.0.0.0:8000 " << endl;
	cout << "Your post should be written in Markdown Language. The suffix of your post's filename will be '.md'." << endl;
	cout << "All posts will be in two parts: YOUR_POST_FILENAME.md, YOUR_POST_FILENAME.txt, the first is your post and the second is the config files, mainly about your post's name, time and other informations." << endl;
	cout << "You should make sure that you've initialized before other commands." << endl;
}		

/* Print error information about pswg.
 * It will appear whenever you make a syntax error.
 */
void printErrorInfo() {
	cout << "Sorry, but you made a syntax error just now" << endl;
}

/* Create directory
 */
bool createFile(string s) {
	boost::filesystem::path pt = boost::filesystem::current_path() / s.c_str();
	if (!boost::filesystem::exists(pt)) {
		boost::filesystem::create_directory(pt);
		return 1;
	}
	return 0;
}

/* getTime: Get the current time.
 * getYear: Get the current year.
 */
string getTime() {
	boost::gregorian::date now = boost::gregorian::day_clock::local_day();
	return boost::gregorian::to_simple_string(now);
}
string getYear() {
	return getTime().substr(0, 4);
}

/* Start server
 */
void startServer() {
	
}

void addcategory(const string &newcat) {
	ifstream fin("category.txt");
	bool flag = 0;
	while (!fin.eof()) {
		string catname;
		{
			char scat[lineLength];
			fin.getline(scat, lineLength - 10);
			catname = scat;
		}
		if (catname == newcat) {
			flag = 1;
			break;
		}
	}

	fin.close();
	if (!flag) {
		ofstream fout("category.txt", ios::app);
		fout << newcat << endl;
		fout.close();
	}
}

/* Add a new post to your website.
 * Usage: 'pswg -a YOUR_POST_NAME'
 */
void addPost() {
	string postname;
	cout << "Please input your post's filename:" << endl;
	{
		char spn[lineLength];
		cin.getline(spn, lineLength - 10);
		postname = spn;
	}

	ifstream fin(("article/" + postname + ".md").c_str());
	if (fin) {
		cout << "Sorry, there exists a post which has the same name with yours." << endl;
		fin.close();
		return ;
	}
	ofstream fout("postname.txt", ios::app);
	fout << postname << endl;
	fout.close();
	
	fout.open(("article/" + postname + ".md").c_str());
	fout.close();
	
	fout.open(("article/" + postname + ".dsb").c_str());
	cout << "Please tell us your post's title:" << endl;
	string title;
	{
		char stitle[lineLength];
		cin.getline(stitle, lineLength - 10);
		title = stitle;
	}
	fout << title << endl;
	fout << getTime() << endl;
	cout << "Please tell us your post's category:" << endl;
	string category;
	{
		char scate[lineLength];
		cin.getline(scate, lineLength - 10);
		category = scate;
	}
	fout << category << endl;	
	addcategory(category);
	fout.close();
	fout.open(("category/" + category + ".txt").c_str(), ios::app);
	fout << postname << endl;
	fout.close();
}

string header = "", footer = "";
void getBasicInfo() {
	header += "<!DOCTYPE html>";
	string language = "", hostname = "", title = "", duoshuo = "", startyear = "";
	{
		ifstream fin("config.txt");
		char slan[lineLength], shost[lineLength], stitle[lineLength], sduoshuo[lineLength], sstart[lineLength];
		fin.getline(slan, lineLength - 10);
		fin.getline(shost, lineLength - 10);
		fin.getline(stitle, lineLength - 10);
		fin.getline(sduoshuo, lineLength - 10);
		fin.getline(sstart, lineLength - 10);
		language = slan; hostname = shost; title = stitle; duoshuo = sduoshuo; startyear = sstart;
		fin.close();
	}
	header += "<html lang=\"" + language + "\">";
	header += "<title>" + title + "</title>";
	header += "<head><meta http-equiv=\"content-type\" content=\"text/html\"; charset=\"UTF-8\"></meta><link href='http://fonts.googleapis.com/css?family=Open+Sans:300,400,600&subset=latin,latin-ext' rel='stylesheet'><link href=\"/css/style.css\" rel=\"stylesheet\">";
	{
		ifstream fin("header.txt");
		char sget[lineLength];
		while (!fin.eof()) {
			fin.getline(sget, lineLength - 10);
			header += sget;
		}
		fin.close();
	}
	header += "</head>";
	header += "<body>";
	{
		header += "<div class=\"nav\">";
		header += "<ul class=\"posleft\">";
		header += "<li>";
		header += "<a href=\"/\">" + title + "</a>";
		header += "</li>";	
		header += "</ul>";
		header += "<ul class=\"posright\">";
		header += "<li><a href=\"/category\">";
		header += "Category";
		header += "</a></li>";
		header += "<li><a href=\"/posts/notes\">";
		header += "Notes";
		header += "</a></li>";
		header += "<li><a href=\"/posts/about\">";
		header += "About";
		header += "</a></li>";
		header += "</ul></div>";
	}
	{
		ifstream fin("footer.txt");
		char sget[lineLength];
		while (!fin.eof()) {
			fin.getline(sget, lineLength - 10);
			footer += sget;
		}
		fin.close();
	}
	footer += "<div class=\"footer\">Copyright &copy;";
	footer += startyear + "-";
   	footer += getYear() + " ";
    footer += "<a href=\"/\">" + title + "</a>. Powered by <a href=\"https://github.com/yzh119/polyswg\">polyswg</a></div></body></html>";
}

string contentofIndex() {
	string ret = "<div class=\"container\">";
	ifstream fin("postname.txt");
	vector <string> headline, postTime, postname;
	while (1) {
		{
			char spname[lineLength];
			fin.getline(spname, lineLength - 10);
			postname.push_back(spname);
		}
		if (fin.eof()) break;
		{
			ifstream fin1(("article/" + postname.back() + ".dsb").c_str());
			char shead[lineLength], stime[lineLength];
			fin1.getline(shead, lineLength - 10);
			headline.push_back(shead);
			fin1.getline(stime, lineLength - 10);
			postTime.push_back(stime);
			
			fin1.close();
		}
	}
	ret += "<ul>";
	for (int i = (int)headline.size() - 1; i >= 0; i--) {
		if (postname[i] == "about") continue;
		if (postname[i] == "notes") continue;
		ret += "<li class=\"article\">";
		ret += "<p class=\"postname\">";
		ret += "<a href=\"posts/" + postname[i] + "/" + "\">" + headline[i] + "</a></p>";
		ret += "<p class=\"info\">";
		ret += postTime[i];
		ret += "</p>";	
		ret += "</li>";
	}
	ret += "</ul>";
	ret += "</div>";
	fin.close();
	return ret;
}

string contentofPost(const string &postname) {
	string ret = "<div class=\"container\">";
	ret += "<div class=\"post\">";
	ret += "<div class=\"posthead\">";
	ret += "<h1>";
	string headline, publishTime, category, duoshuoname, url;
	{
		char chead[lineLength], cpub[lineLength], ccate[lineLength];
		ifstream fin(("article/" + postname + ".dsb").c_str());
		fin.getline(chead, lineLength - 10);
		fin.getline(cpub, lineLength - 10);
		fin.getline(ccate, lineLength - 10);
		headline = chead;
		publishTime = cpub;
		category = ccate;
	}
	{
		char lang[lineLength], cduoshuo[lineLength], curl[lineLength], ctitle[lineLength];
		ifstream fin("config.txt");
		fin.getline(lang, lineLength - 10);
		fin.getline(curl, lineLength - 10);
		fin.getline(ctitle, lineLength - 10);
		fin.getline(cduoshuo, lineLength - 10);
		fin.close();
		duoshuoname = cduoshuo;
		url = curl;
	}
	mdtransform newpost("article/" + postname + ".md");
	url += "/posts/" + postname;
	ret += headline;
	ret += "</h1>";
	ret += "<h2>";
	ret += publishTime;
	ret += "</h2>";
	ret += "</div>";
	ret += "<div class=\"TOC\">";
	ret += "<div class=\"cont\">";
	ret += "<h2>Content</h2>";
	ret += newpost.getTableOfContents();
	ret += "</div>";
	ret += "</div>";
	ret += newpost.getContent();
	ret += "<div class=\"postend\">";
	ret += "<a class=\"backtotop\" style=\"color:#CCCCCC\"  href=\"#\">&uarr;</a>";
	ret += "<p>";
	ret += "Category: <a href=\"/category/" + category + "/\">" + category + "</a>.";
	ret += "</p>";	
	ret += "<div class=\"ds-thread\" data-thread-key=\"" + postname + "\" data-title=\"" + headline + "\" data-url=\"" + url + "\"></div><script type=\"text/javascript\">var duoshuoQuery = {short_name:\"" + duoshuoname + "\"};(function() {var ds = document.createElement('script');ds.type = 'text/javascript';ds.async = true;ds.src = (document.location.protocol == 'https:' ? 'https:' : 'http:') + '//static.duoshuo.com/embed.js';ds.charset = 'UTF-8';(document.getElementsByTagName('head')[0] || document.getElementsByTagName('body')[0]).appendChild(ds);})();</script>";
	ret += "</div></div></div>";
	return ret;
}

string contentofcategory() {
	string ret = "<div class=\"container\">";
	ifstream fin("category.txt");
	ret += "<ul>";
	while (!fin.eof()) {
		string catename;
		{
			char spname[lineLength];
			fin.getline(spname, lineLength - 10);
			catename = spname;
		}
		if (catename == "") continue;
		if (catename == "about") continue;
		if (catename == "notes") continue;
		ret += "<li class=\"article\">";
		ret += "<p class=\"postname\">";
		ret += "<a href=\"" + catename + "/" + "\">" + catename + "</a></p>";
		ret += "</li>";
	}
	ret += "</ul>";
	ret += "</div>";
	fin.close();
	return ret;
}

inline string generateIndexHTML() {
	return header + contentofIndex() + footer;
}

inline string generatePostHTML(const string &postname) {
	return header + contentofPost(postname) + footer;
}

inline string generatecategory() {
	return header + contentofcategory() + footer;
}

void generateCategory() {
	ifstream fin("category.txt");
	while (!fin.eof()) {
		string catname;
		{
			char scat[lineLength];
			fin.getline(scat, lineLength - 10);
			catname = scat;
		}
		if (catname == "about") continue;
		if (catname == "notes") continue;
		if (catname == "") continue;
		createFile("public/category/" + catname);
		ofstream fout(("public/category/" + catname + "/index.html").c_str());
		ifstream fin1(("category/" + catname + ".txt").c_str());
		fout << header << endl;
		fout << "<div class=\"container\">";
		vector <string> headline, postTime, postname;
		while (!fin1.eof()) {
			{
				char spname[lineLength];
				fin1.getline(spname, lineLength - 10);
				postname.push_back(spname);
			}
			if (postname.back() == "") continue;
			{
				ifstream fin2(("article/" + postname.back() + ".dsb").c_str());
				char shead[lineLength], stime[lineLength];
				fin2.getline(shead, lineLength - 10);
				headline.push_back(shead);
				fin2.getline(stime, lineLength - 10);
				postTime.push_back(stime);
				fin2.close();
			}
		}
		fout <<  "<ul>";
		for (int i = (int)headline.size() - 1; i >= 0; i--) {
			fout << "<li class=\"article\">";
			fout << "<p class=\"postname\">";
			fout << "<a href=\"/posts/" + postname[i] + "/" + "\">" + headline[i] + "</a></p>";
			fout <<  "<p class=\"info\">";
			fout << postTime[i];
			fout << "</p>";
			fout << "</li>";
		}
		fout << "</ul>";
		fout << "</div>";
		fout << footer << endl;
		fin1.close();
		fout.close();
	}
	fin.close();
}

/* Generate static websites from your post.
 * Usage: 'pswg -g'
 */
void generateHTML() {
	getBasicInfo();
	ofstream fout("public/index.html");
	fout << generateIndexHTML() << endl;
	fout.close();	
	cout << "index.html finished." << endl;

	fout.open("public/category/index.html");
	fout << generatecategory() << endl;
	fout.close();

	generateCategory();
	cout << "category finished." << endl;

	ifstream fin("postname.txt");
	while (!fin.eof()) {
		string postname;
		{
			char fname[lineLength];
			fin.getline(fname, lineLength);
			postname = fname; 
		}
		if (postname != "") {
			createFile("public/posts/" + postname);
//			if (createFile("public/posts/" + postname)) {
				fout.open(("public/posts/" + postname + "/index.html").c_str());			
				fout << generatePostHTML(postname) << endl;
				fout.close();
//			}
			cout << postname << " finished." << endl;
		}
	}
}

/* Deploy your website.
 * Usage: 'pswg -d'
 * Hint: you should configure correctly at first.
 */
void deploy() {
								
}

/* Initialize your website.
 * Usage: 'pswg -i'
 */
void initialize() {
	createFile("public");
	createFile("article");
	createFile("public/posts");
	createFile("public/category");
	createFile("category");
	ofstream fout1("config.txt");
	char lang[lineLength], hostname[lineLength], title[lineLength], duoshuo[lineLength];
	cout << "Please tell us the language you want to use (zh / en)." << endl;
	cin.getline(lang, lineLength);
	fout1 << lang << endl;
	cout << "Please tell us your website's url:" << endl;
	cin.getline(hostname, lineLength);
	fout1 << hostname << endl;
	cout << "Please tell us your website title:" << endl;
	cin.getline(title, lineLength);
	fout1 << title << endl;
	cout << "Please tell us your duoshuo shortname" << endl;
	cin.getline(duoshuo, lineLength);
	fout1 << duoshuo << endl;
	fout1 << getYear() << endl;
	cout << "If you want to add some code to your HTML's header or footer, please add your code in header.txt and footer.txt." << endl;
	fout1.close();
	fout1.open("header.txt", ios::app);
	fout1.close();
	fout1.open("footer.txt", ios::app);
	fout1.close();
	fout1.open("postname.txt", ios::app);
	fout1.close();
	fout1.open("category.txt", ios::app);
	fout1.close();
}
#endif
