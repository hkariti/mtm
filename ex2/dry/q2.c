#include "stdlib.h"
#include "stdbool.h"

typedef struct node_t *Node;
struct node_t {
	int data;
	Node next;
};

// returns NULL if copy failed
Node copyNode(Node src) {
	Node clone = malloc(sizeof(struct node_t));
	if (NULL == clone) {
		return NULL;
	}
	clone->data = src->data;
	clone->next = src->next;
	return clone;
}

void freeAllNodes(Node first_node) {
	Node next_node;
	while (first_node != NULL) {
		next_node = first_node->next;
		free(first_node);
		first_node = next_node;
	}
}

Node printList(Node first_node, int steps) {
	Node node_to_print = first_node;
	int current_index = 0;
	while (node_to_print != NULL) {
		if (0 == current_index % steps) {
			printf("%d ", node_to_print->data);
		}
		node_to_print = node_to_print->next;
		current_index++;
	}
}

printCheckmarkOrdered(Node first_node) {

	if (NULL == first_node) {
		return true;
	}

	if (NULL == first_node->next) {
		printList(first_node, 1);
		return true;
	}

	Node original_list_iterator = first_node->next;
	Node reversed_list_head = copyNode(first_node->next); //TODO: change to nicer names!!
	Node previous_reversed_list_head; //Q: is ok to leave with garbage?
	reversed_list_head->next = NULL;
	while (original_list_iterator->next != NULL &&
		original_list_iterator->next->next != NULL) {
		previous_reversed_list_head = reversed_list_head;
		reversed_list_head = copyNode(original_list_iterator->next->next);
		if (NULL == reversed_list_head) {
			freeAllNodes(previous_reversed_list_head);
			return false;
		}
		reversed_list_head->next = previous_reversed_list_head;
		original_list_iterator = original_list_iterator->next->next;
	}

	printList(first_node, 2);
	printList(reversed_list_head, 1);

	freeAllNodes(reversed_list_head);
}

int main() { //just tests
	Node first_node = malloc(sizeof(struct node_t));
	first_node->data = -1;
	Node current_node = first_node;
	for (int i = 0; i < 10; i++) {
		current_node->next = malloc(sizeof(struct node_t));
		current_node = current_node->next;
		current_node->data = i;
		current_node->next = NULL;
	}

	printCheckmarkOrdered(first_node);

	char *a;
	scanf("%s", &a);
}