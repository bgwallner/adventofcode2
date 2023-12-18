#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define FILE_NAME          "input.txt"

#define MAX_RED   12
#define MAX_GREEN 13
#define MAX_BLUE  14

/* Status codes */
#define E_OK             0x01
#define E_NOT_OK         0x00

/* Some pre-defined strings */
static const char COLON = ':';
static const char SPACE = ' ';

typedef struct cubeColor_t {
    int nbrOfRed;
    int nbrOfGreen;
    int nbrOfBlue;
} cubeColor_t;


static unsigned int extract_game_number(const char *row)
{
    unsigned int i, N, gameNumber;
    int value;

    /* Find colon */
    i = 0;
    while(row[i] != COLON)
    {
        i++;
    }
    
    /* We are at ':', go backwards to find space between 'X' and 'Game' in Game XYZ: */
    N = 0;
    gameNumber = 0;
    i = i-1;
    while(row[i] != SPACE) 
    {
        /* Convert char to int */
        value = row[i] - '0';

        /* a0 x 10^0 + a1 x 10^1 + ... + aN x 10^N */
        gameNumber += pow(10,N)*value;
        i--;
        N++;
    }

    return gameNumber;
}

static void extract_cube_color_numbers(const char *row, const unsigned int rowLength, struct cubeColor_t *cubeColor)
{
    int tempValue, tempPart;
    unsigned int i, k, N;
    unsigned int maxRed=0, maxBlue=0, maxGreen=0;

    /* Maximum                   i                     i+1                  i+2              */
    /* row[0] row[1] ... row[rowLength-3] = 'r' row[rowLength-2] = 'e' row[rowLength-1] = 'd'*/
    /* red can have maximum index i = rowLength-3                                            */

    i = 0;
    while(i <= (rowLength-3))
    {
        if(row[i] == 'r' && row[i+1] == 'e' && row[i+2] == 'd')
        {
            /* Check number of red cubes */
            k=i-2; /* index for where number preceding color ends */
            N=0;   /* exponent as in 10^N */
            tempPart = 0;
            tempValue = 0;
            while(row[k] != SPACE)
            {
                tempPart = row[k] - '0';
                tempValue+=pow(10,N)*tempPart;
                k--;
                N++;
            }
            
            /* Keep track of current max value for row */
            if(tempValue > maxRed)
            {
                maxRed = tempValue;
            }
            printf("red found with value %d and current max is %d\n", tempValue, maxRed);
        }
        i++;
    }

    /* green can have maximum index i=rowLength-5 */
    i = 0;
    while(i <= (rowLength-5))
    {
        if(row[i] == 'g' && row[i+1] == 'r' && row[i+2] == 'e'&& row[i+3] == 'e' && row[i+4] == 'n')
        {
            /* Check number of green cubes */
            k=i-2; /* index for where number preceding color ends */
            N=0;   /* exponent as in 10^N */
            tempPart = 0;
            tempValue = 0;
            while(row[k] != SPACE)
            {
                tempPart = row[k] - '0';
                tempValue+=pow(10,N)*tempPart;
                k--;
                N++;
            }
            
            /* Keep track of current max value for row */
            if(tempValue > maxGreen)
            {
                maxGreen = tempValue;
            }
            printf("green found with value %d and current max is %d\n", tempValue, maxGreen);
        }
        i++;
    }

    /* blue can have maximum index i=rowLength-4 */
    i = 0;
    while(i <= (rowLength-4))
    {
        if(row[i] == 'b' && row[i+1] == 'l' && row[i+2] == 'u'&& row[i+3] == 'e')
        {
            /* Check number of green cubes */
            k=i-2; /* index for where number preceding color ends */
            N=0;   /* exponent as in 10^N */
            tempPart = 0;
            tempValue = 0;
            while(row[k] != SPACE)
            {
                tempPart = row[k] - '0';
                tempValue+=pow(10,N)*tempPart;
                k--;
                N++;
            }
            
            /* Keep track of current max value for row */
            if(tempValue > maxBlue)
            {
                maxBlue = tempValue;
            }
            printf("blue found with value %d and current max is %d\n", tempValue, maxBlue);
        }
        i++;
    }

    cubeColor->nbrOfRed   = maxRed;
    cubeColor->nbrOfGreen = maxGreen;
    cubeColor->nbrOfBlue  = maxBlue;
}

static unsigned int read_input_from_txt( void )
{
    unsigned int gameNumber, nbrRed, nbrBlue, nbrGreen;
    unsigned int gameNbrSum = 0;
    unsigned int i=0;
    unsigned int j;
    unsigned long sumOfPowers = 0;
    char cValue = 0;
    char rowBuffer[1000]={0};
    cubeColor_t cubeColor;
    FILE* fhInput;

    
    fhInput = fopen(FILE_NAME, "r");

    if (NULL != fhInput)
    {
        while (cValue != EOF)
        {
            cValue = getc(fhInput);

            /* Check if end of row */
            if ('\n' == cValue)
            {
                /* We have the whole row in rowBuffer */
                for(j = 0; j < i; j++)
                {
                    printf("%c", rowBuffer[j]);
                }
                printf("\n");

                /* Get the total number of colors for different cubes  */
                /* i is the rowLength since increased before detecting */
                /* end of line '\n'                                    */
                extract_cube_color_numbers(rowBuffer, i, &cubeColor);

                /* Check that it's a valid game */
                if(cubeColor.nbrOfRed <= MAX_RED && 
                   cubeColor.nbrOfGreen <= MAX_GREEN && 
                   cubeColor.nbrOfBlue <= MAX_BLUE)
                {
                    gameNumber = extract_game_number(rowBuffer);
                    gameNbrSum+=gameNumber;
                    printf("Gamenumber %d is a valid game and current sum is %d.\n", gameNumber, gameNbrSum);
                    printf("\n");
                }
                else
                {
                    printf("NOT a valid GameID\n");
                    printf("\n");
                }

                /* Calculate the sum of powers for the minimum set of cubes needed for this game */
                sumOfPowers+=cubeColor.nbrOfRed * cubeColor.nbrOfGreen * cubeColor.nbrOfBlue;

                /* Reset coloumn index (size) */
                i = 0;
            }
            else
            {
                /* Add to row*/
                rowBuffer[i] = cValue;
                i++;

            }
        }
    }

    printf("The sum of powers is %lu\n", sumOfPowers);
    printf("\n");

    return gameNbrSum;
}


int main()
{
    printf("The sum of valid GameID:s is %d\n",read_input_from_txt());
    printf("\n");
}