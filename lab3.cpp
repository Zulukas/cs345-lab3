//C libraries
#include <signal.h> /* signal, raise, sig_atomic_t */
#include <stdio.h>
#include <unistd.h> /* pause */
#include <wait.h>

//C++ libraries
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <list>
#include <sstream>

using namespace std;

#define BUFFER_SIZE 50

#define MAX_LINE 81

#define HIST_SIZE 10

#define MAX_NUM_CMDS 1000

/******************************************************************************
* TEMPLATE BOX
******************************************************************************/

class History
{
public:
	History()
	{
		location = 0;
		MAX_SIZE = HIST_SIZE;
		OBJ_MAX_NUM_CMDS = MAX_NUM_CMDS;

		commands = new string[this->MAX_SIZE];
	}

	~History()
	{
		delete[] commands;
	}

	void push(const string& command)
	{		
		//Check to see if the last command is the same as the command to push on
		//If it is, don't push it on (this emulates BASH)
		if (location != 0 && commands[(location - 1) % MAX_SIZE] == command)
			return;

		//Rewrite this location with the command.
		commands[location % MAX_SIZE] = command;

		if (location + 1 < OBJ_MAX_NUM_CMDS) //Prevents location from equaling MAX_NUM_CMDS
		{
			location++;
		}
	}

	bool findCommand(const char c, string& stringFound) const
	{
		stringFound = "";

		if (c == '\0') //If c is the NULL character, return the most recent command.
		{
			if (location)
			{
				stringFound = commands[location % MAX_SIZE];
				return true;
			}
			else
			{
				return false;
			}
		}

		int start = (location - MAX_SIZE) ? (0) : (start - MAX_SIZE);
		int end = (start == 0) ? (MAX_SIZE) : (location);

		for (int i = start; i < end && i < location; i++)
		{
			if (commands[i % MAX_SIZE][0] == c)
			{
				stringFound = commands[i % MAX_SIZE];
				return true;
			}
		}

		//Iterate a number of times equal to MAX_SIZE
		//Have a second 'pointer' variable to check our array of string commands
		// for (unsigned short count = 0, i = location % MAX_SIZE; 
		// 	 count < MAX_SIZE; 
		// 	 ++count, ++i % MAX_SIZE) //Increment count, increment i and mod it by MAX_SIZE
		// {
		// 	if (commands[i][0] == c) //Check the first character
		// 	{
		// 		stringFound = commands[i];
		// 		return true;
		// 	}
		// }

		//No match found
		return false;
	}

	void display() const
	{
		int start = (location - MAX_SIZE < 0) ? (0) : (location - MAX_SIZE);
		int end = (start == 0) ? (MAX_SIZE) : (location);

		for (int i = start; i < end && i < location; i++)
		{
			cout << " " << setw(3) << i << "  " << commands[i % MAX_SIZE] << endl;
		}
	}

private:
	string* commands;
	unsigned short location;
	unsigned short MAX_SIZE;
	unsigned short OBJ_MAX_NUM_CMDS;
};

/******************************************************************************
* Add a command to the history.  If the number of command exceeds HIST_SIZE, 
* then the oldest command is replaced with the newest.
******************************************************************************/


int main(int argc, char** argv)
{
	History history;

	for (int i = 0; i < 15; i++)
		history.push("ip -r");

	history.push("ASDF");
	history.push("ip -r");
	history.push("ASDF");
	history.push("ip -r");history.push("ASDF");
	history.push("ip -r");history.push("ASDF");
	history.push("ip -r");history.push("ASDF");
	history.push("ip -r");history.push("ASDF");
	history.push("ip -r");history.push("ASDF");
	history.push("ip -r");history.push("ASDF");
	history.push("ip -r");

	string last;
	history.findCommand('\0', last);

	cout << "The last string was: " << last << endl;

	history.display();
	
	return 0;
}
