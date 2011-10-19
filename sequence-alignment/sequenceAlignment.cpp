/**
 * Introduction to Bioinformatics - Sequence alignment
 * @author: Piotr Wittchen <piotr.wittchen@gmail.com>
 * Macrofaculty, specialization: Informatics
 * @todo cutting D-loop from sequence, global alignment for whole sequences using getFileSize
 */

#include <iostream>
#include <fstream>

/**
 * global variables for storing sequences their names and DPArray
 */
char * sequenceName1;
char * sequenceName2;
char * sequenceData1;
char * sequenceData2;
int ** DPArray;
int sequenceSize = 100;

int getFileSize(char * fileName)
{
    int fileSize;
    std::ifstream file;
    file.open(fileName);
    file.seekg(0,std::ios::end);
    fileSize = file.tellg();
    file.close();
    return fileSize;
}

/**
 * allocating memory for sequences and DPArray
 */
void allocateMemory()
{
    sequenceName1 = new char[sequenceSize];
    sequenceName2 = new char[sequenceSize];
    sequenceData1 = new char[sequenceSize];
    sequenceData2 = new char[sequenceSize];

    DPArray = new int*[sequenceSize];
    for(int DPi = 0; DPi < sequenceSize; DPi++)
        DPArray[DPi] = new int[sequenceSize];
}

/**
 * deallocating memory
 */
void deallocateMemory()
{
    delete[] sequenceName1;
    delete[] sequenceName2;
    delete[] sequenceData1;
    delete[] sequenceData2;
    for(int DPi = 0; DPi < sequenceSize; DPi++)
        delete[] DPArray[DPi];

    delete[] DPArray;
}

/**
 * reading 100 nucleotides starting from the 6000th position
 * in the single mtDna sequence inside the FASTA file
 */
void readMtDNA(char * FASTAfile, int sequenceNumber, int startingPosition = 6000)
{
    std::ifstream file;
    file.open(FASTAfile);

    char fileGet;
    bool sequenceBegin = false;
    int i = 0, j = 0, k = 0;

    while(file.good())
    {
        fileGet = char(file.get());

        if(fileGet == 10) // new line
            sequenceBegin = true;

        if(!sequenceBegin)
        {
            if(sequenceNumber == 1)
                sequenceName1[k++] = fileGet;
            else
                sequenceName2[k++] = fileGet;
        }

        if(sequenceBegin && fileGet != 10 && i < sequenceSize)
        {
            if(j == 0)
            {
                if(sequenceNumber == 1)
                    sequenceName1[k] = '\0';
                else
                    sequenceName2[k] = '\0';
            }

            // reading sequence from the nucleotide on the custom starting position (6000th)
            // in order to perform comparison,
            // because in the earlier parts of mtDNA sequences are exactly the same
            if(j > startingPosition)
            {
                if(sequenceNumber == 1)
                    sequenceData1[i++] = fileGet;
                else
                    sequenceData2[i++] = fileGet;
            }
            j++;
        }
    }
    file.close();
}

/**
 * filling DPArray with a given initial penalty
 */
void fillDPArray(int penalty = -6)
{
    for(int i = 0; i < sequenceSize; i++)
    {
        for(int j = 0; j < sequenceSize; j++)
        {
            if(i == 0)
                DPArray[i][j] = j * penalty;
            else if(j == 0)
                DPArray[i][j] = i * penalty;
            else
            {
                int * choosingArray = new int[3];
                choosingArray[0] = DPArray[i][j - 1] + penalty;
                choosingArray[1] = DPArray[i - 1][j] + penalty;

                if(sequenceData1[i] == sequenceData2[j])
                    choosingArray[2] = DPArray[i - 1][j - 1] + 1;
                else
                    choosingArray[2] = DPArray[i - 1][j - 1];

                int max = choosingArray[0];

                for(int k = 0; k < 3; k++)
                {
                    if(choosingArray[k] > max)
                        max = choosingArray[k];
                }

                delete[] choosingArray;
                DPArray[i][j] = max;
            }
        }
    }
}

/**
 * saving DPArray into Matlab *.m file
 */
void saveDPArray()
{
    std::ofstream file;
    file.open("DPArray.m");
    file << "DPArray = [";
    for(int m = 0; m < sequenceSize; m++)
    {
        for(int n = 0; n < sequenceSize; n++)
            file << DPArray[m][n] << ",";

        file << ";" << std::endl;
    }
    file << "];";
    file.close();
}

/**
 * traceback procedure
 * beginning from DPArray[99][99]
 * ending at DPArray[0][0]
 */
void traceback()
{
    int i = sequenceSize - 1, j = sequenceSize - 1;
    while( !(i <= 0 && j <= 0) )
    {
        if(sequenceData1[i] == sequenceData2[j])
        {
            i--;
            j--;
        }
        else
        {
            if(DPArray[i - 1][j] > DPArray[i][j - 1])
            {
                sequenceData1[i] = '-';
                i--;
            }
            else
            {
                sequenceData2[j] = '-';
                j--;
            }
        }
    }
}

void printSequences()
{
    for(int i = 0; i < sequenceSize; i++)
        std::cout << sequenceData1[i];

    std::cout << std::endl;

    for(int j = 0; j < sequenceSize; j++)
    {
        if( (sequenceData1[j] == sequenceData2[j]) && sequenceData1[j] != '-' )
            std::cout << "|";
        else
            std::cout << " ";
    }

    std::cout << std::endl;

    for(int k = 0; k < sequenceSize; k++)
        std::cout << sequenceData2[k];

    std::cout << std::endl << std::endl;;
}

void printScore()
{
    int score = 0;

    for(int i = 0; i < sequenceSize; i++)
    {
        if( (sequenceData1[i] == sequenceData2[i]) && sequenceData1[i] != '-' )
        {
            score++;
        }
        else if( (sequenceData1[i] != sequenceData2[i]) && (sequenceData1[i] != '-' && sequenceData2[i] != '-') )
        {
            score--;
        }
        // else: if we find the gap '-', we add 0 to the score, so score remains the same
    }
    std::cout << "Score of the alignment is: " << score << std::endl << std::endl;
}

/**
 * running the program
 */
int main()
{
    std::cout << "Sequence Alignment of the parts of the mtDNA" << std::endl << std::endl;

    /**
     * initial value determining what sequences we compare
     * comaprison = 1 for comaprison between Mammuthus primigenius (Mammoth) and Elephas maximus (Indian Elephant)
     * comaprison = 2 for comaprison between Mammuthus primigenius (Mammoth) and Loxodonta africana (African Elephant)
     */

    int comparison = 0;

    allocateMemory();
    readMtDNA("AP008987.fasta",1); // Mammuthus primigenius (Mammoth)

    std::cout << "Choose comparison:" << std::endl;
    std::cout << "[1] Mammuthus primigenius (Mammoth) and Elephas maximus (Indian Elephant)" << std::endl;
    std::cout << "[2] Mammuthus primigenius (Mammoth) and Loxodonta africana (African Elephant)" << std::endl;
    std::cin  >> comparison;

    if(comparison == 1)
        readMtDNA("AJ428946.fasta",2); // Elephas maximus (Indian Elephant)
    else
        readMtDNA("AJ224821.fasta",2); // Loxodonta africana (African Elephant)


    std::cout << "S1: " << sequenceName1 << std::endl;
    std::cout << "S2: " << sequenceName2 << std::endl << std::endl;

    fillDPArray();
    traceback();
    printSequences();
    printScore();
    system("pause");
    deallocateMemory();

    system("pause");
    return 0;
}
