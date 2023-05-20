/*
  ����ʵ��һʵ���
  ʵ���������280�п�ʼ
  ʵ����Ա��032002632��032002622
  ��������ͣ�
  opnum:����������������ʵ��һ����
  deliNum:�������������ʵ��һ����
  grammmarNum:�ķ���������ʵ�������

 */
#include <bits/stdc++.h>
#include"head.h"
#include"transfer.cpp"
using namespace std;
#define keyNum 11
#define opNum 4
#define deliNum 7

#define grammarNum 46



//action�࣬��Ӧaction���ܵ�Ԫ
class Action{
public:
	int nextState;	//���ܵ����ž���Ҫ���뻹�ǹ�Լ�൱��r/s
	char nextChar;	//��һ������ 
	Action(int nextState,char nextChar){
		this->nextChar = nextChar;
		this->nextState = nextState;
	} 
}; 

//�����ķ���״̬��ÿ���ķ��ڲ�ͬ״̬���еı�ʾ��ʽ
class Project{
public:
	int num;	//�ķ���� 
	int now;	//С����λ�� 
	char next;	//չ���� 
	Project(int num,int now,char next){
		this->num = num;
		this->now = now;
		this->next = next;
	}
};

//��Ӧ������Ŀ֮�����ת��ϵ ���������ܵ�ʲô������ת��ʲô״̬
class Tran{
public:
	int begin;
	int nextState;
	char nextChar;
	Tran(int begin,int nextState,char nextChar){
		this->begin = begin;
		this->nextState = nextState;
		this->nextChar = nextChar;
	}
}; 

//���ս������
set<char> cn; 
//�ս�� ����
set<char> ct;
//FIRST��(key�����ս�� value��first�����±� 
map<char,int> firstSet; 
//first�������±�ȡ�� 
vector<set<char>> vec; 
//��Ŀ��
vector<vector<Project>> projectItems;
//ת����
vector<Tran> tranList; 
//action��
vector<vector<Action>> actionTable;
//״̬ջ
stack<int> stateStk;
//����ջ
stack<char> signStk; 

//�ķ� 
string grammarList[grammarNum];
					 
//�ؼ���	
string keyWords[] = {  "int", "float", "char", "bool", "if", "else", "do", "while", "break", "true", "false" }; 
            			
//��ϵ����� 
string relOps[] = {  "<" , "<=" , "==" , "!=" , ">" , ">=" };

//�߼������
string logOps[] = {  "&&" , "||" , "!" }; 

//������
char Ops[] = {  '+' , '-' , '*' , '/'};

//���
char Delimiters[] = {  '(' , ')' , '[' , ']' , '{' , '}' , ';'};

//�ļ������� 
ifstream ifs;
//�ļ������
ofstream ofs; 
//��һ����ȡ���ַ� 
char peek = ' '; 
//��ǰ���������
int line = 0; 

//�����һ�������Ƿ�Ϊ�����ַ�c
bool detectChar(char c,ifstream *ifs){
	peek = ifs->get();
	if (peek==EOF || peek!=c)
		return false;
	
	peek = ' ';
	return true;
} 

//ɨ����� 
string scan(bool *flag,ifstream *ifs){
	label:
	for (;; peek = ifs->get() )
	{
		if ( peek == EOF )
		{
			*flag = false;
			return "����";
		}
		
		if ( peek == ' ' || peek == '\t' || peek == '\r')
		{
			continue;
		}
		else if ( peek == '\n') {
			line++;
		}
		else
		{
			break;
		}
	}
	
	switch (peek) {
		//���1-�ж���&����&& 
		case '&':
			if ( detectChar('&',ifs) ) 
			{
				return "&& logop";
			}
			else
			{
				return "& Error";
			}
			break;
		
		//���2-�ж���|����||
		case '|':
			if ( detectChar('|',ifs) ) 
			{
				return "|| logop";
			}
			else
			{
				return "| Error";
			}
			break; 
			
		//���3-�ж���=����== 
		case '=':
			if ( detectChar('=',ifs) ) 
			{
				return "== relop";
			}
			else
			{
				return "= ��ֵ";
			}
			break; 
			
		//���4-�ж��ǣ����ǣ�= 
		case '!':
			if ( detectChar('=',ifs) ) 
			{
				return "!= relop";
			}
			else
			{
				return "! logop";
			}
			break; 
			
		//���5-�ж���>����>=
		case '>':
			if ( detectChar('=',ifs) ) 
			{
				return ">= relop";
			}
			else
			{
				return "> relop";
			}
			break;
		
		//���6-�ж���<����<=
		case '<':
			if ( detectChar('=',ifs) ) 
			{
				return "<= relop";
			}
			else
			{
				return "< relop";
			}
			break;
		//���7-�ж��Ƿ�Ϊ/
		case '/':
			//�����//ע�����������м����� 
			if ( detectChar('/',ifs) )
			{
				string ss="";
				getline(*ifs,ss);
				peek = ' ';	
				goto label;
			}	
		default:
			break;
	}
	
	//�ж��Ƿ�Ϊ���� 
	if (isdigit(peek))
	{
		int front = 0;
		do {
			front = front * 10 + (peek-'0');
			peek = ifs->get();
		} while (isdigit(peek));
		if (peek != '.')
			return to_string(front) + " num";
		
		float resultR = front;
		float index = 10;
		peek = ifs->get();
		while (isdigit(peek))
		{
			resultR = resultR + (peek-'0')/index;
			index *= 10;
		 	peek = ifs->get();
		}
		return to_string(resultR) + " real";
	}
	
	//�ж��Ƿ�Ϊ��ĸ
	if (isalpha(peek) || peek=='_')
	{
		string resultA = "";
		do {
			resultA += peek;
			peek = ifs->get();
		} while (isalpha(peek) || isdigit(peek) || peek=='_');
		
		for (int i=0;i<keyNum;i++)
			if (keyWords[i]==resultA)
				return resultA + " key";
				
		return resultA + " id"; 	
	} 
	
	char temp = peek;
	peek = ' ';
	
	for (int i=0;i<opNum;i++)
		if (Ops[i] == temp)
		{
			string s = "a";
			s[0] = temp;
			return s + " op";
		}
	
	for (int i=0;i<deliNum;i++)
		if (Delimiters[i] == temp)
		{
			string s = "a";
			s[0] = temp;
			return s + " ���";
		}
		
	string str = "a";
	str[0] = temp;	
			
	return str + " Error";
}
// ����Ϊ�ʷ���������
//--------------------------------------------------------------------------------------
// ����Ϊ�﷨��������

//�ж��Ƿ�Ϊ���ս�� 
bool isCn(char c){
	if (isupper(c))
		return true;
} 

//�ж��Ƿ�Ϊ�ս��
bool isCt(char c){
	return ( ' ' < c && c < '@' ) || ('Z' < c && c <= 128);
} 

//ȷ��չ����
string judgeNext(Project project){
	string next;
	
	bool flag = true;
	//LR1���С����λ�� 
	int now = project.now;
	while (flag){
		flag = false;
		//�����λ�������һ���ַ���ǰ�� 
		if ( now+1 >= grammarList[project.num].substr(3).length() )
		{
			//��ôԭLR1���չ������������µ�LR1���չ���� 
			next.append(1,project.next);
			break;
		}
		
		//��������ڲ���ʽ���һ���ַ���ǰ��λ���õ�δ���ĵڶ������ţ�eg. A->a��BC��������nextCharΪc��
		//(δ���ĵ�һ�����ţ��������B�����Ѿ�ȷ��Ϊ���ս���ˣ�
		char nextChar = grammarList[project.num].substr(3)[now+1];
		//������δ���ĵڶ����������ս��������������Ҫ�ҵ� 
		if (isCt(nextChar))
		{
			next.append(1,nextChar);
		}
		//������ս�����Ǿ�������FIRST����Ȼ��FIRST����ÿ�����Ŷ�������Ϊչ���� 
		else
		{
			int pos = firstSet[nextChar];
			set<char> chars = vec[pos];
			for (set<char>::iterator it=chars.begin();it!=chars.end();it++)
			{
				if (*it != '@')
				{
					next.append(1,*it);
				}
			}
			//���FIRST���а���@��˵��������ս������Ϊ��
			//�Ǿ�now++��������ǰ��һ������ 
			if (chars.count('@')!=0)
			{
				now++;
				flag = true;
			}
		}
	}
	return next;
} 

//�ж�����LR1���Ƿ����
bool isEqual(Project pro1,Project pro2){
	if ( pro1.num != pro2.num )
		return false;
	if ( pro1.now != pro2.now )
		return false;
	if ( pro1.next != pro2.next)
		return false;
	
	return true;
} 

//�ж�ͬһ����Ŀ����LR1���Ƿ��ظ�
bool isRepeat1(Project project,int t){
	vector<Project> item = projectItems[t];
	
	for (int i=0;i<item.size();i++)
	{
		Project pro = item[i];
		if (isEqual(pro,project))
			return true;
	}
	
	return false;
} 

//�ж������ɵ���Ƿ�����ǰ��ظ�
int isRepeat2(){
	vector<Project> newItem = projectItems[projectItems.size()-1];
	for (int i=0;i < projectItems.size()-1;i++)
	{
		int count = 0;
		vector<Project> tmp = projectItems[i];
		if (newItem.size() != tmp.size())
		{
			continue;
		}
		
		for (int j=0;j<newItem.size();j++)
		{
			for (int k=0;k<tmp.size();k++)
			{
				if (isEqual(tmp[k],newItem[j]))
				{
					count++;
					break;
				}
			}
		}
		
		if (count == newItem.size())
			return i;
	} 
	return 0;
} 

//����Ŀ���հ�
void Closure(int t){
	vector<Project>* item = &projectItems[t];
	
	for (int i=0;i<item->size();i++)
	{
		//����ÿһ��LR1�� 
		Project project = (*item)[i];
		
		string grammar = grammarList[project.num];
		int pos = 1;//�ķ�����'-'���±� 
		string nonTerminal = grammar.substr(0,pos);
		string body = grammar.substr(pos+2);
		
		if (body.length() <= project.now)
		{
			// ���� body ĩβ�������ڴ�������չ��Ŀ���ˣ�����continue
			continue;
		} 
		
		//С�����ķ��� 
		char c = body[project.now];
		
		//������ս���Ļ������ڵ�ǰ��Ŀ���У���LR1��ü�����չ
		if (!isCn(c))
		{
			continue;
		}
		
		//С�����Ϊ���ս��������������ķ�
		for (int j=0;j<grammarNum;j++)
		{
			//�ҵ���Ӧ�ķ� 
		 	if ( grammarList[j].substr(0,pos)[0] == c )
			{
				//ȷ��չ���� 
			 	string nexts = judgeNext(project); 
			 	for (int k=0;k<nexts.length();k++)
			 	{
			 	 	Project* tmp = new Project(j,0,nexts[k]);
					if (!isRepeat1(*tmp,t))	
					{
					 	item->push_back(*tmp);	
					}	
				}
			}		
		} 
	} 
	
} 

//����Ϊ�ս���Ĵ���
void inputWithCt(int t,vector<Project> item){
	for (set<char>::iterator it=ct.begin();it!=ct.end();it++) 
	{
	 	vector<Project>* buffer = new vector<Project>;
		for (int k=0;k<item.size();k++)
		{
			Project pro = item[k];
			string grammar = grammarList[pro.num];
			string body = grammar.substr(3);
			if ( pro.now < body.length() && body[pro.now] == *it)
			{
				Project tmp(pro.num,pro.now+1,pro.next);
				buffer->push_back(tmp);
			}	
		} 
		
		if (!buffer->empty())
		{
			projectItems.push_back(*buffer);
			Closure(projectItems.size()-1);
			int state = isRepeat2();
			if (state!=0)
			{
				projectItems.pop_back();
				//action�� 
				Tran* tran = new Tran (t,state,*it);
				tranList.push_back(*tran);
				
			}
			else
			{
				Tran* tran = new Tran (t,projectItems.size()-1,*it);
				tranList.push_back(*tran);
			} 
		}		
	} 
}  

//����Ϊ���ս���Ĵ���
void inputWithCn(int t,vector<Project> item){
	for (set<char>::iterator it=cn.begin();it!=cn.end();it++) 
	{
	 	vector<Project>* buffer = new vector<Project>;
		for (int k=0;k<item.size();k++)
		{
			Project pro = item[k];
			string grammar = grammarList[pro.num];
			string body = grammar.substr(3);
			if ( pro.now < body.length() && body[pro.now] == *it)
			{
				Project tmp(pro.num,pro.now+1,pro.next);
				buffer->push_back(tmp);
			}	
		} 
		
		if (!buffer->empty())
		{
			projectItems.push_back(*buffer);
			Closure(projectItems.size()-1);
			int state = isRepeat2();
			if (state!=0)
			{
				projectItems.pop_back();
				//ת�Ʊ� 
				Tran* tran = new Tran (t,state,*it);
				tranList.push_back(*tran);
				
			}
			else
			{
				Tran* tran = new Tran (t,projectItems.size()-1,*it);
				tranList.push_back(*tran);	
			} 
		}		
	} 
}  

//���action��Ӧ�е�����
char getInputChar(string token,string type){
	
		
	if (type=="key"){
		
		if (token=="int" || token=="float" || token=="char" || token=="bool")
			return 'b';
			
		if (token=="if")
			return 'f';
		
		if (token=="else")
			return 's';
		
		if (token=="do")
			return 'd';
			
		if (token=="while")
			return 'w';
			
		if (token=="break")
			return 'k';
		
		if (token=="true")
			return 't';
		
		if (token=="false")
			return 'm';
	}
		
	if (type=="logop"){
		if (token=="&&")
			return 'a';
		if (token=="||")
			return 'o';
		if (token=="!")
			return '!'; 
	}
	
	if (type=="relop"){
		if (token=="<")
			return '<';
			
		if (token==">")
			return '>';
			
		if (token=="<=")
			return 'l';
		
		if (token==">=")
			return 'g';
			
		if (token=="==")
			return 'h';
			
		if (token=="!=")
			return 'u';
		
	}
	
	if (type=="op")
		return token[0];
	
	if (type=="���")
		return token[0];
	
	if (type=="��ֵ")
		return '=';
	
	if (type=="num")
		return 'n';
	
	if (type=="real")
		return 'r';
	
	if (type=="id")
		return 'i';
} 

//��ӡ������Ϣ
string reportError(char cur){
	if (cur=='b')
		return "int float char bool";
	if (cur=='f')
		return "if"; 
	if (cur=='s')
		return "else";
	if (cur=='d')
		return "do";
	if (cur=='w')
		return "while";
	if (cur=='k')
		return "break";
	if (cur=='t')
		return "true";
	if (cur=='m')
		return "false";
		
	if (cur=='<')
		return "<";
	if (cur=='>')
		return ">"; 
	if (cur=='l')
		return "<=";
	if (cur=='g')
		return ">=";
	if (cur=='h')
		return "==";
	if (cur=='u')
		return "!=";
		
	if (cur=='a')
		return "&&";
	if (cur=='o')
		return "||";
	if (cur=='!')
		return "!";
	
	if (cur=='+')
		return "+";
	if (cur=='-')
		return "-"; 
	if (cur=='*')
		return "*";
	if (cur=='=')
		return "��ֵ��";
	if (cur=='n')
		return "num";
	if (cur=='r')
		return "real";	
	if (cur=='i')
		return "������";
		
	 return "���"; 
	
} 


//���״̬ջ
void outputStateStk(){
	stack<int> tmp;
	
	while (!stateStk.empty()){
		tmp.push(stateStk.top());
		stateStk.pop();
	} 
	
	while (!tmp.empty()){
		ofs << to_string(tmp.top()) << ",";
		stateStk.push(tmp.top());
		tmp.pop();
	}
	
	ofs <<"\t\t";
} 
//�������ջ
void outputSignStk(){
	stack<char> tmp;
	
	while (!signStk.empty()){
		tmp.push(signStk.top());
		signStk.pop();
	} 
	
	string ss="a";
	
	while (!tmp.empty()){
		ss[0] = tmp.top();
        string tt = "";
        if(tran_ch_to_stirng.count(ss[0])){
            tt = tran_ch_to_stirng[ss[0]];
        }else{
            tt = ss[0];
        }

		ofs << tt<< ",";

		signStk.push(tmp.top());
		tmp.pop();
	}
	
	ofs <<"\t\t";
} 
//��ȡ�ķ����������е��ս������ս�� 
int getGrammar(){
	//���������ķ������������ķ����������ս������ս�� 
	ifs.open("lr_grammar.txt",ios::in);
	if (!ifs.is_open())
	{
		cout << "�ļ���ʧ��" <<endl;
		return -1;
	}
	string str="";
	int i = 1;
	//�����ķ� 
	getline(ifs,str);
	grammarList[i++] = str;
	string s = "a";
	s[0] = str[0];	
	string tmp = "'->" + s;
	grammarList[0] = tmp;
	
	 
	while (getline(ifs,str))
	{
		grammarList[i++] = str;
		 
	}
	ifs.close();
	
	//�ռ��ս������ս�� 
 	for (i=1;i<grammarNum;i++)
 	{
 		string grammar = grammarList[i];
 		int length = grammar.length();
 		for (int j=0;j<length;j++)
 			if ( j!=1 && j!=2 )
 				if (isCn(grammar[j]))
 				{
 					cn.insert(grammar[j]);
				}
				else if (isCt(grammar[j]))
				{
					ct.insert(grammar[j]);
				}
	}
	
	return 0;
}
//�������з��ս����FIRST�� 
void buildFirst(){
	//׼��FIRST��������firstSet�Ƿ��ս��������first����vec���±�ļ�ֵ�ԣ���<���ս��,�±�>
	//vec��ÿ�����ս����Ӧ��first���б����±���ȡ 
	int i = 0;
	for (set<char>::iterator it=cn.begin();it!=cn.end();it++)
	{
		firstSet.insert(pair<char,int>(*it,i++));
		set<char> *tmpSet = new set<char>;
		vec.push_back(*tmpSet);
	}
	firstSet.insert(pair<char,int>('\'',i));
	set<char> *tmpSet = new set<char>;
	vec.push_back(*tmpSet);
	

	bool hasChanged = true;
	while (hasChanged){
		hasChanged = false;
		for (int i=0;i<grammarNum;i++)
		{
			bool empty = true;
			int j = 0;
			int pos = 1;//�ķ�����'-'���±� 
			string nonTerminal = grammarList[i].substr(0,pos);
			string body = grammarList[i].substr(pos+2);
			
			int length = body.length();
			
			char c;
			char nT;
			int pos1,pos2;
			set<char> *target;
			set<char> *source;
			
			while ( empty && j < length ){
				empty = false;
				//������ 
				c = body[j];
				//���ķ��ս�� 
				nT = nonTerminal[0];
				
				//first���ϲ� 
				pos1 = firstSet[nT];
				pos2 = firstSet[c];
				target = &vec[pos1];
				source = &vec[pos2];
				
				if ( isCn(c) )
				{
					int preSize = target->size();					
					target->insert(source->begin(),source->end()); 
							
					if (target->size() != preSize)
					{
						hasChanged = true;
					}
					
					if (source->count('@') != 0)
					{
						empty = true;
						j++;
					}
				}
				else if (target->count(c) == 0)
				{
					target->insert(c);
					hasChanged = true;
				}
			}
			
			if ( j == length )
			{
				target->insert('@'); 
			}
		} 
	} 
} 
//������Ŀ���Լ���Ŀ�����״̬ת�ƹ�ϵ
void buildItems(){
	//��Ŀ��0�У���ʼֻ�е�0���ķ����������ķ���С�������±�0λ��չ����Ϊ'#' 
	Project project(0,0,'$'); 
	vector<Project> *item = new vector<Project>;
	item->push_back(project);
	//��ʼ����Ŀ���ļ��ϣ���ʱ֮��ֻ����Ŀ��0
	projectItems.push_back(*item);
	
	//����Ŀ��0�ıհ� 
	Closure(0);
	
	for (int i=0;i<projectItems.size();i++)
	{
		//��ÿһ����Ŀ�����д����ʼʱֻ����Ŀ��0 
		vector<Project> item = projectItems[i];
		
		//��һ�������Ϊ�ս��
		//iΪ�����ţ�itemΪ����� 
		inputWithCt(i,item); 
		
		//��һ�������Ϊ���ս�� 
		inputWithCn(i,item);
	}
} 
//����action��
void buildAction(){

	//�Ƚ��й�Լ�ļ�� 
	
	for (int i=0;i<projectItems.size();i++)
	{
		//��ÿһ�������Լ 
		vector<Project> item = projectItems[i];
		//����һ��actionRow����action���б���Ϊһ�У�����͹�Լ��
		vector<Action>* actionRow = new vector<Action>; 
		//��Row����Action����������Ϊ�գ�ҲΪÿ�����״̬��������һ�У�����������붯����ʹ�ã�
		actionTable.push_back(*actionRow); 
		//������Ŀ����ÿһ��LR1�� 
		for (int j=0;j<item.size();j++)
		{
			Project pro = item[j];	
			string grammar = grammarList[pro.num];
			string body = grammar.substr(3);
			int length = body.length();
			if (pro.now == length || ( pro.now==0 && body[0]=='@' )) 
			{
				int state = (pro.num)*(-1);
				char nextChar = pro.next;
				Action* action = new Action(state,nextChar);
				actionTable[actionTable.size()-1].push_back(*action);
			}
		} 
	} 
	
	//�ٽ�������ļ�� 
	for (int i=0;i<tranList.size();i++)
	{
		Tran tran = tranList[i];
		Action* action = new Action(tran.nextState,tran.nextChar);
		actionTable[tran.begin].push_back(*action); 
	} 
} 
//����������
void getResult(){
	//���з������̵Ĺ���
	int curState = 0;	//״̬ջ�ĳ�ʼջ��
	ifs.open("in.txt",ios::in);
	ofs.open("out.txt",ios::out);
	
	
	string tokenMap;
	string token;
	string type; 
	int spacePos;
	bool flag = true;	//flag��ʶ�ļ��Ƿ���� 
	bool rec = true;	//rec��ʶ�Ƿ������һ�������Ĵ��أ���ֹ����һ��" ",�մ�������ʶע�ͣ� 
	//����һ������Ϊֹ 
	do {
		tokenMap = scan(&flag,&ifs);
		if (tokenMap!=" ")	//���û�ж���ע�� 
		{
			spacePos = tokenMap.find(" ");
			token = tokenMap.substr(0,spacePos);
			type = tokenMap.substr(spacePos+1); 
			rec = false;
		}	
	} while (rec);
	rec = true; 
	
	//cur�����ڵ��������
	char cur = getInputChar(token,type); 
	
	stateStk.push(0);
	signStk.push('$');
	
	//��ʶ�Ƿ������ 
	bool done = false;
	//�Ƿ��д��� 
	bool hasError = false;
	
	ofs << "״̬ջ\t\t����ջ\t\t����\t\t����\n";  	
	while (!done && !hasError){
		ofs.flush();
		//��ȡ״̬ջ����״̬ 
		curState = stateStk.top();
		//�˴����״̬ջ����
		outputStateStk();
		//�˴��������ջ����
		outputSignStk();
		
		//����һ����¼�Ƿ����ı�����������꣬�������������� flagΪfalseʱ����ʱ��ȡ���һ������ 
		if (!flag)
		{
			ofs << "��ȫ������";
			cur = '$';
		} 
		else 
		{
            if (tran_ch_to_stirng.count(cur)){
                ofs << tran_ch_to_stirng[cur];
            }else{
                ofs << cur;
            }

		} 
		//���������һ�����Ŷ�Ӧ����ĸ���ս����
		//���ս��ֻ���ڷ���ջ����
		
		ofs <<"\t";
		
		hasError = true;
		
		vector<Action> actionRow = actionTable[curState];
		for (int i=0;i<actionRow.size();i++)
		{
			if ( actionRow[i].nextChar != cur )	
				continue;
			//ƥ�䵽�ˣ�û�г������Լ���	
			hasError = false;
			int nextState = actionRow[i].nextState;
			if (nextState==0)
			{
				ofs << "��������"<<endl;
				//���==0��˵��������
				done = true;	
			} 
			else if (nextState > 0)
			{
				
				//�ļ�������������
				ofs << "���뵽" << nextState << endl;
				
				stateStk.push(nextState);
				signStk.push(actionRow[i].nextChar);
				
				//����һ������ 
				if (flag) {
					do {
						tokenMap = scan(&flag,&ifs);
						if (tokenMap!=" " && tokenMap!="����")	//���û�ж���ע�� 
						{
							spacePos = tokenMap.find(" ");
							token = tokenMap.substr(0,spacePos);
							type = tokenMap.substr(spacePos+1); 
							cur = getInputChar(token,type);
							
							//cout<<"in while:  "<<token<<"   ---   "<<type<<"   ---   "<<cur<<endl;
							//getchar();
							
							rec = false;
							
						}	
					} while (rec);
				}	
			}
			else
			{
				nextState = nextState*(-1);
				string grammar = grammarList[nextState];
				string g_temp = "";
				for(int i = 0;i<grammar.size();i++){
					if (tran_ch_to_stirng.count(grammar[i])){
						g_temp = g_temp + tran_ch_to_stirng[grammar[i]];
					}
					else{
						g_temp = g_temp + grammar[i];
					}
				}
				// ofs << "�����ķ�" << g_temp << " ��Լ";
				ofs << " ��Լ";

				string body = grammar.substr(3);
				int len = body.length();
				if (body[0]== '@')
				{
					len--;
				}
				for (int j=0;j<len;j++)
				{
					stateStk.pop();
					signStk.pop();
				}
				
				//��Լ��ķ��ս�� 
				char firstChar = grammar.substr(0,1)[0];
				signStk.push(firstChar);
				
				//ѹ���Ҫ�ټ�� 
				vector<Action> actionRow = actionTable[stateStk.top()]; 
				for (int i=0;i<actionRow.size();i++)
				{
					if (actionRow[i].nextChar == firstChar)
					{
						stateStk.push(actionRow[i].nextState);
						ofs << actionRow[i].nextState << endl;
					}
				}
			} 
			
			break;
		} 
		
		if (hasError)
		{
			cout<<"�� "<<(line+1)<<" �з�����������"<<endl;
			cout<<"���ܳ���ķ������ͣ� "<<reportError(cur)<<endl;
			 
			ofstream errOfs; 
			errOfs.open("ErrorReport.txt",ios::out);
			errOfs << "�� "<<(line+1)<<" �з�����������"<<endl;
			errOfs << "���ܳ���ķ������ͣ� "<<reportError(cur)<<endl; 
			errOfs.close();
		}
	
	}
	ifs.close();
	ofs.close();
} 

//�����Ŀ�� 
void outputItems(){
	ofstream itemOfs;
	itemOfs.open("projectItems.txt",ios::out);
	
	for (int i=0;i<projectItems.size();i++){
		itemOfs << "I" <<i<<":"<<endl;
		for (int j=0;j<projectItems[i].size();j++){
			string s = grammarList[projectItems[i][j].num];
			string ss = s.substr(3);
			string left = s.substr(0,1);
			ss.insert(projectItems[i][j].now,1,'.');
			char l = left[0];
			string sl = tran_ch_to_stirng[l];
			string temps = "";
			for(int i = 0;i<ss.size();i++){
				
				string t ="";
				char a = ss[i];
				// t = t + a;

				if (tran_ch_to_stirng.count(ss[i]))
					t = t + tran_ch_to_stirng[ss[i]]+" ";
				else t = t+a +" ";
				
				temps += t;
			}
			
			
			itemOfs << sl <<"->" <<temps << "," << projectItems[i][j].next <<endl;
		}
		itemOfs << "==========================="<<endl;
	}
		
	itemOfs.close();
}

//���action��
void outputAction(){
	ofstream actionOfs;
	actionOfs.open("action.txt",ios::out);
	
	for (int i=0;i<actionTable.size();i++)
		for (int j=0;j<actionTable[i].size();j++){
			string tt = "";
			if (tran_ch_to_stirng.count(actionTable[i][j].nextChar)){
				tt = tt + tran_ch_to_stirng[actionTable[i][j].nextChar];
			}
			else tt = tt + actionTable[i][j].nextChar;
			actionOfs << "(" << i << "," << tt << "," << actionTable[i][j].nextState<<")"<<endl;
		}
	
	actionOfs.close();
} 



int main(){	
	//�õ��ķ� 
	get_map();

//	for(auto it:tran_ch_to_stirng){
//		cout<<it.first<<"----"<<it.second<<endl;
//	}
	int flag = getGrammar();
	//�����ȡ�ķ�û�г��� 
	if (flag!=-1) //lr_grammar.txt �ļ���ʧ��
	{
		//������ս��FIRST�� 
        buildFirst();
		//������Լ�����ת�ƹ�ϵ
		buildItems(); 
		//����action��
        buildAction();
		//����action�������������ļ�in.txt
        getResult();
	} 
	
	//�������
	//�����Ŀ�� 
	outputItems();
	//���action��
    outputAction();
	
	return 0;
} 




            			

