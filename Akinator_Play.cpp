#include "Akinator.h"

void Akinator::Game()
{
	Block_Tree* tree = Tree_;
	char* answer = new char[100];
	bool yes;
	bool no;
	
	std::cout << "Вы начали игру Akinator основаному на бинарном дереве.\nЗагадайте персонажа, которого вы можете описать. Сейчас я его угадаю..." << std::endl;
	std::cout << "Отвечайте на вопросы с помощью \"Yes\" или \"No\"."<< std::endl;
	
	while(true)
	{
		std::cout << tree->data << std::endl;
		
		std::cin >> answer;
		
		if(CorrectScanStr(answer, &yes, &no) == false)
		{
			continue;
		}
		
		if(yes == true)
		{
			if(tree->left->right->data == nullptr && tree->left->left->data == nullptr)
			{
				std::cout << "Вы загадали: " << tree->left->data << ". Верно?" << std::endl;
				std::cin >> answer;
				
				if(CorrectScanStr(answer, &yes, &no) == false)
				{
					continue;
				}
				
				if(yes == true)
				{
					std::cout << "Я так и знал, ахаха, в следующий раз придумай что-то оргинальнее" << std::endl;
					break;
				}
				else
				{
					setInsertForGame(tree->left);
					break;
				} 		
			}
			else 
			{
				tree = tree->left;
				yes = false;
				no = false;
				continue;
			}
		} 
		
		else
		{
			if(tree->right->right->data == nullptr && tree->right->left->data == nullptr)
			{
				std::cout << "Вы загадали: " << tree->right->data << " Верно?" << std::endl;
				std::cin >> answer;
			
				if(CorrectScanStr(answer, &yes, &no) == false)
				{
					continue;
				}
				
				if(yes == true)
				{
					std::cout << "Я так и знал, ахаха, в следующий раз придумай что-то оргинальнее" << std::endl;
					break;
				}
				else
				{
					setInsertForGame(tree->right);	
					break;
				} 		
			}
			else 
			{
				tree = tree->right;
				yes = false;
				no = false;
				continue;
			}	
		}	
	}
	delete answer;
}

bool CorrectScanStr(char* str, bool* yes, bool* no)
{
		if(( strcmp(str, "No\0") * strcmp(str, "no\0"))  == 0)
		{
			*yes = false;
			*no = true;
			return true;
		}
		else if( (strcmp(str, "Yes\0") * strcmp(str, "yes\0")) == 0)
		{
			*yes = true;
			*no = false;
			return true;
		}
		else
		{
			std::cout << "Вы отвечаете неправильно, попробуйте ещё раз." << std::endl;
			return false;
		}
}

void Akinator::setInsertForGame(Block_Tree* tree)
{
		
	char* answer_no = new char[100];
	char* answer_yes = new char[100];
	char* questen = new char[100];
	
	std::cout << "Хорошо, вы меня переиграли, поздравляю Вас. Чтобы я стал лучше, помогите мне :-)" <<std::endl;
	std::cout << "Введите такой вопрос, чтобы ответ \"Yes\" указывал на вашего персонажа" << std::endl;

	size_t i = 0;
	char symbol;
	
	getchar();
	while( (symbol = getchar()) != '?')
	{
		questen[i] = symbol;
		i++;
	} 
	questen[i] = '?';
	questen[i+1] = '\0';
	i = 0;
	
	std::cout<< "Введите персонажа которого вы загадали?" << std::endl;
	
	getchar();
	while((symbol = getchar()) != '\n')
	{
		answer_yes[i] = symbol;
		i++;
	} 
	
	answer_yes[i] = '\0';
	
	
	strcpy(answer_no, tree->data);

	delete tree->data;
	
	tree->data = questen;
	
	setInsert(tree->right, answer_no);
	setInsert(tree->left, answer_yes);
		
	std::cout << "Спасибо за помощь" << std::endl;
}

void Akinator::getPrintInFile()
{
	std::ofstream OUT;
	OUT.open("Akinator_Order.txt");
	
	if (!OUT.is_open()) // если файл не открыт
	{
        	std::cout << "Файл не может быть открыт!" << std::endl;
	}
	
	OUT << "{" << Tree_->data << std::endl;
	
	DumpTxtFile(Tree_->left, &OUT);
	DumpTxtFile(Tree_->right, &OUT);
	
	OUT << "}";
	
	OUT.close();
}

void DumpTxtFile(Block_Tree* tree, std::ofstream* OUT)
{		
	if(tree->left->data != nullptr || tree->right->data != nullptr )
	{
		*OUT << "{" << tree->data << std::endl;
		if(tree->left->data != nullptr)
		{
			DumpTxtFile(tree->left, OUT);
		}
		if(tree->right != nullptr)
		{
			DumpTxtFile(tree->right, OUT);
		}
		*OUT << "}" << std::endl;
	}
	
	if (tree->left->data == nullptr && tree->right->data == nullptr && tree->data != nullptr)
	{
		*OUT << "{" << tree->data << "}" << std::endl;
	}
}
