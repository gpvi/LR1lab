#include<bits/stdc++.h>
using namespace std;
map<string,char> tran_string_to_ch;
map<char,string> tran_ch_to_stirng ;
vector<string> sentences;
vector<string> lefts;
vector<vector<string>> sub_strs;
map<string,char> term_string_to_ch;
map<char,string> term_ch_to_string;
void Stringsplit(string str,  const char split,vector<string>& res)
{
	istringstream iss(str);	// 输入流
	string token;			// 接收缓冲区
	while (getline(iss, token, split))	// 以split为分隔符
	{
		res.push_back(token);
	}
}

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

void isTerminal(){
	set<string> terms;
	char start = 'a';
	int count = 0;
	for(auto it1:sub_strs){
		for(auto it2:it1){
			int f = 0;
			if (!tran_string_to_ch.count(it2) ){
				char temp =' ';
				if (it2 =="#"){
					temp = '@';	
					f = 1;
				}
				else if (it2 == ";"){
					temp = ';';
					f = 1;
				}
				else if(it2 == "(" or it2 ==")" or it2 == "{" or it2 == "}" or it2 == "[" or it2 == "]")
				{
					temp = it2[0];
					f = 1;
					
				}else if(it2=="" or it2 == " ") continue;
				else{
					if (!terms.count(it2)){
						terms.insert(it2);
						temp = start + count;
						count ++;
						f = 1;
					}
				}
				if (f == 1) term_string_to_ch[it2] = temp;

			}
		}
	}
	for(auto it :term_string_to_ch){
		term_ch_to_string[it.second] = it.first;
	}
}


void get_map(){
ifstream infile;
    string path ="no_term.txt";
    vector<string> str;
	read_file(path,str);
    int count = 0;
    char start = 'A';
    for(auto it : str){
        tran_string_to_ch[it] = start + count;
        count += 1;
    }
    for(auto it : tran_string_to_ch){
        tran_ch_to_stirng[it.second] = it.first;
    }    
}

void connect_left_right(){

	string newLefts [lefts.size()];
	for(int i = 0;i<(int)lefts.size();i++){
//		cout<<tran_string_to_ch[lefts[i]]<<endl;
		newLefts[i] = tran_string_to_ch[lefts[i]];
	}
	string newRight [lefts.size()];
	int c = 0;
	for(auto it1:sub_strs){
		string temp = "";
		for(auto it2:it1){
			bool a = it2 == "";
			// cout<<a;
			if (it2 != ""){
				char t ;
				if (tran_string_to_ch.count(it2)){
					t =tran_string_to_ch[it2];
				}
				else if (term_string_to_ch.count(it2))
				{
					t = term_string_to_ch[it2];
				}
				else continue;
				temp += t;
		}
		
	}
		sentences.push_back(newLefts[c]+"->"+temp);
		newRight[c] = temp;
		c++ ;
		// cout<<temp<<endl;
	}
	for(auto it : sentences){
		cout<<it<<endl;
	}
}
void out_put(){
	ofstream outfile("lr_grammer.txt", ios::trunc);
	for(auto it:sentences){
		outfile<<it<<endl;
	}
}
int main(){
    get_map();  
    read_grammer();
	isTerminal();
	connect_left_right();
	out_put();
    return 0;
}
