#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>
#include <string.h>

using namespace std;
int maxSize = -2, minSize = -1;
//bool equality = false;
string nname = "";
bool flname = false;
bool isDot(string name) {
    return ((name[0] == '.') && ((name.length() == 1) ||
                                 ((name[1] == '.') &&
                                  (name.length() == 2))));
}

bool goodSize(int size) {
    if ((maxSize == -2) && (maxSize == -1)) return true;
    return (size > minSize) && ((maxSize == -2) || (size < maxSize));
}
bool goodName (string name) {
  return (!flname)||(nname==name);
}
bool doodInum () {
}
void dfs(string dirName) {
    DIR *dir = opendir(&dirName[0]);
    if (dir == NULL) {
        perror("opening directory");

    } else {
        struct dirent **namelist;
        int n;
        n = scandir(&dirName[0], &namelist, NULL, alphasort);
        if (n > 0) {
            while (n--) {
                string name = namelist[n]->d_name;
		string fullName = dirName + '/' + name; 
                if (((namelist[n]->d_type == DT_DIR) || ((namelist[n]->d_type == DT_REG))) && !(isDot(name))) {
		     
                    struct stat sb;
                    if (stat(&fullName[0], &sb) == -1) {
                        perror("stat");
                        exit(EXIT_FAILURE);
                    }
                    if (goodName(name)&&goodSize(sb.st_size)) cout<<fullName<<endl;
                }
                if (namelist[n]->d_type == DT_DIR) {
                    if (!isDot(name)) dfs(fullName);
                }
                free(namelist[n]);
            }
            free(namelist);
        } else perror("Scandir");
    }

}

int main(int argc, char *const argv[]) {
    //cout << argc << endl;
    if (argc < 2) {
        printf("Error: Expected path of root in arguments\n");
        return EXIT_FAILURE;
    }
    DIR *dir = opendir(argv[1]);
    if (dir == NULL) {
        perror("Expected directory in arg[1]");
        return EXIT_FAILURE;
    }
    int i = 2;
    while (i < argc) {
        if (!strcmp(argv[i], "-size")) {
            i++; //3
//cout << i << " ok\n";
            if (i > argc) {
                printf("Error: ad  Expected modificator to size\n");
                return EXIT_FAILURE;
            }
            //i++;
            string modifier = argv[i];
//cout << i << " ok\n";
            if (modifier.length() < 2) {
                printf("Error: Wrong modificator to size\n");
                return EXIT_FAILURE;
            }
	string ss = modifier.substr(1, modifier.length());
            int ssize = atoi(ss.c_str());
            if (modifier[0] == '-') {
                if ((maxSize == -2) || (ssize < maxSize)) maxSize = ssize;
            } else if (modifier[0] == '+') {
                if (ssize > minSize) minSize = ssize;
            } else if (modifier[0] == '=') {
                if ((maxSize == -2) || (ssize < maxSize)) maxSize = ssize;
                if (ssize > minSize) minSize = ssize;
            } else {
                printf("Error: Expected modificator to size\n");
                return EXIT_FAILURE;
            }
            //i++;
        } else if (!strcmp(argv[i], "-name")) {
		if (flname) {
			printf("Error: have alredy choosen -name\n");
			return EXIT_FAILURE;
		}
		i++;
		if (i > argc) {
                	printf("Error: ad  Expected modificator to name\n");
                	return EXIT_FAILURE;
            	}	
		nname = argv[i];
		flname = true;

	} else {
		printf("Error: Wrong key of modificator\n");
                return EXIT_FAILURE;
	}
        i++;
    }
    dfs(argv[1]);
}
