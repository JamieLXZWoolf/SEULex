#include "Common.h"
#include <queue>
#include <vector>
#include <stack>
#include <algorithm>

using std::unordered_map;
using std::unordered_set;
using std::pair;
using std::queue;
using std::stack;
using std::vector;
using std::string;

/*
ͨ���Ӽ����취��NFAת��ΪDFA��

���룺NFA
�����DFA
������
	һ��ʼ��e-closure��s0����DFAstate�е�Ψһ״̬������δ�ӱ�ǣ���Ǳ�ʾδ����ģ�
	while (��DFAstate����һ��δ���״̬T)�����ﲻ̫���㣬�ѱ�ǻ���δ������а�
	{
		��T���ϱ�ǣ�
		for (ÿ���������a)
		{
			U = e-closure��move��T��a������
			if��U����DFAstate�У�
				��U���뵽DFAstate�У��Ҳ��ӱ�ǣ�
			DFA[T,a]=U��
		}
	}
*/

static int NFA_state_num = 0;

//e_closure(T)���ܹ���T�е�ĳ��NFA״̬s��ʼֻͨ��eת�������NFA״̬����
/*�㷨��
��T������״̬ѹ��stack��
��e_closure(T)��ʼ��ΪT��
while(stack�ǿ�)
{
	��ջ��Ԫ��t������
	for��ÿ����������������u����t������һ�����Ϊe��ת������״̬u��
		if��u����e_closure��T���У�
		{
			��u���뵽e_closure(T)�У�
			��uѹ��ջ��
		}
}
*/
void e_closure(unordered_set<int>& NFAStatesSet, const unordered_map<int, NFAState>& statesMap)//����1��e_closure(T)����ʼʱ����T��������2��T������NFA״̬
{
	stack<int> stack;
	vector<bool> flag(NFA_state_num);//���������Ƿ������״̬

	//��T������״̬��ջ
	for (const auto& s : NFAStatesSet)
	{
		stack.push(s);
		flag[s] = true;
	}

	while (!stack.empty())//stack�ǿ�
	{
		auto bounds = statesMap.find(stack.top())->second.transitionTableMap.equal_range('`');//`����epsilon
		//��statesMap�У��ҵ�����ѭ��������Ǹ�NFA��״̬��ţ���ȡ��Ӧ��NFAstate���������״̬������epsilon�ߣ���������NFA�����Կ��ܻ��ж��ƥ�������Ҫһ��һ�����飬��equal_range����һ��ͷβ�ķ�Χ
		stack.pop();//����ջ��Ԫ��
		auto start = bounds.first;
		auto end = bounds.second;
		while (start != end) 
		{
			int new_item_num = (*start).second;
			if (flag[new_item_num]) continue;//�Ѿ�������������ˣ�ֱ������
			else//u����e_closure��T���У���Ҫ��DFA�м����µ�NFA���
			{
				stack.push(new_item_num);
				NFAStatesSet.emplace(new_item_num);
				start += 1;
			}
		}
	}
}

//move(T,a�����ܹ���T�е�ĳ��״̬s������ͨ�����Ϊa��ת�������NFA״̬�ļ���
bool move(const unordered_set<int>& NFAStatesSet, const unordered_map<int, NFAState>& NFAStatesMap, unordered_set<int>& subset, const char inputC)//����1���������DFA������2��T������NFA״̬������3���Ž���ģ�����4������ѯ�������ַ�
{
	bool flag = false;//��û��������
	for (const auto& state : NFAStatesSet) //NFAStatesSet��ָDFAÿ��״̬����������NFA״̬��ŵļ���
	{
		auto bounds = NFAStatesMap.find(state)->second.transitionTableMap.equal_range(inputC);//��NFAStatesMap�У��ҵ�����ѭ��������Ǹ�NFA��״̬��ţ���ȡ��Ӧ��NFAstate���������״̬�����ıߣ���������NFA�����Կ��ܻ��ж��ƥ�������Ҫһ��һ�����飬��equal_range����һ��ͷβ�ķ�Χ
		auto start = bounds.first;
		auto end = bounds.second;
		//���״̬û�����������ߣ�ֱ��������flagΪfalse
		if (start == end) continue;
		//���״̬������������
		while (start != end)
		{
			subset.insert(start->second);
			++start;
		}
		flag = true;
	}
	return flag;
}

bool find_actions(const unordered_set<int>& NFAStatesSet, unordered_map<int, vector<string>>& finalStatesMap_NFA, string& actions)//����1��DFA״̬�е�NFA״̬������2��NFA�е���̬<��̬��ţ���Ӧ�Ķ���>������3������
{
	decltype(finalStatesMap_NFA.find(0)) endState_Iter;//����ѡ�����̬
	bool find = false;//�Ƿ��Ѿ������DFA������ҵ�����̬
	for (const auto& NFAState : NFAStatesSet)//����DFA�����е�NFA����״̬
	{
		auto Iter = finalStatesMap_NFA.find(NFAState);//����һ�����NFA״̬�ǲ���һ����̬���ü�ֵ���ң�����ɹ��򷵻ض�Ӧ�ĵ����������򷵻�unordered_map::end
		if (Iter != finalStatesMap_NFA.end())//���ҵ���˵����DFA�������̬
		{
			if (find) 
			{
				if (endState_Iter->first > Iter->first)//�����λ�ø���ǰ�ģ�������̬��ѡ��
					endState_Iter = Iter;
			}
			else 
			{
				endState_Iter = Iter;
				find = true;
			}
		}
	}
	if (find)//ȷ����̬
	{
		actions = endState_Iter->second;//������ѡ��
		return true;
	}
	else return false;
}

//�Ӽ����취��ʵ��
void convert_NFA_2_DFA(const NFA &nfa,DFA &dfa)
{	
	int Dcounter = 0;//��DFA״̬����õ�
	NFA_state_num = nfa.statesMap.size();
	//int c = 1;
	const string inputSet(ALL_CHARS);//���е��������

	//һ��ʼ��e - closure��s0����DFAstate�е�Ψһ״̬������δ�ӱ��
	DFAState s0;//s0
	s0.num = Dcounter;//��DFA��ʼ״̬s0���
	Dcounter += 1;
	//�����ʼ���s0
	s0.NFAStatesSet.insert(nfa.startState);
	e_closure(s0.NFAStatesSet, nfa.statesMap);
	//��s0����DFA��
	dfa.statesMap.insert(pair<int, DFAState>(s0.num,s0));

	queue<int> unmarked_DFAStates;//δ�ӱ�ǵ�DFA״̬�ı��
	unmarked_DFAStates.push(s0.num);

	//����DFA��δ����Ľ�㣬�����������룬���ú������Ҹý����NFA����ͨ��������뵽��Ľ�㣬
	//����Щ�ܵ���Ľ�����DFA���½ڵ���
	while (!unmarked_DFAStates.empty())//��DFAstate����һ��δ���״̬T
	{
		//ȡ��һ��Ҫ�����״̬�ı�ţ��൱�ڼ����˱��
		int this_state = unmarked_DFAStates.front();//ָ��ǰ״̬��ָ��
		unmarked_DFAStates.pop();
		//�������뼯�е����������ַ�
		for (const char& inputC : inputSet)
		{
			unordered_set<int> U;
			//�����������
			if (move(dfa.statesMap[this_state].NFAStatesSet, nfa.statesMap, U, inputC))//����move(T,a)
			{
				e_closure(U, nfa.statesMap);//����move(T,a)��e_closure

				int next_state;//ͨ�����Ϊa��ת���ܵ����״̬�ı��

				//���U�ڲ���DFAstate��
				bool in = false;
				for (const auto& s : dfa.statesMap)//����DFA�����н�� 
				{
					if (s.second.NFAStatesSet == U)//���U�Ѿ���DFAstate��
					{
						next_state = s.first; //ָ��ǰ������еĽ��
						in = true;//U��DFAstate��
						break;
					}
				}

				//���U����DFAstate�У���U���뵽DFAstate��(�������½��)���Ҳ��ӱ��
				if (!in)
				{
					DFAState newState;
					newState.num = Dcounter++;//�µı��
					next_state = newState.num; //ָ���½���������
					newState.NFAStatesSet = U;//U�����DFA�½���״̬����
					dfa.statesMap.insert(pair<int, DFAState>(newState.num, newState));
					unmarked_DFAStates.push(newState.num);//�½��ȴ�����
					//�ж��Ƿ�����̬����ȷ����̬��Ӧ����
					string actions;
					if (find_actions(newState.NFAStatesSet, nfa.finalStatesMap, actions))//�������һ����̬
					{ 
						dfa.finalStatesMap.insert(pair<int, string>(newState.num, actions));//�������dfa��̬��Ӧ�Ķ���
					}
				}
				dfa.statesMap[this_state].transitionTableMap.insert(pair<char, int>(inputC, next_state));//DFAͼ�ϼ�һ����
			}
		}
	}
}
