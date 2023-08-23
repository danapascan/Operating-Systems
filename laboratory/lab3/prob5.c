#include <stdlib.h>
#include <stdio.h>

//n2 lungimea lui v2
//n1 lungimea lui v1
//c1 capacitatea lui v1
//pos pozitia pe care trebuie inserat v2 in v1
//-1 daca c1 nu e suficient de mare
int  insert(int* v1, int n1, int c1, int* v2, int n2, int pos)
{
	if (n1 + n2 > c1)
	{
		return -1;
	}
	else
	{
		int* aux=(int*)malloc(sizeof(int));
		int indexaux = 0;
		//copiez ultimele elemente ale lui v1 in aux
		for (int i = pos; i < n1; i++)
		{
			aux[indexaux] = v1[i];
			indexaux++;
		}
		//aici pun elementele din v2 in v1 la poz data
		int index = pos;
		for (int i = 0; i < n2; i++)
		{
			v1[index] = v2[i];
			index++;
		}
		//aici pun inapoi elementele din aux in v1
		for (int i = 0; i < n1-pos; i++)
		{
			v1[i + pos + n2] = aux[i];
		}
		return 0;
	}
}

int main()
{
	int n1 = 6;
	int n2 = 2;
	int c1 = 8;
	int v1[8] = {1,2,3,4,5,6};
	int v2[2] = {10,20};
	int pos = 3;
	//daca functia returneaza 0 atunci se afiseaza vectorul nou
	if(insert(v1, n1, c1, v2, n2, pos)==0)
	{
	  for(int i=0;i<c1;i++)
	    {
	       printf("%d ", v1[i]);
	    }
	}
	return 0;

}
