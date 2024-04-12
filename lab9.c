#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int     id;
    char    name;
    int     order; 
};

// Node for separate chaining
struct Node
{
    struct RecordType record;
    struct Node* next;
};

// HashType - hash table structure
struct HashType
{
    struct Node* head; // pointer to the head of the linked list
};

// Compute the hash function
int hash(int x)
{
    return x % 23; // Modulo 23 hash function
}

// Parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
        // Implement parse data block
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

// Prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// Display records in the hash structure
// Skip the indices which are free
// The output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    struct Node* temp;
    for (int i = 0; i < hashSz; ++i)
    {
        if (pHashArray[i].head != NULL) {
            printf("index %d -> ", i);
            temp = pHashArray[i].head;
            printf("%d, %c, %d", temp->record.id, temp->record.name, temp->record.order);
            temp = temp->next;
            while (temp != NULL)
            {
                printf(" -> %d, %c, %d", temp->record.id, temp->record.name, temp->record.order);
                temp = temp->next;
            }
            printf("\n");
        }
    }
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input_lab_9.txt", &pRecords);
    printRecords(pRecords, recordSz);
    
    // Determine the size of the hash table based on the number of records
    int hashSz = 2 * recordSz; // You can adjust the size according to your needs
    
    // Create the hash table
    struct HashType* pHashArray = (struct HashType*)malloc(sizeof(struct HashType) * hashSz);
    
    // Initialize all records to NULL (free)
    for (int i = 0; i < hashSz; ++i)
    {
        pHashArray[i].head = NULL;
    }

    // Insert records into the hash table using separate chaining
    for (int i = 0; i < recordSz; ++i)
    {
        int index = hash(pRecords[i].id);
        
        // Create a new node
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->record = pRecords[i];
        newNode->next = NULL;
        
        // If head is NULL, insert at head
        if (pHashArray[index].head == NULL) {
            pHashArray[index].head = newNode;
        } else {
            // Find the appropriate position to insert based on ascending order of IDs
            struct Node* current = pHashArray[index].head;
            struct Node* prev = NULL;
            while (current != NULL && current->record.id < pRecords[i].id) {
                prev = current;
                current = current->next;
            }
            if (prev == NULL) {
                newNode->next = pHashArray[index].head;
                pHashArray[index].head = newNode;
            } else {
                prev->next = newNode;
                newNode->next = current;
            }
        }
    }

    // Display records in the hash structure
    displayRecordsInHash(pHashArray, hashSz);

    // Free memory
    free(pRecords);
    for (int i = 0; i < hashSz; ++i)
    {
        struct Node* temp = pHashArray[i].head;
        while (temp != NULL)
        {
            struct Node* next = temp->next;
            free(temp);
            temp = next;
        }
    }
    free(pHashArray);

    return 0;
}
