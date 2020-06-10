#include "Common.h"
#include "pch.h"

using std::unordered_map;
using std::unordered_set;
using std::pair;
using std::queue;
using std::stack;
using std::vector;
using std::string;

static bool scan(const vector<DFAState>&);
static void split_to_sets(const DFA& dfa);

static unordered_map<int, int> statesSetsMap;//�¾�״̬��ŵĶ�Ӧ��ϵ��<ԭ���ı�ţ����ڵı��>
static vector<unordered_set<int>> statesSetsVec;//���еĻ��֣�ÿ��vec��һ�����֣�ÿ��vec���ǻ����е�state
static int count = 0;

/*
DFA��С���㷨һ��

���룺һ��DFA D����״̬����ΪS��������ĸ��ΪA����ʼ״̬Ϊs0������״̬��ΪF
�����һ��DFA D'
������
	(1)���ȹ������������F��S-F�ĳ�ʼ����pi����������ֱ���D�Ľ���״̬��ͷǽ���״̬�顣
	(2)
	��pi_new=pi��
	for(pi�е�ÿ����G)
	{
		��G����Ϊ��С���飬ʹ������״̬s��t��ͬһ��С���е��ҽ����������е��������a��״̬s��t��a�ϵ�ת�����ﵽpi�е�ͬһ�飻
		��pi_new�н�G�滻Ϊ��G���зֻ��õ�����ЩС�飻
	}
	(3)���pi_new=pi����pi_final=pi������ִ�в���4��������pi_new�滻pi���ظ�����2��
	(4)�ڻ���pi_final��ÿ������ѡȡһ��״̬��Ϊ����Ĵ�����Щ����������СDFA D'��״̬��D'���������ְ����²��蹹����
		a.D'�Ŀ�ʼ״̬�ǰ�����D�Ŀ�ʼ״̬����Ĵ���
		b.D'�Ľ���״̬����Щ������D�Ľ���״̬����Ĵ���
		c.��s��pi_final��ĳ����G�Ĵ�������DFA D��������a���뿪s��ת������״̬t����rΪt������H�Ĵ���
		��ô��D'�д���һ����s��r������a�ϵ�ת������ע�⣬��D�У���G�е�ÿһ��״̬��Ȼ������a�Ͻ�����H�е�ĳ��״̬��
		������GӦ���Ѿ�������2�ָ�ɸ�С�����ˡ�


DFA��С���㷨����
(1)����ԭDFA����ÿ����̬��������һ�������У��������̬����һ������
(2)ֱ��û���½�����Ϊֹ���������м��ϣ�
	a.�Ӽ���k������ȡһ��״̬s����������״̬s��
		i.����s���з�����e
		ii.���s����Ҳ��e����s��ͬһ�����ϣ�������һ��
		iii.�����½�����k������s������k����
	b.����½������ˣ�����ֹѭ��
	c.��k����Ԫ�ش�k��ɾ��

*/

void minimize_DFA(const DFA& DFA_origin, DFA& DFA_minimum) 
{
	split_to_sets(DFA_origin);//����

	//���ó�̬
	DFA_minimum.startState = statesSetsMap.find(DFA_origin.startState)->second;
	//������̬
	for (const auto& p : DFA_origin.finalStatesMap)
	{
		DFA_minimum.finalStatesMap.emplace(statesSetsMap.find(p.first)->second, p.second);
	}
	//��������״̬
	vector<DFAState> statesVec;
	for (int k = 0; k < statesSetsVec.size(); ++k)
	{
		for (int i = 0; i < DFA_origin.statesMap.size(); i++)
		{
			statesVec.push_back(DFA_origin.statesMap.find(i)->second);
		}
		auto& pivotState = statesVec[*statesSetsVec[k].cbegin()];
		DFAState newState;
		newState.num = k;
		for (const auto& p : pivotState.transitionTableMap)
		{
			newState.transitionTableMap.emplace(p.first, statesSetsMap.find(p.second)->second);
		}
		DFA_minimum.statesMap.insert({ newState.num,newState });
	}
}

void split_to_sets(const DFA& dfa)//������С�����״̬����
{
	auto& finalStatesMap = dfa.finalStatesMap;
	//auto& statesMap = dfa.statesMap;

	vector<DFAState> statesMap;
	for (const auto& s : dfa.statesMap)
	{
		statesMap.push_back(s.second);
	}


	for (const auto& p : finalStatesMap)//������̬���ռ���̬
	{
		statesSetsVec.push_back(unordered_set<int>{p.first});//��״̬�����ջ��ÿ����̬������
		statesSetsMap.emplace(p.first, count++);//��¼״̬���+�ڼ���
	}

	unordered_set<int> tempSet;
	for (const auto& e : statesMap)//�ռ�����̬
	{
		if (finalStatesMap.find(e.num) == finalStatesMap.end())//�����ǰ״̬�Ƿ���̬
		{
			tempSet.insert(e.num);//����̬��״̬��ŷ���һ��
			statesSetsMap.emplace(e.num, count);
		}
	}

	statesSetsVec.push_back(tempSet);//���ֵļ���
	++count;

	while (scan(statesMap));//��ͣ��ɨ��ֱ�����ٱ仯Ϊֹ
}


bool scan(const vector<DFAState>& statesVec)//ɨ������������������һ��������ȼ�Ҫ��ļ��ϣ�һ��ֻ����һ������
{
	bool flag = false;//�Ƿ�����ٷָ�
	int splitSetNumber = 0;
	unordered_set<int> newSet;
	for (int k = 0; k < statesSetsVec.size(); ++k)//�������л��֣�����һ������
	{
		auto& state = statesSetsVec[k];//ȡ��һ��״̬
		if (state.size() == 1) continue;//�����Ѿ�ֻ�е���״̬�Ļ���
		else 
		{
			auto& standard = statesVec[*(state.begin())];//��һ����Ϊ��׼
			for (const auto& edge : ALL_CHARS)//������׼״̬�����б� 
			{
				for (const auto& i : state)//��鼯��ÿһ��״̬ 
				{
					const auto& state = statesVec[i];
					//�ж��Ƿ������ͬ�ı�����ת��״̬
					auto findStateIt = state.transitionTableMap.find(edge);//��state���ҳ�������
					auto findStandardIt = standard.transitionTableMap.find(edge);//��standard���ҳ�������

					//������Ҫ�ֿ������
					if (findStateIt == state.transitionTableMap.end() && findStandardIt != standard.transitionTableMap.end())//standard�еı��Ҳ��� 
					{
						flag = true;
						newSet.insert(i);
					}
					else if ((findStateIt != state.transitionTableMap.end()) && (findStandardIt == standard.transitionTableMap.end()))//state�еı��Ҳ��� 
					{
						flag = true;
						newSet.insert(i);
					}
					else if (*(statesSetsMap.find(findStandardIt->second)) != *(statesSetsMap.find(findStateIt->second)))//�߶��ҵ��˵���ת�Ƶ���״̬��ͬ
					{
						flag = true;
						newSet.insert(i);
					}
				}
				if (flag) break; //һ�����ֿ��Էָ��ͣ
			}
			if (flag)//�ֿ�һ��״̬��ͣ
			{
				splitSetNumber = k;//��¼�ĸ�������Ҫ���ָ�
				break;
			}
		}
	}
	//�µĻ��֣�����һ���µĻ��֣�С�����Ѹû��֣�С���е�״̬��ԭ�ȵ�k�Ż��֣�����ɾ��
	for (const auto& s : newSet)
	{
		statesSetsVec[splitSetNumber].erase(s);
		statesSetsMap.insert_or_assign(s, count);
	}

	statesSetsVec.push_back(newSet);
	++count;

	return flag;
}