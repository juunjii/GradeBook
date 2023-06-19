#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gradebook.h"


/*
==========================
CSCI2021 Project 1 
Name: Jun Hao Cheh
==========================
*/

// This is the (somewhat famous) djb2 hash
unsigned hash(const char *str)
{
    unsigned hash_val = 5381;
    int i = 0;
    while (str[i] != '\0')
    {
        hash_val = ((hash_val << 5) + hash_val) + str[i];
        i++;
    }
    return hash_val % NUM_BUCKETS;
}

/**
 * @brief Create a new gradebook instance
 *
 * @param class_name the name of the class for grades
 * @return pointer to gradebook_t representing an empty gradebook or
 *         NULL if an error occurs
 */
gradebook_t *create_gradebook(const char *class_name)
{
    gradebook_t *new_book = malloc(sizeof(gradebook_t));
    if (new_book == NULL)
    {
        return NULL;
    }

    strcpy(new_book->class_name, class_name);
    for (int i = 0; i < NUM_BUCKETS; i++)
    {
        new_book->buckets[i] = NULL;
    }

    new_book->size = 0;

    return new_book;
}

/**
 * @brief returns a pointer to the gradebook class name
 *
 * @param book a pointer to a gradebook to get the class name of
 * @return a pointer to the gradebook class name
 */
const char *get_gradebook_name(const gradebook_t *book)
{
    return book->class_name;
}

/**
 * @brief add a new score to the gradebook (insert into hash table)
 *
 * @param book a pointer to a gradebook to add the score to
 * @param name student's name
 * @param score student's score
 * @return 0 if the score was successfully added/updated
 *         or -1 if the score could not be added/updated
 */
int add_score(gradebook_t *book, const char *name, int score)
{
    // Null check
    if (name == NULL)
    {
        return -1;
    }

    if (score >= 0)
    {
        node_t *node = malloc(sizeof(node_t));
        int index = hash(name);

        // Make node storing student data
        strcpy(node->name, name);
        node->score = score;
        node->next = NULL;

        // Checks if bucket is empty
        if (book->buckets[index] != NULL)
        {
            node_t *current_node = book->buckets[index];

            // Traverse the linked list in the bucket
            while (current_node->next != NULL)
            {
                // Checks for existing student name in bucket
                if (strcmp(name, current_node->name) == 0)
                {
                    current_node->score = node->score;
                }
                current_node = current_node->next;
            }
            
            // Add new student into bucket 
            current_node->next = node;
            (book->size)++;
        }

        // Empty bucket
        else
        {
            book->buckets[index] = node;
            (book->size)++;
        }

        return 0;
    }

    else
    {
        return -1;
    }
}

/**
 * @brief Search for a specific student's score in the gradebook
 *
 * @param book a pointer to a gradebook to search for the student score in
 * @param name the student's name
 * @return the student's score if their name is found
 *         or -1 if no matching student name is found
 */
int find_score(const gradebook_t *book, const char *name)
{
    int index = hash(name);
    if (book->buckets[index] != NULL)
    {
        node_t *current_node = book->buckets[index];
        // Traverse the linked list in the bucket
        while (current_node != NULL)
        {
            // Checks for existing student name in bucket
            if (strcmp(name, current_node->name) == 0)
            {
                return current_node->score;
            }
            current_node = current_node->next;
        }
    }

    return -1;
}

/**
 * @brief Frees all memory used to store the contents of the gradebook
 *
 * @param book a pointer to the gradebook to free
 */
void free_gradebook(gradebook_t *book)
{


    // Free each node in buckets 
    for (int i = 0; i < NUM_BUCKETS; i++)
    {
        node_t *current_node = book->buckets[i];

        while (current_node != NULL)
        {
            node_t *temp = current_node;
            current_node = current_node->next;
            free(temp);
        }
    }
    free(book);
}

/**
 * @brief Print out all scores in the gradebook
 *
 * @param book a pointer to the gradebook containing the scores to print
 */
void print_gradebook(const gradebook_t *book)
{
    for (int i = 0; i < NUM_BUCKETS; i++)
    {
        if (book->buckets[i] != NULL)
        {
            node_t *current_node = book->buckets[i];

            // Traverse the linked list in the bucket
            while (current_node != NULL)
            {
                printf("%s: %d\n", current_node->name, current_node->score);
                current_node = current_node->next;
            }
        }
    }
}

/**
 * @brief Write out all scores in the gradebook to a text file
 *
 * @param book a pointer to the gradebook containing the scores to write out
 * @return -1 when unable to write scores in gradebook to a text file
 *          or 0 when able to write scores in gradebook to a text file
 */
int write_gradebook_to_text(const gradebook_t *book)
{
    char file_name[MAX_NAME_LEN + strlen(".txt")];
    strcpy(file_name, book->class_name);
    strcat(file_name, ".txt");

    FILE *f = fopen(file_name, "w");
    if (f == NULL)
    {
        return -1;
    }

    fprintf(f, "%u\n", book->size);
    for (int i = 0; i < NUM_BUCKETS; i++)
    {
        node_t *current = book->buckets[i];
        while (current != NULL)
        {
            fprintf(f, "%s %d\n", current->name, current->score);
            current = current->next;
        }
    }
    fclose(f);
    return 0;
}

/**
 * @brief Read in all scores from a text file and add to a new gradebook
 *
 * @param file_name the name of the text file to read
 * @return a pointer to a new gradebook with all scores as recorded in the file
//         or NULL if the read operation fails
 */
gradebook_t *read_gradebook_from_text(const char *file_name)
{
    FILE *fp;
    char new_file_name[MAX_NAME_LEN];
    char student_name[MAX_NAME_LEN];
    int student_score;
    int gradebook_size = 0;
    int index;

    fp = fopen(file_name, "r");
    if (fp == NULL)
    {
        return NULL;
    }

    // Remove .txt file extension from file_name
    index = strcspn(file_name, ".");
    strncpy(new_file_name, file_name, index);
    new_file_name[index] = '\0';

    gradebook_t *new_gradebook = create_gradebook(new_file_name);

    // Read gradebook size in first line of file
    fscanf(fp, "%d", &gradebook_size);
   

    for (int i = 0; i < gradebook_size; i++)
    {
        // Read student name & score 
        fscanf(fp, "%s %d", student_name, &student_score);
       
        add_score(new_gradebook, student_name, student_score);
      
    }
    fclose(fp);

    return new_gradebook;
}

/**
 * @brief Write all scores in a gradebook to a binary file
 *
 * @param book apointer to a gradebook containing the scores to write out
 * @return -1 when unable to write scores in gradebook to a binary file
 *          or 0 when able to write scores in gradebook to a binary file
 */
int write_gradebook_to_binary(const gradebook_t *book)
{
    
    char file_name[MAX_NAME_LEN + strlen(".bin")];
    strcpy(file_name, book->class_name);
    strcat(file_name, ".bin"); 
    FILE *f = fopen(file_name, "wb");
    int length;

    if (f == NULL)
    {
        return -1;
    }

    // Write size of gradebook (number of students)
    fwrite(&book->size, sizeof(unsigned), 1, f);

    for (int i = 0; i < NUM_BUCKETS; i++)
    {
        node_t *current = book->buckets[i];

        while (current != NULL)
        {
            // Write the length of student's name
            length = strlen(current->name);
            fwrite(&length, sizeof(int), 1, f);

            // Write the name of student
            fwrite(current->name, sizeof(char), length, f);

            // Write the student's score
            fwrite(&current->score, sizeof(int), 1, f);

            current = current->next;
        }
    }
    fclose(f);
    return 0;
}

/**
 * @brief Read in all scores from a binary file and add to a new gradebook
 *
 * @param file_name the name of the binary file to read
 * @return  a pointer to a new gradebook containing all scores as recorded in the file
//          or NULL if the read operation fails
 */
gradebook_t *read_gradebook_from_binary(const char *file_name)
{
    FILE *fp;
    char new_file_name[MAX_NAME_LEN];
    int student_score = 0;
    int name_length = 0;
    int gradebook_size = 0;
    int index;

    fp = fopen(file_name, "rb");

    if (fp == NULL)
    {
        return NULL;
    }

    // Remove .bin file extension from file_name
    index = strcspn(file_name, ".");
    strncpy(new_file_name, file_name, index);
    new_file_name[index] = '\0';

    gradebook_t *new_gradebook = create_gradebook(new_file_name);

    // Read gradebook size
    fread(&gradebook_size, sizeof(int), 1, fp);
   
    for (int i = 0; i < gradebook_size; i++)
    {

        char student_name[MAX_NAME_LEN];
        // Read student name length
        fread(&name_length, sizeof(int), 1, fp);
    
        // Read student name
        fread(student_name, sizeof(char), name_length, fp);
        // Add null terminator at the end of student's name
        student_name[name_length] = '\0';
      
        // Read student score
        fread(&student_score, sizeof(int), 1, fp);
        
        add_score(new_gradebook, student_name, student_score);
       

    }
    
    fclose(fp);
    return new_gradebook;
}
