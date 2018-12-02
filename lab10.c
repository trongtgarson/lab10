#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "myheader.h"
#include "lab10.h"

int main(){

  display_startup_banner();

  full_data data;
  int rows = read_bubble_response_data("full_data.csv", data);
  bubble_response_selection_sort(data, rows);
  printf("read %d rows of bubble response data\n", rows);

  Command command;
  int should_quit = 0;
  do {
    command = prompt_for_command();
    switch(command) {
      case DisplayId:
        command_display_id(data, rows);
        break;

      case DisplayName:
        command_display_name(data, rows);
        break;

      case DisplayRange:
        command_display_range(data, rows);
        break;

      case DisplayLastName:
        command_display_last_name(data, rows);
        break;

      case Quit:
        should_quit = 1;
        break;

      default:
        printf("Invalid option\n");
    };
    printf("\n");
  } while(!should_quit);

  printf("Goodbye\n\n");
}


/*
 * Function:    display_startup_banner
 * Description: Prints out the programmer's name, date and time of compilation
 * Author:      Trong Garson
 * Date:        December 2, 2018
 * Input:       None
 * Output:      None
 * Globals:     PRIME_PROGRAMMER, __DATE__ and __TIME__
 * Returns:     None
 */
void display_startup_banner (){
  printf("DATA MINING MERGED OMR FORM AND STUDENT DATA PROGRAM WRITTEN IN C\n\n");
  printf("Program written by %s.\n", PRIME_PROGRAMMER);
  printf("Program compiled on %s at %s\n\n", __DATE__,__TIME__);

  return;
}

/*
 * Function:    read_bubble_response_data
 * Description: Reads the bubble_response data from the given filename
 * Author:      Trong Garson
 * Date:        December 2, 2018
 * Input:       filename
 * Output:      data
 * Globals:     MAX_RESPONSE_COUNT
 * Returns:     number of bubble_response_data read
 */
int read_bubble_response_data(const char *filename, full_data data){
  FILE *data_file = fopen(filename, "r");

  clear_row(data_file);

  int row;
  for(row = 0; row < MAX_RESPONSE_COUNT; row++){
    if(!read_next_bubble_response(data_file, &data[row])) {
      break;
    } 
    //else {
    //  print_bubble_response(&data[row]);
    //}
  }

  return row;
}

/*
 * Function:    clear_row
 * Description: reads all characters to the end of a line in the file
 *              used to throw away file contents we don't need
 * Author:      Trong Garson
 * Date:        December 2, 2018
 * Input:       data_file
 * Output:      None
 * Globals:     None
 * Returns:     None
 */
void clear_row(FILE *data_file) {
  char c = 0;
  do {
    fscanf(data_file, "%c", &c);
  } while(c != '\n');
}

/*
 * Function:    read_next_bubble_response
 * Description: reads a single line from the given file and puts it into a bubble_response
 * Author:      Trong Garson
 * Date:        December 2, 2018
 * Input:       full_data
 * Output:      response
 * Globals:     MAX_NAME_SIZE, NUMBER_OF_QUESTIONS
 * Returns:     value indicating success or failure in reading a row
 */
int read_next_bubble_response(FILE *full_data, bubble_response *response) {

  // setup variables for scanning the row
  int result;
  char current = 0;
  int i;

  // these arrays contain garbage and need to be cleared to a good known state before use
  memset(response->full_name, 0, sizeof(response->full_name));
  memset(response->responses, 0, sizeof(response->responses));

  // read the id and first comma
  result = fscanf(full_data, "%d,", &(response->id));
  if(result != 1) { return 0; }

  // read the name and second comma
  for(i = 0; i < MAX_NAME_SIZE; i++){
    result = fscanf(full_data, "%c", &current);
    if(result == 1 && current != ',') {
      response->full_name[i] = current;
    } else if(current == ',') {
      // we hit the end of this field and should continue
      break;
    } else {
      // we encountered a scan error or EOF, return failure
      return 0;
    }
  } 

  // read the response data and separating commas
  for(i = 0; i < NUMBER_OF_QUESTIONS; i++) {
    result = fscanf(full_data, "%c,", &(response->responses[i]));
    if(result != 1) { return 0; }
  }

  clear_row(full_data);

  return 1;
}

/*
 * Function:    bubble_response_selection_sort
 * Description: performs selection sort on the data, sorting them by id
 *              based on the implementation from the textbook
 * Author:      Trong Garson
 * Date:        December 2, 2018
 * Input:       data, rows
 * Output:      data
 * Globals:     None
 * Returns:     None
 */
void bubble_response_selection_sort(full_data data, int rows){
  int fill,
      index_of_min;
  bubble_response temp;

  for(fill = 0; fill < rows; fill++){
    index_of_min = bubble_response_selection_sort_min_range(data, fill, rows - 1);

    if(fill != index_of_min) {
      temp = data[index_of_min];
      data[index_of_min] = data[fill];
      data[fill] = temp;
    }
  } 
}

/*
 * Function:    bubble_response_selection_sort_min_range
 * Description: helper method for bubble_response_selection_sort
 *              finds the index of the smallest element in the given range in data 
 * Author:      Trong Garson
 * Date:        December 2, 2018
 * Input:       data, first, last
 * Output:      None
 * Globals:     None
 * Returns:     index of the smallest element in data[first] through data[last] 
 */
int bubble_response_selection_sort_min_range(full_data data, int first, int last){
  int smallest_value_index = first;
  int i;
  for(i = first + 1; i <= last; i++) {
    if(data[i].id < data[smallest_value_index].id) {
      smallest_value_index = i;
    }
  }
  return smallest_value_index;
}

/*
 * Function:    prompt_for_command
 * Description: Displays a menu to the user and returns which option they chose
 * Author:      Trong Garson
 * Date:        December 2, 2018
 * Input:       None
 * Output:      None
 * Globals:     CommandStrings
 * Returns:     Command - enum entry representing which command the user picked
 */
Command prompt_for_command() {
  printf("Bubble Sheet Results Data Mining Menu\n");
  int command;
  for(command = DisplayId; command <= Quit; command++) {
    printf("%d. %s\n", command, CommandStrings[command]);
  }
  printf("\n");
  printf("Enter Choice [1-5]: ");

  command = Invalid;
  int result = scanf("%d", &command);
  printf("\n");
  if(result != 1) {
    clear_stdin();
    return command;
  }

  if(command >= DisplayId && command <= Quit) {
    return command;
  } else {
    return Invalid;
  }
}

/*
 * Function:    command_display_id
 * Description: Implementation for the "Display specific id" command
 *              displays the bubble_response for the specific id
 * Author:      Trong Garson
 * Date:        December 2, 2018
 * Input:       data, rows 
 * Output:      None
 * Globals:     CommandStrings
 * Returns:     None
 */
void command_display_id(full_data data, int rows){
  printf("%s\n", CommandStrings[DisplayId]);
  printf("Enter id: ");
  int id = 0;
  int found = 0;
  if(scanf("%d", &id) == 1) {
    int i;
    for(i = 0; i < rows; i++) {
      if(data[i].id == id) {
        found = 1;
        print_bubble_response(&data[i]);
      }
    }

    if(!found) {
      printf("No entry found for id %d\n", id);
    }

  } else {
    clear_stdin();
  }

  printf("\n");
}

/*
 * Function:    command_display_name
 * Description: Implementation for the "Display specific name" command
 *              displays the bubble_response for any entries where the full_name
 *              contains an entered search string (case insensitive)
 * Author:      Trong Garson
 * Date:        December 2, 2018
 * Input:       data, rows
 * Output:      None
 * Globals:     CommandStrings, MAX_NAME_SIZE
 * Returns:     None
 */
void command_display_name(full_data data, int rows){
  printf("%s\n", CommandStrings[DisplayName]);
  printf("Enter name: ");
  char search_name[MAX_NAME_SIZE] = {0};
  scanf("%s", search_name);

  int i;
  int match_count = 0;
  for(i = 0; i < rows; i++){
    if(strcasestr(data[i].full_name, search_name)){
      match_count++;
      print_bubble_response(&data[i]);
    }
  }
  if(0 == match_count) {
    printf("No names matched '%s'\n", search_name);
  }

  printf("\n");
}

/*
 * Function:    command_display_range
 * Description: Implementation of the "Display range of ids" command
 *              displays the bubble_response for upto ID_RANGE_MAX_RESULTS entries in data
 *              which have id values in a user entered range
 * Author:      Trong Garson
 * Date:        December 2, 2018
 * Input:       data, rows
 * Output:      None
 * Globals:     CommandStrings, ID_RANGE_MAX_SIZE, ID_RANGE_MAX_RESULTS
 * Returns:     None
 */
void command_display_range(full_data data, int rows){
  printf("%s\n", CommandStrings[DisplayRange]);
  printf("Enter first id: ");
  int range_start = 0,
      range_end = 0;
  int result = scanf("%d", &range_start);
  if(1 != result) {
    printf("Error - invalid id\n");
    clear_stdin();
    return;
  }

  printf("Enter second id (must be within %d of first id): ", ID_RANGE_MAX_SIZE);
  result = scanf("%d", &range_end);
  if(1 != result) {
    printf("Error - invalid id\n");
    return;
  }

  // make sure range_start <= range_end
  if(range_end < range_start) {
    int temp = range_end;
    range_end = range_start;
    range_start = temp;
  }

  if(range_end - range_start > ID_RANGE_MAX_SIZE) {
    printf("Error - search range too large. Please limit to %d ids\n", ID_RANGE_MAX_SIZE);
    return;
  }

  bubble_response range[ID_RANGE_MAX_RESULTS];
  int range_size = get_range(range_start, range_end, range, data, rows);
  int i;
  for(i = 0; i < range_size; i++){
    print_bubble_response(&range[i]);
  }
  if(0 == range_size) {
    printf("No results found in the range (%d-%d)\n", range_start, range_end);
  }
  printf("\n");
}

/*
 * Function:    get_range
 * Description: searches the data for elements with an id in the given range
 * Author:      Trong Garson
 * Date:        December 2, 2018
 * Input:       low, high, data, rows
 * Output:      ret
 * Globals:     ID_RANGE_MAX_RESULTS
 * Returns:     number of elements returned in the ret output
 */
int get_range(int low, int high, bubble_response ret[ID_RANGE_MAX_RESULTS], full_data data, int rows) {
  int i;
  int low_index = 0,
      high_index = 0;
  bubble_response current = data[0];

  for(i = 0; i < rows; i++){
    if(data[i].id >= low) { 
      low_index = i; 
      break;
    }
  }

  for(i = rows - 1; i >= 0; i--) {
    if(data[i].id <= high) { 
      high_index = i; 
      break;
    }
  }

  int range_size = (high_index - low_index) + 1;
  if(range_size > ID_RANGE_MAX_RESULTS) {
    printf("Found more than %d items in the search range (%d - %d)\n", ID_RANGE_MAX_RESULTS, low, high);
    range_size = 5;
  }

  for(i = 0; i < range_size; i++) {
    ret[i] = data[low_index + i];
  }

  return range_size;
}

/*
 * Function:    command_display_last_name
 * Description: Implementation for the "Display last name" command
 *              displays entries where last_name contains an 
 *              entered search string (case insensitive)
 * Author:      Trong Garson
 * Date:        December 2, 2018
 * Input:       data, rows
 * Output:      None
 * Globals:     CommandStrings
 * Returns:     MAX_NAME_SIZE
 */
void command_display_last_name(full_data data, int rows){
  printf("%s\n", CommandStrings[DisplayLastName]);
  printf("Enter last name: ");
  char search_name[MAX_NAME_SIZE] = {0};
  scanf("%s", search_name);

  int i;
  int match_count = 0;
  int name_length = 0;
  int last_name_start = 0;
  for(i = 0; i < rows; i++){
    name_length = strlen(data[i].full_name);
    // start at the end and find the last ' ' character -- this will be the
    // beginning of the last name
    for(last_name_start = name_length - 1; last_name_start >= 0; last_name_start--){
      if(' ' == data[i].full_name[last_name_start]) { break; }
    }

    if(-1 == last_name_start) { continue; } //we didn't find a space?

    if(strcasestr(&(data[i].full_name[last_name_start]), search_name)){
      match_count++;
      print_bubble_response(&data[i]);
    }
  }

  if(0 == match_count) {
    printf("No last names matched '%s'\n", search_name);
  }

}

/*
 * Function:    print_bubble_response
 * Description: prints a bubble_response
 * Author:      Trong Garson
 * Date:        December 2, 2018
 * Input:       bubble_response
 * Output:      None
 * Globals:     NUMBER_OF_QUESTIONS
 * Returns:     None
 */
void print_bubble_response(const bubble_response *response) {
  printf("ID: %05d  Name: %s\n\t", response->id, response->full_name);
  int i;
  for(i = 0; i < NUMBER_OF_QUESTIONS; i++) {
    printf("%c", response->responses[i]);
  }
  printf("\n");
}

/*
 * Function:    clear_stdin
 * Description: Reads characters from stdin until a new line is 
 *              reached in order to clear out bad data and prevent
 *              infinite looping on scanf
 * Author:      Trong Garson
 * Date:        December 2, 2018
 * Input:       None
 * Output:      None
 * Globals:     None
 * Returns:     None
 */
void clear_stdin() { while(getchar() != '\n'); }

