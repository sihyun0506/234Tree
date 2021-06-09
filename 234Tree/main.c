//This is Midterm project 2-3-4 Tree.
//2014003209 �ɽ���

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

node* makeNode();				//node �ʱ�ȭ �� node �� �����Լ�
node* root;
int dept;						//Depth

//MENU
void interactive();				//interactive mode �� �����ϴ� �Լ�
void batch(char*);				//batch mode �� �����ϴ� �Լ�

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
	printf("This is 2-3-4 Tree, -MidtermProject 2014003209 �ɽ���-\n");
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

	//�� Node�� Parent�� �ʱ�ȭ
	newNode->ParentNode = NULL;

	//�� Node�� key�� �ʱ�ȭ 
	newNode->key[0] = -1;
	newNode->key[1] = -1;
	newNode->key[2] = -1;
	newNode->key[3] = -1;

	//�� Node�� Child Node�� �ʱ�ȭ
	newNode->ChildNode[0] = NULL;
	newNode->ChildNode[1] = NULL;
	newNode->ChildNode[2] = NULL;
	newNode->ChildNode[3] = NULL;

	return newNode;
}


void insertion(node* newNode, int data) {

	//root�� 1. leaf�� ���, 2. root�� ��� �� ������ ����

	//1. leaf�� ���
	//ChildNode �� ������ leaf�̹Ƿ�, ChildNode[0]==NULL �϶� Node �� leaf �� ��
	if (newNode->ChildNode[0] == NULL) {

		//key�� ���� �� ����� ���
		//key�� ������ �ٷ� ���� �־��ָ� �ǹǷ�, key[0] == -1 �϶� key[0]�� ���� �־���
		if (newNode->key[0] == -1) {
			newNode->key[0] = data;
		}

		//key�� �̹� �ϳ� �����ϴ� ����� ���
		//key[0]�� �����ҽ�, key[0]���� data���� �� ��, ���� �־���
		else if (newNode->key[1] == -1) {
			if (newNode->key[0] < data) {
				newNode->key[1] = data;
			}
			else {
				newNode->key[1] = newNode->key[0];
				newNode->key[0] = data;
			}
		}

		//key�� �̹� �ΰ� �����ϴ� ����� ���
		//key[0],[1]�� �����ҽ�, key[0],[1]���� data���� �� ��, ���� �־���
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

		//key�� �̹� �� �� ����� ���
		//key[0],[1],[2] �� ��� �����ҽ�,
		else {
			node* SplitedRightNode = makeNode();
			SplitedRightNode->key[0] = data;
			//split ����
			newNode = split(newNode, SplitedRightNode);
			//�ʱ�ȭ
			SplitedRightNode = NULL;
			//propagate ����
			propagate(newNode);
			//�ʱ�ȭ
			newNode = NULL;
		}
	}

	//2. root �� ���
	//insertion�Լ��� �̿��Ͽ� recursive �ϰ� ����
	else {
		//newNode�� key[0] > data �� ���
		//newNode�� ChildNode[0]�� recursive�ϰ� insertion
		if (newNode->key[0] > data) {
			insertion(newNode->ChildNode[0], data);
		}

		//newNode�� key[1] > data > newNode�� key[0] ���, data > newNode�� key[0] �� ���
		//newNode�� ChildNode[1]�� recursive�ϰ� insertion
		else if (newNode->key[1] > data || newNode->key[1] == -1) {
			insertion(newNode->ChildNode[1], data);
		}

		//newNode�� key[2] > data > newNode�� key[1] ���, data > newNode�� key[1] �� ���
		//newNode�� ChildNode[2]�� recursive�ϰ� insertion
		else if (newNode->key[2] > data || newNode->key[2] == -1) {
			insertion(newNode->ChildNode[2], data);
		}

		//data > newNode�� key[2] ���
		//newNode�� ChildNode[3]�� recursive�ϰ� insertion
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

	//SplitedLeftNode, SplitedRightNode�� Children ����
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

	//SplitedLeftNode, tempParentNode, SplitedRightNode�� key�� ����
	//tempArrayKey�� ���� 4���� key������ ����
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


	//SplitedLeftNode�� ChildNode�� �θ��带 SplitedLeftNode�� �缳��
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

	//SplitedRightNode�� ChildNode�� �θ��带 SplitedRightNode�� �缳��
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

	//�޾ƿ� newNode = tempParentNode

	//��������� ParentNode�� ����� tempParentNode�� ParentNode(newNode->ParentNode)�� ��������� ParentNode�� ����

	//1. Split�Ǳ� �� ����� ParentNode�� �������� �ʴ� ���(Split �Ǳ� �� ��尡 key[]�� ���� ��� �ϳ����� ���)
	if (newNode->ParentNode == NULL) {
		root = newNode;
		dept++;
		return;
	}

	//Split�Ǳ� �� ����� ParentNode�� �����ϴ� ��� ��
	//2. Split�Ǳ� �� ����� ParentNode�� key[0],[1],[2]�� ������ �����ϴ� ���(Split�Ǳ� �� ����� ParentNode�� �̹� ���� ����� ���)
	if (newNode->ParentNode->key[2] != -1) {
		//tempParentNode�� ���� ���� �÷��ֱ� ���ؼ� recursive�ϰ� ����
		newNode = split(newNode->ParentNode, newNode);
		propagate(newNode);
		return;
	}

	//Split�Ǳ� �� ����� ParentNode�� key[0] Ȥ�� key[0],[1]�� ������ ���
	//3. Split�Ǳ� �� ����� ParentNode�� key[0] > newNode�� key[0](propagate�� ���� �ö���� ��)�� ���
	if (newNode->ParentNode->key[0] > newNode->key[0]) {
		newNode->ParentNode->key[2] = newNode->ParentNode->key[1];
		newNode->ParentNode->key[1] = newNode->ParentNode->key[0];
		newNode->ParentNode->key[0] = newNode->key[0];

		newNode->ParentNode->ChildNode[3] = newNode->ParentNode->ChildNode[2];
		newNode->ParentNode->ChildNode[2] = newNode->ParentNode->ChildNode[1];
		newNode->ParentNode->ChildNode[1] = newNode->ChildNode[1];
		newNode->ParentNode->ChildNode[0] = newNode->ChildNode[0];
	}
	//4. Split�Ǳ� �� ����� ParentNode�� key[1] > newNode�� key[0](propagate�� ���� �ö���� ��)�� ���
	else if (newNode->ParentNode->key[1] > newNode->key[0] || newNode->ParentNode->key[1] == -1) {
		newNode->ParentNode->key[2] = newNode->ParentNode->key[1];
		newNode->ParentNode->key[1] = newNode->key[0];

		newNode->ParentNode->ChildNode[3] = newNode->ParentNode->ChildNode[2];
		newNode->ParentNode->ChildNode[2] = newNode->ChildNode[1];
		newNode->ParentNode->ChildNode[1] = newNode->ChildNode[0];
	}
	//3. Split�Ǳ� �� ����� ParentNode�� key[1] < newNode�� key[0](propagate�� ���� �ö���� ��)�� ���
	else {
		newNode->ParentNode->key[2] = newNode->key[0];

		newNode->ParentNode->ChildNode[3] = newNode->ChildNode[1];
		newNode->ParentNode->ChildNode[2] = newNode->ChildNode[0];
	}


	//newNode->ParentNode�� ChildNode�� �θ��带 newNode->ParentNode�� �缳��
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

	//Deletion ������: ���� ���� ����Ŵ
	//���� data�� ����ִ� ���
	node* DeletionKeyNode;
	//���� data�� ����ִ� ����� key ��ȣ
	int DeletionKeyNumber;

	//2-3-4 Tree ���ο� data�� ���� ���� Ȯ��
	//data �������� �ʴ� ��� return
	if (searching(newNode, data) != 1) {
		return;
	}
	//data�� �����ϴ� ��� � ����� ���° key���� Ȯ��
	//Deletion ������ ����
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

	//delete �� data�� �����ϴ� ��忡 data�� successor�� �����, Deletion �����͸� ���� ������ successor�� ��ġ(leaf)�� �ű�
	//data�� successor�� �������� ������(data�� leaf�� ������) �ƹ��͵� ���� ����, Deletion �����͵� data�ڽ�(leaf)�� ����Ŵ
	DeletionKeyNumber = OverwriteData(DeletionKeyNode, DeletionKeyNumber);
	DeletionKeyNode = tempNode;
	tempNode = NULL;

	//Deletion �����Ͱ� ����Ű�� ���� ����� Tree�� ������
	//1. Tree ������ ������ �ʴ� ���(Deletion �����Ͱ� ����Ű�� ����� key�� 2�� �̻��� ���)
	if (DeletionKeyNode->key[1] != -1) {
		//Deletion �����Ͱ� ����Ű�� key�� ����� Deletion �����Ͱ� ����Ű�� ����� key�� ����
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

	//2. Tree ������ ���ϴ� ���(Deletion �����Ͱ� ����Ű�� ����� key�� 1���� ���)
	else {
		//Deletion �����Ͱ� ����Ű�� key�� �����
		DeletionKeyNode->key[0] = -1;

		//Tree�� ������
		reconstruct(DeletionKeyNode);

		return;
	}
}

int OverwriteData(node* DeletionKeyNode, int DeletionKeyNumber) {

	//SuccessorNode �� data �� �ٷ� ������ �Ʒ��� Node
	node* SuccessorNode = DeletionKeyNode->ChildNode[DeletionKeyNumber + 1];

	//data�� leaf�� ���
	if (SuccessorNode == NULL) {
		tempNode = DeletionKeyNode;
		return DeletionKeyNumber;
	}

	//data�� leaf�� �ƴ� ���
	while (1) {
		//data�� successor�� ã�� ������ ��������
		if (SuccessorNode->key[0] != -1) {
			if (SuccessorNode->ChildNode[0] == NULL) {
				//data�� successor�� ���� ������
				DeletionKeyNode->key[DeletionKeyNumber] = SuccessorNode->key[0];
				//successor ����
				tempNode = SuccessorNode;
				//0�� ��ȯ (successor���� successor ����� key[0]�� �����Ƿ�)
				return 0;
			}
		}
		SuccessorNode = SuccessorNode->ChildNode[0];
	}
}

void reconstruct(node* newNode) {

	int ChildNodeNumber;
	node* tempParentNode;

	//������ Tree�� �� ���
	if (newNode->ParentNode == NULL)
		return;

	//newNode �� ���° ChildNode���� Ȯ�� 
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
		//key�� 2�� �̻��� ��带 ������ �� Node�� ���° ChildNode���� Ȯ���ϰ�
		if (newNode->ParentNode->ChildNode[i]->key[1] != -1) {
			//ChildNodeNumber(�ڽ��� ���° ChildNode����)�� ���Ͽ�
			//�ڽ��� key�� 2�� �̻��� ��庸�� ���� ����̸�
			if (ChildNodeNumber > i)
				//�����̿����κ��� ���� ���������� ������
				transfertoRight(newNode->ParentNode, newNode);
			//�ڽ��� key�� 2�� �̻��� ��庸�� ���� ����̸�
			else
				//�������̿����κ��� ���� �������� ������
				transfertoLeft(newNode->ParentNode, newNode);
			return;
		}
	}

	//fusion
	//newNode �� ParentNode �� 2�� �̻��� key �� ������ fusion ����
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

	//newNode�� ParentNode �� root�� ���
	//Tree�� dept�� 1 ����
	if (newNode->ParentNode->ParentNode == NULL) {
		dept--;
		return;
	}

	//newNode�� ParentNode �� root�� �ƴ� ���
	//tempParentNode �� ���� �ֺ� ���� ���̸� �����ְ�
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
	//recursive�ϰ� Tree�� �籸��
	reconstruct(tempParentNode);
	return;
}

//���� �̿����κ��� ���� ���������� ������
void transfertoRight(node* newNode, node* receiver) {

	int num;
	node* giver;

	for (int i = 1; i < 4; i++) {
		if (newNode->ChildNode[i] == receiver) {

			giver = newNode->ChildNode[i - 1];

			//giver�� key�� ������ Ȯ��
			if (giver->key[2] != -1)
				num = 3;
			else if (giver->key[1] != -1)
				num = 2;
			else if (giver->key[0] != -1)
				num = 1;
			else
				num = 0;

			//key ����
			receiver->key[0] = newNode->key[i - 1];
			newNode->key[i - 1] = giver->key[num - 1];
			giver->key[num - 1] = -1;

			//ChildNode ����
			receiver->ChildNode[1] = receiver->ChildNode[0];
			receiver->ChildNode[0] = giver->ChildNode[num];
			giver->ChildNode[num] = NULL;

			if (receiver->ChildNode[0] != NULL)
				receiver->ChildNode[0]->ParentNode = receiver;

			//giver�� key�� �ϳ����� ���, �ٽ� giver�� �޾� recursive�ϰ� ����
			if (num == 1)
				transfertoRight(newNode, giver);

			return;
		}
	}
}

//������ �̿����κ��� ���� �������� ������
void transfertoLeft(node* newNode, node* receiver) {

	int num;
	node* giver;

	for (int i = 3; i >= 0; i--) {
		if (newNode->ChildNode[i] == receiver) {

			giver = newNode->ChildNode[i + 1];

			//giver�� key�� ������ Ȯ��
			if (giver->key[2] != -1)
				num = 3;
			else if (giver->key[1] != -1)
				num = 2;
			else if (giver->key[0] != -1)
				num = 1;
			else
				num = 0;

			//key ����
			receiver->key[0] = newNode->key[i];
			newNode->key[i] = giver->key[0];
			giver->key[0] = giver->key[1];
			giver->key[1] = giver->key[2];
			giver->key[2] = -1;

			//ChildNode ����
			receiver->ChildNode[1] = giver->ChildNode[0];
			giver->ChildNode[0] = giver->ChildNode[1];
			giver->ChildNode[1] = giver->ChildNode[2];
			giver->ChildNode[2] = giver->ChildNode[3];
			giver->ChildNode[3] = NULL;

			if (receiver->ChildNode[1] != NULL)
				receiver->ChildNode[1]->ParentNode = receiver;

			//giver�� key�� �ϳ����� ���, �ٽ� giver�� �޾� recursive�ϰ� ����
			if (num == 1)
				transfertoLeft(newNode, giver);

			return;
		}
	}
}

//�� ���� ParentNode�� �޾Ƽ� ������
void fusion(node* tempParentNode, node* newNode) {

	int i, j;

	//tempParentnode�� �������϶�(newNode�� ���� ������ ��)
	//newNode�� ������ ParentNode �� fusion
	if (tempParentNode->ChildNode[0] == newNode) {
		//��� ����
		tempParentNode->ChildNode[0] = tempParentNode->ChildNode[1];
		tempParentNode->ChildNode[1] = tempParentNode->ChildNode[2];
		tempParentNode->ChildNode[2] = tempParentNode->ChildNode[3];
		tempParentNode->ChildNode[3] = NULL;

		//key ����
		tempParentNode->ChildNode[0]->key[1] = tempParentNode->ChildNode[0]->key[0];
		tempParentNode->ChildNode[0]->key[0] = tempParentNode->key[0];
		tempParentNode->key[0] = tempParentNode->key[1];
		tempParentNode->key[1] = tempParentNode->key[2];
		tempParentNode->key[2] = -1;

		//ChildNode ����
		tempParentNode->ChildNode[0]->ChildNode[2] = tempParentNode->ChildNode[0]->ChildNode[1];
		tempParentNode->ChildNode[0]->ChildNode[1] = tempParentNode->ChildNode[0]->ChildNode[0];
		tempParentNode->ChildNode[0]->ChildNode[0] = newNode->ChildNode[0];
		if (newNode->ChildNode[0] != NULL)
			tempParentNode->ChildNode[0]->ChildNode[0]->ParentNode = tempParentNode->ChildNode[0];
	}

	//tempParentNode�� �����϶�
	//newNode�� ���� ParentNode �� fusion
	else {
		for (i = 1; i < 4; i++) {
			if (tempParentNode->ChildNode[i] != newNode)
				continue;

			//��� ����
			for (j = i; j < 3; j++) {
				tempParentNode->ChildNode[j] = tempParentNode->ChildNode[j + 1];
			}
			tempParentNode->ChildNode[3] = NULL;

			//key ����
			tempParentNode->ChildNode[i - 1]->key[1] = tempParentNode->key[i - 1];
			for (j = i - 1; j < 2; j++) {
				tempParentNode->key[j] = tempParentNode->key[j + 1];
			}
			tempParentNode->key[2] = -1;

			//ChildNode ����
			tempParentNode->ChildNode[i - 1]->ChildNode[2] = newNode->ChildNode[0];
			if (tempParentNode->ChildNode[i - 1]->ChildNode[2] != NULL)
				tempParentNode->ChildNode[i - 1]->ChildNode[2]->ParentNode = tempParentNode->ChildNode[i - 1];
			break;
		}
	}

	return;
}


//data�� key���� ���ϸ� recursive�ϰ� searching
int searching(node* newNode, int data) {

	//newNode�� key�� data�� �����ϴ� ��� return 1  
	if (newNode->key[0] == data || newNode->key[1] == data || newNode->key[2] == data) {
		tempNode = newNode;	//Deletion���� key�� �˻��ϱ� ���� �ӽó��
		return 1;
	}

	//newNode�� key[0] > data �� ���
	//newNode�� ChildNode[0]���� recursive�ϰ� searching
	else if (newNode->key[0] > data) {
		if (newNode->ChildNode[0] == NULL)
			return 0;
		searching(newNode->ChildNode[0], data);
	}

	//newNode�� key[1] > data > newNode�� key[0] ���, data > newNode�� key[0] �� ���
	//newNode�� ChildNode[1]���� recursive�ϰ� searching
	else if (newNode->key[1] > data || newNode->key[1] == -1) {
		if (newNode->ChildNode[1] == NULL)
			return 0;
		searching(newNode->ChildNode[1], data);
	}

	//newNode�� key[2] > data > newNode�� key[1] ���, data > newNode�� key[1] �� ���
	//newNode�� ChildNode[2]���� recursive�ϰ� searching
	else if (newNode->key[2] > data || newNode->key[2] == -1) {
		if (newNode->ChildNode[2] == NULL)
			return 0;
		searching(newNode->ChildNode[2], data);
	}

	//data > newNode�� key[2] ���
	//newNode�� ChildNode[3]���� recursive�ϰ� searching
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
	//�ش� dept���� node �� = width 
	int width;
	//NodeA�� NodeB�� ���� �����ư��� ���
	node* NodeA[1000];
	node* NodeB[1000];
	//�ʱ�ȭ
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
	//�ش� dept���� node �� = width 
	int width;
	//NodeA�� NodeB�� ���� �����ư��� ���
	node* NodeA[1000];
	node* NodeB[1000];
	//�ʱ�ȭ
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