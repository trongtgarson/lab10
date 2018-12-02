#ifndef LAB10_H
#define LAB10_H

#define MAX_NAME_SIZE 256
#define NUMBER_OF_QUESTIONS 11
#define MAX_RESPONSE_COUNT 256

#define ID_RANGE_MAX_SIZE 3000
#define ID_RANGE_MAX_RESULTS 5

// struct representing a single row of the data file
typedef struct {
  int id;
  char full_name[ MAX_NAME_SIZE + 1 ];
  char responses[ NUMBER_OF_QUESTIONS ];
} bubble_response;

// type representing the entire contents of the data file -- an array of bubble_respones
typedef bubble_response full_data[MAX_RESPONSE_COUNT];

// enum representing available menu commands 
typedef enum Command {
  Invalid = -1,
  DisplayId = 1,
  DisplayName = 2,
  DisplayRange = 3,
  DisplayLastName = 4,
  Quit = 5,
} Command;

// human readable descriptions of the available menu commands
const char* const CommandStrings[] = {
  "", // skip index 0 since the first command is 1
  "Display specific id",
  "Display specific name",
  "Display range of ids",
  "Display last name",
  "End the program"
};

void display_startup_banner();

// read and prepare the file contents
int read_bubble_response_data(const char *filename, full_data data);
void clear_row(FILE *data_file); 
int read_next_bubble_response(FILE *data_file, bubble_response *response);
void bubble_response_selection_sort(full_data data, int rows);
int bubble_response_selection_sort_min_range(full_data data, int first, int last);

// ask the user what they want to do and then do it
Command prompt_for_command();
void command_display_id(full_data data, int rows);
void command_display_name(full_data data, int rows);
void command_display_range(full_data data, int rows);
int get_range(int low, int high, bubble_response ret[ID_RANGE_MAX_RESULTS], full_data data, int rows);
void command_display_last_name(full_data data, int rows);

void print_bubble_response(const bubble_response *response);
void clear_stdin();

#endif // LAB10_H
