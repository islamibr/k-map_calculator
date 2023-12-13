
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

typedef struct implicant {
    int value;
    int mask;
    bool used;
    struct implicant *next;
} Implicant;

// Function prototypes
Implicant *create_implicant(int value, int mask);
void insert_implicant(Implicant **head, int value, int mask);
void free_implicants(Implicant *head);
int count_bits(int n);
int combine_implicants(Implicant *src, Implicant **dst);
void print_truth_table(int n, int *outputs);
void print_karnaugh_map(int n, int *outputs);
char *quine_mccluskey(int n, int *outputs);
bool is_term_duplicate(const char *result, const char *term);

int main() {
    int n;
    scanf("%d", &n);

    int num_combinations = pow(2, n);
    int outputs[num_combinations];

    for (int i = 0; i < num_combinations; i++) {
        scanf("%d", &outputs[i]);
    }

    printf("\nTruth Table:\n");
    print_truth_table(n, outputs);

    printf("\nKarnaugh Map:\n");
    print_karnaugh_map(n, outputs);

    printf("\nOutput Equation:\n");
    char *output_equation = quine_mccluskey(n, outputs);
    printf("%s\n", output_equation);
    free(output_equation);

    return 0;
}

// Create a new implicant with the given value and mask
Implicant *create_implicant(int value, int mask) {
    Implicant *new_implicant = (Implicant *)malloc(sizeof(Implicant));
    new_implicant->value = value;
    new_implicant->mask = mask;
    new_implicant->used = false;
    new_implicant->next = NULL;
    return new_implicant;
}

// Insert a new implicant with the given value and mask into the list
void insert_implicant(Implicant **head, int value, int mask) {
    if (*head == NULL) {
        *head = create_implicant(value, mask);
    } else {
        Implicant *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = create_implicant(value, mask);
    }
}

// Free the memory allocated for the implicants list
void free_implicants(Implicant *head) {
    Implicant *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

// Count the number of bits set in the given integer
int count_bits(int n) {
    int count = 0;
    while (n) {
        count += n & 1;
        n >>= 1;
    }
    return count;
}

// Combine implicants from the source list and insert them into the destination list
int combine_implicants(Implicant *src, Implicant **dst) {
    int combined = 0;
    Implicant *p1, *p2;
    for (p1 = src; p1 != NULL; p1 = p1->next) {
        for (p2 = p1->next; p2 != NULL; p2 = p2->next) {
            int diff = p1->value ^ p2->value;
            if (count_bits(diff) == 1 && p1->mask == p2->mask) {
                insert_implicant(dst, p1->value, p1->mask | diff);
                p1->used = p2->used = true;
                combined++;
            }
        }
    }
    return combined;
}

// Convert an implicant to a string representation
char *implicant_to_string(Implicant *implicant, int n) {
    char *result = malloc(n * 3 + 1); // Allocate enough memory for the worst-case scenario
    result[0] = '\0';

    for (int i = n - 1; i >= 0; i--) {
        if (!(implicant->mask & (1 << i))) {
            sprintf(result + strlen(result), "%c%c%s", 'A' + n - 1 - i, (implicant->value >> i) & 1 ? '\0' : '\'', i > 0 ? " " : "");
        }
    }

    return result;
}

// Quine-McCluskey algorithm implementation
char *quine_mccluskey(int n, int *outputs) {
    int all_outputs_zero = 1;
    for (int i = 0; i < pow(2, n); i++) {
        if (outputs[i]) {
            all_outputs_zero = 0;
            break;
        }
    }

    if (all_outputs_zero) {
        char *result = malloc(2);
        strcpy(result, "0");
        return result;
    }

    Implicant *head = NULL;
    int all_outputs_one = 1;

    for (int i = 0; i < pow(2, n); i++) {
        if (outputs[i]) {
            insert_implicant(&head, i, 0);
        } else {
            all_outputs_one = 0;
        }
    }

    if (all_outputs_one) {
        char *result = malloc(2);
        strcpy(result, "1");
        return result;
    }

    Implicant *temp;
    int combined;
    do {
        temp = NULL;
        combined = combine_implicants(head, &temp);
        if (combined > 0) {
            free_implicants(head);
            head = temp;
        }
    } while (combined > 0);

    bool any_implicant_used = false;

    for (Implicant *p = head; p != NULL; p = p->next) {
        if (p->used) {
            any_implicant_used = true;
            break;
        }
    }

    if (!any_implicant_used) {
        for (Implicant *p = head; p != NULL; p = p->next) {
            p->used = true;
        }
    }

    // Allocate enough memory for the worst-case scenario
    char *result = malloc((n * 3 + 3) * (int) pow(2, n));
    result[0] = '\0';

    bool first_term = true;

    for (Implicant *p = head; p != NULL; p = p->next) {
        if (p->used) {
            char *implicant_str = implicant_to_string(p, n);
            if (!is_term_duplicate(result, implicant_str)) {
                if (!first_term) {
                    strcat(result, " + ");
                }
                strcat(result, implicant_str);
                first_term = false;
            }
            free(implicant_str);
            p->used = true; // Mark the implicant as used
        }
    }

    free_implicants(head);
    return result;
}

// Print the truth table for the given outputs
void print_truth_table(int n, int *outputs) {
    for (int i = 0; i < pow(2, n); i++) {
        for (int j = n - 1; j >= 0; j--) {
            printf("%d ", (i >> j) & 1);
        }
        printf("%d\n", outputs[i]);
    }
}

// Print the Karnaugh map for the given outputs
void print_karnaugh_map(int n, int *outputs) {
    int rows = pow(2, n / 2);
    int cols = pow(2, (n + 1) / 2);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int gray_i = i ^ (i >> 1);
            int gray_j = j ^ (j >> 1);
            int index;
            if (n % 2 == 0) {
                index = (gray_i << (n / 2)) | gray_j;
            } else {
                index = (gray_i * cols) | gray_j;
            }
            printf("%d ", outputs[index]);
        }
        printf("\n");
    }
}

// Check if a term is a duplicate in the result string
bool is_term_duplicate(const char *result, const char *term) {
    const char *found = strstr(result, term);
    if (found == NULL) {
        return false;
    }

    // Check if the found term is not part of another term
    if ((found == result || found[-1] == ' ') && (found[strlen(term)] == '\0' || found[strlen(term)] == ' ')) {
        return true;
    }

    return is_term_duplicate(found + 1, term);
}
