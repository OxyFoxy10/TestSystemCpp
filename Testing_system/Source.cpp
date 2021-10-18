#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<iomanip>
#include<windows.h>
#include<direct.h>
#include<map>
#include <filesystem>
#include <cstddef>
using namespace std;
namespace fst = std::filesystem;
//#define SIZE 5

class  guest {
	string login_;
	string password_;
public:
	guest() = default;
	guest(string login_, string password_) : login_(login_), password_(password_) {}
	void setLogin(string login_) {
		this->login_ = login_;
	}
	void setPassword(string password_) {
		this->password_ = password_;
	}
	string getLogin()const {
		return login_;
	}
	string getPassword() const {
		return password_;
	}
	virtual void readUserFromFile(fstream& fs) {
		if (fs.is_open()) {
			string encrypted;
			fs >> this->login_;
			fs >> encrypted;
			this->password_.clear();
			for (int i = 0; i < encrypted.size(); i++) {
				this->password_.push_back(encrypted[i] - 9);
			}
		}
		else cout << "User not found, create a new one\n";
	}
	virtual void writeUserToFile(fstream& fs) {
		if (!fs.is_open()) {
			cout << "error";
		}
		else {
			string decrypted;
			for (int i = 0; i < password_.size(); i++) {
				decrypted.push_back(password_[i] + 9);
			}
			fs << login_ << " " << decrypted << "\n";
		}
	}
	virtual void print() const {
		cout << "login_: " << login_ << " password_: " << password_ << "\n";
	}
	//>
	bool operator> (const guest& other) {
		return this->login_ > other.login_;
	}
	//<
	bool operator< (const guest& other) {
		return !(*this > other);
	}
	//==
	bool operator== (const guest& other) {
		return this->login_ == other.login_;
	}
	//!=
	bool operator!= (const guest& other) {
		return this->login_ != other.login_;
	}

};
void SplitFilename2(string& str)
{
	size_t found1 = str.find_first_of("\\");
	str = str.substr(found1 + 1);
	size_t found2 = str.find_first_of("_");
	str = str.substr(0, found2);
}
class user :public guest {
	string name;
	string surname;
	int phone;
	map<string, int> mark;  // 12 бальна система
	map<string, int> position;//позиц≥€ на €к≥й юзер зупинив тест
	map<string, int> correct_count;//к≥льк≥сть правильних в≥дпов≥дей
	map<string, int> percent;//в≥дсоток правильних в≥дпов≥дей
	friend ostream& operator<< (ostream& os, user& obj);
public:
	user() {
		name = "no";
		surname = "no";
		phone = 0;
		mark = {};
		position = {};
		percent = {};
		correct_count = {};
	}
	void setName(string name) {
		this->name = name;
	}
	void setSurname(string surname) {
		this->surname = surname;
	}
	void setPhone(int phone) {
		this->phone = phone;
	}
	void setPosition(string test_name, int pos) {
		this->position.insert_or_assign(test_name, pos);
	}
	void setMark(string test_name, int question_count) {
		int mark_t = 12 * correct_count.at(test_name) / question_count;
		this->mark.insert_or_assign(test_name, mark_t);
	}
	void setPercent(string test_name, int question_count) {
		int percent_t = 100 * correct_count.at(test_name) / question_count;
		this->percent.insert_or_assign(test_name, percent_t);
	}
	void setCorrect_count(string test_name, int correct) {
		this->correct_count.insert_or_assign(test_name, correct);
	}
	string getName() const {
		return name;
	}
	string getSurname() const {
		return surname;
	}
	int getPhone() const {
		return phone;
	}
	map <string, int> getCurrentPosition() {
		return position;
	}
	int getPosition(string str) {
		auto it = position.find(str);
		if (it == position.end())
			return -1;
		else
			return it->second;
	}
	int getCorrectCount(string str) {
		auto it = correct_count.find(str);
		if (it == correct_count.end())
			return -1;
		else
			return it->second;
	}
	int getMark(string str) {
		auto it = mark.find(str);
		return it->second;
	}
	int getPercent(string str) {
		auto it = percent.find(str);
		return it->second;
	}
	void readUserFromFile(fstream& fs) override {
		if (!fs.is_open()) {
			cout << "error";
		}
		else {
			guest::readUserFromFile(fs);
			fs >> name;
			fs >> surname;
			fs >> phone;
		}
	}
	void writeUserToFile(fstream& fs) override {
		if (!fs.is_open()) {
			cout << "error";
		}
		else {
			guest::writeUserToFile(fs);
			fs << name << " " << surname << " " << phone << "\n";
		}
	}
	void readUserStatisticsFile(fstream& fs) {
		if (!fs.is_open()) {
			cout << "Statistic file is absent, creating new file\n";
		}
		else {
			string login_;
			string str;
			pair<string, int> temp_mark;
			pair<string, int> temp_position;
			pair<string, int> temp_count;
			pair<string, int> temp_percent;
			fs >> login_;
			this->setLogin(login_);
			fs >> this->name;
			fs >> this->surname;
			fs >> this->phone;
			fs >> temp_mark.first;
			fs >> temp_mark.second;
			mark.insert(temp_mark);
			fs >> temp_position.first;
			fs >> temp_position.second;
			position.insert(temp_position);
			fs >> temp_count.first;
			fs >> temp_count.second;
			correct_count.insert(temp_count);
			fs >> temp_percent.first;
			fs >> temp_percent.second;
			percent.insert(temp_percent);
		}
	}
	void writeStatisticsToFile(fstream& fs) {
		if (!fs.is_open()) {
			cout << "error";
		}
		else {
			fs << guest::getLogin() << " " << name << " " << surname << " " << phone << "\n";
			cout << "Marks:\n";
			for (auto it = mark.begin(); it != mark.end(); it++)
				fs << it->first << " " << it->second << "\n";
			for (auto it = position.begin(); it != position.end(); it++)
				fs << it->first << " " << it->second << "\n";
			for (auto it = correct_count.begin(); it != correct_count.end(); it++)
				fs << it->first << " " << it->second << "\n";
			cout << "Current percent of correct answers: \n";
			for (auto it = percent.begin(); it != percent.end(); it++)
				fs << it->first << " " << it->second << "\n";
		}
	}
	void writeStatisticsToFile2(fstream& fs) {
		if (!fs.is_open()) {
			cout << "error";
		}
		else {
			fs << guest::getLogin() << " " << name << " " << surname << " " << phone << "\n";
			fs << "Marks:\n";
			for (auto it = mark.begin(); it != mark.end(); it++)
				fs << it->first << " | " << it->second << "\n";
			fs << "Current percent of correct answers: \n";
			for (auto it = percent.begin(); it != percent.end(); it++)
				fs << it->first << " | " << it->second << "\n";
		}
	}
	void ShowTestResult(vector<user>& statistics) {
		vector<string> files;
		fstream fs;
		for (auto& e : fst::directory_iterator("Statistics")) {
			files.push_back(e.path().string());
		}
		vector<string> logins;
		logins = files;
		for (size_t i = 0; i < files.size(); i++)
		{
			SplitFilename2(logins[i]);
		}
		for (size_t i = 0; i < files.size(); i++)
		{
			if (this->getLogin() == logins[i]) {
				while (i < files.size() && this->getLogin() == logins[i]) {
					fs.open(files[i], fstream::in);
					this->readUserStatisticsFile(fs);
					fs.close();
					i++;
				}
				statistics.push_back(*this);
			}
		}
		if (!statistics.empty())
			for (size_t i = 0; i < statistics.size(); i++)
			{
				cout << statistics[i] << "\n";
			}
		else cout << "No statistics file found!\n";

		statistics.erase(statistics.begin(), statistics.end());
	}
	void print() const override {
		cout << guest::getLogin() << "\n";
		cout << "Name: " << name << " Surname: " << surname << " phone: " << phone << "\n";
		cout << endl;
	}
};
class  admin :public guest {
public:
	admin() = default;
	admin(const string login_, const string password_) : guest(login_, password_) {}
};
//cout
ostream& operator<< (ostream& os, user& obj) {
	os << "Login: " << obj.getLogin() << "\n";
	os << "Name: " << obj.getName() << " | Surname: " << obj.getSurname() << " | phone: " << obj.getPhone() << "\n";
	cout << "Your marks:\n";
	for (auto el : obj.mark)
	{
		os << setw(10) << left << el.first << " | " << el.second << "\n";
	}
	cout << "Quantity of correct answers:\n";
	for (auto el : obj.correct_count)
	{
		os << setw(10) << left << el.first << " | " << el.second << "\n";
	}
	os << "Current percent of correct answers: \n";
	for (auto el : obj.percent)
	{
		os << setw(10) << left << el.first << " | " << el.second << "%" << endl;
	}
	return os;
}

class database {
	vector<user> datas;
	static int count;
public:
	static int getCount() { return count; }
	void SaveToFile(fstream& fs) {
		for (unsigned int i = 0; i < datas.size(); i++)
		{
			datas[i].writeUserToFile(fs);
		}
	}
	void SaveStatisticToFile(fstream& fs) {
		for (unsigned int i = 0; i < datas.size(); i++)
		{
			datas[i].writeStatisticsToFile(fs);
		}
	}
	void addUser(user u) {
		datas.push_back(u);
		count++;
	}
	int searchPosition(string login) {
		int pos = 0;
		for (unsigned int i = 0; i < datas.size(); i++)
		{
			if (datas[i].getLogin() == login) {
				pos = i;
				return pos;
			}
		}
		return -1;
	}
	user deleteByLogin(string login) {
		user temp;
		int	pos = searchPosition(login);
		if (pos != -1) {
			temp = datas[pos];
			datas.erase(datas.begin() + pos);
			count--;
			return temp;
		}
		else
			throw exception("Not found!\n");
	}
	//¬ар≥ант2 видаленн€ юзера
	void deleteUser(string login_delete) {
		auto it = remove_if(datas.begin(), datas.end(), [login_delete](user c) {
			return c.getLogin() == login_delete;
			});
		if (it == datas.end())
			cout << "\Login " << login_delete << " not found";
		else {
			cout << "\nUser login " << login_delete << " deleted";
			datas.erase(it, datas.end());
			count--;
		}
	}
	user searchByLogin(string login) {
		for (unsigned int i = 0; i < datas.size(); i++)
		{
			if (datas[i].getLogin() == login)
				return datas[i];
		}
		throw exception("Not found!\n");
	}
	void editPasswordByLogin(string login) {
		int	pos = searchPosition(login);
		if (pos != -1) {
			string passwordnew;
			cout << "Enter new password: ";
			cin >> passwordnew;
			datas[pos].setPassword(passwordnew);
			cout << "You have edited password, it can be used during next loging in:\n";
		}
		else cout << "User not found! Please register!\n";
	}
	void Sortup() {
		for (unsigned int i = 0; i < datas.size(); i++)
			for (unsigned int j = 0; j < datas.size() - 1 - i; j++)
				if (datas[j] > datas[j + 1])//€кщо елементи всередин≥ в≥дпов≥дають умов≥
					swap(datas[j], datas[j + 1]); // то м≥н€Їмо адреси цих елемент≥в м≥сц€ми
	}
	void Sortdown() {
		for (unsigned int i = 0; i < datas.size(); i++)
			for (unsigned int j = 0; j < datas.size() - 1 - i; j++)
				if (datas[j] < datas[j + 1])//€кщо елементи всередин≥ в≥дпов≥дають умов≥
					swap(datas[j], datas[j + 1]); // то м≥н€Їмо адреси цих елемент≥в м≥сц€ми
	}
	void clearDataBase() {
		datas.clear();
		count = 0;
	}
	void print() {
		for (unsigned int i = 0; i < datas.size(); i++)
		{
			datas[i].print();
		}
		cout << datas.size();
	}
	void loadUserFromFile(fstream& fs) {
		if (!fs.is_open()) {
			cout << "\nError open file\n";
		}
		else {
			while (!fs.eof()) {
				user temp;
				temp.readUserFromFile(fs);
				if (temp.getLogin() != "") {
					datas.push_back(temp);
					//count++;
				}
			}
		}
	}
	void loadUserStatisticFromFile(fstream& fs) {
		if (!fs.is_open()) {
			cout << "\nError open file\n";
		}
		else {
			while (!fs.eof()) {
				user temp;
				temp.readUserStatisticsFile(fs);
				if (temp.getLogin() != "" && temp.getLogin() != "\n" && temp.getLogin() != "\0") {
					datas.push_back(temp);
					count++;
				}
				else break;
			}
		}
	}
};
int database::count = 0;

class question {
	string body;
	string a;
	string b;
	string c;
	string d;
	char correct;
public:
	question() = default;
	question(string body, string a, string b, string c, string d, char correct) :
		body(body), a(a), b(b), c(c), d(d), correct(correct) {}
	void setQuestion(int number) {
		setBody(number);
		setA();
		setB();
		setC();
		setD();
		setCorrect();
	}
	void setBody(int number) {
		cout << "Enter question: " << "\n";
		getline(cin, body);
		body = "Question " + to_string(number) + ". " + body;
		this->body = body;
	}
	void setA() {
		cout << "Enter answer \"a\" option: " << "\n";
		getline(cin, a);
		this->a = a;
	}
	void setB() {
		cout << "Enter answer \"b\" option: " << "\n";
		getline(cin, b);
		this->b = b;
	}
	void setC() {
		cout << "Enter answer \"c\" option: " << "\n";
		getline(cin, c);
		this->c = c;
	}
	void setD() {
		cout << "Enter answer \"d\" option: " << "\n";
		getline(cin, d);
		this->d = d;
	}
	void setCorrect() {
		cout << "Enter letter of the correct answer( \"a\", \"b\", \"b\", \"d\"): " << "\n";
		cin.get(correct);
		cin.ignore();
		this->correct = correct;
	}
	string getBody() { return body; }
	char getCorrect() { return correct; }
	void readQuestionFromFile(fstream& fs) {
		if (!fs.is_open()) {
			cout << "error";
		}
		else {
			getline(fs, this->body);
			getline(fs, this->a);
			getline(fs, this->b);
			getline(fs, this->c);
			getline(fs, this->d);
			fs.get(this->correct);
			fs.ignore();
		}
	}
	void writeQuestionToFile(fstream& fs) {
		if (!fs.is_open()) {
			cout << "error";
		}
		else {
			fs << body << "\n";
			fs << a << "\n";
			fs << b << "\n";
			fs << c << "\n";
			fs << d << "\n";
			fs << correct << "\n";
		}
	}
	void print() const {
		cout << "body: " << body << "\n";
		cout << "a. " << a << "\n";
		cout << "b. " << b << "\n";
		cout << "c. " << c << "\n";
		cout << "d. " << d << "\n";
	}
};

class test {
	vector<question> data;
	string category;
	string testname;
	int count_test;
	int count_categories;
public:
	test() = default;
	test(string category, string testname) {
		this->category = category;
		this->testname = testname;
		this->count_categories = count_categories;
		count_test++;
	};
	void setCategory(string category) {
		this->category = category;
		count_categories++;
	}
	void setTestname(string testname) {
		this->testname = testname;
	}
	void setCount_categories(int count_categories) {
		this->count_categories = count_categories;
	}
	int getCount_test() { return count_test; }
	string getCategory() { return category; }
	string getTestname() { return testname; }
	void SaveToFile(fstream& fs) {
		for (unsigned int i = 0; i < data.size(); i++)
		{
			data[i].writeQuestionToFile(fs);
		}
	}
	void addQuestion(question q) {
		data.push_back(q);
		count_test++;
	}
	int searchPosition(string temp) {
		int pos = 0;

		for (int i = 0; i < data.size(); i++)
		{
			pos = data[i].getBody().find(temp, i);
			if (pos != -1) {
				cout << pos << " ";
				pos = i;
				return pos;
			}
		}
		return -1;
	}
	question searchByPosition(int pos) {
		question temp;
		if (pos != -1) {
			temp = data[pos];
			return temp;
		}
		else
			throw exception("Not found!\n");
	}
	question deleteByPosition(int pos) {
		question temp;
		if (pos != -1) {
			temp = data[pos];
			data.erase(data.begin() + pos);
			count_test--;
			return temp;
		}
		else
			throw exception("Not found!\n");
	}
	void  editByPosition(int pos) {
		if (pos != -1) {
			data[pos].setQuestion(pos + 1);
			cout << "You have added following question:\n";
			data[pos].print();
		}
		else cout << "Question not found!\n";
	}
	void clearTest() {
		data.clear();
		count_test = 0;
	}
	void print() const {
		for (unsigned int i = 0; i < data.size(); i++)
		{
			data[i].print();
		}
	}
	void loadTestFromFile(fstream& fs) {
		if (!fs.is_open()) {
			cout << "\nFile doesn't exist. Creating new file.\n";
		}
		else {
			while (!fs.eof()) {
				question temp;
				temp.readQuestionFromFile(fs);
				if (temp.getBody() != "") {
					data.push_back(temp);
					count_test++;
				}
			}
		}
	}
};

void SplitFilename(string& str)
{
	size_t found1 = str.find_first_of("\\");
	str = str.substr(found1 + 1);
}
string FindFileName(string entry) {
	string normal;
	if (!entry.empty()) {
		for (unsigned int i = 0; i < entry.size() - 3; i++)
		{
			if (entry[i] == '\\') {
				i++;
				while (entry[i] != '.' && i < entry.size()) {
					normal.push_back(entry[i]);
					i++;
				}
			}
		}
		return normal;
	}
	throw exception("not found\n");
}
void Test_Loading(string category, string test_name, user current) {
	fstream fs;
	test test_now;
	int correct_count = 0;
	user temp;
	test_now.setCategory(category);
	test_now.setTestname(test_name);
	const int size = test_now.getCount_test();
	char* reply = new char[size + 1];
	string user_statistic_directory = "Statistics";
	fst::create_directories(user_statistic_directory);
	string user_statistic = user_statistic_directory + "\\" + current.getLogin() + "_" + test_name + ".txt";
	fs.open(user_statistic, fstream::in);
	temp.readUserStatisticsFile(fs);
	fs.close();
	if (temp.getLogin() == current.getLogin()) {
		current = temp;
		correct_count = current.getCorrectCount(test_name);
		//reply = current.getReply();
	}
	string test_file = "Test\\" + category + "\\" + test_now.getTestname() + ".txt";
	fs.open(test_file, fstream::in);
	test_now.loadTestFromFile(fs);
	fs.close();
	char go_on;
	int lastposition = 0;
	if (current.getPosition(test_name) != -1)
		lastposition = current.getPosition(test_name);
	for (size_t i = lastposition; i < test_now.getCount_test(); ++i)
	{
		test_now.searchByPosition(i).print();
		cout << "Enter letter of your answer( \"a\", \"b\", \"b\", \"d\"): " << "\n";
		cin >> reply[i];
		cin.ignore();
		if (test_now.searchByPosition(i).getCorrect() == reply[i])
			correct_count++;
		cout << "If you want to continue test, enter \"y\"\n";
		cout << "If you want to interrupt test and save your data, enter \"n\"\n";
		cin >> go_on;
		if (go_on == 'n') {
			current.setPosition(test_now.getTestname(), i + 1);
			break;
		}
		else {
			current.setPosition(test_now.getTestname(), i + 1);
		}
	}
	current.setCorrect_count(test_now.getTestname(), correct_count);
	current.setMark(test_now.getTestname(), test_now.getCount_test());
	current.setPercent(test_now.getTestname(), test_now.getCount_test());
	cout << current;
	fs.open(user_statistic, fstream::out);
	current.writeStatisticsToFile(fs);
	fs.close();
	fs.clear();
}
void ReadAllFromDirectory(vector<test>& test_list) {
	vector<string> files_category;
	for (auto& e : fst::directory_iterator("Test")) {
		files_category.push_back(e.path().string());
	}
	test* temp = new test[50];
	vector<string> files_tests;
	for (size_t i = 0; i < files_category.size(); i++)
	{
		for (auto& e : fst::directory_iterator(files_category[i])) {
			files_tests.push_back(e.path().string());
		}
	}
	for (size_t i = 0; i < files_tests.size(); i++)
	{
		string str2(files_tests[i]);
		SplitFilename(str2);
		str2 = FindFileName(str2);
		temp[i].setTestname(str2);
		int a = 0;
		while (a < files_category.size()) {
			size_t found = files_tests[i].find(files_category[a]);
			if (found != std::string::npos) {
				string str1(files_category[a]);
				SplitFilename(str1);
				temp[i].setCategory(str1);
				test_list.push_back(temp[i]);
			}
			a++;
		}
	}
}
void ShowAllTests(vector<test>& test_list) {
	string category_temp;
	auto it = test_list.begin();
	category_temp = it->getCategory();
	int count_test = 2;
	for (size_t i = 0; i < test_list.size(); )
	{
		cout << "Category: " << it->getCategory() << "\n";
		while (it != test_list.end() && it->getCategory() == category_temp) {
			cout << "   Test " << count_test << ". " << it->getTestname() << "\n";
			count_test++;
			it++;
			i++;
		};
		if (it != test_list.end())
			category_temp = it->getCategory();
	}
}

int main() {
	setlocale(LC_ALL, "Ukrainian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	user current;//поточний користувач
	fstream fs;
	user g;// не зареЇстрований користувач
	admin superadmin;
	string adminlogin;
	string adminpassword;
	test test_now;
	database b;
	vector<user> statistics; // вектор з результатами тест≥в вс≥х користувач≥в
	_mkdir("Statistics");
	_mkdir("Users");
	_mkdir("Admin");
	string path1 = "Test";
	string path2 = "Test\\Travel";
	string path3 = "Test\\Natural_Science";
	fst::create_directories(path1);
	fst::create_directories(path2);
	fst::create_directories(path3);
	map<string, int> m;
	int answer = 0;
	char choice;

	//¬иб≥р режиму доступу: јдм≥н чи користувач
	do {
		system("cls");
		cout << endl << string(100, '*');
		cout << "\nWelcome to our Testing system!\n";
		cout << "Please choose your type of entry:\n";
		cout << "1- administrators login\n";
		cout << "2- user login\n";
		cout << "0- Exit\n";
		cout << endl << string(100, '*') << "\n";
		cin >> answer;
		cin.ignore();
		switch (answer) {
		case 1: {
			string adminlogin_, adminpassword_;
			fs.open("Admin\\admin.txt", fstream::in);
			if (!fs.is_open()) {
			}
			else
				superadmin.readUserFromFile(fs);
			fs.close();
			if (superadmin.getLogin() == "") {
				cout << "Welcome to administrator panel, please enter admin login for the first entry: ";
				cin >> adminlogin;
				superadmin.setLogin(adminlogin);
				cout << "Enter admin password: ";
				cin >> adminpassword;
				superadmin.setPassword(adminpassword);
				fs.open("Admin\\admin.txt", fstream::out);
				superadmin.writeUserToFile(fs);
				fs.close();
			}
			else {
				fs.open("Admin\\admin.txt", fstream::in);
				superadmin.readUserFromFile(fs);
				fs.close();
				cout << "Welcome to administrator panel!\n";
				int index = -1;
				cout << "Enter your login: ";
				cin >> adminlogin_;
				if (adminlogin_ == superadmin.getLogin()) {
					int mistakes = 0;
					do {
						cout << "Enter your password: ";
						cin >> adminpassword_;
						if (adminpassword_ == superadmin.getPassword()) {
							cout << "Logging you in...\n";
							//€кщо входить адм≥н, то в≥дкриваЇтьс€ панель адм≥на
							system("pause");
							do {
								system("cls");
								cout << endl << string(100, '*');
								cout << "\nAdministrator panel!";
								cout << "\nPlease choose what to do:" << endl;
								cout << "1 - Open user data base\n";
								cout << "2 - Show list of all tests\n";
								cout << "3 - Create new user account\n";
								cout << "4 - Edit existing user account\n";
								cout << "5 - Delete user\n";
								cout << "6 - Clear user data base\n";
								cout << "7 - View statistics\n";
								cout << "8 - Save statistics to file\n";
								cout << "9 - Add test and categories\n";
								cout << "10 - Delete category\n";
								cout << "11 - Delete test\n";
								cout << "12 - Change admin password and login\n";
								cout << "0 - Exit" << endl;
								cout << endl << string(100, '*');
								cout << "\nMake your choice, please: ";
								cin >> answer;
								cin.ignore();
								if (answer) {
									switch (answer) {
									case 1: {
										cout << "1 - Open user data base\n";
										b.clearDataBase();
										fs.open("Users\\user_data_base.txt", fstream::in);
										b.loadUserFromFile(fs);
										fs.close();
										b.print();
										cout << endl << "____________________________________________________________________________________" << endl;
										cout << "\nShow main menu? Enter \"y\", if yes and \"n\", if no.\n";
										cin >> choice;
										cin.ignore();
										break;
									}
									case 2: {
										cout << "2 - Show list of all tests\n";
										vector<test> test_list;
										ReadAllFromDirectory(test_list);
										ShowAllTests(test_list);
										cout << endl << "____________________________________________________________________________________" << endl;
										cout << "\nShow main menu? Enter \"y\", if yes and \"n\", if no.\n";
										cin >> choice;
										cin.ignore();
										break;
									}
									case 3: {
										cout << "3 - Create new user account\n";
										cout << "Enter new user login: ";
										string login_, password_;
										int index = 0;
										cin >> login_;
										index = b.searchPosition(login_);
										if (index == -1) {
											cout << "Enter password: ";
											cin >> password_;
											user temp;
											temp.setLogin(login_);
											temp.setPassword(password_);
											b.addUser(temp);
											cout << "User " << login_ << " have been created!\n";
											fs.open("Users\\user_data_base.txt", fstream::out);
											b.SaveToFile(fs);
											fs.close();
										}
										else cout << "User already exists, please choose another login.\n";
										//system("pause");
										cout << endl << "____________________________________________________________________________________" << endl;
										cout << "\nShow main menu? Enter \"y\", if yes and \"n\", if no.\n";
										cin >> choice;
										cin.ignore();
										break;
									}
									case 4: {
										cout << "4 - Edit existing user account\n";
										string login_;
										cout << "Enter login for editing: ";
										cin >> login_;
										try {
											b.editPasswordByLogin(login_);
											fs.open("Users\\user_data_base.txt", fstream::out);
											b.SaveToFile(fs);
											fs.close();
										}
										catch (const std::exception& ex)
										{
											cout << ex.what();
											system("pause");
										}
										cout << endl << "____________________________________________________________________________________" << endl;
										cout << "\nShow main menu? Enter \"y\", if yes and \"n\", if no.\n";
										cin >> choice;
										cin.ignore();
										break;
									}
									case 5: {
										cout << "5 - Delete user\n";
										string login_;
										cout << "Enter login to delete: ";
										cin >> login_;
										b.deleteUser(login_);
										fs.open("Users\\user_data_base.txt", fstream::out);
										b.SaveToFile(fs);
										fs.close();
										cout << endl << "____________________________________________________________________________________" << endl;
										cout << "\nShow main menu? Enter \"y\", if yes and \"n\", if no.\n";
										cin >> choice;
										cin.ignore();
										break;
									}
									case 6: {
										cout << "6 - Clear user data base\n";
										b.clearDataBase();
										cout << endl << "____________________________________________________________________________________" << endl;
										cout << "\nShow main menu? Enter \"y\", if yes and \"n\", if no.\n";
										cin >> choice;
										cin.ignore();
										break;
									}
									case 7: {
										cout << "7 - View statistics\n";
										statistics.erase(begin(statistics), end(statistics));
										vector<string> files;
										for (auto& e : fst::directory_iterator("Statistics")) {
											files.push_back(e.path().string());
										}
										vector<string> logins;
										logins = files;
										for (size_t i = 0; i < files.size(); i++)
										{
											SplitFilename2(logins[i]);
										}
										user* temp = new user[50];
										int index = 0;
										for (size_t i = 0; i < files.size(); )
										{
											temp[index].setLogin(logins[i]);
											while (i < files.size() && temp[index].getLogin() == logins[i]) {
												fs.open(files[i], fstream::in);
												temp[index].readUserStatisticsFile(fs);
												fs.close();
												i++;
											}
											statistics.push_back(temp[index]);
											index++;
										}
										delete[] temp;
										if (!statistics.empty())
											for (size_t i = 0; i < statistics.size(); i++)
											{
												cout << statistics[i] << "\n";
											}
										else cout << "No statistics file found!\n";
										cout << endl << "____________________________________________________________________________________" << endl;
										cout << "\nShow main menu? Enter \"y\", if yes and \"n\", if no.\n";
										cin >> choice;
										cin.ignore();
										break;
									}
									case 8: {
										cout << "8 - Save statistics to file\n";
										fst::remove("total_statistics.txt");
										for (size_t i = 0; i < statistics.size(); i++)
										{
											fs.open("total_statistics.txt", fstream::out | fstream::app);
											statistics[i].writeStatisticsToFile2(fs);
											fs.close();
										}
										cout << endl << "____________________________________________________________________________________" << endl;
										cout << "\nShow main menu? Enter \"y\", if yes and \"n\", if no.\n";
										cin >> choice;
										cin.ignore();
										break;
									}
									case 9: {
										cout << "9 - Add test and categories\n";
										test new_test;
										string path;
										string new_test_name;
										question new_question;
										string new_question_body;
										string new_question_answerA;
										string new_question_correct_answer;
										cout << "Please enter category name to add or make changes to existing:\n";
										cin >> path;
										new_test.setCategory(path);
										path = "Test//" + path;
										fst::create_directories(path);
										cout << "Please enter new test name or existing test name for editing:\n";
										cin >> new_test_name;
										new_test.setTestname(new_test_name);
										string path_test = path + "\\" + new_test.getTestname() + ".txt";
										test temp;
										fs.open(path_test, fstream::in);
										temp.loadTestFromFile(fs);
										fs.close();
										if (temp.getCount_test())
											cout << "Current test already have " << temp.getCount_test() << " questions\n";
										int new_question_count;
										cout << "Please enter the quantity of questions you are going to add:\n";
										cin >> new_question_count;
										cin.ignore();
										int q_in_file = 0;
										if (temp.getCount_test())
											q_in_file = temp.getCount_test();

										for (size_t i = q_in_file; i < new_question_count + q_in_file; i++)
										{
											new_question.setQuestion(i + 1);
											new_test.addQuestion(new_question);
										}
										fs.open(path_test, fstream::out | fstream::app);
										new_test.SaveToFile(fs);
										fs.close();
										cout << "You have added following test, its saved in file:\n";
										new_test.print();
										cout << endl << "____________________________________________________________________________________" << endl;
										cout << "\nShow main menu? Enter \"y\", if yes and \"n\", if no.\n";
										cin >> choice;
										cin.ignore();
										break;
									}
									case 10: {
										cout << "10 - Delete category\n";
										vector<test> test_list;
										ReadAllFromDirectory(test_list);
										ShowAllTests(test_list);
										string path;
										cout << "Please enter category name to delete, be aware that all tests in this category also will be deleted!:\n";
										cin >> path;
										path = "Test\\" + path;
										try {
											uintmax_t n = fst::remove_all(path);
											cout << "Deleted " << n << " files or directories\n";
										}
										catch (exception ex) {
											cout << ex.what() << "\n";
										}
										cout << endl << "____________________________________________________________________________________" << endl;
										cout << "\nShow main menu? Enter \"y\", if yes and \"n\", if no.\n";
										cin >> choice;
										cin.ignore();
										break;
									}
									case 11: {
										cout << "11 - Delete test\n";
										vector<test> test_list;
										ReadAllFromDirectory(test_list);
										ShowAllTests(test_list);
										string path;
										string test_name;
										cout << "Please enter category name, in which test should be deleted:\n";
										cin >> path;
										cout << "Please enter test name to delete:\n";
										cin >> test_name;
										path = "Test\\" + path + "\\" + test_name + ".txt";
										try {
											uintmax_t n = fst::remove(path);
											cout << "Deleted " << n << " files or directories\n";
										}
										catch (exception ex) {
											cout << ex.what() << "\n";
										}
										cout << endl << "____________________________________________________________________________________" << endl;
										cout << "\nShow main menu? Enter \"y\", if yes and \"n\", if no.\n";
										cin >> choice;
										cin.ignore();
										break;
									}
									case 12: {
										cout << "10 - Change admin password and login\n";
										cout << "Enter new admin login: ";
										cin >> adminlogin;
										superadmin.setLogin(adminlogin);
										cout << "Enter new admin password: ";
										cin >> adminpassword;
										superadmin.setPassword(adminpassword);
										fs.open("Admin\\admin.txt", fstream::out);
										superadmin.writeUserToFile(fs);
										fs.close();
										cout << endl << "____________________________________________________________________________________" << endl;
										cout << "\nShow main menu? Enter \"y\", if yes and \"n\", if no.\n";
										cin >> choice;
										cin.ignore();
										break;
									}
									default:
										if (answer > 12 || answer < 0)
										{
											cout << "Wrong choice! Try again!\n";
											system("pause");
											//break;
										}
										choice = 'y';
										break;
									}
								}
								else {
									cout << "Thank you for visiting!\n";
									break;
								}
							} while (choice != 'n');
							answer = 0;
							break;
						}
						else {
							mistakes++;
							cout << "Password is incorrect, try again\n";
						}
					} while (mistakes < 5);
					if (mistakes == 5) {
						cout << "You have made 5 mistakes, please cool down and come back later\n";
						system("pause");
					}
				}
				else {
					cout << "Wrong login\n";
					//break;
				}
			}
			system("pause");
			break;
		}
		case 2: {
			fs.open("Users\\user_data_base.txt", fstream::in);
			b.loadUserFromFile(fs);
			fs.close();
			do {
				system("cls");
				cout << endl << string(100, '*');
				cout << "\nIf you already have an account, please log in, if not - please register!\n";
				cout << "Please make your choice:\n";
				cout << "1- to log in with existing account\n";
				cout << "2- to register new account\n";
				cout << "0- Exit\n";
				cout << endl << string(100, '*') << "\n";
				cin >> answer;
				cin.ignore();
				switch (answer) {
				case 1: {
					int index = -1;
					string login_, password_;
					cout << "Enter your login: ";
					cin >> login_;
					try {
						index = b.searchPosition(login_);
					}
					catch (const std::exception& ex)
					{
						cout << ex.what();
						system("pause");
					}
					if (index != -1) {
						int mistakes = 0;
						do {
							cout << "Enter your password: ";
							cin >> password_;
							g = b.searchByLogin(login_);
							if (password_ == g.getPassword()) {
								cout << "Logging you in...\n";
								current = g;
								answer = 0;
								break;
							}
							else {
								mistakes++;
								cout << "Password is incorrect, try again\n";
							}
						} while (mistakes < 5);
						if (mistakes == 5) {
							cout << "You have made 5 mistakes, please cool down and come back later\n";
							system("pause");
						}
					}
					else {
						cout << "User not found!\n";
						system("pause");
					}
					break;
				}
				case 2: {
					cout << "Enter your login for registration: ";
					string login_, password_, name_, surname_;
					int phone_;
					int index = 0;
					cin >> login_;
					index = b.searchPosition(login_);
					if (index == -1) {
						cout << "Enter your password: ";
						cin >> password_;
						cout << "Enter your name: ";
						cin >> name_;
						cout << "Enter your surname: ";
						cin >> surname_;
						cout << "Enter your phone: ";
						cin >> phone_;
						user temp;
						temp.setLogin(login_);
						temp.setPassword(password_);
						temp.setName(name_);
						temp.setSurname(surname_);
						temp.setPhone(phone_);
						b.addUser(temp);
						cout << "User " << login_ << " have been registered, now you can log in!\n";
					}
					else cout << "User already exists, please choose another login.\n";
					system("pause");
					break;
				}

				default: {
					if (answer > 3 || answer < 0)
					{
						cout << " Enter correct choice! \n";
						system("pause");
						//break;
					}
				}
				}
			} while (answer != 0);
			fs.open("Users\\user_data_base.txt", fstream::out);
			b.SaveToFile(fs);
			fs.close();
			answer = 0;
			break;
		}
		default: {
			if (answer > 3 || answer < 0)
			{
				cout << " Enter correct choice! \n";
				system("pause");
				//break;
			}
		}
		}
	} while (answer != 0);

	// якщо входить користувач, то в≥дкриваЇтьс€ панель користувача ≥ тестов≥ завданн€

	if (!current.getLogin().empty()) {
		vector<test> test_list;
		ReadAllFromDirectory(test_list);
		do {
			cout << endl << string(100, '*');
			cout << "\nChoose what to do:" << endl;
			cout << "\n1 - Show test results\n\n";
			cout << "Enter test  number to pass:\n";
			ShowAllTests(test_list);
			cout << "0  - Exit" << endl;
			cout << endl << string(100, '*');
			cout << "\nMake your choice, please: ";
			cin >> answer;
			cin.ignore();
			if (answer) {
				switch (answer) {
				case 1: {
					statistics.erase(begin(statistics), end(statistics));
					current.ShowTestResult(statistics);
					cout << endl << "____________________________________________________________________________________" << endl;
					cout << "\nShow main menu? Enter \"y\", if yes and \"n\", if no.\n";
					cin >> choice;
					cin.ignore();
					break;
				}
				default: {
					if (isdigit(answer) == 0) {
						int index = answer - 2;
						cout << "You have chosen " << test_list[index].getTestname() << "\n";
						string category = test_list[index].getCategory();
						string test_name = test_list[index].getTestname();
						Test_Loading(category, test_name, current);
						cout << endl << "____________________________________________________________________________________" << endl;
						cout << "\nGo on? Enter \"y\", if yes and \"n\", to interapt test.\n";
						cin >> choice;
						cin.ignore();
						break;
					}
					else {
						cout << "Wrong choice! Try again!\n";
						choice = 'y';
						break;
					}
				}
				}
			}
			else {
				cout << "Thank you for visiting!\n";
				break;
			}
		} while (choice != 'n');
	}




	return 0;
}
