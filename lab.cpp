/*
  整合实验一实验二
  实验二函数从280行开始
  实验人员：032002632，032002622
  宏变量解释：
  opnum:操作符数量，用于实验一部分
  deliNum:界符数量，用于实验一部分
  grammmarNum:文法数量用于实验二部分

 */
#include <bits/stdc++.h>
#include"head.h"
#include"transfer.cpp"
using namespace std;
#define keyNum 11
#define opNum 4
#define deliNum 7

#define grammarNum 46



//action类，对应action表功能单元
class Action{
public:
	int nextState;	//接受到符号决定要移入还是归约相当于r/s
	char nextChar;	//下一个符号 
	Action(int nextState,char nextChar){
		this->nextChar = nextChar;
		this->nextState = nextState;
	} 
}; 

//单个文法的状态，每个文法在不同状态集中的表示形式
class Project{
public:
	int num;	//文法序号 
	int now;	//小数点位置 
	char next;	//展望符 
	Project(int num,int now,char next){
		this->num = num;
		this->now = now;
		this->next = next;
	}
};

//对应两个项目之间的跳转关系 ，包括接受到什么符号跳转到什么状态
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

//非终结符集合
set<char> cn; 
//终结符 集合
set<char> ct;
//FIRST集(key：非终结符 value：first集的下标 
map<char,int> firstSet; 
//first集（按下标取） 
vector<set<char>> vec; 
//项目集
vector<vector<Project>> projectItems;
//转换表
vector<Tran> tranList; 
//action表
vector<vector<Action>> actionTable;
//状态栈
stack<int> stateStk;
//符号栈
stack<char> signStk; 

//文法 
string grammarList[grammarNum];
					 
//关键字	
string keyWords[] = {  "int", "float", "char", "bool", "if", "else", "do", "while", "break", "true", "false" }; 
            			
//关系运算符 
string relOps[] = {  "<" , "<=" , "==" , "!=" , ">" , ">=" };

//逻辑运算符
string logOps[] = {  "&&" , "||" , "!" }; 

//操作符
char Ops[] = {  '+' , '-' , '*' , '/'};

//界符
char Delimiters[] = {  '(' , ')' , '[' , ']' , '{' , '}' , ';'};

//文件输入流 
ifstream ifs;
//文件输出流
ofstream ofs; 
//下一个读取的字符 
char peek = ' '; 
//当前处理的行数
int line = 0; 

//检测下一个符号是否为参数字符c
bool detectChar(char c,ifstream *ifs){
	peek = ifs->get();
	if (peek==EOF || peek!=c)
		return false;
	
	peek = ' ';
	return true;
} 

//扫描词素 
string scan(bool *flag,ifstream *ifs){
	label:
	for (;; peek = ifs->get() )
	{
		if ( peek == EOF )
		{
			*flag = false;
			return "结束";
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
		//情况1-判断是&还是&& 
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
		
		//情况2-判断是|还是||
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
			
		//情况3-判断是=还是== 
		case '=':
			if ( detectChar('=',ifs) ) 
			{
				return "== relop";
			}
			else
			{
				return "= 赋值";
			}
			break; 
			
		//情况4-判断是！还是！= 
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
			
		//情况5-判断是>还是>=
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
		
		//情况6-判断是<还是<=
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
		//情况7-判断是否为/
		case '/':
			//如果是//注解则跳过此行继续读 
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
	
	//判断是否为数字 
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
	
	//判断是否为字母
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
			return s + " 界符";
		}
		
	string str = "a";
	str[0] = temp;	
			
	return str + " Error";
}
// 以上为词法分析部分
//--------------------------------------------------------------------------------------
// 以下为语法分析部分

//判断是否为非终结符 
bool isCn(char c){
	if (isupper(c))
		return true;
} 

//判断是否为终结符
bool isCt(char c){
	return ( ' ' < c && c < '@' ) || ('Z' < c && c <= 128);
} 

//确定展望符
string judgeNext(Project project){
	string next;
	
	bool flag = true;
	//LR1项的小数点位置 
	int now = project.now;
	while (flag){
		flag = false;
		//如果点位置在最后一个字符的前后 
		if ( now+1 >= grammarList[project.num].substr(3).length() )
		{
			//那么原LR1项的展望符就是这个新的LR1项的展望符 
			next.append(1,project.next);
			break;
		}
		
		//如果至少在产生式最后一个字符的前两位，得到未来的第二个符号（eg. A->a·BC，则现在nextChar为c）
		//(未来的第一个符号，即这里的B，是已经确定为非终结符了）
		char nextChar = grammarList[project.num].substr(3)[now+1];
		//如果这个未来的第二个符号是终结符，它就是我们要找的 
		if (isCt(nextChar))
		{
			next.append(1,nextChar);
		}
		//如果是终结符，那就求它的FIRST集，然后FIRST集中每个符号都可以作为展望符 
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
			//如果FIRST集中包含@，说明这个非终结符可以为空
			//那就now++，继续往前找一个符号 
			if (chars.count('@')!=0)
			{
				now++;
				flag = true;
			}
		}
	}
	return next;
} 

//判断两个LR1项是否相等
bool isEqual(Project pro1,Project pro2){
	if ( pro1.num != pro2.num )
		return false;
	if ( pro1.now != pro2.now )
		return false;
	if ( pro1.next != pro2.next)
		return false;
	
	return true;
} 

//判断同一个项目集中LR1项是否重复
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

//判断新生成的项集是否与先前项集重复
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

//求项目集闭包
void Closure(int t){
	vector<Project>* item = &projectItems[t];
	
	for (int i=0;i<item->size();i++)
	{
		//对于每一个LR1项 
		Project project = (*item)[i];
		
		string grammar = grammarList[project.num];
		int pos = 1;//文法项中'-'的下标 
		string nonTerminal = grammar.substr(0,pos);
		string body = grammar.substr(pos+2);
		
		if (body.length() <= project.now)
		{
			// 点在 body 末尾，则不用在此项上扩展项目集了，可以continue
			continue;
		} 
		
		//小数点后的符号 
		char c = body[project.now];
		
		//如果是终结符的话，则在当前项目集中，此LR1项不用继续扩展
		if (!isCn(c))
		{
			continue;
		}
		
		//小数点后为非终结符，则查找所有文法
		for (int j=0;j<grammarNum;j++)
		{
			//找到对应文法 
		 	if ( grammarList[j].substr(0,pos)[0] == c )
			{
				//确定展望符 
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

//输入为终结符的处理
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
				//action表 
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

//输入为非终结符的处理
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
				//转移表 
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

//获得action表应有的输入
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
	
	if (type=="界符")
		return token[0];
	
	if (type=="赋值")
		return '=';
	
	if (type=="num")
		return 'n';
	
	if (type=="real")
		return 'r';
	
	if (type=="id")
		return 'i';
} 

//打印错误信息
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
		return "赋值符";
	if (cur=='n')
		return "num";
	if (cur=='r')
		return "real";	
	if (cur=='i')
		return "变量名";
		
	 return "界符"; 
	
} 


//输出状态栈
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
//输出符号栈
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
//读取文法并保存所有的终结符与非终结符 
int getGrammar(){
	//读入所有文法（包括增广文法），保存终结符与非终结符 
	ifs.open("lr_grammar.txt",ios::in);
	if (!ifs.is_open())
	{
		cout << "文件打开失败" <<endl;
		return -1;
	}
	string str="";
	int i = 1;
	//增广文法 
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
	
	//收集终结符与非终结符 
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
//构造所有非终结符的FIRST集 
void buildFirst(){
	//准备FIRST集，其中firstSet是非终结符和它的first集在vec中下标的键值对，即<非终结符,下标>
	//vec是每个非终结符对应的first集列表，以下标来取 
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
			int pos = 1;//文法项中'-'的下标 
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
				//待检测符 
				c = body[j];
				//左侧的非终结符 
				nT = nonTerminal[0];
				
				//first集合并 
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
//构造项目集以及项目集间的状态转移关系
void buildItems(){
	//项目集0中，初始只有第0个文法，即增广文法，小数点在下标0位，展望符为'#' 
	Project project(0,0,'$'); 
	vector<Project> *item = new vector<Project>;
	item->push_back(project);
	//初始化项目集的集合，此时之中只有项目集0
	projectItems.push_back(*item);
	
	//求项目集0的闭包 
	Closure(0);
	
	for (int i=0;i<projectItems.size();i++)
	{
		//对每一个项目集进行处理，最开始时只有项目集0 
		vector<Project> item = projectItems[i];
		
		//下一个输入符为终结符
		//i为项集的序号，item为项集本身 
		inputWithCt(i,item); 
		
		//下一个输入符为非终结符 
		inputWithCn(i,item);
	}
} 
//构造action表
void buildAction(){

	//先进行归约的检测 
	
	for (int i=0;i<projectItems.size();i++)
	{
		//对每一个项集检测归约 
		vector<Project> item = projectItems[i];
		//创建一个actionRow，在action表中表现为一行（移入和归约）
		vector<Action>* actionRow = new vector<Action>; 
		//将Row加入Action表（就算最终为空，也为每个项集（状态）创建了一行，方便后续移入动作的使用）
		actionTable.push_back(*actionRow); 
		//对于项目集中每一个LR1项 
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
	
	//再进行移入的检测 
	for (int i=0;i<tranList.size();i++)
	{
		Tran tran = tranList[i];
		Action* action = new Action(tran.nextState,tran.nextChar);
		actionTable[tran.begin].push_back(*action); 
	} 
} 
//构造分析结果
void getResult(){
	//进行分析过程的构造
	int curState = 0;	//状态栈的初始栈顶
	ifs.open("in.txt",ios::in);
	ofs.open("out.txt",ios::out);
	
	
	string tokenMap;
	string token;
	string type; 
	int spacePos;
	bool flag = true;	//flag标识文件是否读完 
	bool rec = true;	//rec标识是否读到了一个完整的词素（防止读到一个" ",空串用来标识注释） 
	//读到一个词素为止 
	do {
		tokenMap = scan(&flag,&ifs);
		if (tokenMap!=" ")	//如果没有读到注解 
		{
			spacePos = tokenMap.find(" ");
			token = tokenMap.substr(0,spacePos);
			type = tokenMap.substr(spacePos+1); 
			rec = false;
		}	
	} while (rec);
	rec = true; 
	
	//cur是现在的输入符号
	char cur = getInputChar(token,type); 
	
	stateStk.push(0);
	signStk.push('$');
	
	//标识是否处理结束 
	bool done = false;
	//是否有错误 
	bool hasError = false;
	
	ofs << "状态栈\t\t符号栈\t\t输入\t\t动作\n";  	
	while (!done && !hasError){
		ofs.flush();
		//获取状态栈顶的状态 
		curState = stateStk.top();
		//此处输出状态栈内容
		outputStateStk();
		//此处输出符号栈内容
		outputSignStk();
		
		//设置一个记录是否读完的变量，如果读完，输出“输入结束” flag为false时，此时读取最后一个词素 
		if (!flag)
		{
			ofs << "已全部读入";
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
		//否则输出下一个符号对应的字母（终结符）
		//非终结符只会在符号栈出现
		
		ofs <<"\t";
		
		hasError = true;
		
		vector<Action> actionRow = actionTable[curState];
		for (int i=0;i<actionRow.size();i++)
		{
			if ( actionRow[i].nextChar != cur )	
				continue;
			//匹配到了，没有出错，可以继续	
			hasError = false;
			int nextState = actionRow[i].nextState;
			if (nextState==0)
			{
				ofs << "接受输入"<<endl;
				//如果==0，说明接受了
				done = true;	
			} 
			else if (nextState > 0)
			{
				
				//文件输入移入内容
				ofs << "移入到" << nextState << endl;
				
				stateStk.push(nextState);
				signStk.push(actionRow[i].nextChar);
				
				//读下一个词素 
				if (flag) {
					do {
						tokenMap = scan(&flag,&ifs);
						if (tokenMap!=" " && tokenMap!="结束")	//如果没有读到注解 
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
				// ofs << "按照文法" << g_temp << " 归约";
				ofs << " 归约";

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
				
				//归约后的非终结符 
				char firstChar = grammar.substr(0,1)[0];
				signStk.push(firstChar);
				
				//压入后要再检测 
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
			cout<<"第 "<<(line+1)<<" 行发生解析错误。"<<endl;
			cout<<"可能出错的符号类型： "<<reportError(cur)<<endl;
			 
			ofstream errOfs; 
			errOfs.open("ErrorReport.txt",ios::out);
			errOfs << "第 "<<(line+1)<<" 行发生解析错误。"<<endl;
			errOfs << "可能出错的符号类型： "<<reportError(cur)<<endl; 
			errOfs.close();
		}
	
	}
	ifs.close();
	ofs.close();
} 

//输出项目集 
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

//输出action表
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
	//得到文法 
	get_map();

//	for(auto it:tran_ch_to_stirng){
//		cout<<it.first<<"----"<<it.second<<endl;
//	}
	int flag = getGrammar();
	//如果读取文法没有出错 
	if (flag!=-1) //lr_grammar.txt 文件打开失败
	{
		//构造非终结符FIRST集 
        buildFirst();
		//构造项集以及项集间的转移关系
		buildItems(); 
		//构造action表
        buildAction();
		//根据action表来处理输入文件in.txt
        getResult();
	} 
	
	//内容输出
	//输出项目集 
	outputItems();
	//输出action表
    outputAction();
	
	return 0;
} 




            			

