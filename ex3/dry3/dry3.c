#include <stdlib.h>

//TODO: TEST & DOCUMENT ALL FILE

/** Type for defining the Node struct */
typedef struct node_t* Node; 
struct node_t {
	int n;
	Node next;
};

/** Type of function for general int operator */
typedef int (*intOperator) (int, int);

// not to submit
static void destroyList(Node list) {
	while (list != NULL) {
		Node next = list->next;
		free(list);
		list = next;
	}
}

/**
* listOperator:	Create a new list in which the nodes are the result
				of the provided operator on the nodes at the same index 
				in the lists provided.
*
* @param list1 -	pointer to a node's list
*		 list2 -	pointer to a node's list
*		 operator - function pointer which gets 2 ints and returns one int
* @return
*	a new allocated list in which each of its nodes are the result
*	of the two nodes in the same index at two provided lists.
* 	NULL -	if both lists are empty or the lists dont have the same size,
			or if allocations failed.
*/
Node listOperator(Node list1, Node list2, intOperator operator) {
	Node list1_current = list1;
	Node list2_current = list2;
	Node output_list = NULL;
	Node output_current = NULL;
	while (list1_current != NULL && list2_current != NULL) {
		Node new_node = malloc(sizeof(*new_node));
		if (NULL == new_node) {
			destroyList(output_list);
			return NULL;
		}
		new_node->n = operator(list1_current->n, list2_current->n);
		new_node->next = NULL;
		if (NULL == output_list) {
			output_current = new_node;
			output_list = new_node;
		}
		else {
			output_current->next = new_node;
			output_current = output_current->next;
		}
		list1_current = list1_current->next;
		list2_current = list2_current->next;
	}
	if (list1_current != NULL || list2_current != NULL)  { //list are not the same size
		destroyList(output_list);
		return NULL;
	}
	return output_list;
}

/**
* intMax: Return the maximum integer provided.
*
* @param int a - first integer value.
* @param int b - second integer value.
* @return
* 	The maximum of the two integers provided. If theyre both the same,
*	theyre value will be returned.
*/
static int intMax(int a, int b) {
	if (a > b) return a;
	return b;
}

/**
* maxElements:	Creates a list of the maximum nodes from all the lists provided.
*
* @param lists -	pointer to an array of node lists.
*					All the lists in the array must have the same size.
		 size =		The size of the lists array, count how many lists 
					are in the array.
* @return
*	a new allocated list in which each of its nodes are the maximum
	of all the nodes in the same index in all the lists provided.
	If the lists contains only one list, a copy of that list is returned.
* 	NULL - if lists array contains no lists or allocations failed.
*/
Node maxElements(Node* lists, int size) {
	Node max_list = NULL;
	Node previous_max_list = NULL;
	for (int i = 0; i < size; i++) {
		if (0 == i) {
			// the following line will copy the first list into max_list
			max_list = listOperator(lists[0], lists[0], intMax);
		}
		else {
			previous_max_list = max_list;
			max_list = listOperator(max_list, lists[i], intMax);
			destroyList(previous_max_list);
		}
	}
	return max_list;
}

int main() {
	Node* lists = malloc(sizeof(Node*) * 100);
	for (int j = 0; j < 5; j++) {
		lists[j] = malloc(sizeof(struct node_t));
		lists[j]->n = j;
		Node current = lists[j];
		for (int i = 0; i < 10; i++) {
			Node new_node = malloc(sizeof(struct node_t));
			new_node->n = i*j;
			new_node->next = NULL;
			current->next = new_node;
			current = new_node;
		}
	}
	Node list0 = lists[0];
	Node list1 = lists[1];
	Node list2 = lists[2];
	Node list3 = lists[3];
	Node new_list = listOperator(lists[0], lists[1], intMax);
	Node max_list = maxElements(lists, 5);

	free(lists);
}
