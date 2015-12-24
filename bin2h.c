#include <stdio.h>
#include <string.h>

typedef enum
{
    NONE = 0,
    INPUT = 1,
    OUTPUT = 2,
    NAME = 3
} ARG_TYPE;

void print_help()
{
    printf("Usage: bin2c -i <input> -o output -d -z\n");
}

int main(int argc, char *argv[])
{
    int result = 1;
    int i = 0;
    int arg;
    ARG_TYPE arg_type = NONE;
    FILE* input_f = 0;
    FILE* output_f = 0;
    const char* input = 0;
    const char* output = 0;
    const char* name = "data";
    
    unsigned char byte;
    int zero_terminate = 0;
    int decimal = 0;
    
    if (argc <= 1 ||
        (argc == 2 && strcmp(argv[1], "-h") == 0))
    {
        print_help();
        return 0;
    }
    
    for (arg = 1; arg < argc; ++arg)
    {
        if (strcmp(argv[arg], "-i") == 0)
        {
            arg_type = INPUT;
        }
        else if (strcmp(argv[arg], "-o") == 0)
        {
            arg_type = OUTPUT;
        }
        else if (strcmp(argv[arg], "-n") == 0)
        {
            arg_type = NAME;
        }
        else
        {
            switch (arg_type)
            {
                case NONE:
                {
                    if (strcmp(argv[arg], "-z") == 0)
                    {
                        zero_terminate = 1;
                    }
                    else if (strcmp(argv[arg], "-d") == 0)
                    {
                        decimal = 1;
                    }
                    else
                    {
                        printf("Invalid argument: %s\n", argv[arg]);
                        return 1;
                    }
                    
                    break;
                }
                case INPUT:
                {
                    input = argv[arg];
                    break;
                }
                case OUTPUT:
                {
                    output = argv[arg];
                    break;
                }
                case NAME:
                {
                    name = argv[arg];
                    break;
                }
            }
            
            arg_type = NONE;
        }
    }
    
    if (!input)
    {
        printf("No input file given\n");
        goto exit;
    }
    
    if (!output)
    {
        printf("No output file given\n");
        goto exit;
    }
    
    input_f = fopen(input, "rb");
    
    if (!input_f)
    {
        printf("Failed to open input file\n");
        goto exit;
    }
    
    output_f = fopen(output, "w");
    
    if (!output_f)
    {
        printf("Failed to open output file\n");
        goto exit;
    }
    
    fprintf(output_f, "unsigned char %s[] = { ", name);
    
    while (fread(&byte, 1, 1, input_f))
    {
        if (i != 0)
        {
            fprintf(output_f, ", ");
        }
        
        if (decimal)
        {
            fprintf(output_f, "%d", byte);
        }
        else
        {
            fprintf(output_f, "0x%02X", byte);
        }
        
        ++i;
    }
    
    if (zero_terminate)
    {
        if (i != 0)
        {
            fprintf(output_f, ", ");
        }
        
        if (decimal)
        {
            fprintf(output_f, "%d", byte);
        }
        else
        {
            fprintf(output_f, "0x00");
        }
    }
    
    fprintf(output_f, " };");

    result = 0;
exit:
    if (input_f)
    {
        fclose(input_f);
    }
    
    if (output_f)
    {
        fclose(output_f);
    }

    return result;
}