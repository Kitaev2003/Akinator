#include "Akinator.h"

Akinator::Akinator()
{
	Tree_ = new Block_Tree;
	
	size_ = 0;
}

Akinator::~Akinator()
{
    setFree(Tree_);
}

void setFree(Block_Tree*  Tree)
{
 	if (Tree)
	{
		setFree(Tree->left);
		setFree(Tree->right);
		
		delete Tree;
	}
}

void MainDump(Block_Tree* Tree, Block_Tree* Tree_Next, std::ofstream* GRAF)
{
	if(Tree_Next->data != nullptr)//Для предупреждения перехода по нулевому указателю
	{	
		*GRAF << "	" << Tree_Next->num << "[fillcolor = darkolivegreen1, style=\"rounded,filled\", label=\"" << Tree_Next->data <<"\"];" << std::endl;
		
		if(Tree->left == Tree_Next)
		{
			*GRAF << "	"<< Tree->num << "->" << Tree_Next->num << "[label = \"Yes\"];" << std::endl;
		}
		
		else
		{
			*GRAF << "	"<< Tree->num << "->" << Tree_Next->num << "[label = \"No\"];" << std::endl;
		}
	}
	
	if(Tree_Next->left != nullptr)//Для предупреждения перехода по нулевому указателю
	{
		MainDump(Tree_Next, Tree_Next->left, GRAF);// Запускаем рекурсию
	}
	
	if(Tree_Next->right != nullptr)//Для предупреждения перехода по нулевому указателю
	{
		MainDump(Tree_Next, Tree_Next->right, GRAF);//Запускаем рекурсию
	}
}

void Akinator::getDump()
{
	std::ofstream GRAF;//Обьявляем переменную типа fstream
	GRAF.open("Akinator.dot");

	GRAF << "digraph graphname \n {" << std::endl;// digtaph, так как нужно указывать конкретное направление перехода к блоку
	GRAF << "node [shape = note, color = \"red\"]; //Описание блоков графа"<< std::endl;

	GRAF << "	" << Tree_->num << "[fillcolor=darkolivegreen1, style=\"rounded,filled\", label=\"" << Tree_->data <<"\"];" << std::endl;

	MainDump(Tree_, Tree_->left, &GRAF);//Левое дерево описание блоков
	MainDump(Tree_, Tree_->right, &GRAF);//Правое дерево описание блоков	
	
	GRAF << "\n}" << std::endl;
	GRAF.close();//Закрываем файл
	
	system("dot -Tpng Akinator.dot -o Akinator");//Делаем PNG-Дерево
}

void Akinator::setInsert(Block_Tree* elem, char* str)
{
	assert(elem); 
	assert(str);
	
	MakeStringForAkinator(str);
	
	elem->data = str;

	size_++;
	elem->num = size_;//for graphviz 
	

	elem->left = new Block_Tree;	
	elem->right = new Block_Tree;	
}

void MakeStringForAkinator(char* str)
{
	size_t i = 0;
	size_t len = strlen(str);
	
	if(str[0] == '{' || str[0] == '(' || str[0] == '[')
	{
		while(i < len)
		{
			str[i] = str[i + 1];
			i++;
		}
	} 
	
	if(str[len] == '}' || str[len] == ')' || str[len] == ']' || str[len-2] == '}' || str[len-2] == ')' || str[len-2] == ']')
	{
		str[len-2] ='\0';
	}
}

size_t StringDeleteTab(char* str, size_t len)
{
	size_t len_tab = 0;
	size_t i = 0;
	
	while(str[i] == 9)// 9 - TAB in the ASCII
	{
		len_tab++;
		i++;	
	}
	
	i = 0;
	
	while((i+len_tab) <= len)
	{
		str[i] = str[i+len_tab];
		i++;
	}
	
	return len - len_tab; 
}

void Akinator::setAkinator_Reader()
{
	std::ifstream READ;
	READ.open("Akinator_Order.txt");
	
	if (!READ.is_open()) // если файл не открыт
	{
        	std::cout << "Файл не может быть открыт!" << std::endl;
	}
	
	char* buffer = new char [100];
	
	READ.getline(buffer, 100, '\n');
	setInsert(Tree_, buffer);
	
	setAkinator_Make(Tree_->left, &READ);
	setAkinator_Make(Tree_->right, &READ);//
	
	READ.close();
}

void Akinator::setAkinator_Make(Block_Tree* elem, std::ifstream* READ)
{
	char* buffer = new char [100];
	
	READ -> getline(buffer, 100, '\n');//Считываем строку полностью
	
	size_t len = StringDeleteTab(buffer, strlen(buffer));// Удаляем пробелы перед текстом и возвращаем длину buffer после изменений

	//Заполняем дереево
	if(buffer[0] == '{' && buffer[len-1] != '}')// Запускаем рекурсию для узла типа "{Fachit?"
	{
		setInsert(elem, buffer);
	
		assert(elem->left);
		setAkinator_Make(elem->left, READ);//рекурсия для правых
			
		assert(elem->right);
		setAkinator_Make(elem->right, READ);//рекурсия для правых
	}
	
	if(buffer[0] == '{' && buffer[len-1] == '}')//Добавлем в дерево ветви типа "{Umnov}"
	{
		setInsert(elem, buffer);
	}
	
	if(buffer[0] == '}') 
	{//Но в тоже время запускаем рекурсию, так как узел "{Fachit?" спустя несколько строк примет скобку за узел
		setAkinator_Make(elem, READ);
	}
}
