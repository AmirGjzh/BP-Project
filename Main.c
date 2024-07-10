#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Compress_Functions.c"

int main(int argc, char *argv[]) {

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//                            |-- Compressing file --|
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

    if (strcmp(argv[1], "com") == 0) {

//------------------------ Opening file and creating the queue ------------------------

        Queue *head = NULL;

        FILE *input_file = fopen(argv[2], "r");
        //--- Error message!
        if (input_file == NULL) {
            SetColor(RED);
            printf("%s", "File couldn't be opened!");
            SetColor(WHITE);
            return 0;
        }

        int word;
        while (!feof(input_file)) {
            word = fgetc(input_file);
            if (word != -1) {
                head = add_to_queue(head, word);
            }
        }
        fclose(input_file);

//------------------------------- Building huffman tree -------------------------------

        sort_queue(head);

        head = build_huffman_tree(head);

        Tree *root = head->main;

//-------------------------------- Creating guide file ---------------------------------

        char a[100];

        FILE *guide_file = fopen("Guide.txt", "w");
        //--- Error message!
        if (guide_file == NULL) {
            SetColor(RED);
            printf("%s", "File couldn't be created!");
            SetColor(WHITE);
            return 0;
        }

        show_huffman_codes(root, a, 0, guide_file);
        fclose(guide_file);

        SetColor(GREEN);
        printf("%s", "Guide file successfully created.\n");
        SetColor(WHITE);

//-------------------- Reading from guide file and creating hf_queue --------------------

        FILE *read_guide = fopen("Guide.txt", "r");
        //--- Error message!
        if (read_guide == NULL) {
            SetColor(RED);
            printf("%s", "File couldn't be opened!");
            SetColor(WHITE);
            return 0;
        }

        Hf_code *node = NULL;
        char code[100], code1[100];

        while (!feof(read_guide)) {
            int index = 0;
            while ((code[index] = (char) fgetc(read_guide)) != ' ') {
                if (code[index] == EOF) {
                    break;
                }
                index++;
            }
            code[index] = '\0';

            index = 0;
            int k = 0;
            while ((code1[index] = (char) fgetc(read_guide)) != '\n') {
                if (code[index] == EOF) {
                    k = 1;
                    break;
                }
                index++;
            }
            if (k == 0) {
                int value = 0;
                for (int i = 0; i < index; i++) {
                    value += (code1[i] - '0');
                    value *= 10;
                }
                value /= 10;
                node = add_to_hf_queue(node, code, value);
            }
        }

        fclose(read_guide);

//-------------------------------- Building binary file ---------------------------------

        FILE *binary_file, *first_file;

        first_file = fopen(argv[2], "r");
        //--- Error message!
        if (first_file == NULL) {
            SetColor(RED);
            printf("%s", "File couldn't be opened!");
            SetColor(WHITE);
        }
        binary_file = fopen("Binary.txt", "w");
        //--- Error message!
        if (binary_file == NULL) {
            SetColor(RED);
            printf("%s", "File couldn't be opened!");
            SetColor(WHITE);
        }

        while (!feof(first_file)) {
            int w;
            w = fgetc(first_file);
            if (w != -1) {
                fprintf(binary_file, "%s", search_hf_queue(node, w));
            }
        }

        fclose(first_file);
        fclose(binary_file);

        SetColor(GREEN);
        printf("%s", "Binary file successfully created.\n");
        SetColor(WHITE);

//------------------------------- Building compressed file -------------------------------

        FILE *compress_file = fopen("Compressed.txt", "w");
        //--- Error message!
        if (compress_file == NULL) {
            SetColor(RED);
            printf("%s", "File couldn't be opened!");
            SetColor(WHITE);
        }

        binary_file = fopen("Binary.txt", "r");
        //--- Error message!
        if (binary_file == NULL) {
            SetColor(RED);
            printf("%s", "File couldn't be opened!");
            SetColor(WHITE);
        }

        

        while (!feof(binary_file)) {
            int ascii = 0, val;
            for (int i = 0; i < 7; i++) {
                if ((val = fgetc(binary_file)) != -1) {
                    ascii += val - 48;
                    ascii *= 2;
                } 
                else {
                    ascii *= 2;
                }
            }
            fprintf(compress_file, "%c", (ascii + 64) / 2);
        }

        fclose(compress_file);
        fclose(binary_file);

//---------------------------------- Adding goal number ----------------------------------

        FILE *goal_number = fopen("Binary.txt", "r");
        //--- Error message!
        if (goal_number == NULL) {
            SetColor(RED);
            printf("%s", "File couldn't be opened!");
            SetColor(WHITE);
        }

        compress_file = fopen("Compressed.txt", "a");
        //--- Error message!
        if (compress_file == NULL) {
            SetColor(RED);
            printf("%s", "File couldn't be opened!");
            SetColor(WHITE);
        }

        long long int i = -1;
        while (!feof(goal_number)) {
            fgetc(goal_number);
            i++;
        }

        i = 7 - (i % 7);
        
        fprintf(compress_file, "%lld", i);

        fclose(compress_file);
        fclose(goal_number);


        SetColor(GREEN);
        printf("%s", "Compressed file successfully created.\n");
        SetColor(WHITE);

//----------------------------- Free the allocated memory --------------------------------

        free_queue(head);
        free_hf_queue(node);

        return 0;
    }

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//                          |-- Decompressing file --|
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

    else if (strcmp(argv[1], "decom") == 0) {

//------------------------------- Reading the guide file ----------------------------------

        FILE *read_guide = fopen(argv[3], "r");
        //--- Error message!
        if (read_guide == NULL) {
            SetColor(RED);
            printf("%s", "File couldn't be opened!");
            SetColor(WHITE);
        }

        Hf_code *node = NULL;
        char code[100], code1[100];

        while (!feof(read_guide)) {
            int index = 0;
            while ((code[index] = (char) fgetc(read_guide)) != ' ') {
                if (code[index] == EOF) {
                    break;
                }
                index++;
            }
            code[index] = '\0';

            index = 0;
            int k = 0;
            while ((code1[index] = (char) fgetc(read_guide)) != '\n') {
                if (code[index] == EOF) {
                    k = 1;
                    break;
                }
                index++;
            }
            if (k == 0) {
                int value = 0;
                for (int i = 0; i < index; i++) {
                    value += (code1[i] - '0');
                    value *= 10;
                }
                value /= 10;
                node = add_to_hf_queue(node, code, value);
            }
        }

        fclose(read_guide);

//------------------------------- Creating binary2 file ----------------------------------

        FILE *read_file = fopen(argv[2], "r");
        //--- Error message!
        if (read_file == NULL) {
            SetColor(RED);
            printf("%s", "File couldn't be opened!");
            SetColor(WHITE);
        }

        long long int s = -1;
        while (!feof(read_file)) {
            fgetc(read_file);
            s++;
        }

        fclose(read_file);



        FILE *binary_file = fopen("Binary2.txt", "w");
        //--- Error message!
        if (binary_file == NULL) {
            SetColor(RED);
            printf("%s", "File couldn't be opened!");
            SetColor(WHITE);
        }

        read_file = fopen(argv[2], "r");
        //--- Error message!
        if (read_file == NULL) {
            SetColor(RED);
            printf("%s", "File couldn't be opened!");
            SetColor(WHITE);
        }

        for (int i = 0; i < s - 1; i++) {
            int a = fgetc(read_file);
            binary(a - 32, binary_file);
        }
        

        int goal_number = fgetc(read_file);
        goal_number -= 48;

        fclose(read_file);
        fclose(binary_file);



        read_file = fopen("Binary2.txt", "r");
        //--- Error message!
        if (read_file == NULL) {
            SetColor(RED);
            printf("%s", "File couldn't be opened!");
            SetColor(WHITE);
        }

        long long int s1 = -1;
        while (!feof(read_file)) {
            fgetc(read_file);
            s1++;
        }

        fclose(read_file);

        SetColor(BLUE);
        printf("%s", "Binary2 file successfully created.\n");
        SetColor(WHITE);

//----------------------------- Building huffman tree -----------------------------------

        Tree *main = (Tree *) malloc(sizeof(Tree));
        //--- Error message!
        if (main == NULL) {
            SetColor(RED);
            printf("%s", "No memory to allocate!");
            SetColor(WHITE);
        }
        
        main->right = NULL;
        main->left = NULL;

        while (node != NULL) {
            main = add_to_tree(main, node->code, node->data);
            node = node->next;
        }

//---------------------------- Creating Decompress file ----------------------------------

        FILE *decompress = fopen("Decompressed.txt", "w");
        //--- Error message!
        if (decompress == NULL) {
            SetColor(RED);
            printf("%s", "File couldn't be opened!");
            SetColor(WHITE);
        }
    
        binary_file = fopen("Binary2.txt", "r");
        //--- Error message!
        if (binary_file == NULL) {
            SetColor(RED);
            printf("%s", "File couldn't be opened!");
            SetColor(WHITE);
        }
        

        Tree *copy = main;
        for (int i = 0; i < s1 - goal_number; i++) {
            int b = fgetc(binary_file);

            if (b == 49) {
                copy = copy->right;
            }
            else if (b == 48) {
                copy = copy->left;
            }

            if (copy->left == NULL && copy->right == NULL) {
                fprintf(decompress, "%c", copy->data);
                copy = main;
            }
        }

        fclose(decompress);
        fclose(binary_file);

        SetColor(BLUE);
        printf("%s", "Decompess file successfully created.\n");
        SetColor(WHITE);

//----------------------------- Free allocated memory ------------------------------------

        free_hf_queue(node);
        free_tree(main);

        return 0;

    }

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//                                 |-- Help --|
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

    else if (strcmp(argv[1], "help") == 0) {
        SetColor(LIGHT_BLUE);
        printf("%s", "--------------------------------------------------------------------------------------------\n");
        printf("%s", "                             Welcome to the Compression Program                           \n\n");
        printf("%s", "To compress your file, run the program like this in terminal:\n");
        printf("%s", "[program_name] com [file_name]\n\n");
        printf("%s", "Outputs are 3 files:\n");
        printf("%s", "Binary.txt ==> This file is a part of the program process and you have nothing to do with it.\n");
        printf("%s", "Guide.txt ==> This is your guide file and you need this for decompression.\n");
        printf("%s", "Compressed.txt ==> This is your compressed file.\n\n\n");
        printf("%s", "To decompress your file, run the program like this in terminal:\n");
        printf("%s", "[program_name] decom [Compressed_file] [Guide_file]\n\n");
        printf("%s", "Outputs are 2 files:\n");
        printf("%s", "Binary2.txt ==> This file is a part of the program process and you have nothing to do with it.\n");
        printf("%s", "Decompressed.txt ==> This is your Decompressed file.\n");
        printf("%s", "--------------------------------------------------------------------------------------------\n");
        SetColor(WHITE);

	return 0;
    }

    SetColor(RED);
    printf("%s", "No work to do!");
    SetColor(WHITE);

    return 0;
}