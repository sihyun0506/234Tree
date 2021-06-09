//This is Midterm project 2-3-4 Tree.
//2014003209 심시현

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {

	struct node* ParentNode;		//Parent node

	struct node* ChildNode[4];		//Child node

	int key[3];					//Key values

}node;

FILE* fin;
FILE* fout;

node* makeNode();				//node 초기화 및 node 값 설정함수
node* root;
int dept;						//Depth

//MENU
void interactive();				//interactive mode 로 동작하는 함수
void batch(char*);				//batch mode 로 동작하는 함수

//Insertion
void insertion(node*, int);		//insertion
node* split(node*, node*);
void propagate(node*);

//Deletion
void deletion(node*, int);		//deletion
node* tempNode;
int OverwriteData(node*, int);
void reconstruct(node*);
void transfertoRight(node*, node*);
void transfertoLeft(node*, node*);
void fusion(node*, node*);

//Searching
int searching(node*, int);		//searching

//Print
void printNode(node*);			//PrintNode
void printTree(node*);			//PrintTree
void fprintNode(node*);			//fprintNode
void fprintTree(node*);			//fprintTree


int main(int argc, char* argv[])
{
	char mode;


	printf("------------------------------------------------------\n");
	printf("This is 2-3-4 Tree, -MidtermProject 2014003209 심시현-\n");
	printf("------------------------------------------------------\n");
	while (1) {
		printf("mode Menu\n");
		printf(" i) Interactive mode\n");
		printf(" b) batch mode\n");
		printf("Select the mode: ");

		rewind(stdin);
		scanf("%c", &mode);

		//INTERACTIVE
		if (mode == 'i') {
			interactive();
			break;
		}

		//BATCH
		else if (mode == 'b') {
			batch(argv[1]);
			break;
		}

		//ERROR
		else {
			printf("\nError: there is no '%c' menu.\n", mode);
			printf("go back to menu\n\n");
		}
	}

	return 0;
}

void interactive()
{
	char command;
	int data;
	root = makeNode();
	root->ParentNode = NULL;
	dept = 1;

	printf("\n------------------ Interactive mode ------------------\n");
	printf("command Menu\n");
	printf(" i for insertion\n");
	printf(" d for deletion\n");
	printf(" s for searching\n");
	printf("ex) searching 15 : s15\n");

	while (1) {
		printf("\ncommand: ");
		rewind(stdin);
		scanf("%c%d", &command, &data);

		//INSERTION
		if (command == 'i') {
			printf("%c%d\n", command, data);
			insertion(root, data);
			printTree(root);
		}

		//DELETION
		else if (command == 'd') {
			printf("%c%d\n", command, data);
			deletion(root, data);
			printTree(root);
		}

		//SEARCHING
		else if (command == 's') {
			printf("%c%d\n", command, data);
			if (searching(root, data) == 1)
				printf("exist\n");
			else
				printf("not exist\n");
		}

		//ERROR
		else {
			printf("\nError: there is no '%c' command.\n", command);
			printf("go back to command menu\n\n");
		}
	}
}

void batch(char* argv)
{
	char command;
	int data;
	root = makeNode();
	root->ParentNode = NULL;
	dept = 1;

	printf("\n--------------------- Batch mode ---------------------\n");
	printf("Reading input DATA...\n");
	fin = fopen(argv, "r");
	if (fin == NULL) {
		printf("failed\n");
		printf("***Error: NO INPUT FILE***\n\n");
	}
	else {
		printf("success\n");
		printf("creating OUTPUT FILE...\n");
	}
	fout = fopen("output.txt", "w");

	while (1) {
		if (EOF == fscanf(fin, "%c%d ", &command, &data)) {
			fclose(fin);
			fclose(fout);
			printf("OUTPUT FILE CREATED\n");
			return 0;
		}

		//INSERTION
		if (command == 'i') {
			insertion(root, data);
			fprintf(fout, "%c%d\n", command, data);
			fprintTree(root);
		}

		//DELETION
		else if (command == 'd') {
			deletion(root, data);
			fprintf(fout, "%c%d\n", command, data);
			fprintTree(root);
		}

		//SEARCHING
		else if (command == 's') {
			fprintf(fout, "%c%d\n", command, data);
			if (searching(root, data) == 1)
				fprintf(fout, "exist\n");
			else
				fprintf(fout, "not exist\n");
		}

		fprintf(fout, "\n");
	}
}


node* makeNode() {

	node* newNode = NULL;
	newNode = (node*)malloc(sizeof(node));

	//새 Node의 Parent값 초기화
	newNode->ParentNode = NULL;

	//새 Node의 key값 초기화 
	newNode->key[0] = -1;
	newNode->key[1] = -1;
	newNode->key[2] = -1;
	newNode->key[3] = -1;

	//새 Node의 Child Node값 초기화
	newNode->ChildNode[0] = NULL;
	newNode->ChildNode[1] = NULL;
	newNode->ChildNode[2] = NULL;
	newNode->ChildNode[3] = NULL;

	return newNode;
}


void insertion(node* newNode, int data) {

	//root가 1. leaf인 경우, 2. root인 경우 로 나누어 진행

	//1. leaf인 경우
	//ChildNode 가 없으면 leaf이므로, ChildNode[0]==NULL 일때 Node 가 leaf 가 됨
	if (newNode->ChildNode[0] == NULL) {

		//key가 없는 빈 노드의 경우
		//key가 없으면 바로 값을 넣어주면 되므로, key[0] == -1 일때 key[0]에 값을 넣어줌
		if (newNode->key[0] == -1) {
			newNode->key[0] = data;
		}

		//key가 이미 하나 존재하는 노드의 경우
		//key[0]이 존재할시, key[0]값과 data값을 비교 후, 값을 넣어줌
		else if (newNode->key[1] == -1) {
			if (newNode->key[0] < data) {
				newNode->key[1] = data;
			}
			else {
				newNode->key[1] = newNode->key[0];
				newNode->key[0] = data;
			}
		}

		//key가 이미 두개 존재하는 노드의 경우
		//key[0],[1]이 존재할시, key[0],[1]값과 data값을 비교 후, 값을 넣어줌
		else if (newNode->key[2] == -1) {
			if (newNode->key[1] < data) {
				newNode->key[2] = data;
			}
			else if (newNode->key[0] < data) {
				newNode->key[2] = newNode->key[1];
				newNode->key[1] = data;
			}
			else {
				newNode->key[2] = newNode->key[1];
				newNode->key[1] = newNode->key[0];
				newNode->key[0] = data;
			}
		}

		//key가 이미 꽉 찬 노드의 경우
		//key[0],[1],[2] 가 모두 존재할시,
		else {
			node* SplitedRightNode = makeNode();
			SplitedRightNode->key[0] = data;
			//split 진행
			newNode = split(newNode, SplitedRightNode);
			//초기화
			SplitedRightNode = NULL;
			//propagate 진행
			propagate(newNode);
			//초기화
			newNode = NULL;
		}
	}

	//2. root 인 경우
	//insertion함수를 이용하여 recursive 하게 진행
	else {
		//newNode의 key[0] > data 인 경우
		//newNode의 ChildNode[0]에 recursive하게 insertion
		if (newNode->key[0] > data) {
			insertion(newNode->ChildNode[0], data);
		}

		//newNode의 key[1] > data > newNode의 key[0] 경우, data > newNode의 key[0] 인 경우
		//newNode의 ChildNode[1]에 recursive하게 insertion
		else if (newNode->key[1] > data || newNode->key[1] == -1) {
			insertion(newNode->ChildNode[1], data);
		}

		//newNode의 key[2] > data > newNode의 key[1] 경우, data > newNode의 key[1] 인 경우
		//newNode의 ChildNode[2]에 recursive하게 insertion
		else if (newNode->key[2] > data || newNode->key[2] == -1) {
			insertion(newNode->ChildNode[2], data);
		}

		//data > newNode의 key[2] 경우
		//newNode의 ChildNode[3]에 recursive하게 insertion
		else {
			insertion(newNode->ChildNode[3], data);
		}
	}

	return;
}

node* split(node* SplitedLeftNode, node* SplitedRightNode) {

	node* tempParentNode = makeNode();
	tempParentNode->ParentNode = SplitedLeftNode->ParentNode;
	SplitedLeftNode->ParentNode = tempParentNode;
	SplitedRightNode->ParentNode = tempParentNode;
	tempParentNode->ChildNode[0] = SplitedLeftNode;
	tempParentNode->ChildNode[1] = SplitedRightNode;

	if (tempParentNode->ParentNode != NULL) {
		if (tempParentNode->ParentNode->ChildNode[0] == SplitedLeftNode) {
			tempParentNode->ParentNode->ChildNode[0] = SplitedLeftNode;
			if (tempParentNode->ParentNode->ChildNode[1] == SplitedLeftNode) {
				tempParentNode->ParentNode->ChildNode[1] = SplitedLeftNode;
				if (tempParentNode->ParentNode->ChildNode[2] == SplitedLeftNode) {
					tempParentNode->ParentNode->ChildNode[2] = SplitedLeftNode;
					if (tempParentNode->ParentNode->ChildNode[3] == SplitedLeftNode) {
						tempParentNode->ParentNode->ChildNode[3] = SplitedLeftNode;
					}
				}
			}
		}
	}

	//SplitedLeftNode, SplitedRightNode의 Children 세팅
	if (SplitedLeftNode->key[0] > SplitedRightNode->key[0]) {

		SplitedLeftNode->ChildNode[0] = SplitedRightNode->ChildNode[0];
		SplitedRightNode->ChildNode[0] = SplitedLeftNode->ChildNode[2];
		SplitedLeftNode->ChildNode[2] = SplitedLeftNode->ChildNode[1];
		SplitedLeftNode->ChildNode[1] = SplitedRightNode->ChildNode[1];
		SplitedRightNode->ChildNode[1] = SplitedLeftNode->ChildNode[3];
		SplitedLeftNode->ChildNode[3] = NULL;
	}
	else if (SplitedLeftNode->key[1] > SplitedRightNode->key[0]) {

		SplitedLeftNode->ChildNode[1] = SplitedRightNode->ChildNode[0];
		SplitedRightNode->ChildNode[0] = SplitedLeftNode->ChildNode[2];
		SplitedLeftNode->ChildNode[2] = SplitedRightNode->ChildNode[1];
		SplitedRightNode->ChildNode[1] = SplitedLeftNode->ChildNode[3];
		SplitedLeftNode->ChildNode[3] = NULL;
	}
	else if (SplitedLeftNode->key[2] > SplitedRightNode->key[0]) {

		SplitedLeftNode->ChildNode[2] = SplitedRightNode->ChildNode[0];
		SplitedRightNode->ChildNode[0] = SplitedRightNode->ChildNode[1];
		SplitedRightNode->ChildNode[1] = SplitedLeftNode->ChildNode[3];
		SplitedLeftNode->ChildNode[3] = NULL;
	}
	else {

		SplitedLeftNode->ChildNode[3] = NULL;
	}

	//SplitedLeftNode, tempParentNode, SplitedRightNode의 key값 세팅
	//tempArrayKey로 들어온 4개의 key값들을 정렬
	node* tempArrayKey = makeNode();
	tempArrayKey->key[0] = SplitedLeftNode->key[0];
	tempArrayKey->key[1] = SplitedLeftNode->key[1];
	tempArrayKey->key[2] = SplitedLeftNode->key[2];

	if (tempArrayKey->key[2] < SplitedRightNode->key[0]) {
		tempArrayKey->key[3] = SplitedRightNode->key[0];
	}
	else if (tempArrayKey->key[1] < SplitedRightNode->key[0]) {
		tempArrayKey->key[3] = tempArrayKey->key[2];
		tempArrayKey->key[2] = SplitedRightNode->key[0];
	}
	else if (tempArrayKey->key[0] < SplitedRightNode->key[0]) {
		tempArrayKey->key[3] = tempArrayKey->key[2];
		tempArrayKey->key[2] = tempArrayKey->key[1];
		tempArrayKey->key[1] = SplitedRightNode->key[0];
	}
	else {
		tempArrayKey->key[3] = tempArrayKey->key[2];
		tempArrayKey->key[2] = tempArrayKey->key[1];
		tempArrayKey->key[1] = tempArrayKey->key[0];
		tempArrayKey->key[0] = SplitedRightNode->key[0];
	}

	SplitedLeftNode->key[0] = tempArrayKey->key[0];
	SplitedLeftNode->key[1] = tempArrayKey->key[1];
	SplitedLeftNode->key[2] = -1;
	SplitedLeftNode->key[3] = -1;

	tempParentNode->key[0] = tempArrayKey->key[2];
	tempParentNode->key[1] = -1;
	tempParentNode->key[2] = -1;
	tempParentNode->key[3] = -1;

	SplitedRightNode->key[0] = tempArrayKey->key[3];
	SplitedRightNode->key[1] = -1;
	SplitedRightNode->key[2] = -1;
	SplitedRightNode->key[3] = -1;


	//SplitedLeftNode의 ChildNode의 부모노드를 SplitedLeftNode로 재설정
	if (SplitedLeftNode->ChildNode[0] != NULL) {
		SplitedLeftNode->ChildNode[0]->ParentNode = SplitedLeftNode;
		if (SplitedLeftNode->ChildNode[1] != NULL) {
			SplitedLeftNode->ChildNode[1]->ParentNode = SplitedLeftNode;
			if (SplitedLeftNode->ChildNode[2] != NULL) {
				SplitedLeftNode->ChildNode[2]->ParentNode = SplitedLeftNode;
				if (SplitedLeftNode->ChildNode[3] != NULL) {
					SplitedLeftNode->ChildNode[3]->ParentNode = SplitedLeftNode;
				}
			}
		}
	}

	//SplitedRightNode의 ChildNode의 부모노드를 SplitedRightNode로 재설정
	if (SplitedRightNode->ChildNode[0] != NULL) {
		SplitedRightNode->ChildNode[0]->ParentNode = SplitedRightNode;
		if (SplitedRightNode->ChildNode[1] != NULL) {
			SplitedRightNode->ChildNode[1]->ParentNode = SplitedRightNode;
			if (SplitedRightNode->ChildNode[2] != NULL) {
				SplitedRightNode->ChildNode[2]->ParentNode = SplitedRightNode;
				if (SplitedRightNode->ChildNode[3] != NULL) {
					SplitedRightNode->ChildNode[3]->ParentNode = SplitedRightNode;
				}
			}
		}
	}

	return tempParentNode;
}

void propagate(node* newNode) {

	//받아온 newNode = tempParentNode

	//기존노드의 ParentNode가 존재시 tempParentNode의 ParentNode(newNode->ParentNode)는 기존노드의 ParentNode와 동일

	//1. Split되기 전 노드의 ParentNode가 존재하지 않는 경우(Split 되기 전 노드가 key[]가 꽉찬 노드 하나뿐인 경우)
	if (newNode->ParentNode == NULL) {
		root = newNode;
		dept++;
		return;
	}

	//Split되기 전 노드의 ParentNode가 존재하는 경우 중
	//2. Split되기 전 노드의 ParentNode가 key[0],[1],[2]을 가지고 존재하는 경우(Split되기 전 노드의 ParentNode가 이미 꽉찬 노드의 경우)
	if (newNode->ParentNode->key[2] != -1) {
		//tempParentNode가 가진 값을 올려주기 위해서 recursive하게 실행
		newNode = split(newNode->ParentNode, newNode);
		propagate(newNode);
		return;
	}

	//Split되기 전 노드의 ParentNode가 key[0] 혹은 key[0],[1]을 가지는 경우
	//3. Split되기 전 노드의 ParentNode의 key[0] > newNode의 key[0](propagate를 통해 올라오는 값)인 경우
	if (newNode->ParentNode->key[0] > newNode->key[0]) {
		newNode->ParentNode->key[2] = newNode->ParentNode->key[1];
		newNode->ParentNode->key[1] = newNode->ParentNode->key[0];
		newNode->ParentNode->key[0] = newNode->key[0];

		newNode->ParentNode->ChildNode[3] = newNode->ParentNode->ChildNode[2];
		newNode->ParentNode->ChildNode[2] = newNode->ParentNode->ChildNode[1];
		newNode->ParentNode->ChildNode[1] = newNode->ChildNode[1];
		newNode->ParentNode->ChildNode[0] = newNode->ChildNode[0];
	}
	//4. Split되기 전 노드의 ParentNode의 key[1] > newNode의 key[0](propagate를 통해 올라오는 값)인 경우
	else if (newNode->ParentNode->key[1] > newNode->key[0] || newNode->ParentNode->key[1] == -1) {
		newNode->ParentNode->key[2] = newNode->ParentNode->key[1];
		newNode->ParentNode->key[1] = newNode->key[0];

		newNode->ParentNode->ChildNode[3] = newNode->ParentNode->ChildNode[2];
		newNode->ParentNode->ChildNode[2] = newNode->ChildNode[1];
		newNode->ParentNode->ChildNode[1] = newNode->ChildNode[0];
	}
	//3. Split되기 전 노드의 ParentNode의 key[1] < newNode의 key[0](propagate를 통해 올라오는 값)인 경우
	else {
		newNode->ParentNode->key[2] = newNode->key[0];

		newNode->ParentNode->ChildNode[3] = newNode->ChildNode[1];
		newNode->ParentNode->ChildNode[2] = newNode->ChildNode[0];
	}


	//newNode->ParentNode의 ChildNode의 부모노드를 newNode->ParentNode로 재설정
	if (newNode->ParentNode->ChildNode[0] != NULL) {
		newNode->ParentNode->ChildNode[0]->ParentNode = newNode->ParentNode;
		if (newNode->ParentNode->ChildNode[1] != NULL) {
			newNode->ParentNode->ChildNode[1]->ParentNode = newNode->ParentNode;
			if (newNode->ParentNode->ChildNode[2] != NULL) {
				newNode->ParentNode->ChildNode[2]->ParentNode = newNode->ParentNode;
				if (newNode->ParentNode->ChildNode[3] != NULL) {
					newNode->ParentNode->ChildNode[3]->ParentNode = newNode->ParentNode;
				}
			}
		}
	}
	return;
}


void deletion(node* newNode, int data) {

	//Deletion 포인터: 지울 값을 가리킴
	//지울 data가 들어있는 노드
	node* DeletionKeyNode;
	//지울 data가 들어있는 노드의 key 번호
	int DeletionKeyNumber;

	//2-3-4 Tree 내부에 data가 존재 여부 확인
	//data 존재하지 않는 경우 return
	if (searching(newNode, data) != 1) {
		return;
	}
	//data가 존재하는 경우 어떤 노드의 몇번째 key인지 확인
	//Deletion 포인터 생성
	else {
		DeletionKeyNode = tempNode;
		tempNode = NULL;

		if (DeletionKeyNode->key[0] == data)
			DeletionKeyNumber = 0;
		else if (DeletionKeyNode->key[1] == data)
			DeletionKeyNumber = 1;
		else if (DeletionKeyNode->key[2] == data)
			DeletionKeyNumber = 2;
	}

	//delete 할 data가 존재하는 노드에 data의 successor을 덮어쓰고, Deletion 포인터를 값을 가져온 successor의 위치(leaf)로 옮김
	//data의 successor가 존재하지 않으면(data가 leaf에 있으면) 아무것도 하지 않음, Deletion 포인터도 data자신(leaf)을 가리킴
	DeletionKeyNumber = OverwriteData(DeletionKeyNode, DeletionKeyNumber);
	DeletionKeyNode = tempNode;
	tempNode = NULL;

	//Deletion 포인터가 가리키는 값을 지우고 Tree를 재정렬
	//1. Tree 구조가 변하지 않는 경우(Deletion 포인터가 가리키는 노드의 key가 2개 이상인 경우)
	if (DeletionKeyNode->key[1] != -1) {
		//Deletion 포인터가 가리키는 key를 지우고 Deletion 포인터가 가리키는 노드의 key만 정렬
		if (DeletionKeyNumber == 0) {
			DeletionKeyNode->key[0] = DeletionKeyNode->key[1];
			DeletionKeyNode->key[1] = DeletionKeyNode->key[2];
			DeletionKeyNode->key[2] = -1;
		}
		else if (DeletionKeyNumber == 1) {
			DeletionKeyNode->key[1] = DeletionKeyNode->key[2];
			DeletionKeyNode->key[2] = -1;
		}
		else if (DeletionKeyNumber == 2) {
			DeletionKeyNode->key[2] = -1;
		}
	}

	//2. Tree 구조가 변하는 경우(Deletion 포인터가 가리키는 노드의 key가 1개인 경우)
	else {
		//Deletion 포인터가 가리키는 key를 지우고
		DeletionKeyNode->key[0] = -1;

		//Tree를 재정렬
		reconstruct(DeletionKeyNode);

		return;
	}
}

int OverwriteData(node* DeletionKeyNode, int DeletionKeyNumber) {

	//SuccessorNode 는 data 의 바로 오른쪽 아래의 Node
	node* SuccessorNode = DeletionKeyNode->ChildNode[DeletionKeyNumber + 1];

	//data가 leaf일 경우
	if (SuccessorNode == NULL) {
		tempNode = DeletionKeyNode;
		return DeletionKeyNumber;
	}

	//data가 leaf가 아닐 경우
	while (1) {
		//data의 successor을 찾을 때까지 내려가서
		if (SuccessorNode->key[0] != -1) {
			if (SuccessorNode->ChildNode[0] == NULL) {
				//data에 successor의 값을 덮어씌우고
				DeletionKeyNode->key[DeletionKeyNumber] = SuccessorNode->key[0];
				//successor 노드와
				tempNode = SuccessorNode;
				//0을 반환 (successor값은 successor 노드의 key[0]에 있으므로)
				return 0;
			}
		}
		SuccessorNode = SuccessorNode->ChildNode[0];
	}
}

void reconstruct(node* newNode) {

	int ChildNodeNumber;
	node* tempParentNode;

	//지워서 Tree가 빈 경우
	if (newNode->ParentNode == NULL)
		return;

	//newNode 가 몇번째 ChildNode인지 확인 
	for (int i = 0; i < 4; i++) {
		if (newNode->ParentNode->ChildNode[i] == newNode) {
			ChildNodeNumber = i;
			break;
		}
	}

	//transfer
	for (int i = 0; i < 4; i++) {
		if (newNode->ParentNode->ChildNode[i] == NULL)
			break;
		//key가 2개 이상인 노드를 만나면 그 Node가 몇번째 ChildNode인지 확인하고
		if (newNode->ParentNode->ChildNode[i]->key[1] != -1) {
			//ChildNodeNumber(자신이 몇번째 ChildNode인지)와 비교하여
			//자신이 key가 2개 이상인 노드보다 뒤의 노드이면
			if (ChildNodeNumber > i)
				//왼쪽이웃노드로부터 값을 오른쪽으로 가져옴
				transfertoRight(newNode->ParentNode, newNode);
			//자신이 key가 2개 이상인 노드보다 앞의 노드이면
			else
				//오른쪽이웃노드로부터 값을 왼쪽으로 가져옴
				transfertoLeft(newNode->ParentNode, newNode);
			return;
		}
	}

	//fusion
	//newNode 의 ParentNode 가 2개 이상의 key 를 가지면 fusion 진행
	if (newNode->ParentNode->key[1] != -1) {
		fusion(newNode->ParentNode, newNode);
		return;
	}

	//propagate
	if (ChildNodeNumber == 0) {
		newNode->ParentNode->ChildNode[0] = newNode->ChildNode[0];
		if (newNode->ParentNode->ChildNode[0] != NULL)
			newNode->ParentNode->ChildNode[0]->ParentNode = newNode->ParentNode;
		propagate(newNode->ParentNode->ChildNode[1]);
	}
	else {
		newNode->ParentNode->ChildNode[1] = newNode->ChildNode[0];
		if (newNode->ParentNode->ChildNode[1] != NULL)
			newNode->ParentNode->ChildNode[1]->ParentNode = newNode->ParentNode;
		propagate(newNode->ParentNode->ChildNode[0]);
	}

	//newNode의 ParentNode 가 root인 경우
	//Tree의 dept를 1 낮춤
	if (newNode->ParentNode->ParentNode == NULL) {
		dept--;
		return;
	}

	//newNode의 ParentNode 가 root가 아닌 경우
	//tempParentNode 를 통해 주변 노드와 높이를 맞춰주고
	tempParentNode = makeNode();
	tempParentNode->ParentNode = newNode->ParentNode->ParentNode;
	tempParentNode->ChildNode[0] = newNode->ParentNode;
	newNode->ParentNode->ParentNode = tempParentNode;
	if (tempParentNode->ParentNode != NULL)
		for (int i = 0; i < 4; i++) {
			if (tempParentNode->ParentNode->ChildNode[i] == newNode->ParentNode) {
				tempParentNode->ParentNode->ChildNode[i] = tempParentNode;
				break;
			}
		}
	//recursive하게 Tree를 재구성
	reconstruct(tempParentNode);
	return;
}

//왼쪽 이웃노드로부터 값을 오른쪽으로 가져옴
void transfertoRight(node* newNode, node* receiver) {

	int num;
	node* giver;

	for (int i = 1; i < 4; i++) {
		if (newNode->ChildNode[i] == receiver) {

			giver = newNode->ChildNode[i - 1];

			//giver의 key의 갯수를 확인
			if (giver->key[2] != -1)
				num = 3;
			else if (giver->key[1] != -1)
				num = 2;
			else if (giver->key[0] != -1)
				num = 1;
			else
				num = 0;

			//key 설정
			receiver->key[0] = newNode->key[i - 1];
			newNode->key[i - 1] = giver->key[num - 1];
			giver->key[num - 1] = -1;

			//ChildNode 설정
			receiver->ChildNode[1] = receiver->ChildNode[0];
			receiver->ChildNode[0] = giver->ChildNode[num];
			giver->ChildNode[num] = NULL;

			if (receiver->ChildNode[0] != NULL)
				receiver->ChildNode[0]->ParentNode = receiver;

			//giver의 key가 하나뿐인 경우, 다시 giver를 받아 recursive하게 진행
			if (num == 1)
				transfertoRight(newNode, giver);

			return;
		}
	}
}

//오른쪽 이웃노드로부터 값을 왼쪽으로 가져옴
void transfertoLeft(node* newNode, node* receiver) {

	int num;
	node* giver;

	for (int i = 3; i >= 0; i--) {
		if (newNode->ChildNode[i] == receiver) {

			giver = newNode->ChildNode[i + 1];

			//giver의 key의 갯수를 확인
			if (giver->key[2] != -1)
				num = 3;
			else if (giver->key[1] != -1)
				num = 2;
			else if (giver->key[0] != -1)
				num = 1;
			else
				num = 0;

			//key 설정
			receiver->key[0] = newNode->key[i];
			newNode->key[i] = giver->key[0];
			giver->key[0] = giver->key[1];
			giver->key[1] = giver->key[2];
			giver->key[2] = -1;

			//ChildNode 설정
			receiver->ChildNode[1] = giver->ChildNode[0];
			giver->ChildNode[0] = giver->ChildNode[1];
			giver->ChildNode[1] = giver->ChildNode[2];
			giver->ChildNode[2] = giver->ChildNode[3];
			giver->ChildNode[3] = NULL;

			if (receiver->ChildNode[1] != NULL)
				receiver->ChildNode[1]->ParentNode = receiver;

			//giver의 key가 하나뿐인 경우, 다시 giver를 받아 recursive하게 진행
			if (num == 1)
				transfertoLeft(newNode, giver);

			return;
		}
	}
}

//빈 노드와 ParentNode를 받아서 합쳐줌
void fusion(node* tempParentNode, node* newNode) {

	int i, j;

	//tempParentnode가 오른쪽일때(newNode가 가장 왼쪽일 때)
	//newNode가 오른쪽 ParentNode 와 fusion
	if (tempParentNode->ChildNode[0] == newNode) {
		//노드 설정
		tempParentNode->ChildNode[0] = tempParentNode->ChildNode[1];
		tempParentNode->ChildNode[1] = tempParentNode->ChildNode[2];
		tempParentNode->ChildNode[2] = tempParentNode->ChildNode[3];
		tempParentNode->ChildNode[3] = NULL;

		//key 설정
		tempParentNode->ChildNode[0]->key[1] = tempParentNode->ChildNode[0]->key[0];
		tempParentNode->ChildNode[0]->key[0] = tempParentNode->key[0];
		tempParentNode->key[0] = tempParentNode->key[1];
		tempParentNode->key[1] = tempParentNode->key[2];
		tempParentNode->key[2] = -1;

		//ChildNode 설정
		tempParentNode->ChildNode[0]->ChildNode[2] = tempParentNode->ChildNode[0]->ChildNode[1];
		tempParentNode->ChildNode[0]->ChildNode[1] = tempParentNode->ChildNode[0]->ChildNode[0];
		tempParentNode->ChildNode[0]->ChildNode[0] = newNode->ChildNode[0];
		if (newNode->ChildNode[0] != NULL)
			tempParentNode->ChildNode[0]->ChildNode[0]->ParentNode = tempParentNode->ChildNode[0];
	}

	//tempParentNode가 왼쪽일때
	//newNode가 왼쪽 ParentNode 와 fusion
	else {
		for (i = 1; i < 4; i++) {
			if (tempParentNode->ChildNode[i] != newNode)
				continue;

			//노드 설정
			for (j = i; j < 3; j++) {
				tempParentNode->ChildNode[j] = tempParentNode->ChildNode[j + 1];
			}
			tempParentNode->ChildNode[3] = NULL;

			//key 설정
			tempParentNode->ChildNode[i - 1]->key[1] = tempParentNode->key[i - 1];
			for (j = i - 1; j < 2; j++) {
				tempParentNode->key[j] = tempParentNode->key[j + 1];
			}
			tempParentNode->key[2] = -1;

			//ChildNode 설정
			tempParentNode->ChildNode[i - 1]->ChildNode[2] = newNode->ChildNode[0];
			if (tempParentNode->ChildNode[i - 1]->ChildNode[2] != NULL)
				tempParentNode->ChildNode[i - 1]->ChildNode[2]->ParentNode = tempParentNode->ChildNode[i - 1];
			break;
		}
	}

	return;
}


//data와 key값을 비교하며 recursive하게 searching
int searching(node* newNode, int data) {

	//newNode의 key에 data가 존재하는 경우 return 1  
	if (newNode->key[0] == data || newNode->key[1] == data || newNode->key[2] == data) {
		tempNode = newNode;	//Deletion에서 key를 검색하기 위한 임시노드
		return 1;
	}

	//newNode의 key[0] > data 인 경우
	//newNode의 ChildNode[0]에서 recursive하게 searching
	else if (newNode->key[0] > data) {
		if (newNode->ChildNode[0] == NULL)
			return 0;
		searching(newNode->ChildNode[0], data);
	}

	//newNode의 key[1] > data > newNode의 key[0] 경우, data > newNode의 key[0] 인 경우
	//newNode의 ChildNode[1]에서 recursive하게 searching
	else if (newNode->key[1] > data || newNode->key[1] == -1) {
		if (newNode->ChildNode[1] == NULL)
			return 0;
		searching(newNode->ChildNode[1], data);
	}

	//newNode의 key[2] > data > newNode의 key[1] 경우, data > newNode의 key[1] 인 경우
	//newNode의 ChildNode[2]에서 recursive하게 searching
	else if (newNode->key[2] > data || newNode->key[2] == -1) {
		if (newNode->ChildNode[2] == NULL)
			return 0;
		searching(newNode->ChildNode[2], data);
	}

	//data > newNode의 key[2] 경우
	//newNode의 ChildNode[3]에서 recursive하게 searching
	else {
		if (newNode->ChildNode[3] == NULL)
			return 0;
		searching(newNode->ChildNode[3], data);
	}
}


void printNode(node* newNode) {

	if (newNode->key[0] == -1)
		return;
	else if (newNode->key[1] == -1)
		printf("(%d) ", newNode->key[0]);
	else if (newNode->key[2] == -1)
		printf("(%d %d) ", newNode->key[0], newNode->key[1]);
	else
		printf("(%d %d %d) ", newNode->key[0], newNode->key[1], newNode->key[2]);

	return;
}

void printTree(node* newNode) {
	//해당 dept에서 node 수 = width 
	int width;
	//NodeA와 NodeB를 통해 번갈아가며 출력
	node* NodeA[1000];
	node* NodeB[1000];
	//초기화
	memset(NodeA, 0, sizeof(NodeA));
	memset(NodeB, 0, sizeof(NodeB));
	NodeA[0] = newNode;

	for (int i = 0; i < dept; i++) {
		width = 0;
		if (i % 2 == 0) {
			for (int j = 0; j < 1000; j++) {
				if (NodeA[j] == NULL)
					break;
				printNode(NodeA[j]);
				for (int k = 0; k < 4; k++) {
					if (NodeA[j]->ChildNode[k] == NULL)
						break;
					NodeB[width] = NodeA[j]->ChildNode[k];
					width++;
				}
			}
		}
		else {
			for (int j = 0; j < 1000; j++) {
				if (NodeB[j] == NULL)
					break;
				printNode(NodeB[j]);
				for (int k = 0; k < 4; k++) {
					if (NodeB[j]->ChildNode[k] == NULL)
						break;
					NodeA[width] = NodeB[j]->ChildNode[k];
					width++;
				}
			}
		}
		printf("\n");
	}
	return;
}

void fprintNode(node* newNode) {

	if (newNode->key[0] == -1)
		return;
	else if (newNode->key[1] == -1)
		fprintf(fout, "(%d) ", newNode->key[0]);
	else if (newNode->key[2] == -1)
		fprintf(fout, "(%d %d) ", newNode->key[0], newNode->key[1]);
	else
		fprintf(fout, "(%d %d %d) ", newNode->key[0], newNode->key[1], newNode->key[2]);

	return;
}

void fprintTree(node* newNode) {
	//해당 dept에서 node 수 = width 
	int width;
	//NodeA와 NodeB를 통해 번갈아가며 출력
	node* NodeA[1000];
	node* NodeB[1000];
	//초기화
	memset(NodeA, 0, sizeof(NodeA));
	memset(NodeB, 0, sizeof(NodeB));
	NodeA[0] = newNode;

	for (int i = 0; i < dept; i++) {
		width = 0;
		if (i % 2 == 0) {
			for (int j = 0; j < 1000; j++) {
				if (NodeA[j] == NULL)
					break;
				fprintNode(NodeA[j]);
				for (int k = 0; k < 4; k++) {
					if (NodeA[j]->ChildNode[k] == NULL)
						break;
					NodeB[width] = NodeA[j]->ChildNode[k];
					width++;
				}
			}
		}
		else {
			for (int j = 0; j < 1000; j++) {
				if (NodeB[j] == NULL)
					break;
				fprintNode(NodeB[j]);
				for (int k = 0; k < 4; k++) {
					if (NodeB[j]->ChildNode[k] == NULL)
						break;
					NodeA[width] = NodeB[j]->ChildNode[k];
					width++;
				}
			}
		}
		fprintf(fout, "\n");
	}
	return;
}