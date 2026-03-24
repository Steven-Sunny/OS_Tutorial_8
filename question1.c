#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Process structure
typedef struct proc {
    char parent[256];
    char name[256];
    int priority;
    int memory;
} proc_t;

// Binary tree node
typedef struct node {
    proc_t data;
    struct node *left;
    struct node *right;
} node_t;

// Function to create a new node
node_t* create_node(proc_t val) {
    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    if (new_node == NULL) return NULL;
    new_node->data = val;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

// Recursive function to find a parent node by name
node_t* find_node(node_t* root, char* name) {
    if (root == NULL) return NULL;
    if (strcmp(root->data.name, name) == 0) return root;

    node_t* left_search = find_node(root->left, name);
    if (left_search != NULL) return left_search;

    return find_node(root->right, name);
}

// Function to add a child to a specific parent
void add_to_tree(node_t** root, proc_t val) {
    // If tree is empty and parent is NULL, this is the root
    if (*root == NULL) {
        *root = create_node(val);
        return;
    }

    // Else, find the parent node in the tree
    node_t* parent_node = find_node(*root, val.parent);

    // If parent node is found, add the new node as a child
    if (parent_node != NULL) {
        if (parent_node->left == NULL) {
            parent_node->left = create_node(val);
        } else if (parent_node->right == NULL) {
            parent_node->right = create_node(val);
        } else {
            printf("Error: Parent %s already has two children.\n", val.parent);
        }
    }
}

// Recursive function to print the tree (Pre-order traversal)
void print_tree(node_t* root) {
    if (root == NULL) return;

    printf("Parent: %-10s | Name: %-10s | Priority: %-3d | Memory: %d MB\n", 
           root->data.parent, root->data.name, root->data.priority, root->data.memory);
    
    print_tree(root->left);
    print_tree(root->right);
}

int main() {
    node_t* root = NULL;
    FILE *file = fopen("processes_tree.txt", "r");

    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    char line[512];
    // Read 7 lines as specified
    while (fgets(line, sizeof(line), file)) {
        proc_t temp_proc;
        // Parse CSV or .txt format: parent, name, priority, memory
        sscanf(line, " %[^,], %[^,], %d, %d", 
               temp_proc.parent, temp_proc.name, &temp_proc.priority, &temp_proc.memory);
        
        add_to_tree(&root, temp_proc);
    }

    fclose(file);

    print_tree(root);

    return 0;
}