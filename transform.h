#ifndef MD2HTML
#define MD2HTML

#include <cstdlib>
#include <fstream>
#include <vector>
#include <cstring>
#include <utility>
#include <string>
#include <cctype>
#include <cstdio>
using namespace std;

class mdtransform{
	private:
		enum{
			maxLength		= 10000,
			maxTag			= 20, 
			nul			= 0,
			paragraph		= 1,
			href			= 2,
			ul			= 3,
			ol			= 4,
			li			= 5,
			em			= 6,
			strong			= 7,
			hr			= 8,
			br			= 9,
			image			= 10,
			quote			= 11,
			h1			= 12,
			h2			= 13,
			h3			= 14,
			h4			= 15,
			h5			= 16,
			h6			= 17,
			blockcode		= 18,
			code			= 19
		};

		inline string convert(int x) {
			if (x == 0) return "0";
			string ret = "";
			char rev[10], cnt = 0;
			while (x) {
				rev[cnt++] = '0' + (x % 10);
				x = x / 10;
			}
			for (int i = cnt - 1; i >= 0; i--) {
				ret += rev[i];
			}
			return ret;
		}

		char s[maxLength];
		const string frontTag[maxTag] = {
			"",
			"<p>",
			"",
			"<ul>",
			"<ol>",
			"<li>",
			"<em>",
			"<strong>",
			"<hr color=#CCCCCC size=1 />",
			"<br />",
			"",
			"<blockquote>",
			"<h1 ",
			"<h2 ",
			"<h3 ",
			"<h4 ",
			"<h5 ",
			"<h6 ",
			"<pre><code>",
			"<code>"
		};
		const string backTag[maxTag] = {
			"",
			"</p>",
			"",
			"</ul>",
			"</ol>",
			"</li>",
			"</em>",
			"</strong>",
			"",
			"",
			"",
			"</blockquote>",
			"</h1>",
			"</h2>",
			"</h3>",
			"</h4>",
			"</h5>",
			"</h6>",
			"</code></pre>",
			"</code>"
		};

		/* TypeList: 
		 * 0: null							|start
		 * 1: <p>							|paragraph
		 * 2: <a href=" ">...</a>					|hyperreference
		 * 3: <ul>							|unordered list
		 * 4: <ol>							|ordered list
		 * 5: <li>							|list
		 * 6: <em>							|emphasize
		 * 7: <strong>							|strong
		 * 8: <hr />							|cut=line
		 * 9: <br />							|break
		 * 10: <img alt="" src="" />					|image
		 * 11: <blockquote>						|quote
		 * 12: <h1>							|h1							
		 * 13: <h2>							|h2
		 * 14: <h3>							|h3
		 * 15: <h4>							|h4
		 * 16: <h5>							|h5
		 * 17: <h6>							|h6
		 * 18: <pre><code>                  				|blockcode
		 * 19: <code>							|inlinecode
		 */

		string content, TOC;

		int cntTag = 0;

		struct Cnode {
			vector <Cnode *> ch;
			string heading;
			string tag;
			Cnode (const string &hd): heading(hd) {}
		}*Croot;

		struct node {
			int type;
			vector <node *> ch;
			string elem[3];
			node (int tp): type(tp) {}
			~node() {}
		}*root, *now;

		inline bool isHeading(node *v) {return (v->type >= h1 && v->type <= h6);}
		inline bool isImage(node *v) {return (v->type == image);}
		inline bool isHref(node *v) {return (v->type == href);}

		void Cdestroy(Cnode *v) {
			for (int i = 0; i < (int)v->ch.size(); i++) {
				Cdestroy(v->ch[i]);
			}
			delete v;
		}

		void destroy(node *v) {
			for (int i = 0; i < (int)v->ch.size(); i++) {
				destroy(v->ch[i]);
			}
			delete v;
		}

		void Cdfs(Cnode *v, string index) {
			TOC += "<li>\n";
			TOC += "<a href=\"#" + v->tag + "\">" + index + " " + v->heading + "</a>\n";
			int n = (int)v->ch.size();
			if (n) {
				TOC += "<ul>\n";
				for (int i = 0; i < n; i++) {
					Cdfs(v->ch[i], index + convert(i + 1) + ".");
				}
				TOC += "</ul>\n";
			}
			TOC += "</li>\n";
		}

		void Cins(Cnode *v, int x, const string &hd, int tag) {
			int n = (int)v->ch.size();
			if (x == 1) {
				v->ch.push_back(new Cnode(hd));
				v->ch.back()->tag = "tag" + convert(tag);
				return ;
			}

			if (!n || v->ch.back()->heading.empty()) v->ch.push_back(new Cnode(""));
			Cins(v->ch.back(), x - 1, hd, tag);
		}

		void dfs(node *v) {
			if (v->type == paragraph && v->elem[0].empty() && v->ch.empty()) return ;
			content += frontTag[v->type];
			bool flag = true;
			if (isHeading(v)) {
				content += "id=\"" + v->elem[0] + "\">";
				flag = false;
			}

			if (isHref(v)) {
				content += "<a href=\"" + v->elem[1] + "\" title=\"" + v->elem[2] + "\">" + v->elem[0] + "</a>";
				flag = false;
			}	

			if (isImage(v)) {
				content += "<img alt=\"" + v->elem[0] + "\" src=\"" + v->elem[1] + "\" title=\"" + v->elem[2] + "\" />"; 
				flag = false;
			}

			if (flag) {
				content += v->elem[0];
				flag = false;	
			}

			for (int i = 0; i < (int)v->ch.size(); i++) {
				dfs(v->ch[i]);
			}

			content += backTag[v->type];
		}

		inline bool isCutline(char *src) {
			int cnt = 0;
			char *ptr = src;
			while (*ptr) {
				if (*ptr != ' ' && *ptr != '\t' && *ptr != '-' && *ptr != '*') return false;	
				if (*ptr == '-' || *ptr == '*') cnt++;
				ptr++;
			}
			return (cnt >= 3);
		}

		inline void mkpara(node *v) {
			if (v->ch.size() == 1u && v->ch.back()->type == paragraph) return;
			if (v->type == paragraph) return ;
			if (v->type == nul) {v->type = paragraph; return ;}
			node *x = new node(paragraph);
			x->ch = v->ch;
			v->ch.clear();
			v->ch.push_back(x);
		}

		inline pair <int, char *> start(char *src) {
			int n = (int)strlen(src);
			if (n == 0) return make_pair(0, nullptr);
			int cntspace = 0, cnttab = 0;
			char *ptr = src;
			bool flag = true;
			for (int i = 0; i < n; i++) {
				if (src[i] == ' ') {cntspace++; continue;}
				if (src[i] == '\t') {cnttab++; continue;}
				flag = false; ptr = src + i; break;
			}
			if (flag) return make_pair(0, nullptr);
			return make_pair(cnttab + cntspace / 4, ptr);
		}

		inline pair <int, char *> JudgeType(char *src) {
			char *ptr = src;
			while (*ptr == '#') ptr++;
			if (ptr - src > 0 && *ptr == ' ') {
				return make_pair(ptr - src + h1 - 1, ptr + 1);
			}
			ptr = src;
			if (strncmp(ptr, "```", 3) == 0) return make_pair(blockcode, ptr + 3);  
			if (*ptr == '*' || *ptr == '+' || *ptr == '-' && (ptr[1] == ' ')) return make_pair(ul, ptr + 1);
			if (*ptr == '>' && (ptr[1] == ' ')) return make_pair(quote, ptr + 1);
			char *ptr1 = ptr;
			while (*ptr1 && (isdigit(*ptr1))) ptr1++;
			if (ptr1 != ptr && *ptr1 == '.' && ptr1[1] == ' ') return make_pair(ol, ptr1 + 1);
			return make_pair(paragraph, ptr);
		}

		inline node* findnode(int depth) {
			node *ptr = root;
			while (!ptr->ch.empty() && depth != 0) {
				ptr = ptr->ch.back();
				if (ptr->type == li) depth--;
			}
			return ptr;
		}

		inline string conver(char *src) {
			string ret = "";
			while (*src) {
				char x = *(src++);
				ret += x;
			}
			return ret;
		}

		inline string rep(char ch) {
			string re = "";
			switch (ch) {
				case '>': re = "&gt;"; break;
				case '<': re = "&lt;"; break;
				case '&': re = "&amp;"; break;
				case '"': re = "&quot;"; break;
				case '\'': re = "&apos;"; break;
				default: re += ch; break;
			}
			return re;
		}

		void insert(node *v, const string &src) {
			int n = (int)src.size();
			bool 
				incode = false,
					   inmath = false,
					   inem = false,
					   instrong = false,
					   inautolink = false;
			v->ch.push_back(new node(nul));

			for (int i = 0; i < n; i++) {
				char ch = src[i];
				if (ch == '\\' && !inmath) {
					ch = src[++i];
					v->ch.back()->elem[0] += rep(ch);
					continue;
				}

				if (ch == '`') {
					if (incode) {
						v->ch.push_back(new node(nul));
					} else v->ch.push_back(new node(code));
					incode = !incode;
					continue;
				}

				if (ch == '$' && !incode) {
					bool blk = false;
					if (i < n - 1 && src[i + 1] == '$') {ch = src[++i]; blk = true;}
					if (inmath) {
						v->ch.back()->elem[0] += (blk ? "$$": "$");
						v->ch.push_back(new node(nul));		
					} else {
						v->ch.push_back(new node(nul));
						v->ch.back()->elem[0] += (blk ? "$$": "$");
					}
					inmath = !inmath;
					continue;
				}

				if ((ch == '*' || ch == '_') && (i < n - 1 && (src[i + 1] == '*' || src[i + 1] == '_')) && !incode && !inmath) {
					++i;
					if (instrong) {
						v->ch.push_back(new node(nul));
					} else v->ch.push_back(new node(strong));
					instrong = !instrong;
					continue;
				}

				if ((ch == '*' || ch == '_') && !incode && !inmath && !instrong) {
					if (inem) {
						v->ch.push_back(new node(nul));
					} else v->ch.push_back(new node(em));
					inem = !inem;
					continue;
				}

				if (ch == '<' && !inautolink && !incode && !inmath && !instrong && !inem) {
					v->ch.push_back(new node(href));
					inautolink = true;
					continue;
				}

				if (ch == '>' && inautolink) {
					v->ch.push_back(new node(nul));
					inautolink = false;
					continue;
				}

				if (ch == '!' && (i < n - 1 && src[i + 1] == '[') && !incode && !inmath && !instrong && !inem && !inautolink) {
					v->ch.push_back(new node(image));
					for (i += 2; i < n - 1 && src[i] != ']'; i++)
						v->ch.back()->elem[0] += rep(src[i]);
					i++;
					for (i++; i < n - 1 && src[i] != ' ' && src[i] != ')'; i++)
						v->ch.back()->elem[1] += rep(src[i]);
					if (src[i] != ')')
						for (i++; i < n - 1 && src[i] != ')'; i++)
							if (src[i] != '"') v->ch.back()->elem[2] += rep(src[i]);
					v->ch.push_back(new node(nul));
					continue;
				}

				if (ch == '[' && !incode && !inmath && !instrong && !inem && !inautolink) {
					v->ch.push_back(new node(href));
					for (i++; i < n - 1 && src[i] != ']'; i++)
						v->ch.back()->elem[0] += rep(src[i]);
					i++;
					for (i++; i < n - 1 && src[i] != ' ' && src[i] != ')'; i++)
						v->ch.back()->elem[1] += rep(src[i]);
					if (src[i] != ')')
						for (i++; i < n - 1 && src[i] != ')'; i++)
							if (src[i] != '"') v->ch.back()->elem[2] += rep(src[i]);
					v->ch.push_back(new node(nul));
					continue;	
				}

				v->ch.back()->elem[0] += rep(ch);
				if (inautolink) v->ch.back()->elem[1] += rep(ch);
			}	
			if (src.size() >= 2) if (src.at(src.size() - 1) == ' ' && src.at(src.size() - 2) == ' ') v->ch.push_back(new node(br));
		}

	public:
		mdtransform(const string &filename) {
			Croot = new Cnode("");
			root = new node(nul); now = root;	
			ifstream fin(filename.c_str());
			bool newpara = false;
			bool inblock = false;
			while (!fin.eof()) {
				fin.getline(s, maxLength);	

				if (!inblock && isCutline(s)) {
					now = root;
					now->ch.push_back(new node(hr));
					newpara = false;
					continue;
				}

				pair <int, char *> ps = start(s);

				if (!inblock && ps.second == nullptr) {
					now = root;
					newpara = true;
					continue;
				}

				pair <int, char *> tj = JudgeType(ps.second);

				if (tj.first == blockcode) {
					if (inblock) {
						now->ch.push_back(new node(nul));
					} else {
						now->ch.push_back(new node(blockcode));
					}
					inblock = !inblock;
					continue;
				}

				if (inblock) {
					for (char *ch = s; *ch; ch++) now->ch.back()->elem[0] += rep(*ch);
					now->ch.back()->elem[0] += "\n";
					continue;
				}	

				if (tj.first == paragraph) {
					if (now == root) now = findnode(ps.first);
					if (now == root) {
						now->ch.push_back(new node(paragraph));
						now = now->ch.back();
					}
					bool flag = false;
					if (newpara && !now->ch.empty()) {
						node* ptr = nullptr;
						for (auto i: now->ch) {
							if (i->type == nul) ptr = i;
						}
						if (ptr != nullptr) mkpara(ptr);
						flag = true;
					}
					if (flag) {
						now->ch.push_back(new node(paragraph));
						now = now->ch.back();
					}
					now->ch.push_back(new node(nul));
					insert(now->ch.back(), conver(tj.second));
					newpara = false;
					continue;
				}	

				now = findnode(ps.first);

				if (tj.first >= h1 && tj.first <= h6) {
					now->ch.push_back(new node(tj.first));
					now->ch.back()->elem[0] = "tag" + convert(++cntTag);
					insert(now->ch.back(), conver(tj.second));
					Cins(Croot, tj.first - h1 + 1, conver(tj.second), cntTag);
				}

				if (tj.first == ul) {
					if (now->ch.empty() || now->ch.back()->type != ul) {
						now->ch.push_back(new node(ul));
					}
					now = now->ch.back();
					bool flag = false;
					if (newpara && !now->ch.empty()) {
						node* ptr = nullptr;
						for (auto i: now->ch) {
							if (i->type == li) ptr = i;
						}
						if (ptr != nullptr) mkpara(ptr);
						flag = true;
					}
					now->ch.push_back(new node(li));
					now = now->ch.back();
					if (flag) {
						now->ch.push_back(new node(paragraph));
						now = now->ch.back();
					}
					insert(now, conver(tj.second));
				}

				if (tj.first == ol) {
					if (now->ch.empty() || now->ch.back()->type != ol) {
						now->ch.push_back(new node(ol));
					}
					now = now->ch.back();
					bool flag = false;
					if (newpara && !now->ch.empty()) {
						node* ptr = nullptr;		
						for (auto i: now->ch) {
							if (i->type == li) ptr = i;
						}		
						if (ptr != nullptr) mkpara(ptr);
						flag = true;
					}
					now->ch.push_back(new node(li));
					now = now->ch.back();
					if (flag) {
						now->ch.push_back(new node(paragraph));
						now = now->ch.back();
					}
					insert(now, conver(tj.second));
				}

				if (tj.first == quote) {
					if (now->ch.empty() || now->ch.back()->type != quote) {
						now->ch.push_back(new node(quote));
					}	
					now = now->ch.back();
					if (newpara || now->ch.empty()) now->ch.push_back(new node(paragraph));
					insert(now->ch.back(), conver(tj.second));
				}

				newpara = false;

			}	
			fin.close();

			dfs(root);
			TOC += "<ul>";
			for (int i = 0; i < (int)Croot->ch.size(); i++) Cdfs(Croot->ch[i], convert(i + 1) + ".");
			TOC += "</ul>";
		}

		string getContent() {
			return content;
		}

		string getTableOfContents() {
			return TOC;
		}

		~mdtransform() {
			destroy(root);
			Cdestroy(Croot);
		}
};
#endif
