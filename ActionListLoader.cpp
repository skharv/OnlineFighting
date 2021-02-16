#include "ActionListLoader.h"

std::vector<Action*> ActionListLoader::ReadList(std::string Filepath)
{
	std::vector<Action*> Actions;

	std::ifstream file = std::ifstream(Filepath, std::ifstream::in);

	Action* newAction = new Action();

	std::string line;
	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		
		if (line[0] == '#')
			continue;

		if (line.find('[', 0) != std::string::npos)
		{
			if (line == "[End Action]")
			{
				Actions.push_back(newAction);
				continue;
			}

			newAction = new Action();

			int startIndex = line.find(' ') + 1;
			int endIndex = line.find(']');
			std::string *actionName = new std::string(line.substr(startIndex, endIndex - startIndex));
			newAction->name = actionName->c_str();
		}

		if (line.find("COMM", 0) != std::string::npos)
		{
			int direction = 0;
			std::vector<std::string> directions = std::vector<std::string>();
			std::vector<std::string> buttons = std::vector<std::string>();
			std::string button = "";

			if (line.find(',') != std::string::npos)
			{
				int startIndex = line.find(' ') + 1;
				int comma = line.find(',') + 1;

				try
				{
					direction = std::stoi(line.substr(startIndex, (comma - startIndex) - 1));
				}
				catch (std::invalid_argument const& e)
				{
					std::cout << "nah bruh...";
					button = line.substr(startIndex, (comma - startIndex) - 1);

				}

				try
				{
					direction = std::stoi(line.substr(comma + 1, line.length() - (comma + 1)));
				}
				catch (std::invalid_argument const& e)
				{
					std::cout << "no go bro...";
					button = line.substr(comma + 1, line.length() - (comma + 1));
				}
			}
			else
			{
				int startIndex = line.find(' ') + 1;

				try
				{
					direction = std::stoi(line.substr(startIndex, line.length() - startIndex));
				}
				catch (std::invalid_argument const& e)
				{
					std::cout << "nah bruh...";
					button = line.substr(startIndex, line.length() - startIndex);
				}
			}

			switch (direction)
			{
			case 1:
				directions.push_back("Down");
				directions.push_back("Left");
				break;
			case 2:
				directions.push_back("Down");
				break;
			case 3:
				directions.push_back("Down");
				directions.push_back("Right");
				break;
			case 4:
				directions.push_back("Left");
				break;
			case 6:
				directions.push_back("Right");
				break;
			case 7:
				directions.push_back("Up");
				directions.push_back("Left");
				break;
			case 8:
				directions.push_back("Up");
				break;
			case 9:
				directions.push_back("Up");
				directions.push_back("Right");
				break;
			default:
				break;
			}

			if (button == "KR")
				buttons.push_back("Kick Right");
			if (button == "KL")
				buttons.push_back("Kick Left");
			if (button == "PR")
				buttons.push_back("Punch Right");
			if (button == "PL")
				buttons.push_back("Punch Left");
			if (button == "M")
				buttons.push_back("Movement");
			if (button == "B")
				buttons.push_back("Block");

			newAction->direction = direction;
			newAction->directions = directions;
			newAction->buttons = buttons;
		}

		if (line.find("ANIM", 0) != std::string::npos)
		{
			Anim newInfo = Anim();

			int startIndex = line.find(' ') + 1;
			int firstComma = line.find(',') + 1;
			int lastComma = line.find_last_of(',') + 1;

			std::string* animName = new std::string(line.substr(startIndex, firstComma - (startIndex + 1)));
			newInfo.name = animName->c_str();

			try
			{
				newInfo.frames = std::stoi(line.substr(firstComma, lastComma - (firstComma + 1)));
			}
			catch (std::invalid_argument const& e)
			{
				std::cout << "whoops...";
			}

			if (line.substr(lastComma, line.length() - lastComma) == "true")
				newInfo.loop = true;
			else if (line.substr(lastComma, line.length() - lastComma) == "false")
				newInfo.loop = false;

			newAction->animations.push_back(newInfo);
		}

		if (line.find("VECT", 0) != std::string::npos)
		{
			int startIndex = line.find(' ') + 1;
			int comma = line.find(',') + 1;

			float x = 0;
			float y = 0;
			try
			{
				x = std::stof(line.substr(startIndex, comma - (startIndex + 1)));
			}
			catch (std::invalid_argument const& e)
			{
				std::cout << "that X is wrong...";
			}

			try
			{
				x = std::stof(line.substr(comma, line.length() - comma));
			}
			catch (std::invalid_argument const& e)
			{
				std::cout << "that Y is wrong...";
			}
		}

		if (line.find("HBOX", 0) != std::string::npos)
		{
			int startIndex = line.find(' ') + 1;
			int comma = line.find(',') + 1;

			Hitbox hitbox = Hitbox();

			std::string* hBoxName = new std::string(line.substr(startIndex, comma - (startIndex + 1)));
			hitbox.name = hBoxName->c_str();

			try
			{
				hitbox.damage = std::stof(line.substr(comma, line.length() - comma));
			}
			catch (std::invalid_argument const& e)
			{
				std::cout << "that hitbox man...";
			}

			newAction->animations[newAction->animations.size() - 1].hitboxes.push_back(hitbox);
		}


		if (line.find("INTR", 0) != std::string::npos)
		{
			int startIndex = line.find(' ') + 1;
			int firstComma = line.find(',') + 1;
			int lastComma = line.find_last_of(',') + 1;

			Interrupt interruption = Interrupt();

			std::string* interruptName = new std::string(line.substr(startIndex, firstComma - (startIndex + 1)));
			interruption.name = interruptName->c_str();

			try
			{
				interruption.startFrame = std::stof(line.substr(firstComma, lastComma - (firstComma + 1)));
			}
			catch (std::invalid_argument const& e)
			{
				std::cout << "that startframe is wrong...";
			}

			try
			{
				interruption.endFrame = std::stof(line.substr(lastComma, line.length() - lastComma));
			}
			catch (std::invalid_argument const& e)
			{
				std::cout << "that endframe is wrong...";
			}

			newAction->interruptions.push_back(interruption);
		}


		if (line.find("WAIT", 0) != std::string::npos)
		{

		}

		if (line == "")
		{

		}

	}

	return Actions;
}

ActionListLoader::ActionListLoader()
{
}

ActionListLoader::~ActionListLoader()
{
}
