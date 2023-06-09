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
float lambda = 2;

double chanceToRemove = (double)((1 / (1 + lambda)));
double chanceToAdd = (double)(lambda / (1 + lambda));

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

void tick(bool vertices [nSquared], bool verts2[nSquared], int& difference, int v, double chance, int& actionPerformed) {
    if (details) {
        cout << "Vertcies are: " << endl;
        printChain(vertices);
    }


    if (vertices[v]) { // Vertex is in set, remove w.p. 1/(1+lambda)
        if (chance <= chanceToRemove && actionPerformed != 1) {
            vertices[v] = false;
            actionPerformed = 2;
            
            if (!verts2[v]) difference--;
            else difference++;
            
            if (details) cout << "Vertex was in set, has been removed" << endl;
        }
    }
    else {
        if (chance <= chanceToAdd && actionPerformed != 2) { // Add vertex w.p. lambda/(lambda+1)

            //Verify new set is independent
            int n1, n2, n3, n4;
            getNeighbors(v, n1, n2, n3, n4);
            if (vertices[n1] || vertices[n2] || vertices[n3] || vertices[n4])
                return;

            vertices[v] = true;
            actionPerformed = 1;
            if (verts2[v]) difference--;
            else difference++;
            if (details) cout << "Vertex added to set" << endl;
        }
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

bool areEqual(bool c1[nSquared], bool c2 [nSquared]) {
    for (int i = 0; i < nSquared; i++)
        if (c1[i] != c2[i])
            return false;
    return true;
}


int main()
{
    srand((unsigned)time(NULL));
    
    vector<int> avgs;

    float increment = 0.1f;
   
    for (int i = 0; i < 38; i++) {
        lambda = 0.20f + increment * i;
        cout << "Lambda: " << lambda << endl;
        int tickSum = 0;
        int trials = 1;
        chanceToRemove = (double)((1 / (1 + lambda)));
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
                double chance = (double)rand() / RAND_MAX;
                if (details) cout << "Random vertex: " << randomVertex << endl;

                if (details) cout << "C1" << endl;
                int actionPerformed = 0;
                tick(chain1, chain2, chainDifference, randomVertex, chance, actionPerformed);
                if (details) cout << "C2" << endl;

                tick(chain2, chain1, chainDifference, randomVertex, chance, actionPerformed);
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
                    cout << "Chain 1: " << endl;
                    printChain(chain1);
                    cout << "Chain 2: " << endl;
                    printChain(chain2);
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










/*while (true) {
     int k;
     cin >> k;
     int n1, n2, n3, n4;
     getNeighbors(k, n1, n2, n3, n4);
     cout << n1 << ", " << n2 << ", " << n3 << ", " << n4 << endl;
 }
 */
