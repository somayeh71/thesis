// CllularLearningAutomata.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <istream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <time.h>
#include <map>
#include <list>
#include <algorithm>
#include <iterator>
#include <unordered_set>
#include <random>
#include <time.h>
using namespace std;

class SubNode{

public:
	SubNode()
	{
		probility=0;
		similarity=0;
		Wt=1;
		Zt=5;
		Dt=Wt/Zt;
	}

	double probility;
	double similarity;
	double Wt;
	double Zt;
	double Dt;

};

class QM{

public:
	QM()
	{
		ls=0;
		outLs=0;
		ds=0;
	}

	int ls;
	int outLs;
	int ds;
};

class Node{

public:

	Node(int select,int member,bool ee)
	{
		selectNode=select;
		membership=member;
		environment=ee;
		strongSence=ee;
		localSence=ee;
		sumDegree=0;//new
	}

	int selectNode;
	int membership;
	bool environment;
	bool strongSence;
	bool localSence;
	int optimalEstimateNode;
	double optimalEstimateValue;
	double sumDegree;//new
	

	map<int,SubNode> adjacency;
};

vector<Node>graphNodes;
vector<vector<int>*>solutionTree;
vector<QM>modularityDetail;
vector<int>comminutySenceDatil;
int numberOfNodesGraph=0;
int numberOfEdgeGraph=0;
int numberOfCommunity=0;
double a = 0.01;

void ReadGraph(string name)
{
	
	string line;
	ifstream infile;
	infile.open(name);

	if(infile.is_open())
	{
		int a=0;
		//cout<<"Open File \n";
	}

	while(!infile.eof()) 
	{
		getline(infile,line); 

		if(line[0]=='p')
		{
			int i,j;
			sscanf(line.c_str(),"%*c %d %d", &i, &j);
			numberOfNodesGraph=i;
			numberOfEdgeGraph=j;
			//cout<<"nodes: "<<i<<"  edges: "<<j<<endl;
			j=i;

			for(;0<=i;i--)
			{
				graphNodes.push_back(Node(0,j-i,true));
				solutionTree.push_back(new vector<int>);
				comminutySenceDatil.push_back(0);
			}
		}
		else if (line[0]=='e')
		{
			int i,j;
			sscanf(line.c_str(),"%*c %d %d", &i, &j);
			
			graphNodes.at(i).adjacency.insert(make_pair(j,SubNode()));
			graphNodes.at(j).adjacency.insert(make_pair(i,SubNode()));
		}
	}
}

void InitialProbilityVector()
{
	
	for(int i=1;i<=numberOfNodesGraph;i++)
	{
		double probility=1.0/graphNodes.at(i).adjacency.size();
		for(map<int,SubNode>::iterator its=graphNodes.at(i).adjacency.begin();its!=graphNodes.at(i).adjacency.end();++its)
			its->second.probility=probility; 
			
	}
}

void InitialProbilityVectorDByDegreeNew()
{
	for(int i=1;i<=numberOfNodesGraph;i++)
	{
		for(auto its=graphNodes.at(i).adjacency.begin();its!=graphNodes.at(i).adjacency.end();++its)
		{
			graphNodes.at(i).sumDegree+=graphNodes.at(its->first).adjacency.size();
		}
		for(auto its=graphNodes.at(i).adjacency.begin();its!=graphNodes.at(i).adjacency.end();++its)
		{
			its->second.probility=graphNodes.at(its->first).adjacency.size()/graphNodes.at(i).sumDegree;
		}
	}

}

void InitialEstimateVectorDByMaxDegree()
{
	for(int i=1;i<=numberOfNodesGraph;i++)
	{
		int maxDegree=100;
		int maxNode=0;
		for(map<int,SubNode>::iterator its=graphNodes.at(i).adjacency.begin();its!=graphNodes.at(i).adjacency.end();++its)
			if(graphNodes.at(its->first).adjacency.size()<maxDegree){maxDegree=graphNodes.at(its->first).adjacency.size();maxNode=its->first;}

				graphNodes.at(i).optimalEstimateNode=maxNode;
	}
}

void InitialEstimateVectorDByMaxDegreeUpdate()
{
	for(int i=1;i<=numberOfNodesGraph;i++)
	{
		int maxDegree=-1;
		int maxNode=0;
		int sumDegree=0;
		for(map<int,SubNode>::iterator its=graphNodes.at(i).adjacency.begin();its!=graphNodes.at(i).adjacency.end();++its)
		{
			sumDegree+=graphNodes.at(its->first).adjacency.size();
			int degree=graphNodes.at(its->first).adjacency.size();
			if(maxDegree < degree)
			{
				maxDegree=degree;
				maxNode=its->first;
			}
		}

		for(map<int,SubNode>::iterator its=graphNodes.at(i).adjacency.begin();its!=graphNodes.at(i).adjacency.end();++its)
		{
			
		}

		graphNodes.at(i).optimalEstimateNode=maxNode;
	}
}

void InitialEstimateVectorDByRandomChoice()
{
	int i=1;
	srand (clock()*i-time(NULL));
	for(;i<=numberOfNodesGraph;i++)
	{
		double rd=(rand() % 100)/(double)100;
		double maxNode=0;
		for(map<int,SubNode>::iterator its=graphNodes.at(i).adjacency.begin();its!=graphNodes.at(i).adjacency.end();++its)
		{
			its->second.Wt=rd;
			if(maxNode<rd)
			{
				maxNode=rd;
				graphNodes.at(i).optimalEstimateNode=its->first;
			}
		}
	}
}

void InitialEstimateVectorDByRandomChoiceUpdate()
{
	int i=1;
	srand (clock()*i-time(NULL));
	for(;i<=numberOfNodesGraph;i++)
	{
		double rd=(rand() % 100)/(double)100;
		double maxNode=0;
		for(map<int,SubNode>::iterator its=graphNodes.at(i).adjacency.begin();its!=graphNodes.at(i).adjacency.end();++its)
		{
			its->second.Wt=rd;
			if(maxNode<rd)
			{
				maxNode=rd;
				graphNodes.at(i).optimalEstimateNode=its->first;
			}
		}
	}
}

void InitialEstimateVectorDBySimilarity()
{
	for(int i=1;i<=numberOfNodesGraph;i++)
	{
		int sub=0;
		double maxSimilarity=0;
		for(map<int,SubNode>::iterator its=graphNodes.at(i).adjacency.begin();its!=graphNodes.at(i).adjacency.end();++its)
		{
			sub=0;
			
			if(its->second.similarity==0)
			{
				if(graphNodes.at(i).adjacency.size()<=graphNodes.at(its->first).adjacency.size())
				{
					for(auto temp=graphNodes.at(i).adjacency.begin();temp!=graphNodes.at(i).adjacency.end();++temp)
						if(graphNodes.at(its->first).adjacency.find(temp->first)!=graphNodes.at(its->first).adjacency.end())
							sub++;

					sub+=2;
					double tempSimilarity=(double)sub/sqrt(double((graphNodes.at(i).adjacency.size()+1)*((graphNodes.at(its->first).adjacency.size()+1))));
					its->second.similarity=tempSimilarity;
					graphNodes.at(its->first).adjacency.find(i)->second.similarity=tempSimilarity;
				}
				else
				{
					for(auto temp=graphNodes.at(its->first).adjacency.begin();temp!=graphNodes.at(its->first).adjacency.end();++temp)
						if(graphNodes.at(i).adjacency.find(temp->first)!=graphNodes.at(i).adjacency.end())
							sub++;

					sub+=2;
					double tempSimilarity=(double)sub/sqrt(double((graphNodes.at(i).adjacency.size()+1)*((graphNodes.at(its->first).adjacency.size()+1))));
					its->second.similarity=tempSimilarity;
					graphNodes.at(its->first).adjacency.find(i)->second.similarity=tempSimilarity;
				}
			}
			
		}
	}

	for(int i=1;i<=numberOfNodesGraph;i++)
	{
		double tempSim=0;
		int tempNode=0;
		for(auto its=graphNodes.at(i).adjacency.begin();its!=graphNodes.at(i).adjacency.end();++its)
		{
			if(tempSim<its->second.similarity)
			{
				tempSim=its->second.similarity;
				tempNode=its->first;
			}
		}

		graphNodes.at(i).adjacency.find(tempNode)->second.Wt*=30;
		graphNodes.at(i).optimalEstimateNode=tempNode;
		graphNodes.at(i).optimalEstimateValue=(double)graphNodes.at(i).adjacency.find(tempNode)->second.Wt/(double)graphNodes.at(i).adjacency.find(tempNode)->second.Zt;
	}
}

void InitialEstimateVectorDBySimilarityUpdate()
{
	for(int i=1;i<=numberOfNodesGraph;i++)
	{
		int sub=0;
		double maxSimilarity=0;
		for(map<int,SubNode>::iterator its=graphNodes.at(i).adjacency.begin();its!=graphNodes.at(i).adjacency.end();++its)
		{
			sub=0;
			
			if(its->second.similarity==0)
			{
				if(graphNodes.at(i).adjacency.size()<=graphNodes.at(its->first).adjacency.size())
				{
					for(auto temp=graphNodes.at(i).adjacency.begin();temp!=graphNodes.at(i).adjacency.end();++temp)
						if(graphNodes.at(its->first).adjacency.find(temp->first)!=graphNodes.at(its->first).adjacency.end())
							sub++;

					sub+=2;
					double tempSimilarity=(double)sub/sqrt(double((graphNodes.at(i).adjacency.size()+1)*((graphNodes.at(its->first).adjacency.size()+1))));
					its->second.similarity=tempSimilarity;
					graphNodes.at(its->first).adjacency.find(i)->second.similarity=tempSimilarity;
				}
				else
				{
					for(auto temp=graphNodes.at(its->first).adjacency.begin();temp!=graphNodes.at(its->first).adjacency.end();++temp)
						if(graphNodes.at(i).adjacency.find(temp->first)!=graphNodes.at(i).adjacency.end())
							sub++;

					sub+=2;
					double tempSimilarity=(double)sub/sqrt(double((graphNodes.at(i).adjacency.size()+1)*((graphNodes.at(its->first).adjacency.size()+1))));
					its->second.similarity=tempSimilarity;
					its->second.Dt=tempSimilarity;   //TO DO
					graphNodes.at(its->first).adjacency.find(i)->second.similarity=tempSimilarity;
					graphNodes.at(its->first).adjacency.find(i)->second.Dt=tempSimilarity;   //TO DO
				}
			}
			
		}
	}

	for(int i=1;i<=numberOfNodesGraph;i++)
	{
		double tempSim=0;
		int tempNode=0;
		for(auto its=graphNodes.at(i).adjacency.begin();its!=graphNodes.at(i).adjacency.end();++its)
		{
			its->second.Wt=its->second.similarity*10;
			its->second.Dt=10;


			if(tempSim<its->second.similarity)
			{
				tempSim=its->second.similarity;
				tempNode=its->first;
			}
		}

		graphNodes.at(i).optimalEstimateNode=tempNode;
		graphNodes.at(i).optimalEstimateValue=graphNodes.at(i).adjacency.find(tempNode)->second.Dt;
	}
}

void Similarity()
{
	for(int i=1;i<=numberOfNodesGraph;i++)
	{
		int sub=0;
		double maxSimilarity=0;
		for(map<int,SubNode>::iterator its=graphNodes.at(i).adjacency.begin();its!=graphNodes.at(i).adjacency.end();++its)
		{
			sub=0;
			
			if(its->second.similarity==0)
			{
				if(graphNodes.at(i).adjacency.size()<=graphNodes.at(its->first).adjacency.size())
				{
					for(auto temp=graphNodes.at(i).adjacency.begin();temp!=graphNodes.at(i).adjacency.end();++temp)
						if(graphNodes.at(its->first).adjacency.find(temp->first)!=graphNodes.at(its->first).adjacency.end())
							sub++;

					sub+=2;
					double tempSimilarity=(double)sub/sqrt(double((graphNodes.at(i).adjacency.size()+1)*((graphNodes.at(its->first).adjacency.size()+1))));
					its->second.similarity=tempSimilarity;
					graphNodes.at(its->first).adjacency.find(i)->second.similarity=tempSimilarity;
				}
				else
				{
					for(auto temp=graphNodes.at(its->first).adjacency.begin();temp!=graphNodes.at(its->first).adjacency.end();++temp)
						if(graphNodes.at(i).adjacency.find(temp->first)!=graphNodes.at(i).adjacency.end())
							sub++;

					sub+=2;
					double tempSimilarity=(double)sub/sqrt(double((graphNodes.at(i).adjacency.size()+1)*((graphNodes.at(its->first).adjacency.size()+1))));
					its->second.similarity=tempSimilarity;
					//its->second.Dt=tempSimilarity;   //TO DO
					graphNodes.at(its->first).adjacency.find(i)->second.similarity=tempSimilarity;
					//graphNodes.at(its->first).adjacency.find(i)->second.Dt=tempSimilarity;   //TO DO
				}
			}
			
		}
	}

	
}

void RandomSelect()
{
	int i=1;
	srand (clock()*i-time(NULL));
	for(;i<=numberOfNodesGraph;i++)
	{
		double rd=(rand() % 100)/(double)100;
		double temp=0;
		for(map<int,SubNode>::iterator its=graphNodes.at(i).adjacency.begin();its!=graphNodes.at(i).adjacency.end();++its)
		{
			temp+=its->second.probility;
			if(rd<=temp)
			{
				graphNodes.at(i).selectNode=its->first;
				break;
			}
		}
	}
}

void ComponentTree()
{
	for(int i=1;i<=numberOfNodesGraph;i++)
	{
		solutionTree.at(i)->push_back(graphNodes.at(i).selectNode);
		solutionTree.at(graphNodes.at(i).selectNode)->push_back(i);
	}

	vector <int> queue;
	int comminuty=1;

	for(int i=1;i<=numberOfNodesGraph;i++)
	{
		if(solutionTree.at(i)->size()!=0)
		{
			queue.insert(queue.end(),solutionTree.at(i)->begin(),solutionTree.at(i)->end());
			//copy(solutionTree.at(i)->begin(),solutionTree.at(i)->end(),queue.end());
			solutionTree.at(i)->clear();

			for(int j=0;j<queue.size();j++)
			{
				graphNodes.at(queue.at(j)).membership=comminuty;
				//copy(solutionTree.at(queue.at(j))->begin(),solutionTree.at(queue.at(j))->end(),queue.end());
				queue.insert(queue.end(),solutionTree.at(queue.at(j))->begin(),solutionTree.at(queue.at(j))->end());
				solutionTree.at(queue.at(j))->clear();
			}
			queue.clear();
			comminuty++;
		}
	}

	numberOfCommunity=comminuty-1;
	
	
}

void mmd()
{
	graphNodes.at(1).membership=2;
	graphNodes.at(2).membership=1;
graphNodes.at(3).membership=1;
graphNodes.at(4).membership=1;
graphNodes.at(5).membership=2;
graphNodes.at(6).membership=2;
graphNodes.at(7).membership=2;
graphNodes.at(8).membership=3;
	//graphNodes.at(1).membership=2;
}

void CommunitySence()
{
	modularityDetail.clear();
	for(int i=0;i<=numberOfCommunity;i++)modularityDetail.push_back(QM());

	for(int i=1;i<=numberOfNodesGraph;i++)
	{
		int tempIn=0;//local sence
		int tempOut=0;//local sence

		for(map<int,SubNode>::iterator its=graphNodes.at(i).adjacency.begin();its!=graphNodes.at(i).adjacency.end();++its)
		{
			if(graphNodes.at(i).membership==graphNodes.at(its->first).membership)
			{
					modularityDetail.at(graphNodes.at(i).membership).ls++;
					tempIn++;
			}
			else
			{
				modularityDetail.at(graphNodes.at(i).membership).outLs++;
				tempOut++;
			}
		}

		if(tempOut<tempIn)graphNodes.at(i).localSence=true; else graphNodes.at(i).localSence=false; //local sence 

		modularityDetail.at(graphNodes.at(i).membership).ds+=graphNodes.at(i).adjacency.size();
	}

	for(int i=1;i<=numberOfCommunity;i++)
	{
			modularityDetail.at(i).ls=modularityDetail.at(i).ls/2;
			modularityDetail.at(i).outLs=modularityDetail.at(i).outLs/2;
	}
}

bool CommunitySenceCheker(int number)
{
	bool check=true;

	if(modularityDetail.at(number).ls<=modularityDetail.at(number).outLs)check=false;

	return check;
}

double Modularity()
{
	double Q=0;
	int counter=0;
	
	int ls=0;
	int ds=0;
	for(int i=1;i<=numberOfCommunity;i++)
	{
		ls+=modularityDetail.at(i).ls;
		ds+=modularityDetail.at(i).ds*modularityDetail.at(i).ds;
	}

	int temmm=pow((numberOfEdgeGraph*2),2.0);

	Q=((double)ls/(double)numberOfEdgeGraph)-((double)ds/(double)temmm);

	return Q;
}

void Reward (int node,double aa)
{
	double leverage=(double)graphNodes.at(node).adjacency.find(graphNodes.at(node).selectNode)->second.Wt/(double)graphNodes.at(node).adjacency.find(graphNodes.at(node).selectNode)->second.Zt;  
		aa=aa*leverage;

	for(map<int,SubNode>::iterator its=graphNodes.at(node).adjacency.begin();its!=graphNodes.at(node).adjacency.end();++its)
		{
			if(its->first==graphNodes.at(node).optimalEstimateNode)
				its->second.probility+=aa*(1.0-its->second.probility);
			else
				its->second.probility=(1.0-aa)*its->second.probility;
		}
}

void Penalized(int node,double bb)
{
	double leverage=(double)graphNodes.at(node).adjacency.find(graphNodes.at(node).selectNode)->second.Wt/(double)graphNodes.at(node).adjacency.find(graphNodes.at(node).selectNode)->second.Zt;  
		bb=bb*(1-leverage);
	for(map<int,SubNode>::iterator its=graphNodes.at(node).adjacency.begin();its!=graphNodes.at(node).adjacency.end();++its)
		{
			if(its->first==graphNodes.at(node).optimalEstimateNode)
				its->second.probility=(1-bb)* its->second.probility;
			else
				its->second.probility=bb/(graphNodes.at(node).adjacency.size()-1) +(1-bb) *its->second.probility;
		}
}

void UpdateProbilityVector()
{
	for(int i=1;i<=numberOfNodesGraph;i++)
	{
		if(graphNodes.at(i).environment)
		{
			if(graphNodes.at(i).strongSence) 
				a=0.09;
			else  
				a=0.05;
		}
		else
		{
			if(graphNodes.at(i).strongSence)  
				a=0.01;
			else
				a=0;
		}

		//if(graphNodes.at(i).environment&&graphNodes.at(i).strongSence&&graphNodes.at(i).localSence)
			//a+=a*graphNodes.at(i).adjacency.find(graphNodes.at(i).selectNode)->second.similarity;

		//a+=a*graphNodes.at(i).adjacency.find(graphNodes.at(i).optimalEstimateNode)->second.Dt;
		
		for(map<int,SubNode>::iterator its=graphNodes.at(i).adjacency.begin();its!=graphNodes.at(i).adjacency.end();++its)
		{
			if(its->first==graphNodes.at(i).optimalEstimateNode)
				its->second.probility+=a*(1.0-its->second.probility);
			else
				its->second.probility=(1.0-a)*its->second.probility;
		}

	}
}

void UpdateProbilityVectorWithPenalti()
{
	for(int i=1;i<=numberOfNodesGraph;i++)
	{
		if(graphNodes.at(i).environment)
		{
			if(graphNodes.at(i).strongSence) 
			{
				a=0.1;
				Reward(i,a);
			}
			else  
			{
				a=0.08;
				Reward(i,a);
			}
		}
		else
		{
			if(graphNodes.at(i).strongSence) 
			{
				a=0.06;
				Reward(i,a);
			}
			else
			{
				a=0.1;
				Penalized(i,a);
			}
		}
		
		for(map<int,SubNode>::iterator its=graphNodes.at(i).adjacency.begin();its!=graphNodes.at(i).adjacency.end();++its)
		{
			if(its->first==graphNodes.at(i).optimalEstimateNode)
				its->second.probility+=a*(1.0-its->second.probility);
			else
				its->second.probility=(1.0-a)*its->second.probility;
		}

	}
}

void UpdateProbilityVectorUpdate()
{
	for(int i=1;i<=numberOfNodesGraph;i++)
	{
		if(graphNodes.at(i).environment)
		{
			if(graphNodes.at(i).localSence)  
				a=0.4;
			else
				a=0.2;
		}
		else 
		{
			if(graphNodes.at(i).localSence) 
				a=0.1;
			else 
				a=0.07;
		}
		
		
		

		double leverage=graphNodes.at(i).adjacency.find(graphNodes.at(i).selectNode)->second.Dt;  
		a=a*leverage;

		for(map<int,SubNode>::iterator its=graphNodes.at(i).adjacency.begin();its!=graphNodes.at(i).adjacency.end();++its)
		{
			if(its->first==graphNodes.at(i).selectNode)
				its->second.probility+= a*(1.0-its->second.probility);
			else
				its->second.probility=(1.0-a)*its->second.probility;
		}

	}
}

void UpdateEstimateVectorD()
{
	for(int i=1;i<=numberOfNodesGraph;i++)
	{
		map<int,SubNode> ::iterator tempIts=graphNodes.at(i).adjacency.find(graphNodes.at(i).selectNode);

		if(graphNodes.at(i).environment)
			tempIts->second.Wt++;
		tempIts->second.Zt++;
		if(graphNodes.at(i).localSence)
		{
			tempIts->second.Wt++;
			tempIts->second.Zt++;
		}
		tempIts->second.Dt=tempIts->second.Wt/tempIts->second.Zt;



		if(graphNodes.at(i).optimalEstimateValue<=tempIts->second.Dt)
		{
			graphNodes.at(i).optimalEstimateNode=tempIts->first;
			graphNodes.at(i).optimalEstimateValue=tempIts->second.Dt;
		}
		else if(graphNodes.at(i).optimalEstimateNode==graphNodes.at(i).selectNode)
		{
			int tempNode=0;
			double tempValue=0;

			for(map<int,SubNode>::iterator its=graphNodes.at(i).adjacency.begin();its!=graphNodes.at(i).adjacency.end();++its)
			{
				if(tempValue<its->second.Dt)
				{
					tempNode=its->first;
					tempValue=its->second.Dt;
				}
			}

			graphNodes.at(i).optimalEstimateNode=tempNode;
			graphNodes.at(i).optimalEstimateValue=tempValue;

		}

	}
}

void UpdateEstimateVectorDUpdate()
{
	
	for(int i=1;i<=numberOfNodesGraph;i++)
	{
		map<int,SubNode> ::iterator tempIts=graphNodes.at(i).adjacency.find(graphNodes.at(i).selectNode);

		if(graphNodes.at(i).localSence)
			tempIts->second.Wt++;

		tempIts->second.Zt++;

		tempIts->second.Dt=(double)tempIts->second.Wt/(double)tempIts->second.Zt;
	}
}





int _tmain(int argc, _TCHAR* argv[])
{
	ofstream out("result.txt");
    streambuf *coutbuf = cout.rdbuf(); //save old buf
    cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!
	cout.precision(15);
	clock_t t;

	clock_t tLocal;

	vector<double> averageQ;
	vector<int> averageItration;
	vector<float> averageTime;
	int itration=1;
	
	/*for(int cc=0;cc<20;cc++)
	{*/
		double lastQ=0;
		ReadGraph("zachary.txt");

		tLocal=clock();
		//InitialProbilityVector();
		InitialProbilityVector();
		tLocal = clock() - tLocal;
		//cout<<"Local  InitialProbilityVector TIME     It took me %d clicks (%f seconds)."<<((float)tLocal)/CLOCKS_PER_SEC<<endl;

		tLocal=clock();
		Similarity();
		//InitialEstimateVectorDByRandomChoice();
		//InitialEstimateVectorDByMaxDegreeUpdate();
		//InitialEstimateVectorDBySimilarityUpdate();   //Newwwwwwwwwww
		//InitialEstimateVectorDByRandomChoice();
		//InitialEstimateVectorDByMaxDegree();
		tLocal = clock() - tLocal;
		//cout<<"Local  InitialEstimateVectorD TIME     It took me %d clicks (%f seconds)."<<((float)tLocal)/CLOCKS_PER_SEC<<endl;

		double QMax=0;
		itration=1;
		int numberOfCommunityMax=0;
		double QNew=0.0000000000000000000000000000001;
		double QOld=1;

		t = clock();

		int mohammadsho=0;

		while(  mohammadsho!=4)
		{
			if(QOld== QNew)mohammadsho++;
			else mohammadsho=0;

			//cout<<"itration= "<<itration<<endl;
			if(QMax<QNew){QMax=QNew;numberOfCommunityMax=numberOfCommunity;}
			QOld=QNew;

			tLocal=clock();
			RandomSelect();
			tLocal = clock() - tLocal;
			//cout<<"Local  RandomSelect TIME     It took me %d clicks (%f seconds)."<<((float)tLocal)/CLOCKS_PER_SEC<<endl;

			tLocal=clock();
			ComponentTree();
			tLocal = clock() - tLocal;
			//cout<<"Local  ComponentTree TIME     It took me %d clicks (%f seconds)."<<((float)tLocal)/CLOCKS_PER_SEC<<endl;

			tLocal=clock();
			CommunitySence();
			tLocal = clock() - tLocal;
			//cout<<"Local  CommunitySence TIME     It took me %d clicks (%f seconds)."<<((float)tLocal)/CLOCKS_PER_SEC<<endl;

			tLocal=clock();
			QNew=Modularity();
			cout<<QNew<<endl;

			

			lastQ=QNew;
			tLocal = clock() - tLocal;
			//cout<<"Local  Modularity TIME     It took me %d clicks (%f seconds)."<<((float)tLocal)/CLOCKS_PER_SEC<<endl;

			bool modularityBool=true;
			if(QNew<QMax) modularityBool=false;

			for(int j=1;j<=numberOfNodesGraph;j++)
				{
					if(CommunitySenceCheker(graphNodes.at(j).membership))
						graphNodes.at(j).strongSence=true;
					else
						graphNodes.at(j).strongSence=false;

					graphNodes.at(j).environment=modularityBool;
				}


			tLocal=clock();
			UpdateEstimateVectorD();
			tLocal = clock() - tLocal;
			//cout<<"Local  UpdateEstimateVectorD TIME     It took me %d clicks (%f seconds)."<<((float)tLocal)/CLOCKS_PER_SEC<<endl;


			tLocal=clock();
			UpdateProbilityVector();
			tLocal = clock() - tLocal;
			//cout<<"Local  UpdateProbilityVector TIME     It took me %d clicks (%f seconds)."<<((float)tLocal)/CLOCKS_PER_SEC<<endl;

			//cout<<"******************************************"<<endl<<endl;
			itration++;

		} 

	//t = clock() - t;
	//cout<<"EXE TIME     It took me %d clicks (%f seconds)."<<((float)t)/CLOCKS_PER_SEC<<endl;
	////printf ("EXE TIME     It took me %d clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
	//cout<<"max modularity= "<<QMax<<endl;
	//cout<<"last modularity= "<<lastQ<<endl;
	//cout<<"number of community in max modularity= "<<numberOfCommunityMax<<endl;
	//cout<<"itration= "<<itration<<endl;
	//cout<<"******************************************"<<endl<<endl;

	//averageQ.push_back(QMax);
	//averageItration.push_back(itration);
	//averageTime.push_back((float)t);

	//}

	//double avrModularity=0;
	//int avrItration=0;
	//float avrTime=0;
	//double maximumQ=0;
	//for(int i=0;i<20;i++)
	//{
	//		avrModularity+=averageQ.at(i);
	//		avrItration+=averageItration.at(i);
	//		avrTime+=averageTime.at(i);
	//}

	//for(int i=0;i<20;i++)
	//{
	//		if(maximumQ<averageQ.at(i))maximumQ=averageQ.at(i);
	//}
	//cout<<"\n avreage Modularity= "<<avrModularity/20.0;
	//cout<<"\n max= "<<maximumQ;
	//cout<<"\n avreage Itration= "<<avrItration/20.0;
	//cout<<"\n avreage Time= "<<avrTime/(20.0*CLOCKS_PER_SEC)<<endl<<endl;

	////printf ("\n avreage= ",(float)mamamama/20.0);
	////printf ("\n max= ",t,(float)maxx,"\n");

	////cout<<numberOfCommunity<<"=>"<<QMax<<endl;


	//system("PAUSE");
	return 0;
}
