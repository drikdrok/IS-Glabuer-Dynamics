#include <iostream>
#include <vector>
#include<cstdlib>
#include <set>
#include <string>
using namespace std;
#include <fstream>
#include <cmath>

const int n = 100;
float lambda = 2;
vector<vector<int>> grid;

bool details = false;

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

void tick(bool vertices [n * n], bool verts2[n*n], int& difference) {
    //if (details) {
        /*cout << "Vertcies are: " << endl;
        for (int y = 0; y < n; y++) {
            for (int x = 0; x < n; x++) {
                cout << vertices[y*n+x] << ", ";
            }
            cout << endl;
        }
        cout << endl;
          */  
    //}

    int v = rand() % (n * n);
   // if (details) cout << "Random vertex: " << v << endl;

    if (vertices[v]) { // Vertex is in set, remove w.p. 1/(1+lambda)
        if ((double)rand() / RAND_MAX <= (double)((1 / (1 + lambda)))) {
            vertices[v] = false;
            
            if (!verts2[v]) difference--;
            else difference++;
            
            //if (details) cout << "Vertex was in set, has been removed" << endl;
        }
    }
    else {
        if ((double)rand() / RAND_MAX <= (double)(lambda / (1 + lambda))) { // Add vertex w.p. lambda/(lambda+1)

            //Verify new set is independent
            int n1, n2, n3, n4;
            getNeighbors(v, n1, n2, n3, n4);
            if (vertices[n1] || vertices[n2] || vertices[n3] || vertices[n4])
                return;

            vertices[v] = true;
            if (verts2[v]) difference--;
            else difference++;
            //if (details) cout << "Vertex added to set" << endl;
        }
    }

}

void initChain(int parity, bool vertices[n * n]) {

    for (int i = 0; i < n; i++) { // Add all L or R vertices to set
        for (int j = 0; j < n; j++) {
            if (i % 2 == 0 && j % 2 == parity) {
                vertices[i * n + j] = true;
            }
            else if (i % 2 == 1 && j % 2 != parity) {
                vertices[i * n + j] = true;
            }
        }
    }
}

bool areEqual(bool c1[n * n], bool c2 [n * n]) {
    for (int i = 0; i < n*n; i++)
        if (c1[i] != c2[i])
            return false;
    return true;
}


int main()
{
    srand((unsigned)time(NULL));
    
    vector<int> avgs;

    float increment = 0.25f;
    

    for (int i = 0; i < 16; i++) {
        lambda = 0.5f + increment * i;
        cout << "Lambda: " << lambda << endl;
        int tickSum = 0;
        int trials = 80;
        for (int j = 0; j < trials; j++) {

            int lowestD = n*n;
            int chainDifference = n * n;

            bool chain1[n * n];
            memset(chain1, 0, sizeof(chain1));
            initChain(0, chain1);
            bool chain2[n * n];
            memset(chain2, 0, sizeof(chain2));
            initChain(1, chain2);

            int ticks = 0;
            while (chainDifference > 0) {
               // if (details) cout << "C1" << endl;
                tick(chain1, chain2, chainDifference);
                //if (details) cout << "C2" << endl;
                tick(chain2, chain1, chainDifference);
                ticks++;
                lowestD = min(chainDifference, lowestD);
                
                if (ticks % 1000000 == 0) {
                    system("cls");
                    cout << "Tick: " << ticks << endl;
                    details = true;
                    cout << "Difference: " << chainDifference << endl;
                    cout << "Lowest Diff: " << lowestD << endl;
                    
                }
                
            }
        
            cout << "Mixing time: " << ticks << endl;
            return 0;
            tickSum += ticks;
        }
        avgs.push_back(tickSum / trials);
        cout << "Average for lambda " << lambda << ": " << tickSum / trials;
        
    }

    cout << "-----" << endl;
    cout << "Averages: " << endl;
    for (int i = 0; i < avgs.size(); i++) {
        cout << "lambda: " << 0.25f + increment*i <<" - " << avgs[i] << endl;
    }

    ofstream myfile;
    myfile.open("output.csv");
    myfile << "This is the first cell in the first column.\n";
    myfile << "lambda;tmix\n";
    //myfile << 0.25f + increment * i << ";" << avgs[i] << ";\n";
    
    myfile.close();

    return 0;
}

