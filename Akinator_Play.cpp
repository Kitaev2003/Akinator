#include <iostream>
#include <fstream>
#include <cassert>
#include <cstring>

typedef struct Block
{
	char* data  = nullptr;
	
	Block* left =  nullptr;
	
	Block* right = nullptr;
	
	int num;
}Block_Tree;

class Akinator 
{
	public:
	Block_Tree* Tree_ = nullptr;
	size_t size_ = 0; 
	
	Akinator();
	~Akinator();
	
	void getDump();
	void setAkinator_Reader();
	void Insert(Block_Tree* elem, char* str);
	void Akinator_Make(Block_Tree* elem, std::ifstream* READ, int *OpenClose);
};

void getMainDump(Block_Tree* Tree, Block_Tree* Tree_Next, std::ofstream* GRAF);
void MakeStringForAkinator(char* str);
void setFree(Block_Tree*  Tree);

int main()
{
	Akinator play;
	play.setAkinator_Reader();
	play.getDump();

	return 0;
} 

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

void getMainDump(Block_Tree* Tree, Block_Tree* Tree_Next, std::ofstream* GRAF)
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
		getMainDump(Tree_Next, Tree_Next->left, GRAF);// Запускаем рекурсию
	}
	
	if(Tree_Next->right != nullptr)//Для предупреждения перехода по нулевому указателю
	{
		getMainDump(Tree_Next, Tree_Next->right, GRAF);//Запускаем рекурсию
	}
}

void Akinator::getDump()
{
	std::ofstream GRAF;//Обьявляем переменную типа fstream
	GRAF.open("Akinator.dot");

	GRAF << "digraph graphname \n {" << std::endl;// digtaph, так как нужно указывать конкретное направление перехода к блоку
	GRAF << "node [shape = note, color = \"red\"]; //Описание блоков графа"<< std::endl;

	GRAF << "	" << Tree_->num << "[fillcolor=darkolivegreen1, style=\"rounded,filled\", label=\"" << Tree_->data <<"\"];" << std::endl;

	getMainDump(Tree_, Tree_->left, &GRAF);//Левое дерево описание блоков
	getMainDump(Tree_, Tree_->right, &GRAF);//Правое дерево описание блоков	
	
	GRAF << "\n}" << std::endl;
	GRAF.close();//Закрываем файл
	
	system("dot -Tpng Akinator.dot -o Akinator");//Делаем PNG-Дерево
}

void Akinator::Insert(Block_Tree* elem, char* str)
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

void Akinator::setAkinator_Reader()
{
	std::ifstream READ;
	READ.open("Akinator_Order.txt");
	
	if (!READ.is_open()) // если файл не открыт
	{
        	std::cout << "Файл не может быть открыт!" << std::endl;
	}
	
	char* buffer = new char [100];
	char symbol[2];
	int OpenClose = 0;//OpenClose создан для контроля открывания и закрывания всех скобок
	READ >> buffer;
	Insert(Tree_, buffer);
	
	Akinator_Make(Tree_->left, &READ, &OpenClose);	
	
	if(OpenClose != 0)
	{
		std::cout << "Пропущена скобка в файле: \"Akinator_Order.txt\"" << std::endl;
	}
	READ >> symbol;
	
	Akinator_Make(Tree_->right, &READ, &OpenClose);
	
	if(OpenClose != 0)
	{
		std::cout << "Пропущена скобка в файле: \"Akinator_Order.txt\"" << std::endl;
	}
	
	READ.close();
}

void Akinator::Akinator_Make(Block_Tree* elem, std::ifstream* READ, int* OpenClose)
{
	char* buffer = new char [100];
	
	*READ >> buffer;
	
	size_t len = strlen(buffer);
	
	//Рассчет OpenClose
	if(buffer[0] == '{')
	{
		*OpenClose++;
	}
	if(buffer[len - 1] == '}' || buffer[0] == '}')
	{
		*OpenClose--;
	}
	
	//Заполняем дереево
	if(buffer[0] == '{' && buffer[len-1] != '}' && *OpenClose != 0)// Запускаем рекурсию для узла типа "{Fachit?"
	{
		Insert(elem, buffer);
	
		assert(elem->left);
		Akinator_Make(elem->left, READ, OpenClose);//рекурсия для правых
			
		assert(elem->right);
		Akinator_Make(elem->right, READ, OpenClose);//рекурсия для правых
	}
	
	if( ((buffer[0] == '{' && buffer[len-1] == '}') || *OpenClose == 0))//Добавлем в дерево ветви типа "{Umnov}"
	{
		Insert(elem, buffer);
	}
	
	if(buffer[0] == '}')//Учитываем подсчет скобки OpenClose 
	{//Но в тоже время запускаем рекурсию, так как узел "{Fachit?" спустя несколько строк примет скобку за узел
		Akinator_Make(elem, READ, OpenClose);
	}	
}
