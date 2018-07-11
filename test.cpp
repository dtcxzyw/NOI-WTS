#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cctype>
typedef wchar_t Char;
typedef std::wstring String;
struct Problem{
	String problem;
	String answer;
};
bool filter(Char c){
	return c==EOF || iswcntrl(c);
}
void input(std::wistream& in,String& string){
	Char c;
	do c=in.get();
	while((c==L'#' || c==L'@' || (&in==&std::wcin && (c==L'\r' || c==L'\n'))) && in);
	while(c!=L'#' && c!=L'@' && (&in!=&std::wcin || (c!=L'\r' && c!=L'\n'))){
		string.push_back(c);
		if(in)c=in.get();
		else break;
	}
	while(string.size() && filter(string.front()))string.erase(string.begin());
	while(string.size() && filter(string.back()))string.pop_back();
}
bool compareS(const String& a,const String& b){
	return a==b;
}
std::vector<String> cut(const String& a){
	std::vector<String> res;
	int lp=0;
	for(int i=0;i<a.size();++i){
		if(a[i]==L' '){
			res.push_back(String(a.begin()+lp,a.begin()+i));
			lp=i+1;
		}
	}
	res.push_back(String(a.begin()+lp,a.end()));
	return res;
}
bool find(const std::vector<String>& a,const String& b){
	for(int i=0;i<a.size();++i)
		if(compareS(a[i],b))
			return true;
	return false;
}
bool compareM(const String& a,const String& b){
	std::vector<String> va=cut(a);
	std::vector<String> vb=cut(b);
	if(va.size()!=vb.size())return false;
	for(int i=0;i<va.size();++i)
		if(!find(vb,va[i]))
			return false;
	for(int i=0;i<vb.size();++i)
		if(!find(va,vb[i]))			
			return false;
	return true;
}
typedef bool (*compareFunc)(const String& a,const String& b);
void readProblems(const char* path,std::vector<Problem>& problem){
	std::wifstream in(path);
	while(in){
		Problem p;
		input(in,p.problem);
		input(in,p.answer);
		problem.push_back(p);
	}
}
int test(const std::vector<Problem>& ps,std::wostream& out,bool single){
	int cnt=0;
	compareFunc cmp=single?compareS:compareM;
	for(int i=0;i<ps.size();++i){
		std::wcout<<L"["<<i+1<<L"]"<<ps[i].problem<<std::endl;
		if(!single)std::wcout<<L"（多选）"<<std::endl; 
		String ans;
		input(std::wcin,ans);
		if(cmp(ans,ps[i].answer))++cnt;
		else{
			out<<L"----------------------------------"<<std::endl; 
			if(&out!=&std::wcout){
				out<<ps[i].problem<<std::endl;
				if(!single)out<<L"（多选）"<<std::endl;
			}
			out<<L"你的答案:"<<ans<<std::endl;
			out<<L"正确答案:"<<ps[i].answer<<std::endl;
			out<<L"----------------------------------"<<std::endl; 
		}
	}
	return cnt;
}
int main(){
	std::locale::global(std::locale(""));
	std::wcout<<L"NOI2018笔试模拟系统"<<std::endl;
	std::wcout<<L"-----------------------------------"<<std::endl;
	std::vector<Problem> problemS,problemM;	
	std::wcout<<L"正在加载题目..."<<std::endl; 
	readProblems("problemS.db",problemS);
	readProblems("problemM.db",problemM);
	std::wcout<<L"题目加载完毕,共"<<problemS.size()<<L"道单选题，"<<problemM.size()<<L"道多选题"<<std::endl;
	std::wcout<<L"请选择模式：A 全部题目 B 模拟测试"<<std::endl;
	Char mode;
	do mode=std::wcin.get();
	while(mode!=L'A' && mode!=L'B');
	srand(clock());
	std::random_shuffle(problemS.begin(),problemS.end());
	std::random_shuffle(problemM.begin(),problemM.end());
	if(mode==L'B'){
		if(problemS.size()<45){
			std::wcout<<L"单选题数目不够"<<std::endl;
			system("pause");
			return 0;
		}
		if(problemM.size()<5){
			std::wcout<<L"多选题数目不够"<<std::endl;
			system("pause");
			return 0;
		}
		problemS.resize(45);
		problemM.resize(5);
	}
	std::wofstream history("history.log"); 
	std::wostream& out=(mode=='A'?std::wcout:history);
	int cnt=0;
	cnt+=test(problemS,out,true);
	cnt+=test(problemM,out,false);
	if(mode==L'A'){
		int tot=problemS.size()+problemM.size();
		std::wcout.precision(2);
		std::wcout<<L"共"<<tot<<L"题，对"<<cnt<<L"题，得分率"<<std::fixed<<100.0*cnt/tot<<L"%"<<std::endl; 
	}
	else{
		std::wcout<<L"考试得分："<<2*cnt<<std::endl;
		std::wcout<<L"错题已保存至history.log中"<<std::endl; 
	} 
	for(int i=0;i<6;++i)
		std::wcin.get();
	return 0;
}

