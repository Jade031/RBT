#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

enum color { red, black };
typedef enum color color;

typedef struct Node* NodePtr;
struct Node {
	int val;
	NodePtr left, right, parent;
	color color;
};

NodePtr node_alloc(int newval) {
	NodePtr self = (NodePtr)malloc(sizeof(struct Node));
	self->val = newval;
	self->left = NULL;
	self->right = NULL;
	self->parent = NULL;
	return self;
}

typedef struct RBT* RBTPtr;
struct RBT {
	NodePtr root;
};

RBTPtr bst_alloc() {
	RBTPtr self = (RBTPtr)malloc(sizeof(struct RBT));
	self->root = NULL;
	return self;
}

NodePtr tree_minimum(NodePtr tree) {
	while (tree->left != NULL) tree = tree->left;
	return tree;
}

NodePtr tree_maximum(NodePtr tree) {
	while (tree->right != NULL) tree = tree->right;
	return tree;
}

NodePtr successor(NodePtr tree) {
	if (tree->right != NULL) return tree_minimum(tree->right);
	NodePtr succ = tree->parent;
	while (succ != NULL && tree == succ->right)
	{
		tree = succ;
		succ = succ->parent;
	}
	return succ;
}

NodePtr left_rotate(NodePtr tree, NodePtr base)
{
	NodePtr y = base->right;
	base->right = y->left;

	if (y->left != NULL)
		y->left->parent = base;

	y->parent = base->parent;

	if (base->parent == NULL)
		tree = y;
	else if (base == base->parent->left)
		base->parent->left = y;
	else
		base->parent->right = y;

	y->left = base;
	base->parent = y;

	return tree;
}


NodePtr right_rotate(NodePtr tree, NodePtr base)
{
	NodePtr y = base->left;
	base->left = y->right;

	if (y->right != NULL)
		y->right->parent = base;

	y->parent = base->parent;

	if (base->parent == NULL)
		tree = y;
	else if (base == base->parent->right)
		base->parent->right = y;
	else
		base->parent->left = y;

	y->right = base;
	base->parent = y;

	return tree;
}

NodePtr rbt_insert_fixup(NodePtr tree, NodePtr n)
{
	NodePtr y;
	if (n->parent == NULL);
	else
	{
		while (n->parent->color == red)
		{
			if (n->parent->parent == NULL);
			else if ((n->parent) == (n->parent->parent->left))
			{
				y = n->parent->parent->right;
				if (y->color == red)
				{
					n->parent->color = black;
					y->color = black;
					n->parent->parent->color = red;
					n = n->parent->parent;
				}
				else
				{
					if (n == n->parent->right)
					{
						n = n->parent;
						left_rotate(tree, n);
					}
					n->parent->color = black;
					n->parent->parent->color = red;
					right_rotate(tree, n->parent->parent);
				}
			}
			else
			{
				y = n->parent->parent->left;
				if (y->color == red)
				{
					n->parent->color = black;
					y->color = black;
					n->parent->parent->color = red;
					n = n->parent->parent;
				}
				else
				{
					if (n == n->parent->left)
					{
						n = n->parent;
						tree = right_rotate(tree, n);
					}
					n->parent->color = black;
					n->parent->parent->color = red;
					tree = left_rotate(tree, n->parent->parent);
				}
			}
		}
	}
	tree->color = black;
	return tree;
}

void rbt_insert(RBTPtr self, NodePtr tree, NodePtr n) {
	if (self->root == NULL)
		self->root = n;
	else if (tree == NULL);
	else if (n->val < tree->val) {
		if (tree->left == NULL)
		{
			tree->left = n;
			tree->left->parent = tree;
		}
		else
			rbt_insert(self, tree->left, n);
	}
	else {
		if (tree->right == NULL)
		{
			tree->right = n;
			tree->right->parent = tree;
		}
		else
			rbt_insert(self, tree->right, n);
	}
	tree->color = red;
	rbt_insert_fixup(tree, n);
}

void transplant(RBTPtr self, NodePtr origin, NodePtr replacement)
{
	NodePtr origin_parent = origin->parent;
	if (origin == NULL) return;
	else if (origin_parent == NULL) self->root = replacement;
	else if (origin == origin_parent->left) origin->parent->left = replacement;
	else origin_parent->right = replacement;
	if (replacement != NULL) replacement->parent = origin->parent;
}



void rbt_delete_fixup(NodePtr tree, NodePtr put)
{
	NodePtr w;
	while ((put != tree) && (put->color == black))
	{
		if (put == put->parent->left)
		{
			w = put->parent->right;
			if (w->color == red)
			{
				w->color = black;
				put->parent->color = red;
				tree = left_rotate(tree, put->parent);
				w = put->parent->right;
			}
			if ((w->left->color == black) && (w->right->color == black))
			{
				w->color = red;
				put = put->parent;
			}
			else
			{
				if (w->right->color == black)
				{
					w->left->color = black;
					w->color = red;
					tree = right_rotate(tree, w);
					w = put->parent->right;
				}
				w->color = put->parent->color;
				put->parent->color = black;
				w->right->color = black;
				tree = left_rotate(tree, put->parent);
				put = tree;
			}
		}
		else
		{
			w = put->parent->left;
			if (w->color == red)
			{
				w->color = black;
				put->parent->color = red;
				tree = right_rotate(tree, put->parent);
				w = put->parent->left;
			}
			if ((w->right->color == black) && (w->left->color == black))
			{
				w->color = red;
				put = put->parent;
			}
			else
			{
				if (w->left->color == black)
				{
					w->right->color = black;
					w->color = red;
					tree = left_rotate(tree, w);
					w = put->parent->left;
				}
				w->color = put->parent->color;
				put->parent->color = black;
				w->left->color = black;
				tree = right_rotate(tree, put->parent);
				put = tree;
			}
		}
	}
	put->color = black;
	return;
}
void rbt_delete(RBTPtr self, NodePtr tree, NodePtr put)
{
	NodePtr y;
	NodePtr x;
	if ((put->left == NULL) || (put->right == NULL))
		y = put;
	else
		y = successor(put);
	if (put->left != NULL)
		x = y->left;
	else
		x = y->right;

	x->parent = y->parent;

	if (y->parent == NULL)
		tree = x;
	else
	{
		if (y == y->parent->left)
			y->parent->left = x;
		else
			y->parent->right = x;
	}
	if (y != put)
		put->val = y->val;
	if (y->color == black)
		rbt_delete_fixup(tree, x);

	return;
}



void rbt_print(RBTPtr self, NodePtr tree, int level) {
	if (tree->right != NULL)
		rbt_print(self, tree->right, level + 1);
	for (int i = 0; i < level; i++)
		printf("    ");
	printf("%d\n", tree->val);
	if (tree->left != NULL)
		rbt_print(self, tree->left, level + 1);
}

void rbt_inorder(RBTPtr self, NodePtr tree) {
	if (tree == NULL)
		return;
	else {
		rbt_inorder(self, tree->left);
		printf("%d ", tree->val);
		rbt_inorder(self, tree->right);
	}
}

NodePtr rbt_search(RBTPtr self, NodePtr tree, int key)
{
	if (tree == NULL || key == tree->val) return tree;
	if (key < tree->val) return rbt_search(self, tree->left, key);
	else return rbt_search(self, tree->right, key);
}


int get_node_count(NodePtr tree)
{
	int count = 0;
	if (tree == NULL) {
		return count;
	}

	NodePtr q = tree;
	q = tree_minimum(q);
	count += 1;
	do
	{
		q = successor(q);
		count += 1;
	} while (q != NULL);

	return count - 1;
}


int get_blacknode_count(NodePtr tree) {
	int blcount = 0;

	if (tree == NULL) {
		return blcount;
	}

	NodePtr q = tree;
	q = tree_minimum(q);
	if (q->color == black)
		blcount += 1;
	do
	{
		q = successor(q);
		if (q->color == black)
			blcount += 1;
	} while (q != NULL);

	return blcount - 1;
}

int get_bh(NodePtr tree) {
	int bh = 0;
	while (tree != NULL) {
		if (tree->color == black) {
			bh += 1;
		}
		tree = tree->left;
	}
	return bh;
}


int main(void)
{
	FILE *pFile, *fp;
	RBTPtr self = bst_alloc();
	NodePtr tree = node_alloc(NULL);
	int n;
	NodePtr val = node_alloc(NULL);
	NodePtr m = node_alloc(NULL);

	pFile = fopen("input.txt", "r");
	if (pFile != NULL) {
		while (fscanf(pFile, "%d", &n) != EOF) {
			if (n > 0) {
				m->val = n;
				rbt_insert(self, tree, m);
			}
			else if (n < 0) {
				n *= -1;
				val = rbt_search(self, tree, n);
				if (val == NULL) {
					fp = fopen("test.txt", "w");
					n *= -1;
					fclose(fp);
				}
				else rbt_delete(self, tree, val);
			}
			else if (n == 0)
				break;
		}
		if (n == 0) {

			printf("total = ");
			printf("%d\n", get_node_count(tree));

			printf("nb = ");
			printf("%d\n", get_blacknode_count(tree));

			printf("bh = ");
			printf("%d\n", get_bh(tree));

			rbt_inorder(self, tree);
		}
	}
	else if (pFile == NULL)
		printf("파일이 없습니다.\n");
	fclose(pFile);
	return 0;
}