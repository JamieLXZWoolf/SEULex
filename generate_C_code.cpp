#include "Common.h"
#include "pch.h"

using namespace std;

//��ӡ���飬nameΪ��������sizeΪ�����С��valueΪ����ֵ��outΪд����ļ���
static void print_array(string name, int size, const int* value, ofstream& out);

struct Token
{
	string token_type;
	string token_value;
};

//����.c�ļ�,arraysΪ���������������������endVecΪ��̬��Ӧ�Ķ���
int generate_C_code(vector<pair<int*, int>>& arrays, vector<vector<string>>& endVec, string& codeBegin, string& codeEnd, int startState, int mode)
{
	ofstream out;
	if (mode == 0)//lex mode
	{
		out.open("lex.c", ios::out);
	}
	if (mode == 1)//yacc mode
	{
		out.open("lex.h", ios::out);
	}
	//�����ж�size�Ĵ�С�Ƿ�Ϊ4
	if (arrays.size() != 4)
	{
		return -1;
	}
	//�������Ŀ�ʼ
	out << "#define _CRT_SECURE_NO_WARNINGS" << endl;
	out << "#define START_STATE " << startState << endl;
	//out << "#include\"stdio.h\"" << endl;
	out << "#include \"stdlib.h\"" << endl;
	out << "#include<string.h>" << endl;
	if (mode == YACC_TEST)
	{
		out << "#include <string>" << endl;
		out << "#include <vector>" << endl;
		out << "using namespace std;" << endl;
	}
	out << codeBegin;
	
	//��������
	out << "char* getCharPtr(const char* fileName);" << endl;
	out << "int findAction(int action);" << endl; 

	//�������ec��,base��,next��,accept��
	vector<string> array_name;
	array_name.push_back(string("yy_ec"));
	array_name.push_back(string("yy_base"));
	array_name.push_back(string("yy_next"));
	array_name.push_back(string("yy_accept"));

	for (int i = 0; i < 4; i++)
	{
		print_array(array_name[i], arrays[i].second, arrays[i].first, out);
		out << endl;
	}

	if (mode == YACC_TEST)
	{
		//�������ݽṹ
		out << "struct Token" << endl;
		out << "{" << endl;
		out << "	string token_type;" << endl;
		out << "	string token_value;" << endl;
		out << "};" << endl;
	}

	//�������
	out << "int yy_current_state = START_STATE;" << endl;
	out << "int yy_last_accepting_state = -1;" << endl;
	out << "char *yy_cp = NULL;" << endl;
	out << "char *yy_last_accepting_cpos = NULL;" << endl;
	out << "int yy_act = 0;" << endl;
	out << "int isEnd = 0;" << endl;
	out << "int yy_c = -1;" << endl;
	out << "int correct = 1;" << endl;
	out << "int flag_mlc = 0;" << endl;

	if (mode == YACC_TEST)
	{
		out << "vector <Token> tokens;" << endl;
		out << "string str_temp = \"\";" << endl;
	}

	out << endl;

	//��ʼ��
	out << "void lex_init(const char* fileName)" << endl;
	out << "{" << endl;
	out << "	yy_cp = getCharPtr(fileName);" << endl;//����char* getCharPtr(char* fileName)�õ��ļ��ַ�ָ��
	out << "}" << endl;
	out << endl;

	if (mode == YACC_TEST)
	{
		out << "vector<Token> yy_lex(const char* fileName)" << endl;
		out << "{" << endl;
	}

	if (mode == LEX_TEST)
	{
		out << "int main( int argc, char** argv )" << endl;
		out << "{" << endl;
		out << "	if( argc == 2 )" << endl;
		out << "	{" << endl;
		out << "		lex_init(argv[1]);" << endl;
		out << "	}" << endl;
		out << "	else" << endl;
		out << "	{" << endl;
		out << "		printf(\"ERROR: invalid argument!\\n\");" << endl;
		out << "		return -1;" << endl;
		out << "	}" << endl;
		out << endl;

		out << "	if (isEnd && correct)" << endl;
		out << "	{" << endl;
		out << "		return -1;" << endl;
		out << "	}" << endl;
		out << "	else if (isEnd && !correct)" << endl;
		out << "	{" << endl;
		out << "		return -2;" << endl;
		out << "	}" << endl;
		out << endl;
	}
	if (mode == YACC_TEST)
	{
		out << "	lex_init(fileName);" << endl << endl;
		out << "	if (isEnd && correct)" << endl;
		out << "	{" << endl;
		out << "		return tokens;" << endl;
		out << "	}" << endl;
		out << "	else if (isEnd && !correct)" << endl;
		out << "	{" << endl;
		out << "		return tokens;" << endl;
		out << "	}" << endl;
		out << endl;
	}

	out << "	int result = 0;" << endl;
	out << "	while (*yy_cp != 0)" << endl;
	out << "	{" << endl;
	out << "		yy_c = yy_ec[(int)*yy_cp];" << endl;
	if (mode == YACC_TEST)
	{
		out << "		str_temp = str_temp + *(yy_cp);" << endl;
	}
	out << "		if (yy_accept[yy_current_state])" << endl;
	out << "		{" << endl;
	out << "			yy_last_accepting_state = yy_current_state;" << endl;
	out << "			yy_last_accepting_cpos = yy_cp;" << endl;
	//out << "			str_temp = str_temp + *(yy_cp);" << endl;
	out << "		}" << endl;
	out << "		if (yy_next[yy_base[yy_current_state] + yy_c] == -1 && yy_last_accepting_state != -1)" << endl;
	out << "		{" << endl;
	out << "			yy_current_state = yy_last_accepting_state;" << endl;
	out << "			yy_cp = yy_last_accepting_cpos;" << endl;
	out << "			yy_act = yy_accept[yy_current_state];" << endl;
	out << "			if (flag_mlc == 1)" << endl;
	out << "			{" << endl;
	out << "				flag_mlc = 0;" << endl;
	out << "				yy_current_state = START_STATE;" << endl;
	out << "				yy_last_accepting_state = -1;" << endl;
	out << "				++yy_cp;" << endl;
	out << "				yy_current_state = yy_next[yy_base[yy_current_state] + yy_c];" << endl;
	out << "				continue;" << endl;
	out << "			}" << endl;
	out << "			result = findAction(yy_act);" << endl;

	if (mode == YACC_TEST)
	{
		out << "			if (result != -1)" << endl;
		out << "			{" << endl;
		out << "				yy_current_state = START_STATE;" << endl;
		out << "				yy_last_accepting_state = -1;" << endl;
		out << "				++yy_cp;" << endl;
		out << "				yy_current_state = yy_next[yy_base[yy_current_state] + yy_c];" << endl;
		out << "				break;" << endl;
		out << "			}" << endl;
		out << "			if (result == -1)" << endl;
		out << "			{" << endl;
		out << "				yy_current_state = START_STATE;" << endl;
		out << "				yy_last_accepting_state = -1;" << endl;
		out << "				++yy_cp;" << endl;
		out << "				yy_current_state = yy_next[yy_base[yy_current_state] + yy_c];" << endl;
		out << "				continue;" << endl;
		out << "			}" << endl;
	}
	else if (mode == LEX_TEST)
	{
		out << "			printf(\" \");" << endl;
		out << "			yy_current_state = START_STATE;" << endl;
		out << "			yy_last_accepting_state = -1;" << endl;
		out << "			++yy_cp;" << endl;
		out << "			yy_current_state = yy_next[yy_base[yy_current_state] + yy_c];" << endl;
		out << "			continue;" << endl;

	}

	out << "		}" << endl;
	out << "		if (yy_next[yy_base[yy_current_state] + yy_c] == -1 && yy_last_accepting_state == -1)" << endl;
	out << "		{" << endl;
	out << "			printf(\"ERROR DETECTED IN INPUT FILE !\");" << endl;

	if (mode == LEX_TEST)
	{
		out << "			return -1;" << endl;
	}
	out << "		}" << endl;
	out << "		if (yy_next[yy_base[yy_current_state] + yy_c] != -1) " << endl;
	out << "		{" << endl;
	out << "			yy_current_state = yy_next[yy_base[yy_current_state] + yy_c];" << endl;
	out << "			++yy_cp;" << endl;
	out << "		}" << endl;
	out << "	}" << endl;
	out << endl;
	out << "	if (*yy_cp == 0)" << endl;
	out << "	{" << endl;
	out << "		isEnd = 1;" << endl;
	out << "		if (yy_accept[yy_current_state] && yy_cp == yy_last_accepting_cpos + 1)" << endl;
	out << "		{" << endl;
	out << "			yy_act = yy_accept[yy_current_state];" << endl;
	if (mode == YACC_TEST)
	{
		out << "			str_temp += *(yy_cp - 1);" << endl;
	}
	out << "			result = findAction(yy_act);" << endl;
	out << "		}" << endl;
	out << "		else " << endl;
	out << "		{" << endl;
	out << "			printf(\"ERROR DETECTED IN INPUT FILE !\");" << endl;
	out << "			correct = 0;" << endl;

	if (mode == LEX_TEST)
	{
		out << "			return -1;" << endl;
	}

	out << "		}" << endl;
	out << "	}" << endl;

	if (mode == LEX_TEST)//lex
	{
		out << "	return 0;" << endl;
	}
	else //yacc
	{
		out << "	return tokens;" << endl;
	}
	out << "}" << endl;
	out << endl;
	//lex_mian��������


	//int findAction(int action)����
	out << "int findAction(int action)" << endl;
	out << "{" << endl;

	if (mode == LEX_TEST)
	{
		out << "	switch (action) " << endl;//����endVec��ӡswitch���
		out << "	{" << endl;
		out << "		case 0:" << endl;
		out << "		break;" << endl;
		for (int i = 0; i < endVec.size(); i++)
		{
			out << "		case " << i + 1 << ":" << endl;
			for (int j = 0; j < endVec[i].size(); j++)
			{
				out << "		" << endVec[i][j] << endl;//�������
			}
			out << "		break;" << endl;
		}
	}

	//���������������������������������������������������yacc�õġ���������������������������������������������������
	if (mode == YACC_TEST)
	{
		string s;
		//int space_flag = 0;//����1��ʱ��˵����ע�ͻ��߿ո�
		out << "	Token temp;" << endl;
		out << "	string s;" << endl;
		out << "	string temp_s;" << endl;
		//out << "	int flag_mult_line_comment = 0;" << endl;//���ڶ���ע�͵����⴦��������յ�����ע�ͣ���һ��flagΪ��Ч��������Ч��flagʱ������ս�̬�����κζ���
		out << "	switch (action) " << endl;//����endVec��ӡswitch���
		out << "	{" << endl;
		out << "		case 0:" << endl;
		out << "		break;" << endl;

		for (int i = 0; i < endVec.size(); i++)
		{
			out << "		case " << i + 1 << ":" << endl;
			/*out << "		if (flag_mult_line_comment == 1)" << endl;
			out << "		{" << endl;
			out << "			str_temp = str_temp[str_temp.size()-1];" << endl;
			out << "			flag_mult_line_comment = 0;" << endl;
			out << "			break;" << endl;
			out << "		}" << endl;*/
			for (int j = 0; j < endVec[i].size(); j++)
			{

				//�����һ����Ӧ����д��.c����ģ�Ҫ��ÿ������̬��һ�Σ��ѵ�ǰ��token��¼����
				if (endVec[i][j].find_first_of('p', 0) == 0)//�����printf��һ�У��Ѷ����г���
				{
					s = endVec[i][j].erase(0, 8);//ɾ��printf("
					s.erase(s.end() - 1);//ɾ��;
					s.erase(s.end() - 1);//ɾ��)
					s.erase(s.end() - 1);//ɾ��"
					//temp.token_type = s;
					//��һ���жϣ����s��ע��/�ո��ǾͲ�Ҫ�ӽ�ȥ�ˣ�����flag����Ϊ��Ч
					if (s == "SPACE" || s == "MULTI_LINE_COMMENT" || s == "SINGLE_LINE_COMMENT")
					{
						//space_flag = 1;
						/*if (s == "MULTI_LINE_COMMENT")
						{
							out << "		flag_mult_line_comment = 1;" << endl;
						}*/
						out << "		str_temp = str_temp[str_temp.size()-1];" << endl;//��str_temp������
						out << "		break;" << endl;
						out << endl;
					}
					else
					{
						//space_flag = 0;
						out << "		s = \"" << s << "\";" << endl;
						//out << "		str_temp.erase(str_temp.end() - 1);" << endl;
						out << "		temp_s = str_temp;" << endl;
						out << "		temp_s.erase(temp_s.end() - 1);" << endl;
						out << "		temp.token_type = s;" << endl;
						out << "		temp.token_value = temp_s;" << endl;
						out << "		str_temp = str_temp[str_temp.size()-1];" << endl;
						out << "		tokens.push_back(temp);" << endl;
						out << "		break;" << endl;
						out << endl;
					}
				}
				else {
					out << "		" << endVec[i][j] << endl;//�������
				}
			}
		}
	}
	//���������������������������������������������������yacc�õġ���������������������������������������������������

	out << "		default:" << endl;
	out << "		break;" << endl;
	out << "	}" << endl; 
	out << "	return -1;" << endl;
	out << "}" << endl; 
	out << endl;
	//int findAction(int state����������

	//char* getCharPtr(char* fileName)����(��ȡ����lex�ļ�����
	out << "char* getCharPtr(const char* fileName)" << endl;
	out << "{" << endl;
	out << "	char* cp=NULL;" << endl;
	out << "	FILE *fp;" << endl;
	out << "	fp=fopen(fileName,\"r\");" << endl;
	out << "	if(fp==NULL)" << endl;
	out << "	{" << endl;
	out << "		printf(\"can't open file\");" << endl;
	out << "		exit(0);" << endl;
	out << "	}" << endl;
	out << endl;
	out << "	fseek(fp,0,SEEK_END);" << endl;
	out << "	int flen = ftell(fp);" << endl; //�õ��ļ���С
	out << "	cp = (char *)malloc(flen + 1);" << endl; //�����ļ���С��̬�����ڴ�ռ�
	out << "	if (cp == NULL)" << endl;
	out << "	{" << endl;
	out << "		fclose(fp);" << endl;
	out << "		return 0;" << endl;
	out << "	}" << endl;
	out << "	rewind(fp);" << endl; //��λ���ļ���ͷ
	out << "	memset(cp,0,flen+1);" << endl;
	out << "	fread(cp, sizeof(char), flen, fp);" << endl; //һ���Զ�ȡȫ���ļ�����
	out << "	cp[flen] = 0; " << endl;//�����ַ���������־
	out << "	return cp;" << endl;
	out << "}" << endl;

	out << codeEnd;

	out.close();
	return 0;
}

void print_array(string name, int size, const int* value, ofstream& out)//���
{
	const int* array_buf = value;
	out << "static int	" << name << "[" << size << "]" << " =" << endl;
	out << "	{	0," << endl;
	for (int i = 1; i < size; i++)
	{
		out << setw(4) << left << array_buf[i];
		if (i != size - 1)
		{
			out << ",";
		}
		if (i % 20 == 0)//1��20��
		{
			out << endl;
		}
	}
	out << "};" << endl;
}

