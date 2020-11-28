#include <iostream>
#include <bits/stdc++.h>
using namespace std;

//Number of literals, clauses and vector to store all the clauses.
//The literals are of the form variables like 1, 2, 3 ...
int literal;
int clause;
vector<set<int>> clauses1;
class input{
public:int i=0,j=0,k=0,l=-2;
       string line;
       void takeInput()
       {
         ifstream fin;
         fin.open("input6.txt");
         while(fin)
         {
           getline(fin,line);
           stringstream ss(line);
           while(ss>>i) {
               if (k == 0) {
                 k = 1;
                 literal = i;
               } else if (j == 0) {
                 j = 1;
                 clause = i;
                 clauses1.resize(clause+1);
               } else {
                 if(i != 0)
                 {
                   clauses1[l].insert(i);
                 }
               }
           }
           l++;
         }
         clauses1.erase(clauses1.end());
       }
};

//Checking if any clause has become empty which indicates that it has become false => output is false
int checkEmptyClause(vector<set<int>> &clauses)
{
  int i;
  for(i=0;i<clauses.size();i++)
  {
    if(clauses[i].empty())
      return 1;
  }
  return 0;
}

//Printing all the literals and the values assigned to them
void printAssignment(vector<int> &assigned)
{
  int i;
  for(i=0;i<assigned.size();i++)
  {
    if(assigned[i] < 0)
    {
      cout<<abs(assigned[i])<<" - "<<"false"<<endl;
    }
    else
    {
      cout<<assigned[i]<<" - "<<"true"<<endl;
    }
  }
}

// Function for assigning the chosen literal a value in all clauses => eliminating it if false and removing entire clause if true as the clause
// will become true.
void RemoveLiteral(vector<int> &assigned,vector<set<int>> &clauses)
{
  int i,lit = assigned.back();
  for(i=0;i<clauses.size();i++)
  {
    //Remove the clause as being disjunctive a true literal will always make the clause true.
    if(clauses[i].find(lit) != clauses[i].end())
    {
      clauses.erase(clauses.begin()+i);
      i--;
    }
    //If the value is false then remove the literal from the clause as the others will decide the final output of clause.
    else if(clauses[i].find(-1 * lit) != clauses[i].end())
    {
      clauses[i].erase(clauses[i].find(-1 * lit));
    }
  }
}

//Unit Clause Heuristic : Only one literal in clause
int unitClause(vector<set<int>> &clauses)
{
  int i;
    for(i=0;i<clauses.size();i++) {
      if (clauses[i].size() == 1) {
        auto it = clauses[i].begin();
        int lit = *it;
        cout << "Unit Literal " << lit << " chosen for assignment" << endl;
        return lit;
      }
    }
  return 0;
}

//Check for a pure literal - The one occurring with the same sign in all clauses (Used when the number of )
int pureLiteral(vector<set<int>> &clauses)
{
  vector<int> litCheck(literal+1,0);
  int i;
  for(i=0;i<clauses.size();i++)
  {
    for(int val : clauses[i])
    {
      if(val < 0)
      {
        if(litCheck[-1*val] == 1)
          litCheck[-1*val] = 2;
        else if(litCheck[-1*val] == 0)
          litCheck[-1*val] = -1;
        else if(litCheck[-1*val] == 2)
          litCheck[-1*val] = 2;
      }
      else
      {
        if(litCheck[val] == -1)
          litCheck[val] = 2;
        else if(litCheck[val] == 0)
          litCheck[val] = 1;
        else if(litCheck[val] == 2)
          litCheck[val] = 2;
      }
    }
  }
  // Literals with check value 2 are the ones which have occurred in both forms and the literals with check value 1 or -1 are
  // only in that form
  for(i=1;i<=literal;i++)
  {
    if(litCheck[i] == 1 || litCheck[i] == -1)
    {
      cout<<"Pure Literal "<<litCheck[i]*i<<" chosen for assignment"<<endl;
      return litCheck[i]*i;
    }
  }
  return 0;
}


//Choose the maximum occurring literal and assign it a value that wil make most clauses true
int MaxOccurringLiteral(vector<set<int>> &clauses)
{
  int i;
  unordered_map<int,int> mp;
  if(!clauses.empty())
    {
      for (i = 0; i < clauses.size(); i++) {
        for (auto j:clauses[i])
          mp[j]++;
      }
      int ass=0, maxi = INT_MIN;
      for (auto j:mp) {
        if (maxi < j.second)
        {
          maxi = j.second;
          ass = j.first;
        }
      }
      int count=0;
      //Checking if only one maximum literal exists if more than one go for pure literal
      for(auto j:mp)
      {
        if(maxi == j.second)
          count++;
      }
      if(count > 1)
      {
       return pureLiteral(clauses);
      }
      if(ass != 0) { cout << "Maximum occurring literal " << abs(ass) << " chosen for assignment" << endl; }
      return ass;
    }
  return 0;
}

//Recursive DPLL algorithm using the above three heuristic
bool DPLLAlgo(int newAss,vector<int> &assigned,vector<set<int>> &clauses)
{
    int newAssign;
    assigned.push_back(newAss);
    RemoveLiteral(assigned,clauses);
    if(clauses.empty())
    {
      printAssignment(assigned);
      return true;
    }
    if (checkEmptyClause(clauses) == 1)
    {
      return false;
    }
    int u = unitClause(clauses);
    int m = MaxOccurringLiteral(clauses);
    if(u != 0)
    {
      newAssign = u;
    }
    else if(m != 0)
    {
      newAssign = m;
    }
    else
    {
      newAssign = *(clauses[0].begin());
      cout<<"Literal "<<abs(newAssign)<<" chosen for assignment"<<endl;
    }
    return DPLLAlgo(newAssign,assigned,clauses) || DPLLAlgo(-1 * newAssign,assigned,clauses);
}


int main() {
  input ip;
  ip.takeInput();
  vector<int> assigned;
  vector<set<int>> clauses = clauses1;
  int firstAssign;
  int u = unitClause(clauses);
  int m = MaxOccurringLiteral(clauses);
  if(u != 0)
  {
    firstAssign = u;
  }
  else if(m != 0)
  {
    firstAssign = m;
  }
  else
  {
    firstAssign = *(clauses[0].begin());
    cout<<"Initial assignment, literal "<<abs(firstAssign)<<" chosen for assignment"<<endl;
  }
  bool ans = DPLLAlgo(firstAssign,assigned,clauses) || DPLLAlgo(-1 * firstAssign,assigned,clauses);
  if(ans)
    cout<<"Model Possible"<<endl;
  else
    cout<<"Model not possible"<<endl;
  return 0;
}
