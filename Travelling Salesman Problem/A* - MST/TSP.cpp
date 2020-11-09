#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
using namespace  std;

vector<vector<int>> distMatrix;  //The distances between two cities is contained by it.
unordered_map<string ,int> MSTMap;  //This map stores all MSTs generated for our search and their values so that whenever same MST occurs we don't have to compute again.  BCD DCB
unordered_map<string ,int> finalList; //This list consists of paths generated during search to keep tracks of nodes extended and the cost for that path or state.   ABCD   ACDB  => ABCD -> cost
int optimalCost = INT_MAX;
int TotalNodesExpanded = 1, TotalNodesGenerated = 1;
class Node
{
public: int cityNum;
        char name;
        int citiesNotVisited;
        string pathSoFar;
        int hCost;
        int aCost;
        int totalCost;
        string state;
        vector<int> citiesRemaining;
        //less than operator for nodes in priority queue in increasing order of total cost.
        bool operator <(Node other) const
        {
          return this->totalCost > other.totalCost;
        }

        Node(){};
        Node(int cityNum,string &pathSoFar,char name,int citiesNotVisited,int hCost,int aCost,int totalCost,string &state,vector<int> &citiesRemaining):cityNum(cityNum),pathSoFar(pathSoFar),name(name),citiesNotVisited(citiesNotVisited),hCost(hCost),aCost(aCost),totalCost(totalCost),state(state),citiesRemaining(citiesRemaining){}

}City[1];


priority_queue<Node> aStar; //Fringe List


Node CreateNode(int cityNum,string pathSoFar,char name,int citiesNotVisited,int hCost,int aCost,int totalCost,string state,vector<int> citiesRemaining)
{
  Node temp(cityNum,pathSoFar,name,citiesNotVisited,hCost,aCost,totalCost,state,citiesRemaining);
  return temp;
}

//For taking input from file.
class TSP
{
public: int cities;
        void input()
        {
          ifstream fin;
          fin.open("input6.txt");
          int flag=0,j=0;
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
//Minimum Spanning tree Heuristic
int MinimumSpanningTree(vector<int> &vertices,vector<char> &nodesLeft)
{
  int i,j,n = vertices.size();
  if(n==1)
  {
    return 0;
  }
  vector<int> pCity,pDist;
  string cities;
  sort(nodesLeft.begin(),nodesLeft.end());
  for(i=0;i<n;i++)
  {
    pCity.push_back(vertices[i]);
    pDist.push_back(INT_MAX);
    cities+=nodesLeft[i];
  }
  if(MSTMap.find(cities) != MSTMap.end())
  {
    return MSTMap[cities];
  }
  int newCity = vertices[n-1];
  int length = 0,minIndex;
  for(i=n-1;i>0;i--)
  {
    minIndex = -1;
    int minDist = INT_MAX;
    for(j=0;j<i;j++)
    {
      int dis1 = distance(newCity,pCity[j]);
      if(pDist[j] > dis1)
      {
        pDist[j] = dis1;
      }
      if(minDist > pDist[j])
      {
        minDist = pDist[j];
        minIndex = j;
      }
    }
    length+=minDist;
    newCity = pCity[minIndex];
    pCity[minIndex] = pCity[i-1];
    pDist[minIndex] = pDist[i-1];
  }
  MSTMap[cities] = length;
  return length;
}
//My heuristic is MST + closest node to goal + nearest unvisited node which is in fact an admissible heuristic.
int calculateHeuristic(vector<int> &vertices,vector<char> nodesLeft,int currentCity)
{
  int i,j,n = vertices.size();
  if(n==0)
  {
    return distance(currentCity,0);
  }
  if(n==1)
  {
    return distance(currentCity,vertices[0]) + distance(vertices[0],0);
  }
  int mst = MinimumSpanningTree(vertices,nodesLeft);
  vector<int> pCity;
  for(i=0;i<n;i++)
  {
    pCity.push_back(vertices[i]);
  }
  int nearestUnvisitedNode = INT_MAX,closestToGoal=INT_MAX;
  for(i=0;i<n;i++)
  {
    int dis1 = distance(pCity[i],currentCity);
    if(dis1 < nearestUnvisitedNode)
      nearestUnvisitedNode = dis1;

    int dis2 = distance(pCity[i],0);
    if(dis2 < closestToGoal)
      closestToGoal = dis2;
  }
  cout<<"MST cost : "<<mst<<" Closest Unvisited Node + Closest To goal : "<<nearestUnvisitedNode+closestToGoal<<endl;
  j = mst+nearestUnvisitedNode+closestToGoal;
  return j;
}

string FindTSP()
{
  int i,j;
  string res;
  while(!aStar.empty() && aStar.top().totalCost < optimalCost)
  {
    Node current;
    current = aStar.top();
    aStar.pop();
    vector<int> UnvisitedCities;
    vector<char> NamesOfCitiesNotVisited;
    vector<int> vertices = current.citiesRemaining;
    string pathSoFar = current.pathSoFar;
    string state = current.state;
    if(current.citiesNotVisited == 0)
    {
      int tCost = current.aCost+distance(current.cityNum,0);
      if(tCost < optimalCost)
      {
        optimalCost = tCost;
        res = pathSoFar;
      }
      continue;
    }
    if(finalList.find(state)!= finalList.end() && finalList[state] < current.totalCost ) { continue; }
    if(finalList.find(state)!= finalList.end() && finalList[state] > current.totalCost)
    {
      finalList[state] = current.totalCost;
    }
    else if(finalList.find(state) == finalList.end())
    {
      finalList[state] = current.totalCost;
    }
    TotalNodesExpanded++;
    cout<<"Expanded Node "<<current.name<<" Total Cost : "<<current.totalCost<<endl;
    for(i=0;i<vertices.size();i++)
    {
      int next = vertices[i];
      UnvisitedCities.clear();
      NamesOfCitiesNotVisited.clear();
      for(j=0;j<vertices.size();j++)
      {
        if(vertices[j]!=next)
        {
          UnvisitedCities.push_back(vertices[j]);
          if(vertices[i] > 25)
          {
            NamesOfCitiesNotVisited.push_back('a'+vertices[j]-26);
          }
          else
          {
            NamesOfCitiesNotVisited.push_back('A'+vertices[j]);
          }
        }
      }
      int hn = calculateHeuristic(UnvisitedCities,NamesOfCitiesNotVisited,next);
      int aCost = current.aCost + distance(next,current.cityNum);
      int tCost = hn + aCost;
      char name;
      if(next > 25)
      {
        name = 'a'+next-26;
      }
      else
      {
        name = 'A' + next;
      }
      string path = pathSoFar + name;
      string temp = path;
      sort(temp.begin(),temp.end());
      string nState = temp + name;
      TotalNodesGenerated++;
      cout<<"Node Generated : "<<name<<" Total Cost : "<<tCost<<endl;
      aStar.push(CreateNode(next,path,name,current.citiesNotVisited-1,hn,aCost,tCost,nState,UnvisitedCities));

    }
    cout<<endl;
  }
  return res;
}



int main() {
  int i;
  TSP t;
  t.input();
  int numCities = t.cities;
  vector<int> v1;
  vector<char> v2;
  for(i=1;i<numCities;i++)
  {
    v1.push_back(i);
    if(i>25)
    {
      v2.push_back('a'+i-26);
    }
    else {
      v2.push_back('A'+i);
    }
  }
  int initialHeuristic = calculateHeuristic(v1,v2,0);
  City[0] = CreateNode(0,"A",'A',numCities-1,initialHeuristic,0,initialHeuristic,"AA",v1);
  aStar.push(City[0]);
  string res = FindTSP();
  cout<<"Optimal Cost is "<<optimalCost<<" The path is "<<res+'A'<<endl<<endl;
  cout<<"Total number of nodes expanded are : "<<TotalNodesExpanded<<endl;
  cout<<"Total number of nodes generated are : "<<TotalNodesGenerated<<endl;
  return 0;
}
