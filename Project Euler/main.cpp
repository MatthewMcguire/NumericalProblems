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

unsigned long nextCollatz(unsigned long n)
{
    if((n & 1) == 0)
        return n/2;
    else
        return (3 * n + 1);
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

void eulerSolution(int problem)
{
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
        default:
            std::cout << "sorry no solution yet for that one!\n";
    }

}



int main(int argc, const char * argv[]) {

    std::cout << "Project Euler Results:\n";
    eulerSolution(13);
    eulerSolution(14);
    eulerSolution(15);
    eulerSolution(16);
    std::cout << std::endl;
    return 0;
}
