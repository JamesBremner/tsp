#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <climits>
#include <vector>
#include <algorithm>

using namespace std;

vector< vector< int > > theCostMatrix;
vector< int > theTour;
vector< int > theBestTour;

bool ReadCostMatrix()
{
    ifstream ifs("LittleMurthyCostMatrix.txt");
    if( ! ifs )
    {
        cout << "Cannot open cost matrix\n";
        return false;
    }
    std::string line;
    while( getline( ifs, line ))
    {
        string a;
        vector<int> row;
        for(stringstream sst(line); getline(sst, a, ' '); )
        {
            row.push_back( atoi( a.c_str() ));
        }
        theCostMatrix.push_back( row );
    }

    // check we have a square matrix with 'infinity' on the leading diagonal
    int count = 0;
    for( auto& vr : theCostMatrix )
    {
        if( vr.size() != theCostMatrix.size() )
        {
            cout << "Cost matrix is not square\n";
            return false;
        }
        vr[count++] = INT_MAX;
    }

    return true;
}
void RandomCostMatrix( int s )
{
    for( int row = 0; row < s; row++ )
    {
        vector <int> vrow;
        for( int col = 0; col < s; col++ )
        {
            vrow.push_back( rand() % 100 + 1 );
        }
        theCostMatrix.push_back( vrow );
    }

    int count = 0;
    for( auto& vr : theCostMatrix )
    {
        if( vr.size() != theCostMatrix.size() )
        {
            cout << "Cost matrix is not square\n";
            return;
        }
        vr[count++] = INT_MAX;
    }
}

void DumpCostMatrix()
{
    for( auto& vr : theCostMatrix )
    {
        for( int v : vr )
        {
            if( v == INT_MAX )
                cout << "~\t";
            else
                cout << v << "\t";
        }
        cout << "\n";
    }
}


int TourCost()
{
    int cost = 0;
    for( int k = 1; k < (int)theTour.size(); k++ )
    {

        //cout << k-1 << " to " << k << " cost " <<theCostMatrix[k-1][k] << "\n";
        cost += theCostMatrix[theTour[k-1]][theTour[k]];
    }
    return cost;
}

void DumpBestTour()
{
    theTour = theBestTour;
    for( int city : theBestTour )
    {
        cout << city << " ";
    }
    cout << " = " << TourCost() << "\n";
}

/** An exhaustive search of all possible tours to find absolute cheapest */
void ExhaustiveSearch()
{
    int minCost = INT_MAX;
    vector< int > bestTour;

    // start with visiting all cities in indexed order
    for( int k = 0; k < (int)theCostMatrix.size(); k++ )
        theTour.push_back( k );

    // loop over all possible permutations of visiting order
    do
    {

        // calculate tour cost
        int cost = TourCost();
        if( cost < minCost )
        {
            // found a cheaper tour
            minCost = cost;
            bestTour = theTour;
        }

    }
    while( next_permutation(theTour.begin(), theTour.end() ));

    DumpBestTour();
}
/** Implement greedy heuristic.

    Starting from each city in turn,
    always visit the next cheapest city that has not already been visited
    until all cities have been visited
    then select the cheapest tour found from any starting city
*/
void Greedy()
{
    int cheapestTourCost = INT_MAX;

    // loop over possible starting cities
    for( int start = 0; start < theCostMatrix.size(); start++ )
    {
        theTour.clear();
        theTour.push_back( start );
        // loop over steps in tour
        for( int kstep = 1; kstep < (int)theCostMatrix.size(); kstep++ )
        {
            int cheapestNextStepCost = INT_MAX;
            int cheapestNext;
            // loop over possible next step
            for( int next = 0; next <  (int)theCostMatrix.size(); next++  )
            {
                // check that we have not already visited
                auto it = find( theTour.begin(), theTour.end(), next );
                if( it != theTour.end() )
                    continue;

                // check if this is the cheapest possible next step
                int cost = theCostMatrix[theTour.back()][next];
                if( cost < cheapestNextStepCost )
                {
                    cheapestNextStepCost = cost;
                    cheapestNext = next;
                }
            }
            // add cheapest next step to tour
            theTour.push_back( cheapestNext );
        }

        // check if the tour starting from this city is the cheapest found
        int cost = TourCost();
        if( cost < cheapestTourCost )
        {
            cheapestTourCost = cost;
            theBestTour = theTour;
        }
    }

    DumpBestTour();
}

int main()
{
    //ReadCostMatrix();
    RandomCostMatrix(50);
    DumpCostMatrix();


    if( theCostMatrix.size() < 10 )
    {
        // for small tours, an exhaustive search is practical
        ExhaustiveSearch();
    }
    else
    {
        // for large tours, the greedy heuristic usually gives a reasonable result
        Greedy();
    }

    return 0;
}
