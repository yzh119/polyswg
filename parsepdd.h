#ifndef parsepdd
#define parsepdd
#include <string>
#include <fstream>
#include <cstring>
using namespace std;
namespace pdd{
	const int maxLength = 10000;
	string transform(const string &filename) {
		ifstream fin(("article/" + filename + ".pdd").c_str()); 
		string ret = "";
		bool ul = false, ol = false;
		while (!fin.eof()) {
			{
				char cpara[maxLength]; 
				string startMark = "", closeMark = "";
				fin.getline(cpara, maxLength);
				if (strcmp(cpara, "```") == 0) {
					ret += "<pre><code>";
					while(!fin.eof()) {
						fin.getline(cpara, maxLength);
						if (strcmp(cpara, "```") == 0) {
							ret += "</code></pre>";
							break;
						}
						ret += cpara;
						ret += "\n";
					}
					continue;
				}
				int cnt = 0;
				while (cpara[cnt] == '#') cnt++;
				switch (cnt) {
					case 0: 
						startMark = "<p>";
						closeMark = "</p>";
						break;
					case 1:
						startMark = "<h1>";
						closeMark = "</h1>";
						break;
					case 2:
						startMark = "<h2>";
						closeMark = "</h2>";
						break;
					case 3:
						startMark = "<h3>";
						closeMark = "</h3>";
						break;
					case 4:
						startMark = "<h4>";
						closeMark = "</h4>";
						break;
					case 5:
						startMark = "<h5>";
						closeMark = "</h5>";
					case 6:
						startMark = "<h6>";
						closeMark = "</h6>";
					default:
						startMark = "<p>";
						closeMark = "</p>";
						break;
				}
				ret += startMark + (cpara + cnt) + closeMark;
			}	
		}
		fin.close();
		return ret;
	}
}
#endif
