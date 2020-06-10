#pragma once
#ifndef COMMON_H
#define COMMON_H

#define LEX_TEST 0
#define YACC_TEST 1

#include "pch.h"

constexpr auto EPSILON = '`';

struct REDefinition
{
	std::string name;
	std::string regex;
};

struct RERule
{
	std::string regex; // Regular Expression
	std::vector<std::string> actions; // Action(s) associated with it
};

struct NFAState
{
	int num;
	std::unordered_multimap<char, int> transitionTableMap;//<���ϵ�ֵ����ָ��Ľ����>
};

//struct NFA
//{
//	int startState;
//	unordered_map<int, RERule> finalStatesMap;
//	unordered_map<int, NFAState> statesMap;
//};

struct NFA
{
	int startState;
	std::unordered_map<int, std::vector<std::string>> finalStatesMap;//<��̬��ţ���Ӧ�Ķ���>
	std::unordered_map<int, NFAState> statesMap;//NFA�н��ı������state�Ķ�Ӧ��ϵ
};

struct DFAState
{
	int num;
	std::unordered_map<char, int> transitionTableMap;//<���ϵ�ֵ����ָ��Ľ����>
	std::unordered_set<int> NFAStatesSet;
};

//struct DFA
//{
//	int startState;
//	unordered_map<int, RERule> finalStatesMap;
//	unordered_map<int, DFAState> statesMap;
//};

struct DFA
{
	int startState;
	std::unordered_map<int, std::vector<std::string>> finalStatesMap;//�洢��̬�Ͷ�Ӧ�Ķ���
	std::unordered_map<int, DFAState> statesMap;//DFA�н��ı������state�Ķ�Ӧ��ϵ
};

const std::string OPERATOR_CHARS("\\[]^-?.*+|()$/{}%<>");

const std::string ALL_CHARS("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!\"#%'()*+,-./:;<=>\?[\\]^{|}_ \n\t\v\f~&");


#endif // !COMMON_H
