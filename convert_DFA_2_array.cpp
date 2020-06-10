#include "Common.h"
#include "pch.h"

using std::unordered_map;
using std::unordered_set;
using std::pair;
using std::queue;
using std::stack;
using std::vector;
using std::string;
using std::cout;
using std::endl;


//extern vector<RERule> regexRulesVec;

void convert_DFA_2_array(const DFA& dfa, vector<pair<int*, int>>& arrays, vector<vector<string>>& endVec)
{
	string allset(ALL_CHARS);
	const int set_size = allset.size();

	//�����ĸ���

	//1��ec���ñ����ڽ��ַ���ascii��ӳ��ΪDFA״̬ת�����е�������
	//����Ϊchar��ascii�룬ֵ�Ƕ�Ӧ���кš�
	int* ec = new int[256];//����
	memset(ec, 0, 256 * sizeof(int));
	//���±�Ϊ1��ʼ��char��ֵ��0����Ƿ��ַ�
	for (int i = 0; i < set_size; ++i)
	{
		ec[int(allset[i])] = i + 1;//¼�����п���������ַ�
	}
	arrays.push_back(pair<int*, int>(ec, 256));

	//2��base��������״̬���У�ֵ������*��ȡ�
	//��״̬��Ӧ���е��׵�ַӳ�䵽һά�����еľ���λ�á���������״̬���У�ֵ��״̬���*�ַ��п�ȡ�
	const int DFA_size = dfa.statesMap.size();
	int* base = new int[DFA_size];
	memset(base, 0, DFA_size * sizeof(int));

	//3��next��������base+ec��ֵ����һ����ת״̬��
	//��Ĵ�С��base*ec
	int sizeOfNext = DFA_size * (set_size + 1);
	int* next = new int[sizeOfNext];//set��1��ʼ����
	//��ʼ��
	for (int i = 0; i < sizeOfNext; i++)
	{
		next[i] = -1;
	}
	vector<DFAState> statesVec;
	for (int i = 0; i < dfa.statesMap.size(); i++)
	{
		statesVec.push_back(dfa.statesMap.at(i));
	}

	for (int i = 0; i < dfa.statesMap.size(); i++)
	{
		base[statesVec[i].num] = i * (set_size + 1);
		for (auto it = statesVec[i].transitionTableMap.begin(); it != statesVec[i].transitionTableMap.end(); it++) 
		{
			next[base[statesVec[i].num] + ec[int(it->first)]] = it->second;//��base+ec��Ϊ������ѯ��һ����ת״̬�ı��
		}
	}
	arrays.push_back(pair<int*, int>(base, DFA_size));
	arrays.push_back(pair<int*, int>(next, sizeOfNext));

	//4��accept����������̬״̬�ţ�ֵ�Ƕ�Ӧ�����кš�
	int* accept = new int[DFA_size];//����
	memset(accept, 0, DFA_size * sizeof(int));

	int numOfend = 0;//����
	for (auto it = dfa.finalStatesMap.begin(); it != dfa.finalStatesMap.end(); ++it)
	{
		accept[it->first] = numOfend + 1;
		//endVec.push_back(regexRulesVec[it->second]);
		endVec.push_back(it->second);
		++numOfend;//����
	}

	arrays.push_back(pair<int*, int>(accept, DFA_size));
}