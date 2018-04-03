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
bool equality = false;

bool isDot(string name) {
    return ((name[0] == '.') && ((name.length() == 1) ||
                                 ((name[1] == '.') &&
                                  (name.length() == 2))));
}

bool goodSize(int size) {
    if ((maxSize == -2) && (maxSize == -1)) return true;
    return (size > minSize) && ((maxSize == -2) || (size < maxSize));
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
                if (((namelist[n]->d_type == DT_DIR) || ((namelist[n]->d_type == DT_REG))) && !(isDot(name))) {
                    struct stat sb;
                    if (stat(&(dirName + '/' + name)[0], &sb) == -1) {
                        perror("stat");
                        exit(EXIT_FAILURE);
                    }


                    if (goodSize(sb.st_size)) printf("%s\n", namelist[n]->d_name);
                }
                if (namelist[n]->d_type == DT_DIR) {
                    if (!isDot(name)) dfs(dirName + '/' + name);
                }
                free(namelist[n]);
            }
            free(namelist);
        } else perror("Scandir");
    }

}

int main(int argc, char *const argv[]) {
	cout << argc << endl;
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
	cout << i << argv[i] << endl;
        if (strcmp(argv[i],"-size")) {
            i++;
            if (i > argc) {
                printf("Error: ad  Expected modificator to size\n");
                return EXIT_FAILURE;
            }
            string modifier = argv[i];
            if (modifier.length() < 2) {
                printf("Error: Wrong modificator to size\n");
                return EXIT_FAILURE;
            }
            int ssize = atoi(modifier.substr(1, modifier.length()).c_str());
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
        } i++;
    }
    dfs(argv[1]);
}
