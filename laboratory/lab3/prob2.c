#include <stdio.h>

#define SUCCESS 0
#define ERROR_READ 1

int read_sets(int* set1, int* set2, int* size1, int* size2)
{

    int i, sz1, sz2;
    *size1 = 0;
    *size2 = 0;

    if (scanf("%d", &sz1) != 1) {

        printf("Error reading size 1!\n");
        return ERROR_READ;

    }

    if (scanf("%d", &sz2) != 1) {

        printf("Error reading size 2!\n");
        return ERROR_READ;

    }

    for (i = 1; i <= sz1; i++) {

        if (scanf("%d", &set1[i]) != 1) {

            printf("Error reading element %d from set 1\n", i);
            return ERROR_READ;

        }

    }

    for (i = 1; i <= sz2; i++) {

        if (scanf("%d", &set2[i]) != 1) {

            printf("Error reading element %d from set 2\n", i);
            return ERROR_READ;

        }

    }

    *size1 = sz1;
    *size2 = sz2;
    return SUCCESS;

}



float compute(int* set1, int* set2, int size1, int size2)
{

    int i = 1, j = 1, common = 0;

    while (i <= size1 && j <= size2) {

        if (set1[i] == set2[j]) {

            i++;
            common++;

        }
        else if (set1[i] < set2[j]) {

            i++;

        }
        else {

            j++;

        }

    }

    return 1.0 * common / (size1 + size2 - common);

}



int main(int argc, char** argv)

{

    int set1[150], set2[150], size1, size2, res;

    res = read_sets(set1, set2, &size1, &size2);

    if (res != SUCCESS) {

        return res;

    }

    printf("result is %.2f!\n", compute(set1, set2, size1, size2));

    return SUCCESS;

}
