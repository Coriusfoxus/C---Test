#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

#define NUM_LINES   4
#define NUM_COLUMNS 3
#define COLUMN_WIDTH 20

void PrintTableHeader();
void PrintTableBody();

int main()
{
  PrintTableHeader();
  PrintTableBody();
  
  return 0;
}

void PrintTableBody()
{
  ifstream input("table-data.txt");

  /* Error checking should be in a real program */
  // for(int k = 0; k < NUM_LINES; k++)
  //   {
  //     /* Do the dumb things you gotta do */
  //     int intValue;
  //     double doubleValue;
  //     input >> intValue >> doubleValue;
      
  //     cout << setw(COLUMN_WIDTH - 1) << (k + 1) << "|";
  //     cout << setw(COLUMN_WIDTH - 1) << intValue << "|";
  //     cout << setw(COLUMN_WIDTH) << doubleValue << endl;
  //   }

  int rowNum = 0;
  while( true )
    {
      int intValue;
      double doubleValue;

      rowNum++;
      input >> intValue >> doubleValue;

      if( true == input.fail() )
	{
	  /* Bad read. Time to mosey on out of this loop */
	  break;
	}

      cout << setw(COLUMN_WIDTH - 1) << (rowNum) << "|";
      cout << setw(COLUMN_WIDTH - 1) << intValue << "|";
      cout << setw(COLUMN_WIDTH) << doubleValue << endl;


    }
}

void PrintTableHeader()
{

  for( int ii = 0; ii < NUM_COLUMNS; ii++ )
    {
      cout << setfill('-') << setw(COLUMN_WIDTH) << '+';
    }
  cout << endl << setfill(' ');

}
