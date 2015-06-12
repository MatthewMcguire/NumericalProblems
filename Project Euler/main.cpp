//
//  main.cpp
//  Project Euler
//
//  Created by Matthew McGuire on 5/21/15.
//  Copyright (c) 2015 Matthew McGuire. All rights reserved.
//
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <ctime>

time_t beginTime, endTime;

std::map<int,std::string> nameDigit;

int quantityLettersInString(std::string s)
{   // returns the number of alphabetic characters in the input string
    int sum = 0;
    for (int i = 0; i < s.length(); i++)
    {
        if(isalpha(s[i])) sum++;
    }
    return sum;
}

std::string wordsForNumber(int n)
{   // turns an integer value into a string describing the number e.g 45 -> "forty-five"
    // it works for any positive integer below 20999
    std::string phraseString = "";
    int d = 0;
    // get thousands digit
    if(n>999)
    {
        d = int(n/1000);
        phraseString = nameDigit[d] + " thousand ";
        n -= (d*1000);
    }
    // get hundreds digit
    if(n>99)
    {
        d = int(n/100);
        phraseString += nameDigit[d] + " hundred ";
        n -= (d*100);
    }
    // get tens digit
    if(n>0)
    {
        if(n <= 20)
        {
            if (phraseString != "") phraseString +=  + "and ";
            phraseString += nameDigit[n];
        }
        else
        {
            d = (int(n/10))*10;
            if (phraseString != "") phraseString +=  + "and ";
            phraseString += nameDigit[d];
            n -= d;
            if (n>0) phraseString += ("-" + nameDigit[n]);
        }
    }
    return phraseString;
}

unsigned long nextCollatz(unsigned long n)
{
    if((n & 1) == 0)
        return n/2;
    else
        return (3 * n + 1);
}

void problem13Solver(void)
{
    /*--- prepare to open the file containing the 100 numbers of 50 digits               ---*/
    std::ifstream dataFile;
    std::string line;
    std::vector<uint> lineOfDigits, finalResult;            // individual number rows
    std::vector<std::vector<uint>> allNumbers;              // container for all number rows
    dataFile.open ("problem13", std::ios::in);
    if (!dataFile.is_open())                                // check if file actually opened
    {
        std::cout << "couldn't open the problem 13 file.\n";
        exit(1);
    }

    /*--- read every line and parse it into vectors of digits                             ---*/
    /*--- each line is read into 'lineOfDigits' then the line is pushed into 'allNumbers' ---*/
    while ( std::getline (dataFile,line) )
    {
        std::cout << line << '\n';
        lineOfDigits.clear();
        for (int i = 0; i< line.length(); i++)
        {
            lineOfDigits.push_back(std::stoi(line.substr(i,1), nullptr, 10));
        }
        allNumbers.push_back(lineOfDigits);
    }

    /*--- iterate over each of the number lists, adding each column, right to left        ---*/
    /*--- summation digits are placed in vector "finalResult"                             ---*/
    finalResult.clear();
    uint numDigits = uint(lineOfDigits.size());
    std::vector<uint>::iterator it = finalResult.begin();
    finalResult.insert(it,numDigits, 0);          // fill finalResult vector with 50 zeroes
    std::vector<std::vector<uint>>::iterator verticalIter;  // to iterate over all number vectors
    std::vector<uint>::iterator horizontalIter;             // to iterate across each number vector
    verticalIter = allNumbers.begin();
    while(verticalIter < allNumbers.end())
    {
        int i = 0;
        horizontalIter = (*verticalIter).end();
        while (horizontalIter >= (*verticalIter).begin())
        {
            finalResult[numDigits-i] += *horizontalIter;
            i++;
            horizontalIter--;
        }
        verticalIter++;
    }
    /*--- loop through finalResult (right to left) and with repeated %10, 'carry to the left' ---*/
    horizontalIter = finalResult.end();
    uint carryLeft = 0;
    uint actualDigit = 0;
    while (horizontalIter > finalResult.begin())
    {
        --horizontalIter;
        actualDigit = ((*horizontalIter)+carryLeft)%10;
        carryLeft = ((*horizontalIter)+carryLeft - actualDigit)/10;
        *horizontalIter =actualDigit;
    }
    /*--- insert the last carryLeft value at the front of the finalResult vector              ---*/
    finalResult.insert(finalResult.begin(), carryLeft);

    /*--- print the sum based on the vector of resulting digits                               ---*/
    std::cout << "The sum of the previous numbers is:\n";
    for(horizontalIter =finalResult.begin(); horizontalIter<finalResult.end();horizontalIter++)
        std::cout << (*horizontalIter);
    std::cout << std::endl;
    dataFile.close();
}

void problem14Solver(void)
{
    /*--- fill a set of potential initial values with all integers 2 to 10^6             ---*/
    // max set size= 576460752303423487 on my old mac. Should be at least 10^6 on your machine!
    const unsigned long MAX_INIT = 1000000;
    std::set<int> notIdentified;
    for (int i = 2; i <= MAX_INIT; i++)
    {
        notIdentified.insert(notIdentified.end(), i);
    }

    std::map<uint, unsigned long> chainLengths;
    chainLengths[1] = 1;
    std::set<int>::iterator setIt = notIdentified.begin();
    std::map<uint, unsigned long>::iterator mapIt;
    uint firstLink;
    unsigned long length, currentLink;

    /*--- test all possible values in the set for collatz length                        ---*/
    while (!notIdentified.empty())
    {
        firstLink = *setIt;
        notIdentified.erase(setIt);
        length = 1;
        currentLink = firstLink;
        while (currentLink != 1)
        {
            currentLink =nextCollatz(currentLink);
            if(chainLengths.count(currentLink)>0)       // if the next link has already been identified...
            {
                chainLengths[firstLink] = length + chainLengths[currentLink];
                //std::cout << "chain[" <<firstLink<<"]= "<< chainLengths[firstLink] << std::endl;
                break;
            }
            length++;
            if(currentLink <= MAX_INIT)        // delete each link of the chain from the set of candidates
                notIdentified.erase(currentLink);
        }

        setIt = notIdentified.begin();
    }

    /*--- Iterate over the chain lengths and find the longest one                       ---*/
    uint longestChain = 0, longestLength = 0;
    for (mapIt = chainLengths.begin(); mapIt != chainLengths.end(); mapIt++)
    {
        if (mapIt->second > longestLength)
        {
            longestLength =mapIt->second;
            longestChain = mapIt->first;
        }
    }
    std::cout << "longest chain with initial value less than " << MAX_INIT << " is ";
    std::cout << "of length " << longestLength << ", and begins with " << longestChain << "\n";

}

void problem15Solver(void)
{
    const int boundary = 21;
    long latticePaths[boundary][boundary];
    latticePaths[0][0] = 0;
    for(int i = 1; i < boundary; i++)
    {
        latticePaths[i][0] = 1;
        latticePaths[0][i] = 1;
    }
    for(int i = 1; i < boundary; i++)
    {
        for(int j = 1; j < boundary; j++)
        {
            latticePaths[i][j] = latticePaths[i-1][j] + latticePaths[i][j-1];
        }
    }
    std::cout << "for (20,20) sized lattice, there are " << latticePaths[20][20]
    << " paths from upper left to lower right." << std::endl;
}

void problem16Solver(void)
{
    uint sum = 0;
    std::string twoToThousand = "10715086071862673209484250490600018105614048117055336074437503883703510511249361224931983788156958581275946729175531468251871452856923140435984577574698574803934567774824230985421074605062371141877954182153046474983581941267398767559165543946077062914571196477686542167660429831652624386837205668069376";
    for (int i = 0; i < twoToThousand.length(); i++)
    {
        sum += std::stoi(twoToThousand.substr(i,1), nullptr, 10);
    }
    std::cout << "Summing the digits of2^1000 (as expressed in base 10: " << sum << std::endl;
}

void problem17Solver(uint n)
{
    nameDigit[1] = "one";
    nameDigit[2] = "two";
    nameDigit[3] = "three";
    nameDigit[4] = "four";
    nameDigit[5] = "five";
    nameDigit[6] = "six";
    nameDigit[7] = "seven";
    nameDigit[8] = "eight";
    nameDigit[9] = "nine";
    nameDigit[10] = "ten";
    nameDigit[11] = "eleven";
    nameDigit[12] = "twelve";
    nameDigit[13] = "thirteen";
    nameDigit[14] = "fourteen";
    nameDigit[15] = "fifteen";
    nameDigit[16] = "sixteen";
    nameDigit[17] = "seventeen";
    nameDigit[18] = "eighteen";
    nameDigit[19] = "nineteen";
    nameDigit[20] = "twenty";
    nameDigit[30] = "thirty";
    nameDigit[40] = "forty";
    nameDigit[50] = "fifty";
    nameDigit[60] = "sixty";
    nameDigit[70] = "seventy";
    nameDigit[80] = "eighty";
    nameDigit[90] = "ninety";

    uint sum = 0;
    std::string word = "";
    int wordSum = 0;
    for(int i = 1; i <= n; ++i)
    {
        word = wordsForNumber(i);
        wordSum = quantityLettersInString(word);
        sum += wordSum;
        std::cout << i << "  " << word << std::endl;
    }
    std::cout << "Sum of word lengths: " << sum << std::endl;
}

void problem18Solver(void)
{
    // create the initial problem triangle as a vector of vector<int>
    std::vector<std::vector<int>> numberPyramid;
    numberPyramid.push_back({75});
    numberPyramid.push_back({95,64});
    numberPyramid.push_back({17,47,82});
    numberPyramid.push_back({18,35,87,10});
    numberPyramid.push_back({20,4,82,47,65});
    numberPyramid.push_back({19,1,23,75,3,34});
    numberPyramid.push_back({88,2,77,73,7,63,67});
    numberPyramid.push_back({99,65,04,28,6,16,70,92});
    numberPyramid.push_back({41,41,26,56,83,40,80,70,33});
    numberPyramid.push_back({41,48,72,33,47,32,37,16,94,29});
    numberPyramid.push_back({53,71,44,65,25,43,91,52,97,51,14});
    numberPyramid.push_back({70,11,33,28,77,73,17,78,39,68,17,57});
    numberPyramid.push_back({91,71,52,38,17,14,91,43,58,50,27,29,48});
    numberPyramid.push_back({63,66,4,68,89,53,67,30,73,16,69,87,40,31});
    numberPyramid.push_back({4,62,98,27,23,9,70,98,73,93,38,53,60,4,23});

    size_t pyramidHeight = numberPyramid.size();

    // create the calculation vector of ints
    std::vector<int> currentSums = numberPyramid[pyramidHeight-1];

    // create the vector to store the paths chosen at each step upwards
    std::vector<std::vector<int>> optimalPaths(pyramidHeight);

    int iLeft=0, iRight=0;
    std::vector<int> tempPath;
    for (int currentRow = (int(pyramidHeight)-2); currentRow >= 0; currentRow--)
    {
        for (int currentElement = 0; currentElement<= currentRow; currentElement++)
        {
            std::cout << "row = " << currentRow << " element = " << currentElement
            << " value = " << numberPyramid[currentRow][currentElement]<< std::endl;
            iLeft = numberPyramid[currentRow][currentElement] + currentSums[currentElement];
            iRight = numberPyramid[currentRow][currentElement] + currentSums[currentElement+1];
            if(iRight > iLeft)
            {
                currentSums[currentElement] = iRight;
                tempPath =optimalPaths[currentElement+1];
                tempPath.push_back(currentElement+1);
                optimalPaths[currentElement] = tempPath;
            }
            else
            {
                currentSums[currentElement] = iLeft;
                tempPath =optimalPaths[currentElement];
                tempPath.push_back(currentElement);
                optimalPaths[currentElement] = tempPath;
            }
        }
        currentSums.pop_back();
        //optimalPaths.pop_back();

    }
    optimalPaths[0].push_back(0);
    std::cout << "The highest value path sums to: " << currentSums[0] <<std::endl;
    std::cout << "The path itself is (going top to bottom):\n ";
    for(int i = 0; i < pyramidHeight; i++)
        std::cout << numberPyramid[i][optimalPaths[0][(pyramidHeight-1-i)]] << " ";
    std::cout << std::endl;
}

void eulerSolution(int problem)
{
    time(&beginTime);
    switch(problem)
    {
        case 13:
            std::cout << "Problem 13 sums 100 fifty-digit numbers.\n";
            problem13Solver();
            break;
        case 14:
            std::cout << "Problem 14 finds the longest Collatz sequence when beginning with a number < 10^6.\n";
            problem14Solver();
            break;
        case 15:
        std::cout << "Problem 15 finds the number of lattice paths on an mxn grid when motion is only allowed to the right or down.\n";
        problem15Solver();
        break;
        case 16:
        std::cout << "Problem 16 finds the sum of the digits of 2^1000.\n";
        problem16Solver();
        break;
        case 17:
        std::cout << "Problem 17 sums word lengths of one through one thousand.\n";
        problem17Solver(1000);
        break;
        case 18:
        std::cout << "Problem 18 finds the highest-value path down a triangle of numbers.\n";
        problem18Solver();
        break;
        default:
            std::cout << "sorry no solution yet for that one!\n";
    }
    time(&endTime);
    std::cout << "Execution took " << difftime(endTime, beginTime) << " second(s)\n";

}



int main(int argc, const char * argv[]) {

    std::cout << "Project Euler Results:\n";
    eulerSolution(13);
    eulerSolution(14);
    eulerSolution(15);
    eulerSolution(16);
    eulerSolution(17);
    eulerSolution(18);
    std::cout << std::endl;
    return 0;
}
