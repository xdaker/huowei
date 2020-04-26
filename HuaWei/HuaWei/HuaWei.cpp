// HuaWei.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <list>
using namespace std;

void split(const string& s, vector<int>& sv, const char flag = ' ') {
	sv.clear();
	istringstream iss(s);
	string temp;

	while (getline(iss, temp, flag)) {
		sv.push_back(stoi(temp));
	}
	return;
}

class transfer_accounts_model
{
public:
	transfer_accounts_model();
	~transfer_accounts_model();
	//int Original = 0;
	bool ClosedLoop = false;
	int Form = 0;
	int To = 0;
	int Lenght = 0;
	string History = "";
	void AddTo(int to, transfer_accounts_model*);
	void AddFrom(int form,transfer_accounts_model*);
	string GetForm() const;
	string GetTo() const;
	void IsQualified() const;
	list < transfer_accounts_model> *front;
	list<transfer_accounts_model> *next;
	bool *Read;
	transfer_accounts_model Copy() const;

	bool operator == (transfer_accounts_model box);
private:

};

transfer_accounts_model::transfer_accounts_model()
{
	front = new list<transfer_accounts_model>;
	next = new list<transfer_accounts_model>;
};

transfer_accounts_model::~transfer_accounts_model()
= default;

bool transfer_accounts_model::operator == (transfer_accounts_model box)
{
	transfer_accounts_model *a = this;
	transfer_accounts_model *b = &box;
	return *a == *b;
}

transfer_accounts_model Establish(int form, int to) {
	transfer_accounts_model *model = new transfer_accounts_model;
	//model.Original = form;
	model->Form = form;
	model->To = to;
	model->Lenght = 1;
	bool a = false;
	model->Read = &a;
	return *model;
}

transfer_accounts_model transfer_accounts_model::Copy() const
{
	transfer_accounts_model model;
	model.ClosedLoop = ClosedLoop;
	//model.Original = Original;
	model.Form = Form;
	model.To = To;
	model.Lenght = Lenght;
	model.History = History;
	return model;
}

void transfer_accounts_model::AddTo(int to, transfer_accounts_model *model)
{
	model->front->push_back(*this);
	next->push_back(*model);
	//Lenght++;
	return;
}

void transfer_accounts_model::AddFrom(int form, transfer_accounts_model *model)
{
	model->next->push_back(*this);
	this->front->push_back(*model);
	//Lenght++;
	return;
}

string transfer_accounts_model::GetForm() const
{
	return to_string(Form);
}

string transfer_accounts_model::GetTo() const
{
	return to_string(To);
}
//发起转账人为键值
map<int, list<transfer_accounts_model>> nodeForm;
//收款人为键值
map<int, list<transfer_accounts_model>> nodeTo;


struct qualified_result {
	string text="";
	bool isOk=false;
	int deep=0;
	vector<int> list;
	string get_text();
	string number_text = "";
	vector<bool> ReadResult;
	bool is_qualified();
};
//合格的结果集
list<qualified_result> qualifiedList;

bool IsExistence(qualified_result *result)
{
	bool resut = false;
	auto text = result->get_text();
	for (auto itr = qualifiedList.begin(); itr != qualifiedList.end(); ++itr)
	{
		if (itr->get_text().find(text) != string::npos)
		{
			resut = true;
			break;
		}
	}
	return resut;
}

//递归-前序遍历，向前遍历
void preTraversalFront(transfer_accounts_model* root, 
	transfer_accounts_model* original,qualified_result *result)
{
	if(root ==NULL || result->deep == 7)
		return;
	if (result->deep > 0)
		result->text = root->GetForm()+","+ result->text;
	else
	{
		result->text =  root->GetForm();
	}
	result->list.push_back(root->Form);
	result->deep++;
	//访问跟节点

		for (auto itr = root->front->begin(); itr != root->front->end(); ++itr)
		{
			if (original->To == itr->Form)
			{
				if (result->deep > 2 && result->deep < 8)
				{
					//result->isOk = true;
					//result->text = root->GetForm() + "," + result->text;
					if (!IsExistence(result))
						qualifiedList.push_back(*result);
				}
				break;
			}
			preTraversalFront(&*itr, original,result);
		}
}

//递归-前序遍历，向后遍历
void preTraversalTo(transfer_accounts_model* root,
	transfer_accounts_model *original, qualified_result *result)
{
	if (root == NULL || result->deep == 7)
		return;
	if (result->deep > 0)
	{
		result->text += "," + root->GetTo();
		result->list.push_back(root->To);	
	}
	else
	{
		result->text = root->GetForm() + "," + root->GetTo();
		result->list.push_back(root->Form);
		result->list.push_back(root->To);
	}
	result->deep++;
	
	//访问跟节点
	for (auto itr = root->next->begin(); itr != root->next->end(); ++itr)
	{
		if (original->Form == itr->To)
		{
			if (result->deep > 1 && result->deep < 7)
			{
				//result->isOk = true;
				//result->text += "," + root->GetTo();
				if (!IsExistence(result))
					qualifiedList.push_back(*result);
			}
			break;
		}
		preTraversalTo(&*itr, original, result);
	}
}

void transfer_accounts_model::IsQualified() const
{
	//向前搜索，包不包含自己
	list<transfer_accounts_model> *models= this->front;
	for (auto itr = models->begin(); itr != models->end(); ++itr)
	{
		//itr->AddTo(model.To, model);
	}
	//向后收锁，包不包含自己
	return ;
}

void AddToNode(transfer_accounts_model *model)
{
	if(nodeForm.find(model->Form) == nodeForm.end())
	{
		//找不到
		nodeForm.insert(std::pair<int, list<transfer_accounts_model>>
			(model->Form, list<transfer_accounts_model>{
			 *model
			}));
	}
	else
	nodeForm[model->Form].push_back(*model);

	if (nodeTo.find(model->Form) == nodeTo.end())
	{
		//找不到
		nodeTo.insert(std::pair<int, list<transfer_accounts_model>>
			(model->To, list<transfer_accounts_model>{
			 *model
				}));
	}
	else
	nodeTo[model->To].push_back(*model);
}
int sum = 0, Count = 0;
void dfs(transfer_accounts_model *model, 
	transfer_accounts_model *original ,qualified_result result)
{
	//cout << model->Form << " ";
	sum++;//每访问一个节点sum就++
	result.ReadResult.push_back(model->ClosedLoop);
	model->ClosedLoop = true;
	result.text += model->GetForm();
	result.deep++;
	if (original->Form == model->To)//找到环
	{
		if (result.is_qualified())
		{
			qualifiedList.push_back(result);
		}
		return;
	}
	//超过最大深度
	if(result.deep>7)
		return;
	//if (sum == Count)
	//	return;//所有的顶点已经访问过直接退出
	for (auto itr = model->next->begin(); itr != model->next->end(); ++itr)
	{
		result.text += ",";//后面还有假都好分隔
		dfs(&*itr, original, result);
	}
}

int main()
{
	//ifstream fin("/data/test_data.txt", std::ios::in);
	ifstream fin("D:\\MyGit\\huowei.git\\trunk\\HuaWei\\x64\\Debug\\test_data.txt", std::ios::in);
	char line[1024] = { 0 };
	string lineText = "";
	while (fin.getline(line, sizeof(line)))
	{
		stringstream word(line);
		vector<int> sv;
		word >> lineText;
		split(lineText, sv, ',');
		transfer_accounts_model modelt = Establish(sv[0], sv[1]);
		//在节点中查找谁转给了当前转账人
		if (nodeTo.find(modelt.Form) != nodeTo.end())
		{
			list<transfer_accounts_model> *moldes = &nodeTo[modelt.Form];
			list<transfer_accounts_model>::iterator itr;
			for (itr = moldes->begin(); itr != moldes->end(); ++itr)
			{
				itr->AddTo(modelt.To, &modelt);
			}
		}
		//在节点中查找谁转给了当前转账人
		if (nodeForm.find(modelt.To) != nodeForm.end())
		{
			list<transfer_accounts_model> *moldes = &nodeForm[modelt.To];
			list<transfer_accounts_model>::iterator itr;
			for (itr = moldes->begin(); itr != moldes->end(); ++itr)
			{
				itr->AddFrom(modelt.Form, &modelt);
			}
		}

		AddToNode(&modelt);
		Count++;
	}
	fin.clear();
	fin.close();
	Count = 0;
	//第二轮
	std::map<int, list<transfer_accounts_model>>::iterator it;
	ofstream ofs;
	// 3 指定路径和打开方式 
	//ofs.open("D:\\MyGit\\huowei.git\\trunk\\HuaWei\\x64\\Debug\\text111111.txt", ios::out);
	//ofs.open("/projects/student/result.txt", ios::out);
	
	for (it = nodeForm.begin(); it != nodeForm.end(); ++it) {
		for (list<transfer_accounts_model>::iterator itr = it->second.begin(); itr != it->second.end(); ++itr)
		{
			//向前查找
			/*qualified_result result1;
			preTraversalFront(&*itr, &*itr, &result1);*/
			//向后查找
			/*qualified_result result2;
			preTraversalTo(&*itr, &*itr, &result2);*/
			qualified_result result2;
			dfs(&*itr, &*itr, result2);
		}
	}
	cout << "循环数: "<<sum << endl;
	//// 4 写内容
	//ofs << qualifiedList.size() << endl;
	cout << qualifiedList.size() << '\n';
	for (list<qualified_result>::iterator itr2 = qualifiedList.begin(); itr2 != qualifiedList.end(); itr2++)
	{
		//ofs << itr2->text << endl;
		cout << itr2->text << '\n';
	}

	//// 5 关闭文件
	//ofs.close();
	//int a;
	//cin >> a;
	return 0;
}

vector<int> sort(vector<int> a)
{
	int n, i, k;
	for (k = 0; k < a.size(); k++)
		for (i = 0; i < a.size()-1 - k; i++)
			if (a[i] > a[i + 1])
			{
				n = a[i + 1];
				a[i + 1] = a[i];
				a[i] = n;
			}
	return a;
}

string qualified_result::get_text()
{
	if (number_text != "")
		return number_text;
	list = sort(list);
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		number_text += to_string(*itr);
	}
	return number_text;
}

bool qualified_result::is_qualified()
{
	if (ReadResult.size() < 3)
		return false;
	bool result = false;
	for (auto itr = ReadResult.begin(); itr!= ReadResult.end();itr++)
	{
		if(!*itr)//包含未读过的数据
		{
			result = true;
			break;
		}
	}
	return result;
}
