#Polyswg Ver 1.0
一个简易的静态博客生成器

#功能
*	添加文章，生成归档。
*	加载了highlight.js和Mathjax，支持数学公式输入。
*	支持多说评论。
*	使用Markdown标记语言

#安装
需要预先安装boost库，boost库安装教程可见http://www.boost.org.  
安装完成之后运行
`g++ -std=c++11 pswg.cpp -o pswg -O2 -lboost_filesystem -lboost_system -lboost_date_time`
即可。

#使用
*	初始化：在安装目录下运行
	`./pswg -i`
*	添加文章: `./pswg -a`
*	生成网页: `./pswg -g`
*	服务: 进入`public`目录，运行`python -m SimpleHTTPServer`
	进入http://127.0.0.1:8000 即可看到已经生成的页面。
*	部署: 如果是虚拟主机，将public文件夹内的内容复制到主机的public_html文件夹下，如果是github pages，按照官网上的步骤进行。

#说明
如果要添加文章内容，进入article目录，用任意一种文本编辑器编辑postname.md。
