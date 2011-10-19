#include "core.cpp"
#define MAX 100

int ReadSet(char *szFile, int *vArray, int N)
{
	char szBuf[1024];
	FILE *f = fopen(szFile, "rt");

	if (f)
	{
		fgets(szBuf, 1024, f);
		char *p = szBuf;
		for (;;)
		{
			vArray[atoi(p)]++;
			p = strchr(p, ' ');

			if (p)
				p++;
			else
				break;
		}

		return 1;
	}

	return -1;
}

int WriteSet(char *szFile, const int *vArray, int N)
{
	char szBuf[1024] = {0};
	char szNum[32] = {0};
	FILE *f = fopen(szFile, "wt");

	if (f)
	{
		for (int i = 0; i < N; i++)
		{
			// place each element given number of times
			for (int j = 0; j < vArray[i]; j++)
			{
				sprintf(szBuf, "%s %s", szBuf, itoa(i, szNum, 10)); 
			}
		}

		// ignore first space
		fputs(szBuf + 1, f);

		return 1;
	}

	return -1;
}

int ReadSetToReassemble(char *szFile, int *vArray, int N)
{
    std::ifstream f(szFile, std::ifstream::in );
    int i = 0;
    char c;
    
    if(f)
    {
        while(f.good())
        {
            if( isspace(f.get()) )
            {
                c = char(f.get());
                vArray[i] = atoi( &c );
                i++;
            }
        }
        f.close();
        return 1;
    }
    else
    {
        f.close();
        return -1;    
    }
}

int WriteSetToReassemble(char *szFile, int *vArray, int N)
{
    std::ofstream f(szFile);

	if (f)
	{
		for (int i = 0; i < N; i++)
		{
            if(vArray[i] || i == 0)
                f << vArray[i] << " ";
		}
		f.close();
		return 1;
	}
    f.close();
	return -1;
}

int main(int argc, char **argv)
{
	if (argc != 4)
	{
		printf(
			"USAGE:\n" 
			"Generating DX from X: pdp.exe -g fileX fileDX\n"
			"Reassembling X from L: pdp.exe -s fileL fileX\n"); 
	}
	else
	{
		if (!strcmp(argv[1], "-g"))
		{
			int X[MAX] = {0}, DX[MAX] = {0};
			
			ReadSet(argv[2], X, MAX);
			Generate(X, DX, MAX);
			WriteSet(argv[3], DX, MAX);
		}
		else if (!strcmp(argv[1], "-s"))
		{
			int X[MAX] = {0}, L[MAX] = {0};
			
			ReadSetToReassemble(argv[2], L, MAX);
            Reassemble(L, X, MAX);
			WriteSetToReassemble(argv[3], X, MAX);
		}
		else
		{
			printf("Incorrect command.\n");
		}
	}
	
	return 0;
}
