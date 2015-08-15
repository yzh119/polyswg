#Polyswg ver 0.1
一个轻便的静态网页生成器（功能还在完善中）。
---
功能：
*	添加文章，生成归档。
*	加载了highlight.js和Mathjax，支持数学公式输入。
*	支持多说评论。

待实现的功能：
*	完整的语法解析
*	ftp上传
*	本地服务器

---
#安装
需要预先安装boost库，boost库安装教程可见www.boost.org.
安装完成之后运行
> g++ pswg.cpp -o pswg -lboost_filesystem -lboost_system -lboost_date_time

即可。
#使用
*	初次使用：在安装目录下运行
	> ./pswg -i
	按照提示依次输入信息(网站的url，多说用户名，标题等）。

*	添加文章: ./pswg -a
*	生成网页: ./pswg -g
*	服务: 进入public目录，运行python -m SimpleHTTPServer
	进入http://127.0.0.1:8000 即可看到已经生成的页面。
*	部署: 尚未完成此功能，所以用ftp将public文件夹中的内容上传到主机的public_html文件夹即可。
#说明
如果要添加文章内容，进入article目录，用任一种文本编辑器（如vim）打开POSTNAME.pdd(POSTNAME是你的文章的文件名)，输入内容即可。
目前支持的语法有：用#，## ..表示一级标题、二级标题等，用两对三个反引号把代码括起来实现代码高亮。
可用通过$ $或者$$ $$输入数学公式，公式用Mathjax渲染。
#示例页面
http://expye.com
