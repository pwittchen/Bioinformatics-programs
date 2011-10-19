#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "core.h"

int Generate(int *X, int *DX, int N)
{
	memset(DX, 0, N * sizeof(int));
	
	for (int i = 0; i < N; i++)
	{
		for (int j = i + 1; j < N; j++)
		{
			// if elements i and j exist
			if (X[i] && X[j])
			{
				DX[j - i]++;
			}
		}
	}
	return 1;
}

int Reassemble(int *L, int *X, int N)
{	
    std::vector<int> Lvector, Lremovedvector, Xvector, DXvector;
    
    for(int i = 0; i < N; i++)
        Lvector.push_back( (L[i]) ? L[i] : false );

    while(!Lvector.empty())
    {
        DXvector.clear();
        Xvector.push_back( (Xvector.empty()) ? 0 : false );
        Xvector.push_back(Lvector.back());
        
        for(std::vector<int>::iterator it0 = Xvector.begin(); it0 != Xvector.end(); ++it0)
            DXvector.push_back( ((* it0) != Lvector.back()) ? abs( Lvector.back() - (* it0) ) : false );
        
        Lremovedvector.clear();

        for(std::vector<int>::iterator it1 = Lvector.begin(); it1 != Lvector.end(); ++it1)
        {
            for(std::vector<int>::iterator it2 = DXvector.begin(); it2 != DXvector.end(); ++it2)
            {           
                if( (* it1 == * it2) * (find(Lremovedvector.begin(), Lremovedvector.end(), (* it1)) == Lremovedvector.end()) )
                {
                    Lremovedvector.push_back( * it1 );
                    it1 = Lvector.erase( it1 );
                    --it1;
                }
            }
        }
    }
    
    std::sort(Xvector.begin(), Xvector.end());
    Xvector.erase(std::unique(Xvector.begin(), Xvector.end()), Xvector.end());
        
    int j = 0;
    for(std::vector<int>::iterator it3 = Xvector.begin(); it3 != Xvector.end(); ++it3, ++j)
        X[j] = ( * it3 );
    
    return 1;
}
