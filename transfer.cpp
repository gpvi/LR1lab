//#include<bits/stdc++.h>
//using namespace std;
#include"head.h"
void Stringsplit(string str,  const char split,vector<string>& res)
{
	istringstream iss(str);	// 输入流
	string token;			// 接收缓冲区
	while (getline(iss, token, split))	// 以split为分隔符
	{
		res.push_back(token);
	}
}
// 读取文件返回vector<string>
void read_file(string path,vector<string> &strs){
    ifstream infile;
    infile.open(path, ios::in);
	if (!infile.is_open())
	{
		cout << "读取文件失败" << endl;
        return ;
	}
	char buf[1024];
	while (infile.getline(buf,sizeof(buf)))
	{
		
        string temp = buf;

        strs.push_back(temp);

	}

}
//读取语法并分词
void read_grammer(){
    string path = "lr_sentence.txt";
    vector<string> strs;
    read_file(path,strs);
    for(auto it:strs){
        string temp = it;
        int index = temp.find("@");
        string left = temp.substr(0,index);
        string right = temp.substr(index+1,temp.length());
        vector<string> sub_strings;
        Stringsplit(right,' ',sub_strings);
		lefts.push_back(left);
        sub_strs.push_back(sub_strings);
    }	
}
//生成单个字符和文法关键词的映射
void get_map(){
	ifstream infile;
    //map文件保存
	string path = "./map.txt";
	vector<string> str;
	read_file(path,str);

	for(auto it :str){
		// cout<<it<<endl;
		int index = it.find(" ");
		string origin  = it.substr(0,index);
		char map_ch = it.substr(index+1,it.length())[0];
		// cout<<origin<<"***"<<map_ch<<endl;
		tran_string_to_ch[origin] = map_ch;
	}
	map<char,string> remap;
	for(auto it: tran_string_to_ch){
		tran_ch_to_stirng[it.second] = it.first;
	}

}
//连接生成式
void connect_left_right(){

	string newLefts [lefts.size()];
	for(int i = 0;i<(int)lefts.size();i++){
		newLefts[i] = tran_string_to_ch[lefts[i]];
	}
	string newRight [lefts.size()];
	int c = 0;
	for(auto it1:sub_strs){
		string temp = "";
		for(auto it2:it1){
//			bool a = it2 == "";
			// cout<<a;
			if (it2 != ""){
				char t ;
				if (tran_string_to_ch.count(it2)){
					t =tran_string_to_ch[it2];
					temp += t;
				}
				else temp += it2;				
		}
		
	}
		sentences.push_back(newLefts[c]+"->"+temp);
		newRight[c] = temp;
		c++ ;
		// cout<<temp<<endl;
	}
}
//映射后的文法
void out_put(){
	ofstream outfile("lr_grammar.txt", ios::trunc);
	for(auto it:sentences){
		outfile<<it<<endl;
	}
}
int get_file(){
    get_map();  
    read_grammer();
	// isTerminal();
	connect_left_right();
	out_put();
    return 0;
}
