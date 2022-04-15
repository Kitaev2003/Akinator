#include <iostream>
#include <fstream>
#include <cassert>
#include <cstring>

typedef struct Block
{	
	Block* left =  nullptr;
	
	Block* right = nullptr;
	
	char* data  = nullptr;
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
	void getPrintInFile();
	void Game();
	void setAkinator_Reader();
	void setInsert(Block_Tree* elem, char* str);
	void setInsertForGame(Block_Tree* tree);
	void setAkinator_Make(Block_Tree* elem, std::ifstream* READ);
};

void DumpTxtFile(Block_Tree* tree, std::ofstream* OUT);
void MainDump(Block_Tree* Tree, Block_Tree* Tree_Next, std::ofstream* GRAF);
bool CorrectScanStr(char* str, bool* yes, bool* no);
size_t StringDeleteTab(char* str, size_t len);
void MakeStringForAkinator(char* str);
void setFree(Block_Tree*  Tree);
