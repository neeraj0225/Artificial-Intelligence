#include <bits/stdc++.h>
#include <iostream>
#include <random>
using namespace  std;

vector<vector<int>> distMatrix;  //The distances between two cities is contained by it.
int cities; //Number of cities
int pop_size = 500;  //The number of chromosomes in the state space
map<vector<int>,int> mp;  //Map for storing the chromosome and its fitness value.
vector<vector<int>> parent; //2-D matrix containing all the chromosomes.
vector<int> best; //Consists of the best solution we have got.
//For taking input from file.
class TSP
{
public:
    static void input()
    {
      ifstream fin;
      fin.open("input5.txt");
      int flag=0,j;
      int i=-1;
      while(fin) {
        int data;
        string line;
        getline(fin, line);
        stringstream atr(line);
        j = 0;
        while (atr >> data) {
          if (flag == 0) {
            cities = data;
            distMatrix.resize(cities);
            flag = 1;
          }
          else
          {
            distMatrix[i].resize(cities);
            distMatrix[i][j++]=data;
          }
        }
        i++;
      }
      fin.close();
    }
};

int distance(int c1,int c2)
{
  return distMatrix[c1][c2];
}

//Shuffle the population chromosome
vector<int> popShuffle()
{
  vector<int> population(cities+1);
  population[0]=0;
  for(int i=1;i<cities;i++)
  {
    population[i]=i;
  }
  population[cities]=0;
  shuffle(population.begin()+1,population.end()-1, std::mt19937(std::random_device()()));
  return population;
}

//Evaluates the path cost of the chromosome
int evaluateCost(vector<int> p)
{
  int i,sum=0;
  for(i=0;i<p.size()-1;i++)
  {
    sum+=distance(p[i],p[i+1]);
  }
  return sum;
}

//Fitness value is inversely proportional to path cost that is lower the path cost fitter is the chromosome
int fitnessFunction(vector<int> &a)
{
  int f=INT_MAX,c;
  c = evaluateCost(a);
  if(c!=0)
    f = 1/c;
  return f;
}

//Crossover generates a off spring from the two parent chromosomes passed to it

vector<int> crossOver(vector<int> p1,vector<int> p2)
{
  p1.pop_back();
  p2.pop_back();
  int i,n = p1.size();
  vector<bool> visited(n); //Visited array to keep track of nodes visited in first parent.
  vector<int> index(n); //Index consists of the indexes of i'th city in parent 1.
  for(i=0;i<n;i++)
  {
    index[p1[i]]=i;
    visited[i]= false;
  }
  int r = rand()%cities; //Random partition
  visited[r] = true;
  int ind = index[p2[r]];
  while(!visited[ind])   //Until cycle repeats
  {
    visited[ind]=true;
    ind = index[p2[ind]];
  }
  vector<int> res(cities+1);
  for(i=0;i<n;i++)
  {
    if(visited[i])
      res[i]=p1[i]; //Visited in parent1 take it as it is
    else
      res[i]=p2[i];  //Else take from parent2
  }
  res[cities]=0;
  return res;
}

//Mutation here is swapping two random elements in the chromosome

vector<int> mutateChild(vector<int> &a)
{
  int n = cities-1;
  int r1 = 1 + rand()%(n);
  int r2 = 1 + rand()%(n);
  while(r1 == r2)
  {
    r1 = 1 + rand()%(n);
    r2 = 1 + rand()%(n);
  }
  int temp = a[r1];
  a[r1]=a[r2];
  a[r2]=temp;
  return a;
}

//Initial function for generating population

void populationGenerate()
{
  int i,n=pop_size;
  parent.clear();
  parent.resize(n);
  for(i=0;i<n;i++)
  {
    parent[i].resize(cities+1);
    parent[i] = popShuffle();
  }
}

// Comparator that will be useful to sort the population according to their value

bool comparator(const vector<int> &v1 , const vector<int> &v2)
{
  return (mp[v1] > mp[v2]);
}

//Evaluate the cost of parent vector and keep the best pop_size/2 chromosome

void populationEvaluate()
{
  int i,n=pop_size;
  for(i=0;i<n;i++)
  {
    mp[parent[i]]=fitnessFunction(parent[i]);
  }
  sort(parent.begin(),parent.end(),comparator);
  for(i=0;i<n/2;i++)
    parent.pop_back();
}

//Function for generating crossovers between pairs of parents
void populationCrossover()
{
  int i,n=parent.size();
  for(i=0;i<n-1;i=i+2)
  {
    parent.push_back(crossOver(parent[i],parent[i+1]));
  }
}

//Function for making mutations of the best pop_size/4 chromosomes and pushing it to parent
void populationMutation()
{
  int i,n=pop_size;
  for(i=0;i<n/4;i++)
  {
    parent.push_back(mutateChild(parent[i]));
  }
}

//Function for printing path of the best route

void PrintPath(vector<int> b)
{
  int i,n=b.size();
  string str;
  for(i=0;i<n;i++)
  {
    if(b[i] > 25)
    {
      str.push_back('a'+b[i]-26);
    }
    else
    {
      str.push_back('A'+b[i]);
    }
  }
  cout<<" The path is : "<<str<<endl;
}

int main() {
  TSP::input();
  populationGenerate();
  populationEvaluate();
  int current_best_cost = INT_MAX;
  best = parent[0];
  int iterations = 0;
  int max_iterations = 1000;
  while(max_iterations > 0)
  {
    populationCrossover();
    populationMutation();
    populationEvaluate();
    int cost = evaluateCost(parent[0]);
    if(current_best_cost > cost)
    {
      current_best_cost = cost;
      best = parent[0];
    }
    iterations++;
    max_iterations = max_iterations -1;
  }
  cout<<" Number of iterations : "<<iterations<<endl;
  cout<<" Optimal Cost is : "<<current_best_cost<<endl;
  PrintPath(best);
  return 0;
}
