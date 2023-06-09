#include <iostream>
#include <vector>
#include<cstdlib>
#include <set>
#include <string>
using namespace std;
#include <fstream>
#include <cmath>

const int n = 100;
const int nSquared = n * n;
float lambda;

double chanceToAdd; // = (double)(lambda / (1 + lambda));

bool details = false;

void printChain(bool vertices[n * n]) {
    int size = 10;
    if (n < size) size = n;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cout << vertices[i * n + j] << ", ";
        }
        cout << endl;
    }
}

void printChains(bool chain1[n * n], bool chain2[n * n] ) {
    int size = 10;
    if (n < size) size = n;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cout << "(" << chain1[i * n + j] << ", " << chain2[i * n + j] << ")";
        }
        cout << endl;
    }
}

void getNeighbors(int v, int& n1, int& n2, int& n3, int& n4) {
    int x = v % n;
    int y = floor(v / n);

    //Wrap around graph
    int left = x - 1;
    if (left == -1)
        left = n - 1;

    int right = x + 1;
    if (right == n)
        right = 0;

    int up = y - 1;
    if (up == -1)
        up = n - 1;

    int down = y + 1;
    if (down == n)
        down = 0;

    n1 = left + y * n;
    n2 = right + y * n;
    n3 = x + up * n;
    n4 = x + down * n;
}

void addVertex(int v, bool chain[nSquared], bool otherChain[nSquared], int& difference) {
    if (!chain[v]) {
        
        int n1, n2, n3, n4;
        getNeighbors(v, n1, n2, n3, n4);
        if (chain[n1] || chain[n2] || chain[n3] || chain[n4])
            return;

        chain[v] = true;
        if (otherChain[v]) difference--;
        else difference++;
    }
}

void removeVertex(int v, bool chain[nSquared], bool otherChain[nSquared], int& difference) {
    if (chain[v]) {
        chain[v] = false;
        if (!otherChain[v]) difference--;
        else difference++;
    }
}

void initChain(int parity, bool vertices[n * n]) {
    for (int i = 0; i < n; i++) { // Add all L or R vertices to set
        for (int j = 0; j < n; j++) {
            if (i % 2 == 0 && j % 2 == parity) 
                vertices[i * n + j] = true;
            else if (i % 2 == 1 && j % 2 != parity) 
                vertices[i * n + j] = true;
        }
    }
}



int main()
{
    srand((unsigned)time(NULL));
    
    vector<int> avgs;

    float increment = 0.005f;
   
    for (int i = 0; i < 38; i++) {
        lambda = 3.70f + increment * i;
        cout << "Lambda: " << lambda << endl;
        int tickSum = 0;
        int trials = 1;
        chanceToAdd = (double)(lambda / (1 + lambda));

        for (int j = 0; j < trials; j++) {

            int lowestD = n*n;
            int chainDifference = n * n;


            bool chain1[n * n];
            memset(chain1, 0, sizeof(chain1));
            initChain(0, chain1);
            bool chain2[n * n];
            memset(chain2, 0, sizeof(chain2));
            initChain(1, chain2);

            long long ticks = 0;
            while (lowestD > 0) {

                int randomVertex = rand() % (nSquared);
                double pr = (double)rand() / RAND_MAX;
                int action = 0;
                if (pr <= chanceToAdd) {
                    addVertex(randomVertex, chain1, chain2, chainDifference);
                    lowestD = min(chainDifference, lowestD);
                    addVertex(randomVertex, chain2, chain1, chainDifference);
                    action = 1;
                }
                else {
                    removeVertex(randomVertex, chain1, chain2, chainDifference);
                    lowestD = min(chainDifference, lowestD);
                    removeVertex(randomVertex, chain2, chain1, chainDifference);
                    action = 2;
                }

                if (details) {
                    cout << "Tick: " << ticks << endl;
                    cout << "Current Difference: " << chainDifference << endl;
                    cout << "Min Difference: " << lowestD << endl;
                    cout << "Random vertex: " << randomVertex << endl;
                    if (action == 1) cout << "Added Vertex" << endl;
                    if (action == 2) cout << "Removed Vertex" << endl;
                    printChains(chain1, chain2);
                    cout << "--------------------------------------------------" << endl;
                }


                ticks++;
                lowestD = min(chainDifference, lowestD);
               
              /*
                cout << "Difference: " << chainDifference << endl;
                cout << "Lowest Diff: " << lowestD << endl;
                cout << "----------------------------------" << endl;
               */ 

                if (ticks % 10000000 == 0) {
                    system("cls");
                    cout << "Lambda: " << lambda << endl;
                    cout << "Tick: " << ticks << endl;
                    cout << "Difference: " << chainDifference << endl;
                    cout << "Lowest Diff: " << lowestD << endl;
                    printChains(chain1, chain2);
                }
                
            }
        
            cout << "Mixing time: " << ticks << endl;
            ofstream myFile;
            myFile.open("output.csv", ios::app);
            myFile << 0.2f + increment * i << ";" << ticks << "\n";
            myFile.close();

            tickSum += ticks;
        }
        avgs.push_back(tickSum / trials);
        cout << "Average for lambda " << lambda << ": " << tickSum / trials << endl;
        
    }

    cout << "-----" << endl;
    cout << "Averages: " << endl;
    for (int i = 0; i < avgs.size(); i++) {
        cout << "lambda: " << 0.6f + increment*i <<" - " << avgs[i] << endl;
    }

   

    return 0;
}






//Unused helper function
/*
int findDifference(bool chain1[n * n], bool chain2[n * n]) {
    int d = 0;
    for (int i = 0; i < n * n; i++) {
        if (chain1[i] != chain2[i]) d++;
    }
    return d;
}

bool areEqual(bool c1[nSquared], bool c2 [nSquared]) {
    for (int i = 0; i < nSquared; i++)
        if (c1[i] != c2[i])
            return false;
    return true;
}

*/



/*while (true) {
     int k;
     cin >> k;
     int n1, n2, n3, n4;
     getNeighbors(k, n1, n2, n3, n4);
     cout << n1 << ", " << n2 << ", " << n3 << ", " << n4 << endl;
 }
 */


