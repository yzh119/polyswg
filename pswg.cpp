#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include "pswglib.h"
using namespace std;
int main(int argc, char *argv[]) {
	switch (argc) {
		case 1:
			printWelcomeInfo();
			break;
		case 2:
			if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-help") == 0) {
				printHelpInfo();		
			}
			if (strcmp(argv[1], "-a") == 0 || strcmp(argv[1], "-add") == 0) {
				addPost();
			}
			if (strcmp(argv[1], "-g") == 0 || strcmp(argv[1], "-generate") == 0) {
				generateHTML();
			}
			if (strcmp(argv[1], "-d") == 0 || strcmp(argv[1], "-deploy") == 0) {
				deploy();
			}	
			if (strcmp(argv[1], "-i") == 0 || strcmp(argv[1], "-initialize") == 0) {
				initialize();
			}
			if (strcmp(argv[1], "-s") == 0 || strcmp(argv[1], "-server") == 0) {
				startServer();
			}
			break;
		default:
			printErrorInfo();
			break;
	}
	return 0;	
}
