#include <iostream>
#include <fstream>

static void print_help(const char* name)
{
    std::cout << "OVERVIEW: Converts binary file to C header\n\n";
    std::cout << "USAGE: " << name << " [-i <input>] [-o <output>] [-n <name>] [-d] [-e] [-s] [-z] [-c]\n\n";
    std::cout << "OPTIONS:\n"
        << "	-d        Print decimal instead of hex literals\n"
        << "	-e        Output a variable that holds a pointer to the last element of the data\n"
        << "	-i <in>   Specify the input file\n"
        << "	-n <name> Specify the variable name\n"
        << "	-o <out>  Specify the output file\n"
        << "	-s        Output a variable for data size\n"
        << "	-z        Add a zero to the end of the array\n"
        << "	-c        Collums for a new line (0x1 to 0xF). '0': No line change.\n";
}

static void generate_output(std::ifstream* input_s, std::ofstream* output_s, const char* name, uint8_t flags)
{
    uint8_t byte, cols = (flags >> 4) & 15;
    size_t i = 0;

    *output_s << "unsigned char " << name << "[] =\n{\n ";

    while (!input_s->eof())
    {
        byte = input_s->get();
        if (i != 0) { *output_s << ", "; if (cols) { if (!(i % cols)) { *output_s << "\n "; } } }        
        if (flags & 8) { *output_s << (short)byte; }
        else { *output_s << "0x" << std::hex << byte; }        
        ++i;        
    }

    if (flags & 1)
    {
        if (i != 0) { *output_s << ", "; }
        *output_s << (flags & 8) ? "0" : "0x00";
        ++i;
    }

    *output_s << "\n};\n";

    if (i > 0 && flags & 4)
        *output_s << "unsigned char* " << name << "_end = " << name << " + " << i - 1 << ";\n";

    if (flags & 2)
        *output_s << "unsigned int " << name << "_size = " << i << ";\n";
}

int main(int argc, char* argv[])
{
    int result = EXIT_FAILURE;
    int arg;
    std::ifstream input_f;
    std::ofstream output_f;
    const char* input = NULL;
    const char* output = NULL;
    const char* name = "data";
    
    // & f0 = Collums till a line, '0' means no line change;
    // & 0f = Flags: Decimal, End Var., Size Var., Zero Terminate;
    uint8_t f_col_desz = 0;

    for (arg = 1; arg < argc; ++arg)
    {
        if (strcmp(argv[arg], "-h") == 0)
        {
            print_help(argv[0]);
            return EXIT_SUCCESS;
        }
        else if (strcmp(argv[arg], "-i") == 0)
        {
            if (++arg >= argc)
            {
                fprintf(stderr, "Missing argument for -i\n");
                return EXIT_FAILURE;
            }
            input = argv[arg];
        }
        else if (strcmp(argv[arg], "-o") == 0)
        {
            if (++arg >= argc)
            {
                fprintf(stderr, "Missing argument for -o\n");
                return EXIT_FAILURE;
            }
            output = argv[arg];
        }
        else if (strcmp(argv[arg], "-n") == 0)
        {
            if (++arg >= argc)
            {
                fprintf(stderr, "Missing argument for -n\n");
                return EXIT_FAILURE;
            }
            name = argv[arg];
        }
        else if (strcmp(argv[arg], "-z") == 0)
            f_col_desz |= 1;
        else if (strcmp(argv[arg], "-s") == 0)
            f_col_desz |= 2;
        else if (strcmp(argv[arg], "-e") == 0)
            f_col_desz |= 4;
        else if (strcmp(argv[arg], "-d") == 0)
            f_col_desz |= 8;
        else if (strcmp(argv[arg], "-c") == 0)
        {
            //char* temp = NULL;
            char* temp = NULL;
            if (++arg >= argc)
            {
                fprintf(stderr, "Missing argument for -c\n");
                return EXIT_FAILURE;
            }
            temp = argv[arg];
            if (temp[0] > 47 && temp[0] < 58) { f_col_desz |= (temp[0] - 48) << 4; }
            else if (temp[0] > 64 && temp[0] < 72) { f_col_desz |= (temp[0] - 55) << 4; }
            else if (temp[0] > 96 && temp[0] < 104) { f_col_desz |= (temp[0] - 87) << 4; }
            else { f_col_desz &= 15; }
        }
        else
        {
            fprintf(stderr, "Invalid argument: %s\n", argv[arg]);
            return EXIT_FAILURE;
        }
    }

    if (input)
    {
        input_f.open(input, std::ios::binary);

        if (!input_f.is_open())
        {
            fprintf(stderr, "Failed to open input file\n");
            goto exit;
        }
    }

    if (output)
    {
        output_f.open(output);

        if (!output_f.is_open())
        {
            fprintf(stderr, "Failed to open output file\n");
            goto exit;
        }
    }

    if (input_f.is_open() && output_f.is_open())
    {
        generate_output(&input_f, &output_f, name, f_col_desz);
    }
    result = EXIT_SUCCESS;
exit:
    if (input_f.is_open()) { input_f.close(); }
    if (output_f.is_open()) { output_f.close(); }

    return result;
}
