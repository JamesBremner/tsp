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
    for( int k = 1; k < theTour.size(); k++ )
    {

        //cout << k-1 << " to " << k << " cost " <<theCostMatrix[k-1][k] << "\n";
        cost += theCostMatrix[theTour[k-1]][theTour[k]];
    }
    return cost;
}

void ExhaustiveSearch()
{
    int minCost = INT_MAX;
    vector< int > bestTour;

    for( int k = 0; k < theCostMatrix.size(); k++ )
        theTour.push_back( k );

    do
    {

        int cost = TourCost();
        if( cost < minCost )
        {
            minCost = cost;
            bestTour = theTour;
        }

    }
    while( next_permutation(theTour.begin(), theTour.end() ));

    for( int city : bestTour )
    {
        cout << city << " ";
    }
    cout << " = " << minCost << "\n";
}

int main()
{
    //ReadCostMatrix();
    RandomCostMatrix(50);
    DumpCostMatrix();
    ExhaustiveSearch();


    return 0;
}
