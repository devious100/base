
#include "ogoid++.h"

#include "cheats/cheats.h"
#include "misc/encrypted_strings.h"

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;


#define PRINTERROR(cause) {	LOGMSG( "Error at line " << line_num << ": " << cause ); return FALSE; }



// a string tokenizer taken from
// http://www.linuxselfhelp.com/HOWTO/C++Programming-HOWTO-7.html
void Tokenize(const string& str,
		  vector<string>& tokens,
		  const string& delimiters = " ")
{
	// Skip delimiters at beginning.
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	string::size_type pos     = str.find_first_of(delimiters, lastPos);

	while (string::npos != pos || string::npos != lastPos)
	{
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
}





//////////////////////////////////////////////////////////////////////////
BOOL Configure(void)
{

	ifstream file(config_file.c_str());
	if(file.fail())
	{
		LOGMSG( "Can't open config file, keeping defaults." );
		return TRUE;
	}

	LOGMSG( "Loading config: " << config_file << "..." );

	char			line[256];
	unsigned int	line_num = 0;

	istringstream stream;

	EncryptedString estring;

	string	command;
	int		param_int;
	float	param_float;
	string	param_string;

	unsigned int	param_num;


	BOOL							found_command_owner;
	vector<Cheat*>::iterator		cheat_iter;
	vector<ConfigParam>::iterator	param_iter;

	while(1)
	{
		file.getline(line,256);
		if(file.eof())
			break;

		line_num++;

		stream.clear();
		stream.str(line);

		if(!(stream >> command) || command[0] == ';')
			continue;

		transform(command.begin(), command.end(), command.begin(), tolower);

		found_command_owner = FALSE;

		for(cheat_iter = cheat_list.begin();
			!found_command_owner &&
			cheat_iter != cheat_list.end();
			cheat_iter++
			)
		{
			if(!estring.Decrypt((*cheat_iter)->encrypted_name))
				continue;
			transform(estring.GetString()->begin(), estring.GetString()->end(), estring.GetString()->begin(), tolower);

			if(command == *estring.GetString())
			{
				found_command_owner = TRUE;

				for(param_num = 0,
					param_iter = (*cheat_iter)->config_parameters.begin();
					param_iter != (*cheat_iter)->config_parameters.end();
					param_num++,
					param_iter++
					)
				{

					switch(param_iter->type)
					{
					case ConfigParam::param_bool:
						if(!(stream >> param_string))
							PRINTERROR("expected a bool as argument " << param_string);

						transform(param_string.begin(),
								  param_string.end(),
								  param_string.begin(),
								  toupper
								  );

						if(param_string == "1"
							|| param_string == "ON"
							|| param_string == "ENABLE"
							|| param_string == "ENABLED"
							|| param_string == "TRUE"
							)
							*((BOOL*) param_iter->address) = TRUE;

						else if( param_string == "0"
							|| param_string == "OFF"
							|| param_string == "DISABLE"
							|| param_string == "DISABLED"
							|| param_string == "FALSE"
							)
							*((BOOL*) param_iter->address) = FALSE;

						else
							PRINTERROR("expected a bool as argument " << param_string);

						break;

					case ConfigParam::param_int:
						if(!(stream >> param_int))
							PRINTERROR("expected an integer as argument " << param_num);

						*((int*) param_iter->address) = param_int;
						break;

					case ConfigParam::param_float:
						if(!(stream >> param_float))
							PRINTERROR("expected a float as argument " << param_num);

						*((float*) param_iter->address) = param_float;
						break;

					case ConfigParam::param_vkey:
						if(!(stream >> param_string))
							PRINTERROR("expected a string as argument " << param_num);

						transform(param_string.begin(),
								  param_string.end(),
								  param_string.begin(),
								  toupper
								  );

						if(param_string.substr(0,3) == "VK_")
							param_string= param_string.substr(3,param_string.length()-3);

						if(param_string == "NONE"
							|| param_string == "DISABLE"
							|| param_string == "DISABLED"
							)
							*((int*) param_iter->address) = 0;

						else if(param_string == "LBUTTON")
							*((int*) param_iter->address) = VK_LBUTTON;

						else if(param_string == "RBUTTON")
							*((int*) param_iter->address) = VK_RBUTTON;

						else if(param_string == "CANCEL")
							*((int*) param_iter->address) = VK_CANCEL;

						else if(param_string == "MBUTTON")
							*((int*) param_iter->address) = VK_MBUTTON;

						else if(param_string == "BACK")
							*((int*) param_iter->address) = VK_BACK;

						else if(param_string == "TAB")
							*((int*) param_iter->address) = VK_TAB;

						else if(param_string == "CLEAR")
							*((int*) param_iter->address) = VK_CLEAR;

						else if(param_string == "RETURN")
							*((int*) param_iter->address) = VK_RETURN;

						else if(param_string == "SHIFT")
							*((int*) param_iter->address) = VK_SHIFT;

						else if(param_string == "CONTROL")
							*((int*) param_iter->address) = VK_CONTROL;

						else if(param_string == "MENU")
							*((int*) param_iter->address) = VK_MENU;

						else if(param_string == "PAUSE")
							*((int*) param_iter->address) = VK_PAUSE;

						else if(param_string == "CAPITAL")
							*((int*) param_iter->address) = VK_CAPITAL;

						else if(param_string == "ESCAPE")
							*((int*) param_iter->address) = VK_ESCAPE;

						else if(param_string == "SPACE")
							*((int*) param_iter->address) = VK_SPACE;

						else if(param_string == "PRIOR")
							*((int*) param_iter->address) = VK_PRIOR;

						else if(param_string == "NEXT")
							*((int*) param_iter->address) = VK_NEXT;

						else if(param_string == "END")
							*((int*) param_iter->address) = VK_END;

						else if(param_string == "HOME")
							*((int*) param_iter->address) = VK_HOME;

						else if(param_string == "LEFT")
							*((int*) param_iter->address) = VK_LEFT;

						else if(param_string == "UP")
							*((int*) param_iter->address) = VK_UP;

						else if(param_string == "RIGHT")
							*((int*) param_iter->address) = VK_RIGHT;

						else if(param_string == "DOWN")
							*((int*) param_iter->address) = VK_DOWN;

						else if(param_string == "SELECT")
							*((int*) param_iter->address) = VK_SELECT;

						else if(param_string == "PRINT")
							*((int*) param_iter->address) = VK_PRINT;

						else if(param_string == "EXECUTE")
							*((int*) param_iter->address) = VK_EXECUTE;

						else if(param_string == "SNAPSHOT")
							*((int*) param_iter->address) = VK_SNAPSHOT;

						else if(param_string == "INSERT")
							*((int*) param_iter->address) = VK_INSERT;

						else if(param_string == "DELETE")
							*((int*) param_iter->address) = VK_DELETE;

						else if(param_string == "HELP")
							*((int*) param_iter->address) = VK_HELP;

						else if(param_string == "0")
							*((int*) param_iter->address) = 0x30;

						else if(param_string == "1")
							*((int*) param_iter->address) = 0x31;

						else if(param_string == "2")
							*((int*) param_iter->address) = 0x32;

						else if(param_string == "3")
							*((int*) param_iter->address) = 0x33;

						else if(param_string == "4")
							*((int*) param_iter->address) = 0x34;

						else if(param_string == "5")
							*((int*) param_iter->address) = 0x35;

						else if(param_string == "6")
							*((int*) param_iter->address) = 0x36;

						else if(param_string == "7")
							*((int*) param_iter->address) = 0x37;

						else if(param_string == "8")
							*((int*) param_iter->address) = 0x38;

						else if(param_string == "9")
							*((int*) param_iter->address) = 0x39;

						else if(param_string == "A")
							*((int*) param_iter->address) = 0x41;

						else if(param_string == "B")
							*((int*) param_iter->address) = 0x42;

						else if(param_string == "C")
							*((int*) param_iter->address) = 0x43;

						else if(param_string == "D")
							*((int*) param_iter->address) = 0x44;

						else if(param_string == "E")
							*((int*) param_iter->address) = 0x45;

						else if(param_string == "F")
							*((int*) param_iter->address) = 0x46;

						else if(param_string == "G")
							*((int*) param_iter->address) = 0x47;

						else if(param_string == "H")
							*((int*) param_iter->address) = 0x48;

						else if(param_string == "I")
							*((int*) param_iter->address) = 0x49;

						else if(param_string == "J")
							*((int*) param_iter->address) = 0x4a;

						else if(param_string == "K")
							*((int*) param_iter->address) = 0x4b;

						else if(param_string == "L")
							*((int*) param_iter->address) = 0x4c;

						else if(param_string == "M")
							*((int*) param_iter->address) = 0x4d;

						else if(param_string == "N")
							*((int*) param_iter->address) = 0x4e;

						else if(param_string == "O")
							*((int*) param_iter->address) = 0x4f;

						else if(param_string == "P")
							*((int*) param_iter->address) = 0x50;

						else if(param_string == "Q")
							*((int*) param_iter->address) = 0x51;

						else if(param_string == "R")
							*((int*) param_iter->address) = 0x52;

						else if(param_string == "S")
							*((int*) param_iter->address) = 0x53;

						else if(param_string == "T")
							*((int*) param_iter->address) = 0x54;

						else if(param_string == "U")
							*((int*) param_iter->address) = 0x55;

						else if(param_string == "V")
							*((int*) param_iter->address) = 0x56;

						else if(param_string == "W")
							*((int*) param_iter->address) = 0x57;

						else if(param_string == "X")
							*((int*) param_iter->address) = 0x58;

						else if(param_string == "Y")
							*((int*) param_iter->address) = 0x59;

						else if(param_string == "Z")
							*((int*) param_iter->address) = 0x5a;

						else if(param_string == "NUMPAD0")
							*((int*) param_iter->address) = VK_NUMPAD0;

						else if(param_string == "NUMPAD1")
							*((int*) param_iter->address) = VK_NUMPAD1;

						else if(param_string == "NUMPAD2")
							*((int*) param_iter->address) = VK_NUMPAD2;

						else if(param_string == "NUMPAD3")
							*((int*) param_iter->address) = VK_NUMPAD3;

						else if(param_string == "NUMPAD4")
							*((int*) param_iter->address) = VK_NUMPAD4;

						else if(param_string == "NUMPAD5")
							*((int*) param_iter->address) = VK_NUMPAD5;

						else if(param_string == "NUMPAD6")
							*((int*) param_iter->address) = VK_NUMPAD6;

						else if(param_string == "NUMPAD7")
							*((int*) param_iter->address) = VK_NUMPAD7;

						else if(param_string == "NUMPAD8")
							*((int*) param_iter->address) = VK_NUMPAD8;

						else if(param_string == "NUMPAD9")
							*((int*) param_iter->address) = VK_NUMPAD9;

						else if(param_string == "SEPARATOR")
							*((int*) param_iter->address) = VK_SEPARATOR;

						else if(param_string == "SUBTRACT")
							*((int*) param_iter->address) = VK_SUBTRACT;

						else if(param_string == "DECIMAL")
							*((int*) param_iter->address) = VK_DECIMAL;

						else if(param_string == "DIVIDE")
							*((int*) param_iter->address) = VK_DIVIDE;

						else if(param_string == "F1")
							*((int*) param_iter->address) = VK_F1;

						else if(param_string == "F2")
							*((int*) param_iter->address) = VK_F2;

						else if(param_string == "F3")
							*((int*) param_iter->address) = VK_F3;

						else if(param_string == "F4")
							*((int*) param_iter->address) = VK_F4;

						else if(param_string == "F5")
							*((int*) param_iter->address) = VK_F5;

						else if(param_string == "F6")
							*((int*) param_iter->address) = VK_F6;

						else if(param_string == "F7")
							*((int*) param_iter->address) = VK_F7;

						else if(param_string == "F8")
							*((int*) param_iter->address) = VK_F8;

						else if(param_string == "F9")
							*((int*) param_iter->address) = VK_F9;

						else if(param_string == "F10")
							*((int*) param_iter->address) = VK_F10;

						else if(param_string == "F11")
							*((int*) param_iter->address) = VK_F11;

						else if(param_string == "F12")
							*((int*) param_iter->address) = VK_F12;

						else if(param_string == "F13")
							*((int*) param_iter->address) = VK_F13;

						else if(param_string == "F14")
							*((int*) param_iter->address) = VK_F14;

						else if(param_string == "F15")
							*((int*) param_iter->address) = VK_F15;

						else if(param_string == "F16")
							*((int*) param_iter->address) = VK_F16;

						else if(param_string == "F17")
							*((int*) param_iter->address) = VK_F17;

						else if(param_string == "F18")
							*((int*) param_iter->address) = VK_F18;

						else if(param_string == "F19")
							*((int*) param_iter->address) = VK_F19;

						else if(param_string == "F20")
							*((int*) param_iter->address) = VK_F20;

						else if(param_string == "F21")
							*((int*) param_iter->address) = VK_F21;

						else if(param_string == "F22")
							*((int*) param_iter->address) = VK_F22;

						else if(param_string == "F23")
							*((int*) param_iter->address) = VK_F23;

						else if(param_string == "F24")
							*((int*) param_iter->address) = VK_F24;

						else if(param_string == "NUMLOCK")
							*((int*) param_iter->address) = VK_NUMLOCK;

						else if(param_string == "SCROLL")
							*((int*) param_iter->address) = VK_SCROLL;

						else if(param_string == "LSHIFT")
							*((int*) param_iter->address) = VK_LSHIFT;

						else if(param_string == "RSHIFT")
							*((int*) param_iter->address) = VK_RSHIFT;

						else if(param_string == "LCONTROL")
							*((int*) param_iter->address) = VK_LCONTROL;

						else if(param_string == "RCONTROL")
							*((int*) param_iter->address) = VK_RCONTROL;

						else if(param_string == "LMENU")
							*((int*) param_iter->address) = VK_LMENU;

						else if(param_string == "RMENU")
							*((int*) param_iter->address) = VK_RMENU;

						else if(param_string == "PLAY")
							*((int*) param_iter->address) = VK_PLAY;

						else if(param_string == "ZOOM")
							*((int*) param_iter->address) = VK_ZOOM;

						else
							PRINTERROR("unknown virtual key: " << param_string);

						// alt key has a special function
						if(*((int*) param_iter->address) == VK_MENU)
							PRINTERROR("the ALT key can't be used as a hotkey");

						break;
					}


				}
				if(! (*cheat_iter)->Configure())
					PRINTERROR( command << "failed to initialize. bad values as parameters?");
			}
		}

		if(!found_command_owner)
			PRINTERROR("unknown command \"" << command << "\"");
		
		if(stream >> param_string && param_string[0] != ';')
			PRINTERROR("excess parameters to command \"" << command << "\"");

	}

	return TRUE;
}

