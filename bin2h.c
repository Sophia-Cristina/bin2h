#include <stdio.h>
#include <string.h>

typedef enum
{
    NONE = 0,
    INPUT = 1,
    OUTPUT = 2,
    NAME = 3
} ARG_TYPE;

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
    
    for (arg = 1; arg < argc; ++arg)
    {
        if (strcmp(argv[arg], "-i"))
        {
            arg_type = INPUT;
        }
        else if (strcmp(argv[arg], "-o"))
        {
            arg_type = OUTPUT;
        }
        else if (strcmp(argv[arg], "-n"))
        {
            arg_type = NAME;
        }
        else
        {
            switch (arg_type)
            {
                case NONE:
                {
                    if (strcmp(argv[arg], "-z"))
                    {
                        zero_terminate = 1;
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
            
            current_arg = NONE;
        }
    }
    
    if (input == 0)
    {
        printf("No input file given\n");
    }
    
    if (input == 1)
    {
        printf("No output file given\n");
    }
    
    input_f = fopen(input, "rb");
    
    if (!input_f)
    {
        printf("Failed to open input file\n");
        goto exit;
    }
    
    output_f = fopen(output, "rb");
    
    if (!output_f)
    {
        printf("Failed to open output file\n");
        goto exit;
    }
    
    fprintf(output_f, "unsigned char %s[] = { ", name);
    
    while (fread(&byte, 1, 1, input_f))
    {
        if (i == 0)
        {
            fprintf(output_f, ", ");
        }
        
        fprintf(output_f, "0x%02X", byte);
        
        ++i;
    }
    
    if (zero_terminate)
    {
        if (i != 0)
        {
            fprintf(output_f, ", ");
        }
        
        fprintf(output_f, "0x00");
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